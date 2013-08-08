/*
 * @file    in_sc68.c
 * @brief   sc68-ng plugin for winamp 5.5.
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-08-09 00:54:15 ben>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* generated config header */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef NOVTABLE
# define NOVTABLE
#endif

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

/* libc */
#include <stdio.h>
#include <ctype.h>

/* sc68 */
#include <sc68/sc68.h>
#include <sc68/file68_features.h>
#include <sc68/file68_str.h>

/* windows */
#include <windows.h>

/* winamp 2 */
#include "winamp/in2.h"

#ifdef __cplusplus
/* winamp 3 */
#include "api/syscb/api_syscb.h"
#include "api/syscb/callbacks/syscb.h"
#include "api/service/waservicefactory.h"
#include "api/service/services.h"
#include "api/service/api_service.h"
#include "api/memmgr/api_memmgr.h"
#include "../Agave/Language/api_language.h"
#define WITH_API_SERVICE
static api_service * g_service;
#endif

#undef   _MSC_VER                       /* fix intptr_t redefinition */
#define  _MSC_VER 2000
#include "winamp/wa_ipc.h"

/* Post this to the main window at end of file. */
#define WM_WA_MPEG_EOF WM_USER+2
static char appname[] = "winamp";
static char *argv[] = { appname };

/*******************************************************************************
 * Plugin private data.
 ******************************************************************************/

typedef struct wasc68_s wasc68_t;

static struct wasc68_s {
  HANDLE      lock;   /* mutex handle           */
  int         zero_A; /* zeroed from this point */

  HANDLE      thdl;   /* thread handle          */
  DWORD       tid;    /* thread id              */

  char      * uri;        /* allocated URI          */
  sc68_t    * sc68;       /* sc68 emulator instance */
  sc68_music_info_t mi;   /* music info             */
  int         code;       /* sc68 process code      */
  int         samplerate; /* sampling rate in hz    */
  int         maxlatency; /* max latency in ms      */
  int         position;   /* current position in ms */
  int         allin1;     /* play all tracks as one */

  volatile int paused;    /* pause status           */
  volatile int stop_req;  /* request playback stop  */

  int         zero_B; /* zeroed to this point */

  /* Sample buffer (x2 for DSP) */
  char        spl[576*2*2*2];
} g_wasc68;


/*******************************************************************************
 * Cache
 ******************************************************************************/

enum {
  MAX_CACHED_DISK = 8
};

static struct {
  HANDLE lock;   /* mutex handle for diskinfo cache */
  struct cache_entry {
    int ref;                            /* reference count */
    char * uri;                         /* uri  (key)      */
    sc68_disk_t disk;                   /* disk (val)      */
  } e [MAX_CACHED_DISK];
} g_cache;

static sc68_disk_t wasc68_get_disk(const char * uri);
static void        wasc68_rel_disk(sc68_disk_t disk, int dont_keep);

/*******************************************************************************
 * Declaration
 ******************************************************************************/


/* The decode thread */
static DWORD WINAPI playloop(LPVOID b);
static void dump_music_info(const sc68_music_info_t * const mi, const char * uri);

/*******************************************************************************
 * Debug
 ******************************************************************************/

static void dbg_va(const char * fmt, va_list list)
{
  char s[1024];
  vsnprintf(s, sizeof(s), fmt, list);
    OutputDebugString(s);
}

static void dbg(const char * fmt, ...)
{
  va_list list;
  va_start(list,fmt);
  dbg_va(fmt,list);
  va_end(list);
}

static void msg(const int bit, void *userdata, const char *fmt, va_list list)
{
  dbg_va(fmt,list);
}

/*******************************************************************************
 * LOCKS
 ******************************************************************************/
static wasc68_t * lock(void) {
  return WaitForSingleObject(g_wasc68.lock, INFINITE) == WAIT_OBJECT_0
    ? &g_wasc68
    : 0
    ;
}

static int cache_lock(void) {
  return WaitForSingleObject(g_cache.lock, INFINITE) == WAIT_OBJECT_0;
}

static void unlock(wasc68_t * wasc68) {
  if (wasc68)
    ReleaseMutex(wasc68->lock);
}

static void cache_unlock(void) {
  ReleaseMutex(g_cache.lock);
}



static void init();
static void quit();
static void config(HWND);
static void about(HWND);
static  int infobox(const char *, HWND);
static  int isourfile(const char *);
static void pause();
static void unpause();
static  int ispaused();
static  int getlength();
static  int getoutputtime();
static void setoutputtime(int);
static void setvolume(int);
static void setpan(int);
static  int play(const char *);
static void stop();
static void getfileinfo(const in_char *, in_char *, int *);
static void seteq(int, char *, int);

static
/*******************************************************************************
 * THE INPUT MODULE
 ******************************************************************************/
In_Module g_mod =
{
  IN_VER,               /* Input plugin version as defined in in2.h */
  (char*)
  "Atari ST and Amiga music player",    /* Description */
  0,                          /* hMainWindow (filled in by winamp)  */
  0,                          /* hDllInstance (filled in by winamp) */
  (char*)
  "sc68\0" "sc68 file (*.sc68)\0"
  "snd\0" "sndh file (*.snd)\0" "sndh\0" "sndh file (*.sndh)\0",
  0,                                  /* is_seekable */
  1,                                  /* uses output plug-in system */

  config,
  about,
  init,
  quit,
  getfileinfo,
  infobox,
  isourfile,
  play,
  pause,
  unpause,
  ispaused,
  stop,

  getlength,
  getoutputtime,
  setoutputtime,

  setvolume,
  setpan,

  0,0,0,0,0,0,0,0,0,     /* visualization calls filled in by winamp */
  0,0,                   /* dsp calls filled in by winamp */
  seteq,                 /* set equalizer */
  NULL,                  /* setinfo call filled in by winamp */
  0                      /* out_mod filled in by winamp */

};

static
/*******************************************************************************
 * CONFIG DIALOG
 ******************************************************************************/
void config(HWND hwndParent)
{
  MessageBox(hwndParent,
             "No configuration yet",
             "Configuration",
             MB_OK);
}

static
/*******************************************************************************
 * ABOUT DIALOG
 ******************************************************************************/
void about(HWND hwndParent)
{
  MessageBox(hwndParent,
             "sc68 - Atari ST and Amiga music player, by Benjamin Gerard",
             "About sc68 for winamp",
             MB_OK);
}


static
/*******************************************************************************
 * INFO DIALOG
 ******************************************************************************/
int infobox(const char *fn, HWND hwnd)
{
/* #define INFOBOX_EDITED 0 */
/* #define INFOBOX_UNCHANGED 1 */
  return INFOBOX_UNCHANGED;
}

static
/*******************************************************************************
 * FILE DETECTION
 ******************************************************************************/
int isourfile(const char * uri)
{
  if (uri && *uri) {
    if (!strncmp68(uri,"sc68:",5))
      return 1;
    else {
      const char * ext = strrchr(uri,'.');
      if (ext &&
          (0
           || !strcmp68(ext,".sc68")
#ifdef FILE68_Z
           || !strcmp68(ext,".sc68.gz")
#endif
           || !strcmp68(ext,".snd")
           || !strcmp68(ext,".sndh")
            ))
        return 1;
    }
  }
  return 0;
}

static
/*******************************************************************************
 * PAUSE
 ******************************************************************************/
int setpause(int val)
{
  wasc68_t * wasc68;
  if (wasc68 = lock(), wasc68) {
    if (val >= 0)
      g_mod.outMod -> Pause(wasc68->paused = val);
    else
      val = wasc68->paused;
    unlock(wasc68);
  }
  return val;
}

static void pause() {
  setpause(1);
}

static void unpause() {
  setpause(0);
}

static int ispaused() {
  return setpause(-1);
}

static
/*******************************************************************************
 * GET LENGTH (MS)
 ******************************************************************************/
int getlength()
{
  int ms = 0;
  wasc68_t * wasc68;
  if (wasc68 = lock(), wasc68) {
    ms = wasc68->mi.dsk.time_ms;
    unlock(wasc68);
  }
  return ms;
}

static
/*******************************************************************************
 * GET CURRENT POSITION (MS)
 ******************************************************************************/
int getoutputtime()
{
  int ms = 0;
  wasc68_t * wasc68;
  if (wasc68 = lock(), wasc68) {
    ms = wasc68->position +
      + g_mod.outMod->GetOutputTime()
      - g_mod.outMod->GetWrittenTime()
      ;
    unlock(wasc68);
  }
  return ms;
}

static
/*******************************************************************************
 * SET CURRENT POSITION (MS)
 ******************************************************************************/
void setoutputtime(int ms)
{
/* Not supported ATM */
#if 0
  wasc68_t * wasc68;
  if (wasc68 = lock(), wasc68) {
    ms = sc68_seek(wasc68->sc68, SC68_SEEK_PLAY, ms, 0);
    dbg("wasc68::setoutputtime - seek => [%d ms] \n",ms);
    unlock(wasc68);
  }
#endif
}

static
/*******************************************************************************
 * SET VOLUME
 ******************************************************************************/
void setvolume(int volume)
{
  g_mod.outMod->SetVolume(volume);
}

static
/*******************************************************************************
 * SET PAN
 ******************************************************************************/
void setpan(int pan)
{
  g_mod.outMod->SetPan(pan);
}

static
/*******************************************************************************
 * SET EQUALIZER : Do nothing to ignore
 ******************************************************************************/
void seteq(int on, char data[10], int preamp) {}


static void clean_close(wasc68_t * wasc68)
{
  if (!wasc68)
    return;

  if (wasc68->thdl) {
    TerminateThread(wasc68->thdl,1);
    CloseHandle(wasc68->thdl);
    wasc68->thdl = 0;
    dbg("wasc68::clean - %s\n","thread cleaned");
  }
  wasc68->mi.trk.track = 0;
  if (wasc68->sc68) {
    sc68_destroy(wasc68->sc68);
    wasc68->sc68 = 0;
    dbg("wasc68::clean - %s\n","sc68 cleaned");
  }
  if (wasc68->uri) {
    free(wasc68->uri);
    wasc68->uri = 0;
    dbg("wasc68::clean - %s\n","uri cleaned");
  }
  if ( 1 /* wasc68->mod */) {
    /* Close output system. */
    g_mod.outMod->Close();
    /* Deinitialize visualization. */
    g_mod.SAVSADeInit();
  }
}

static
/*******************************************************************************
 * STOP
 ******************************************************************************/
void stop()
{
  wasc68_t * wasc68;
  wasc68 = lock();
  if (wasc68) {
    wasc68->stop_req = 1;
    if (wasc68->thdl) {
      switch ( WaitForSingleObject(wasc68->thdl,10000) ) {
      case WAIT_OBJECT_0:
        CloseHandle(wasc68->thdl);
        wasc68->thdl = 0;
        break;
      default:
        dbg("wasc68::stop - thread did not exit normally\n");
      }
    }
    clean_close(wasc68);
    unlock(wasc68);
  }
}


static
/*******************************************************************************
 * PLAY
 *
 * @retval  0 on success
 * @reval  -1 on file not found
 * @retval !0 stopping winamp error
 ******************************************************************************/
int play(const char *fn)
{
  int err = 1;
  wasc68_t * wasc68;
  sc68_disk_t disk;

  if (!fn || !*fn)
    return -1;

  if (wasc68 = lock(), !wasc68)
    goto inused;

  /* Safety net */
  if (wasc68->sc68 || wasc68->thdl || wasc68->uri)
    goto inused;

  memset(&wasc68->zero_A, 0, (char *)&wasc68->zero_B-(char *)&wasc68->zero_A);
  /* dbg("wasc68::play - cleared\n"); */

  /* ony mode supported ATM */
  wasc68->allin1 = 1;

  /* Create sc68 emulator instance */
  if (!wasc68->sc68)
    wasc68->sc68 = sc68_create(0);
  if (!wasc68->sc68)
    goto exit;

  /* Duplicate filename */
  wasc68->uri = strdup(fn);
  if (!wasc68->uri || sc68_load_uri(wasc68->sc68, wasc68->uri)) {
    err = -1;
    goto exit;
  }
  dbg("wasc68::play - got a file -- %s\n", wasc68->uri);

  /* Get sampling rate */
  wasc68->samplerate = sc68_cntl(wasc68->sc68, SC68_GET_SPR);
  if (wasc68->samplerate <= 0) {
    dbg("wasc68::play - sc68 query sampling rate failed\n");
    goto exit;
  }
  dbg("wasc68::play - got a filesampling rate -- %d\n", wasc68->samplerate);

  /* Get disk and track info */
  if (sc68_play(wasc68->sc68, 1, SC68_DEF_LOOP) < 0) {
    dbg("wasc68::play - sc68 play track #1 failed\n");
    goto exit;
  }
  dbg("wasc68::play\n");

  wasc68->code = sc68_process(wasc68->sc68, 0, 0);
  if (wasc68->code == SC68_ERROR) {
    dbg("wasc68::play - sc68 process failed\n");
    goto exit;
  }
  dbg("wasc68::process -- %x\n", wasc68->code);

  if (sc68_music_info(wasc68->sc68, &wasc68->mi, SC68_CUR_TRACK, 0)) {
    dbg("wasc68::play - sc68 get music info failed\n");
    goto exit;
  }
  dump_music_info(&wasc68->mi, wasc68->uri);

  /* Init output module */
  wasc68->maxlatency = g_mod.outMod->Open(wasc68->samplerate, 2, 16, -1, -1);
  if (wasc68->maxlatency < 0) {
    dbg("wasc68::play - open output module failed\n");
    goto exit;
  }
  /* wasc68->mod = &mod; */
  g_mod.outMod->SetVolume(-666); /* set default volume */

  /* Init info and visualization stuff */
  g_mod.SetInfo(0, wasc68->samplerate/1000, 2, 1);
  g_mod.SAVSAInit(wasc68->maxlatency, wasc68->samplerate);
  g_mod.VSASetInfo(wasc68->samplerate, 2);

  dbg("wasc68::mod initialized\n");

  /* Init play thread */
  wasc68->thdl = (HANDLE)
    CreateThread(NULL,                  /* Default Security Attributs */
                 0,                     /* Default stack size  */
                 (LPTHREAD_START_ROUTINE)playloop, /* Thread function */
                 (LPVOID) wasc68,                  /* Thread Cookie   */
                 0,                                /* Thread status   */
                 &wasc68->tid                      /* Thread Id       */
      );

  dbg("wasc68::play - thread %p\n", wasc68->thdl);

  err = !wasc68->thdl;

exit:
  if (err) {
    clean_close(wasc68);
  }

inused:
  unlock(wasc68);

  dbg("} wasc68::play(%s) =>[%d]\n",fn?fn:"(nil)", err);
  return err;
}

static
const char * get_tag(const sc68_cinfo_t * const cinfo, const char * const key) {
  int i;
  for (i=0; i<cinfo->tags; ++i)
    if (!strcmp(cinfo->tag[i].key, key))
      return cinfo->tag[i].val;
  return 0;
}

static
void updatetrack(wasc68_t * wasc68)
{
  if (wasc68)
    sc68_music_info(wasc68->sc68, &wasc68->mi, -1, 0);
}

static
void dump_music_info(const sc68_music_info_t * const mi, const char * uri)
{
  int j, len = 11;

  dbg("%s\n", "***********************");
  dbg("DUMP MUSIC-INFO for %s\n", uri ? uri : "N/A");
  dbg("%s\n", "========================");
  dbg("%-*s : %s %s\n", len, "Disk"   , mi->dsk.time, mi->album);
  dbg("%-*s : %s %s\n", len, "Track"  , mi->trk.time, mi->title);
  dbg("%-*s : %s\n",    len, "Artist" , mi->artist);
  /* dbg("%-*s : %02u:%02u,%02u\n", len, "Loop-Len", */
  /*         mi->loop_ms / 60000u, mi->loop_ms / 1000u % 60u, mi->loop_ms % 1000u / 100); */
  dbg("%s\n", "------------------------");
  dbg("Disk tags:\n");
  for (j=0; j<mi->dsk.tags; ++j)
    dbg("* %c%-*s : %s\n",
        toupper(*mi->dsk.tag[j].key),
        len-3,
        mi->dsk.tag[j].key+1,
        mi->dsk.tag[j].val);

  dbg("%s\n", "------------------------");
  dbg("Tracks tags:\n");
  for (j=0; j<mi->trk.tags; ++j)
    dbg("* %c%-*s : %s\n",
        toupper(*mi->trk.tag[j].key),
        len-3,
        mi->trk.tag[j].key+1,
        mi->trk.tag[j].val);
  dbg("%s\n", "========================");
}

static
/*******************************************************************************
 * GET FILE INFO
 ******************************************************************************/
/*
 * this is an odd function. It is used to get the title and/or length
 * of a track.  If filename is either NULL or of length 0, it means
 * you should return the info of lastfn. Otherwise, return the
 * information for the file in filename.  if title is NULL, no title
 * is copied into it.  if length_in_ms is NULL, no length is copied
 * into it.
 ******************************************************************************/
void getfileinfo(const in_char * uri, in_char * title, int * length_in_ms)
{
  const int max = GETFILEINFO_TITLE_LENGTH;

  dbg("wasc68::getfileinfo('%s') {\n",uri?uri:"(nil)");

  if (!uri || !*uri) {
    wasc68_t * wasc68;

    return;

    if (wasc68 = lock(), wasc68) {
      sc68_music_info_t * const mi = &wasc68->mi;
      dbg("wasc68::getfileinfo - locked\n");
      if (!mi->trk.track)
        sc68_music_info(wasc68->sc68, mi, -1, 0); /* just in case ... */

      if (title) {
        int i = 0;
        const char * const artist   = mi->artist;
        const char * const    aka   = get_tag(&mi->trk, "aka");
        const char * const composer = get_tag(&mi->trk, "composer");
        const char * const original = composer ? composer : get_tag(&mi->trk, "original");

        i += snprintf(title+i, max-i, "%s - %02d", mi->album, mi->trk.track);
        if (i < max && strcmp(mi->album,mi->title))
          i += snprintf(title+i, max-i, " - %s", mi->title);
        if (i < max)
          i += snprintf(title+i, max-i, " - By %s", aka ? aka : artist);
        if (i < max && (aka && strcmp(aka,original)) && strcmp(artist,original) )
          i += snprintf(title+i, max-i, " - Original by %s", original);
        if (i < max)
          i += snprintf(title+i, max-i, " - Hardware %s", mi->trk.hw);
        title[max-1] = 0;
      }

      if (length_in_ms)
        *length_in_ms = mi->dsk.time_ms;
      unlock(wasc68);
      dbg("wasc68::getfileinfo - unlock\n");
    }

  } else {
    sc68_disk_t disk;
    sc68_music_info_t mi;

    if (disk = wasc68_get_disk(uri), disk) {
      if (!sc68_music_info(0, &mi, SC68_DEF_TRACK, disk)) {
        dump_music_info(&mi, uri);
        if (title) {
          const char * const    aka = get_tag(&mi.dsk, "aka");
          const char * const artist = aka ? aka : mi.artist;
          if (mi.tracks == 1) {
            if (strcmp(mi.album,mi.title))
              snprintf(title, max, "%s - %s - %s", artist, mi.album, mi.title);
            else
              snprintf(title, max, "%s - %s", artist, mi.album);
          } else {
            snprintf(title, max, "%s - %s [%d tracks]", artist, mi.album, mi.tracks);
          }
          title[max-1] = 0;
          dbg("wasc68::getfileinfo - set title -- '%s'\n", title);
        }
        if (length_in_ms) {
          *length_in_ms = mi.dsk.time_ms;
          dbg("wasc68::getfileinfo - set time -- '%d ms'\n", *length_in_ms);
        }
      }
      wasc68_rel_disk(disk, 0);
      }
    }
  }

static
/*******************************************************************************
 * LOOP
 ******************************************************************************/
DWORD WINAPI playloop(LPVOID _wasc68)
{
  wasc68_t * wasc68 = (wasc68_t *)_wasc68;
  const int get_pos =
    wasc68->allin1 ? SC68_GET_PLAYPOS :SC68_GET_POS;

  dbg("wasc68::playloop(%p) {\n",_wasc68);

  while (!wasc68->stop_req) {
    int seeking = 0, l;

    wasc68->position = sc68_cntl(wasc68->sc68, get_pos);

    if (seeking)
      g_mod.outMod->Flush(wasc68->position);

    if (g_mod.outMod->CanWrite() >= (576 << (2+!!g_mod.dsp_isactive())) ) {
      /* CanWrite() returns the number of bytes you can write, so we
         check that to the block size. the reason we multiply the
         block size by two if g_mod.dsp_isactive() is that DSP plug-ins
         can change it by up to a factor of two (for tempo
         adjustment).
      */
      int n = 576;
      wasc68->code = sc68_process(wasc68->sc68, wasc68->spl, &n);
      if (wasc68->code & SC68_END) {
        break;
      }
      if (wasc68->code & SC68_CHANGE) {
        if (!wasc68->allin1)
          break;
        sc68_music_info(wasc68->sc68, &wasc68->mi, -1, 0);
        SendMessage(g_mod.hMainWindow,WM_WA_IPC,0,IPC_UPDTITLE);
        /* SendMessage(g_mod.hMainWindow, */
        /*             WM_WA_IPC, */
        /*             (WPARAM)(char*)"title", */
        /*             IPC_METADATA_CHANGED); */
      }

      /* Give the samples to the vis subsystems */
      g_mod.SAAddPCMData (wasc68->spl, 2, 16, wasc68->position);
      g_mod.VSAAddPCMData(wasc68->spl, 2, 16, wasc68->position);

      /* If we have a DSP plug-in, then call it on our samples */
      l = (
        g_mod.dsp_isactive()
        ? g_mod.dsp_dosamples((short *)wasc68->spl, n, 16, 2, wasc68->samplerate)
        : n ) << 2;

      /* Write the pcm data to the output system */
      g_mod.outMod->Write(wasc68->spl, l);
    }
    else {
      Sleep(20);                        /* wait a while */
    }
  }

  /* Some output drivers need CanWrite to be called on a regular
   * basis.
   */
  while (!wasc68->stop_req) {
    g_mod.outMod->CanWrite();
    if (!g_mod.outMod->IsPlaying()) {
      /* Done playing: tell Winamp and quit the thread */
      PostMessage(g_mod.hMainWindow, WM_WA_MPEG_EOF, 0, 0);
      break;
    } else {
      Sleep(15);              // give a little CPU time back to the system.
    }
  }

  dbg("} wasc68::playloop(%p) => [0]\n",_wasc68);
  return 0;
}


static
/*******************************************************************************
 * PLUGIN INIT
 ******************************************************************************/
void init()
{
  sc68_init_t init68;

  memset(&init68,0,sizeof(init68));
  init68.argv = argv;
  init68.argc = sizeof(argv) / sizeof(*argv);
#ifdef DEBUG
  init68.debug_set_mask =  0;
  init68.debug_clr_mask =  0;
  init68.msg_handler = (sc68_msg_t)msg;
#endif
  sc68_init(&init68);

  /* clear and init private */
  memset(&g_wasc68,0,sizeof(g_wasc68));
  g_wasc68.lock = CreateMutex(NULL, FALSE, NULL);

  /* clear and init cacke */
  memset(&g_cache,0,sizeof(g_cache));
  g_cache.lock = CreateMutex(NULL, FALSE, NULL);

#ifdef WITH_API_SERVICE
  /* Get WASABI service */
  if (!g_service) {
    g_service = (api_service *)
      SendMessage(g_mod.hMainWindow,WM_WA_IPC,0,IPC_GET_API_SERVICE);
    if (g_service == (api_service *)1)
      g_service = 0;
  }

  static int fcc[] = {
    WaSvc::NONE,
    WaSvc::UNIQUE,
    WaSvc::OBJECT,
    WaSvc::CONTEXTCMD,
    WaSvc::DEVICE,
    WaSvc::FILEREADER,
    WaSvc::FILESELECTOR,
    WaSvc::STORAGEVOLENUM,
    WaSvc::IMAGEGENERATOR,
    WaSvc::IMAGELOADER,
    WaSvc::IMAGEWRITER,
    WaSvc::ITEMMANAGER,
    WaSvc::PLAYLISTREADER,
    WaSvc::PLAYLISTWRITER,
    WaSvc::MEDIACONVERTER,
    WaSvc::MEDIACORE,
    WaSvc::MEDIARECORDER,
    WaSvc::SCRIPTOBJECT,
    WaSvc::WINDOWCREATE,
    WaSvc::XMLPROVIDER,
    WaSvc::DB,
    WaSvc::SKINFILTER,
    WaSvc::METADATA,
    WaSvc::METATAG,
    WaSvc::EVALUATOR,
    WaSvc::MINIBROWSER,
    WaSvc::TOOLTIPSRENDERER,
    WaSvc::XUIOBJECT,
    WaSvc::STRINGCONVERTER,
    WaSvc::ACTION,
    WaSvc::COREADMIN,
    WaSvc::DROPTARGET,
    WaSvc::OBJECTDIR,
    WaSvc::TEXTFEED,
    WaSvc::ACCESSIBILITY,
    WaSvc::ACCESSIBILITYROLESERVER,
    WaSvc::EXPORTER,
    WaSvc::COLLECTION,
    WaSvc::REDIRECT,
    WaSvc::FONTRENDER,
    WaSvc::SRCCLASSFACTORY,
    WaSvc::SRCEDITOR,
    WaSvc::MP4AUDIODECODER,
    WaSvc::PLAYLISTREADER_WA5,
    WaSvc::PLAYLISTWRITER_WA5,
    WaSvc::PLAYLISTHANDLER,
    WaSvc::TAGPROVIDER,
    WaSvc::NSVFACTORY,
    -1
  };

  if (g_service) {
    int i, n, j;
    waServiceFactory * s;
    dbg("wasc68::init -- SVC api <%p>\n", g_service);

    for (j=0; fcc[j] != -1; ++j) {
      char cc[5];
      cc[0] = fcc[j]>>24; cc[1] = fcc[j]>>16;
      cc[2] = fcc[j]>> 8; cc[3] = fcc[j]; cc[4] = 0;

      n = g_service->service_getNumServices(fcc[j]);
      dbg("wasc68::init -- [%s] got %d service(s)\n", cc, n);
      for (s = g_service->service_enumService(fcc[j], i=0);
           s;
           s = g_service->service_enumService(fcc[j], ++i)) {
        dbg("wasc68::init -- [%s] #%02d SVC factory  '%s'\n",
            cc, i, s ? s->getServiceName() : "(nil)");
      }
    }

    s = g_service->service_getServiceByGuid(languageApiGUID);
    if (s) {
      dbg("wasc68::init -- service factory lang %p '%s'\n",
          s, s?s->getServiceName():"(nil)");
    } else {
      dbg("wasc68::init -- don't have service factory lang\n");
    }

    s = g_service->service_getServiceByGuid(memMgrApiServiceGuid);
    if (s) {
      dbg("wasc68::init -- service factory memman %p '%s'\n",
          s, s?s->getServiceName():"(nil)");
    } else {
      dbg("wasc68::init -- don't have service memman lang\n");
    }
    //if (sf) WASABI_API_LNG = reinterpret_cast<api_language*>(sf->getInterface());

  }
#endif

}

static
/*******************************************************************************
 * PLUGIN SHUTDOWN
 ******************************************************************************/
void quit()
{
  wasc68_t * wasc68;

  wasc68 = lock();
  unlock(wasc68);
  CloseHandle(g_wasc68.lock);
  g_wasc68.lock = 0;
  memset(&g_wasc68,0,sizeof(g_wasc68));

  /* Clean the cache */
  if (cache_lock()) {
    int i;
    for (i=0; i<MAX_CACHED_DISK; ++i) {
      if (g_cache.e[i].ref > 0) {
        dbg("wasc68::quit -- cache #%d has %d references\n",
            i, g_cache.e[i].ref);
        continue;
      }
      g_cache.e[i].ref = 0;
      if (g_cache.e[i].uri) {
        dbg("wasc68::quit -- cache - #%d %d <%p> '%s'",
            i, g_cache.e[i].ref, g_cache.e[i].disk,g_cache.e[i].uri);
        free(g_cache.e[i].uri);
        g_cache.e[i].uri = 0;
      }
      sc68_disk_free(g_cache.e[i].disk);
      g_cache.e[i].disk = 0;
    }
    cache_unlock();
    memset(&g_cache,0,sizeof(g_cache));
  }
  sc68_shutdown();
}

/*******************************************************************************
 * Disk cache
 ******************************************************************************/

static void  wasc68_rel_disk(sc68_disk_t disk, int dont_keep)
{
  if (!disk) return;                    /* safety net */

  if (cache_lock()) {
    int i;
    for (i = 0; i < MAX_CACHED_DISK; ++i) {
      if (disk != g_cache.e[i].disk)
        continue;
      if (--g_cache.e[i].ref <= 0) {
        if (dont_keep) {
          dbg("wasc68::get_disk -- cache - #%d %d <%p> '%s'\n",
              i, g_cache.e[i].ref, g_cache.e[i].disk, g_cache.e[i].uri);
          free(g_cache.e[i].uri);
          g_cache.e[i].uri = 0;
          sc68_disk_free(g_cache.e[i].disk);
          g_cache.e[i].disk = 0;
        }
        if (g_cache.e[i].ref != 0) {
          dbg("wasc68::rel_disk -- !!! reference is %d !!! \n",
              g_cache.e[i].ref);
          g_cache.e[i].ref = 0;
        }
      } else {
        dbg("wasc68::get_disk -- cache ~ #%d %d <%p> '%s'\n",
            i, g_cache.e[i].ref, g_cache.e[i].disk, g_cache.e[i].uri);
      }
      break;
    }
    cache_unlock();
    if (i == MAX_CACHED_DISK) {
      dbg("wasc68::get_disk -- cache miss <%p>\n", disk);
      sc68_disk_free(disk);
    }
  } else {
    dbg("wasc68::rel_disk -- %s\n", "cache lock failed");
  }
}

static sc68_disk_t wasc68_get_disk(const char * uri)
{
  sc68_disk_t disk = 0;

  if (!uri || !*uri) {
    dbg("wasc68::get_disk -- %s\n", "no uri");
    return 0;
  }

  if (cache_lock()) {
    int i, j, k ;
    for (i = 0, j = k = -1; i < MAX_CACHED_DISK; ++i) {
      if (!g_cache.e[i].disk) {
        if (j < 0) j = i;           /* keep track of 1st free entry */
      } else if (!strcmp(uri, g_cache.e[i].uri)) {
        disk = g_cache.e[i].disk;
        g_cache.e[i].ref++;
        dbg("wasc68::get_disk -- cache = #%d %d <%p> '%s'\n",
            i, g_cache.e[i].ref,g_cache.e[i].disk,g_cache.e[i].uri);
        break;
      } else if (k < 0 && ! g_cache.e[i].ref)
        k =i;               /* keep track of 1st unreferenced entry */
    }

    /* Did not find this uri in cache, load the disk. */
    if (!disk) {
      disk = sc68_load_disk_uri(uri);
      if (disk) {
        /* Free or unreferenced entry ? */
        i = j >= 0 ? j : k;
        /* Have a free entry in the cache ? */
        if (i >= 0) {
          free(g_cache.e[i].uri);
          sc68_disk_free(g_cache.e[i].disk);
          g_cache.e[i].disk = 0;
          g_cache.e[i].uri = 0;
          g_cache.e[i].uri = strdup(uri);
          if (g_cache.e[i].uri) {
            g_cache.e[i].ref  = 1;
            g_cache.e[i].disk = disk;
            dbg("wasc68::get_disk -- cache + #%d %d <%p> '%s'\n",
                i, g_cache.e[i].ref,g_cache.e[i].disk,g_cache.e[i].uri);
          } else {
            dbg("wasc68::get_disk -- cache alloc failed -- '%s'\n", uri);
          }
        } else {
          dbg("wasc68::get_disk -- cache full -- '%s'\n", uri);
        }
      } else {
        dbg("wasc68::get_disk -- '%s' -- %s\n",
            uri,
            "could not cache (load failed)");
      }
    }
    cache_unlock();
  } else {
    dbg("wasc68::get_disk -- cache lock failed -- '%s'\n", uri);
    disk = sc68_load_disk_uri(uri);
  }
  dbg("wasc68::get_disk -- '%s' -- <%p>\n", uri, disk);

  return disk;
}

/*******************************************************************************
 * EXPORTED SYMBOL
 ******************************************************************************/

#ifdef __cplusplus
# define EXPORT extern "C" __declspec(dllexport)
#else
# define EXPORT __declspec(dllexport)
#endif

EXPORT
In_Module *winampGetInModule2()
{
  return &g_mod;
}


/**
 * @retval 1 tag handled
 * @retval 0 unsupported tag
 */
EXPORT
int winampGetExtendedFileInfo(const char *fn,
                              const char *data, char *dest, size_t destlen)
{
  sc68_disk_t disk;
  int res = 0;

  if (disk = wasc68_get_disk(fn), disk) {
    sc68_music_info_t * mi, tmpmi;
    const char * value;

    /* dbg("wasc68::%s - disk loaded\n",__FUNCTION__); */

    sc68_music_info(0, mi = &tmpmi, 0, disk);
    res = 1;

    /* "TYPE" is an important value as it tells Winamp if this is an
     * audio or video format */
    if (!strcasecmp(data, "type"))
      strncpy(dest, "0", destlen);       /* audio format */
    else if (!strcasecmp(data,"family"))
      strncpy(dest, "sc68 audio files", destlen);
    if (!strcasecmp(data,"album"))       /* Album name */
      strncpy(dest, mi->album, destlen);
    else if (!strcasecmp(data,"title"))
      strncpy(dest, mi->title, destlen); /* Song title */
    else if (!strcasecmp(data,"artist")) {
      const char * aka = get_tag(&mi->dsk,"aka");
      strncpy(dest, aka ? aka : mi->artist, destlen);
    }
    else if (!strcasecmp(data,"composer")) {
      const char * original = get_tag(&mi->dsk,"original");
      const char * composer = original ? original : get_tag(&mi->dsk,"composer");
      if (composer)
        strncpy(dest, composer, destlen);
      else
        res = 0;
    }
    /* else if (!strcasecmp(data,"track")) */
    /*   snprintf(dest, destlen, "%02d", ti->track); */
    else if (!strcasecmp(data,"genre")) {
      const char * genre = get_tag(&mi->trk,"genre");
      strncpy(dest,
              genre ? genre : (mi->dsk.amiga
                               ? "Amiga chiptune" : "Atari-ST chiptune"),
              destlen);
    }
    /* Other interresting tag we might handle someday: */
    /* "rating" "albumartist" "year" "publisher" "comment" "lossless"
     * "bpm" */
    else {
      res = 0;
    }
    if (!res) {
      dbg("wasc68::%s unhandled TAG '%s'\n", __FUNCTION__, data);
    } else {
      dest[destlen-1] = 0;
      dbg("wasc68::%s TAG '%s' -> '%s'\n", __FUNCTION__, data, dest);
    }
    wasc68_rel_disk(disk, 0);
  }
  return res;
}

/*******************************************************************************
 * DLL stuff
 ******************************************************************************/

BOOL WINAPI _DllMainCRTStartup(HANDLE instance, ULONG reason, LPVOID lp)
{
  return TRUE;
}
