/*
 * @file    in_sc68.c
 * @brief   sc68-ng plugin for winamp 5.5 - main
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2014 Benjamin Gerard
 *
 * Time-stamp: <2014-02-02 19:57:58 ben>
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

/* winamp sc68 declarations */
#include "wasc68.h"

/* libc */
#include <stdio.h>
#include <ctype.h>

/* sc68 */
#include <sc68/sc68.h>
#include <sc68/file68.h>
#include <sc68/file68_features.h>
#include <sc68/file68_str.h>
#include <sc68/file68_msg.h>
#include <sc68/file68_opt.h>

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
  /* sc68_music_info_t mi; */   /* music info             */
  int         code;       /* sc68 process code      */
  int         samplerate; /* sampling rate in hz    */
  int         maxlatency; /* max latency in ms      */
  int         position;   /* current position in ms */
  int         allin1;     /* play all tracks as one */

  volatile int paused;    /* pause status           */
  volatile int stop_req;  /* request playback stop  */

  int         zero_B; /* zeroed to this point */

  /* Sample buffer (16bit x 2 channels x 2 for DSP) */
  BYTE        spl[576*2*2*2];
} g_wasc68;

/*******************************************************************************
 * Declaration
 ******************************************************************************/

/* The decode thread */
static DWORD WINAPI playloop(LPVOID b);
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

EXTERN int fileinfo_dialog(HINSTANCE hinst, HWND hwnd, const char * uri);
EXTERN int config_dialog(HINSTANCE hinst, HWND hwnd);

/*******************************************************************************
 * Debug
 ******************************************************************************/
int wasc68_cat = msg68_NEVER;

/*******************************************************************************
 * LOCKS
 ******************************************************************************/
static wasc68_t * lock(void) {
  return WaitForSingleObject(g_wasc68.lock, INFINITE) == WAIT_OBJECT_0
    ? &g_wasc68
    : 0
    ;
}

static void unlock(wasc68_t * wasc68) {
  if (wasc68)
    ReleaseMutex(wasc68->lock);
}

void set_asid(int asid) {
  wasc68_t * wasc68 = lock();
  if (wasc68) {
    DBG("set asid mode %d\n", asid);
    sc68_cntl(wasc68->sc68,SC68_SET_ASID,asid);
    if (wasc68->sc68)
      sc68_cntl(0,SC68_SET_ASID,asid);
    unlock(wasc68);
  }
}

/* static */
/*******************************************************************************
 * THE INPUT MODULE
 ******************************************************************************/
In_Module g_mod =
{
  IN_VER,               /* Input plugin version as defined in in2.h */
  (char*)
  "sc68 (Atari ST & Amiga music) v" PACKAGE_VERSION, /* Description */
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
void config(HWND hwnd)
{
  if (config_dialog(g_mod.hDllInstance, hwnd) < 0)
    MessageBox(hwnd,
               "No configuration yet",
               "sc68 for winamp",
               MB_OK);
  else
    sc68_cntl(0, SC68_CONFIG_SAVE);
}

static
/*******************************************************************************
 * ABOUT DIALOG
 ******************************************************************************/
void about(HWND hwnd)
{
  char temp[512];
  snprintf(temp,sizeof(temp),
           "sc68 for winamp\n"
           "Atari ST and Amiga music player\n"
           "using %s and %s"
#ifdef DEBUG
           "\n" " !!! DEBUG Build !!! "
#endif
#ifndef NDEBUG
           "\n" "buid on " __DATE__
#endif
           "\n(C) 1998-2014 Benjamin Gerard",
           sc68_versionstr(),file68_versionstr());

  MessageBox(hwnd,
             temp,
             "About sc68 for winamp",
             MB_OK);
}

static
/*******************************************************************************
 * INFO DIALOG
 ******************************************************************************/
int infobox(const char * uri, HWND hwnd)
{
  fileinfo_dialog(g_mod.hDllInstance, hwnd, uri);
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
    const int fct = wasc68->allin1 ? SC68_GET_DSKLEN : SC68_GET_LEN;
    int res = sc68_cntl(wasc68->sc68, fct);
    if (ms != -1)
      ms = res;
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
    ms = g_mod.outMod->GetOutputTime();

    /* DBG("Otime:%d Wtime:%d Dpos:%d Ppos:%d Tpos:%d\n", */
    /*     g_mod.outMod->GetOutputTime(), */
    /*     g_mod.outMod->GetWrittenTime(), */
    /*     sc68_cntl(wasc68->sc68, SC68_GET_PLAYPOS), */
    /*     sc68_cntl(wasc68->sc68, SC68_GET_DSKPOS), */
    /*     sc68_cntl(wasc68->sc68, SC68_GET_POS) */
    /*   ); */

    /* wasc68->position */
    /*   + g_mod.outMod->GetOutputTime() */
    /*   - g_mod.outMod->GetWrittenTime(); */
    /* if (wasc68->allin1) */
    /*   ms = sc68_cntl(wasc68->sc68, SC68_GET_DSKPOS); */
    /* ms += sc68_cntl(wasc68->sc68, SC68_GET_PLAYPOS); */
    /* ms -= g_mod.outMod->GetWrittenTime(); */
    /* ms = wasc68->position + */
    /*   + g_mod.outMod->GetOutputTime() */
    /*   - g_mod.outMod->GetWrittenTime() */
    /*   ; */
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
/* Not supported ATM.
 * It has to signal the play thread it has to seek.
 */
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
  /* wasc68->mi.trk.track = 0; */
  if (wasc68->sc68) {
    sc68_destroy(wasc68->sc68);
    wasc68->sc68 = 0;
    /* dbg("wasc68::clean - %s\n","sc68 cleaned"); */
  }
  if (wasc68->uri) {
    free(wasc68->uri);
    wasc68->uri = 0;
    /* dbg("wasc68::clean - %s\n","uri cleaned"); */
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

  if (!fn || !*fn)
    return -1;

  if (wasc68 = lock(), !wasc68)
    goto inused;

  /* Safety net */
  if (wasc68->sc68 || wasc68->thdl || wasc68->uri)
    goto inused;

  memset(&wasc68->zero_A, 0, (char *)&wasc68->zero_B-(char *)&wasc68->zero_A);

  /* Only mode supported ATM */
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

  /* Get sampling rate */
  wasc68->samplerate = sc68_cntl(wasc68->sc68, SC68_GET_SPR);
  if (wasc68->samplerate <= 0) {
    goto exit;
  }

  /* Get disk and track info */
  if (sc68_play(wasc68->sc68, 1, SC68_DEF_LOOP) < 0) {
    goto exit;
  }

  wasc68->code = sc68_process(wasc68->sc68, 0, 0);
  if (wasc68->code == SC68_ERROR) {
    goto exit;
  }

  /* if (sc68_music_info(wasc68->sc68, &wasc68->mi, SC68_CUR_TRACK, 0)) { */
  /*   goto exit; */
  /* } */
  /* dump_music_info(&wasc68->mi, wasc68->uri); */

  /* Init output module */
  wasc68->maxlatency = g_mod.outMod->Open(wasc68->samplerate, 2, 16, 0, 0);
  if (wasc68->maxlatency < 0) {
    goto exit;
  }
  /* wasc68->mod = &mod; */
  g_mod.outMod->SetVolume(-666); /* set default volume */

  /* Init info and visualization stuff */
  g_mod.SetInfo(0, wasc68->samplerate/1000, 2, 1);
  g_mod.SAVSAInit(wasc68->maxlatency, wasc68->samplerate);
  g_mod.VSASetInfo(wasc68->samplerate, 2);

  /* Init play thread */
  wasc68->thdl = (HANDLE)
    CreateThread(NULL,                  /* Default Security Attributs */
                 0,                     /* Default stack size  */
                 (LPTHREAD_START_ROUTINE)playloop, /* Thread function */
                 (LPVOID) wasc68,                  /* Thread Cookie   */
                 0,                                /* Thread status   */
                 &wasc68->tid                      /* Thread Id       */
      );

  err = !wasc68->thdl;
exit:
  if (err)
    clean_close(wasc68);

inused:
  unlock(wasc68);

  return err;
}

static
const char * get_tag(const sc68_cinfo_t * const cinfo, const char * const key)
{
  int i;
  for (i=0; i<cinfo->tags; ++i)
    if (!strcmp68(cinfo->tag[i].key, key))
      return cinfo->tag[i].val;
  return 0;
}

#if 0

static
void updatetrack(wasc68_t * wasc68)
{
  /* if (wasc68) */
  /*   sc68_music_info(wasc68->sc68, &wasc68->mi, SC68_CUR_TRACK, 0); */
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
#endif

static void xfinfo(char *title, int *msptr, sc68_t *sc68, sc68_disk_t disk)
{
  const int max = GETFILEINFO_TITLE_LENGTH;
  sc68_music_info_t tmpmi, * const mi = &tmpmi;

  if (sc68_music_info(sc68, mi,
                      sc68 ? SC68_CUR_TRACK : SC68_DEF_TRACK, disk))
    return;

  if (title) {
    const char * artist = get_tag(&mi->dsk, "aka");
    if (!artist)
      artist = mi->artist;

    if (mi->tracks == 1 /* || !strcmp68(mi->title, mi->album) */)
      snprintf(title, max, "%s - %s",artist, mi->album);
    else
      snprintf(title, max, "%s - %s [%d tracks]",
               artist, mi->album, mi->tracks);
  }
  if (msptr)
    *msptr = mi->dsk.time_ms;
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
 * is copied into it.  if msptr is NULL, no length is copied
 * into it.
 ******************************************************************************/
void getfileinfo(const in_char * uri, in_char * title, int * msptr)
{
  /* const int max = GETFILEINFO_TITLE_LENGTH; */

  if (title)
    *title = 0;
  if (msptr)
    *msptr = 0;

  if (!uri || !*uri) {
    /* current disk */
    wasc68_t * wasc68 = 0;
    if (wasc68 = lock(), wasc68) {
      if (wasc68->sc68)
        xfinfo(title, msptr, wasc68->sc68, 0);
      unlock(wasc68);
    }
  } else {
    /* some other disk */
    sc68_disk_t disk;
    if (disk = wasc68_cache_get(uri), disk) {
      xfinfo(title, msptr, 0, disk);
      wasc68_cache_release(disk, 0);
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
    wasc68->allin1 ? SC68_GET_PLAYPOS : SC68_GET_POS;

  while (!wasc68->stop_req) {
    int l;

    wasc68->position = sc68_cntl(wasc68->sc68, get_pos);

    /* if (seeking) */
    /*   g_mod.outMod->Flush(wasc68->position); */

    if (g_mod.outMod->CanWrite() >= (576 << (2+!!g_mod.dsp_isactive()))) {
      /* CanWrite() returns the number of bytes you can write, so we
         check that to the block size. the reason we multiply the
         block size by two if g_mod.dsp_isactive() is that DSP
         plug-ins can change it by up to a factor of two (for tempo
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

        /* sc68_music_info(wasc68->sc68, &wasc68->mi, SC68_CUR_TRACK, 0); */
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
      g_mod.outMod->Write((char*)wasc68->spl, l);
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

  return 0;
}

static int onchange_ufi(const option68_t * opt, value68_t * val)
{
  use_ufi = !!val->num;
  DBG("UFI changed to %s\n",use_ufi?"On":"Off");
  return 0;
}

static
/*******************************************************************************
 * PLUGIN INIT
 ******************************************************************************/
void init()
{
  static option68_t opt_ufi =
    OPT68_BOOL(0,"ufi","winamp","Unified file info dialog",1,onchange_ufi);

  sc68_init_t init68;
  const int debug =
#ifdef DEBUG
    1
#else
    0
#endif
    ;

  memset(&init68,0,sizeof(init68));
  init68.argv = argv;
  init68.argc = sizeof(argv) / sizeof(*argv);
#ifndef NDEBUG
  wasc68_cat = msg68_cat("winamp", "winamp input plugin", debug);
  init68.debug_set_mask = debug << wasc68_cat;
  init68.debug_clr_mask = 0;
  init68.msg_handler = (sc68_msg_t) msgfct;
#endif
  init68.flags.no_load_config = 1;      /* disable config load */
  sc68_init(&init68);
  option68_append(&opt_ufi, 1);         /* add our own options */
  sc68_cntl(0,SC68_CONFIG_LOAD);

  /* clear and init private */
  memset(&g_wasc68,0,sizeof(g_wasc68));
  g_wasc68.lock = CreateMutex(NULL, FALSE, NULL);

  /* clear and init cacke */
  wasc68_cache_init();

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
    DBG("SVC api <%p>\n", g_service);

    for (j=0; fcc[j] != -1; ++j) {
      char cc[5];
      cc[0] = fcc[j]>>24; cc[1] = fcc[j]>>16;
      cc[2] = fcc[j]>> 8; cc[3] = fcc[j]; cc[4] = 0;

      n = g_service->service_getNumServices(fcc[j]);
      DBG("[%s] got %d service(s)\n", cc, n);
      for (s = g_service->service_enumService(fcc[j], i=0);
           s;
           s = g_service->service_enumService(fcc[j], ++i)) {
        DBG("[%s] #%02d SVC factory  '%s'\n",
            cc, i, s ? s->getServiceName() : "(nil)");
      }
    }

    s = g_service->service_getServiceByGuid(languageApiGUID);
    if (s) {
      DBG("service factory lang %p '%s'\n",
          s, s?s->getServiceName():"(nil)");
    } else {
      DBG("don't have service factory lang\n");
    }

    s = g_service->service_getServiceByGuid(memMgrApiServiceGuid);
    if (s) {
      DBG("service factory memman %p '%s'\n",
          s, s?s->getServiceName():"(nil)");
    } else {
      DBG("don't have service memman lang\n");
    }
    //if (sf) WASABI_API_LNG = reinterpret_cast<api_language*>(sf->getInterface());

  }
#endif
  DBG("init completed\n");
}

static
/*******************************************************************************
 * PLUGIN SHUTDOWN
 ******************************************************************************/
void quit()
{
  wasc68_t * wasc68;

  DBG("\n");
  wasc68 = lock();
  sc68_cntl(0,SC68_CONFIG_SAVE);
  unlock(wasc68);
  CloseHandle(g_wasc68.lock);
  g_wasc68.lock = 0;
  memset(&g_wasc68,0,sizeof(g_wasc68));
  wasc68_cache_kill();
  msg68_cat_free(wasc68_cat);
  wasc68_cat = msg68_NEVER;
  sc68_shutdown();

}

static int xinfo(const char *data, char *dest, size_t destlen,
                 sc68_t * sc68, sc68_disk_t disk)
{
  sc68_music_info_t tmpmi, * const mi = &tmpmi;
  const char * value = 0;

  if (!strcasecmp(data, "type")) {
    /* "TYPE" is an important value as it tells Winamp if this is an
     * audio or video format */
    value = "0";
  }
  else if (!strcasecmp(data,"family")) {
    value = "sc68 audio files";
  /* Get the info for the default track only. */
  }
  else if (sc68_music_info(sc68, mi,
                           sc68 ? SC68_CUR_TRACK : SC68_DEF_TRACK, disk)) {
  }
  else if (!strcasecmp(data,"album")) { /* Album name */
    value = mi->album;
  }
  else if (!strcasecmp(data,"title")) { /* Song title */
    value = mi->title;
  }
  else if (!strcasecmp(data,"artist")) { /* Song artist */
    value = get_tag(&mi->trk,"aka");
    if (!value)
      value = mi->artist;
  }
  else if (!strcasecmp(data,"track")) {
    value = "01";
    /* snprintf(dest, destlen, "%02d", track); */
  }
  else if (!strcasecmp(data,"albumartist")) {
    value = get_tag(&mi->dsk,"aka");
    if (!value) value = get_tag(&mi->dsk,"artist");
    if (!value) value = mi->artist;
  }
  else if (!strcasecmp(data,"composer")) {
    value = get_tag(&mi->trk,"original");
    if (!value) value = get_tag(&mi->trk,"composer");
    if (!value) value = get_tag(&mi->dsk,"original");
    if (!value) value = get_tag(&mi->dsk,"composer");
  }
  else if (!strcasecmp(data,"track")) {
  /*   snprintf(dest, destlen, "%02d", ti->track); */
  } else if (!strcasecmp(data,"genre")) {
    value = mi->genre;
  }
  else if (!strcasecmp(data,"length")) {
    /* length in ms */
    /* $$$ Right now disk length but that might change in the future */
    snprintf(dest, destlen, "%u", mi->dsk.time_ms);
    value = dest;
  }
  else if (!strcasecmp(data,"year")) {
    value = get_tag(&mi->dsk,"year");
    if (!value)
      value = get_tag(&mi->trk,"year");
  }
  else if (!strcasecmp(data,"publisher")) {
    value = get_tag(&mi->trk,"converter");
    if (!value) value = get_tag(&mi->dsk,"converter");
    value = get_tag(&mi->trk,"converter");
  }
  else if (!strcasecmp(data,"streamtype")) {
  }
  else if (!strcasecmp(data, "lossless")) {
    value = "1";
  }
  else if (!strcasecmp(data,"")) {
    /* Other interresting tag we might handle someday: */
    /* "rating" "albumartist" "year" "publisher" "comment" "lossless"
     * "bpm" */
    DBG("unhandled TAG '%s'\n", data);
  }

  if (!value)
    return 0;

  if (value != dest)
    strncpy(dest, value, destlen);
  dest[destlen-1] = 0;
  /* dbg("wasc68::%s TAG '%s' -> '%s'\n", __FUNCTION__, data, dest); */
  return 1;
}


/**
 * Provides the extended meta tag support of winamp.
 *
 * @param  uri   URI or file to get info from (0 or empty for playing)
 * @param  data  Tag name
 * @param  dest  Buffer for tag value
 * @param  max   Size of dest buffer
 *
 * @retval 1 tag handled
 * @retval 0 unsupported tag
 */
EXPORT
int winampGetExtendedFileInfo(const char *uri,
                              const char *data,
                              char *dest, size_t max)
{
  sc68_disk_t disk;
  int res = 0;

  if (data && *data && dest && max > 2) {
    if (!uri || !*uri) {
      wasc68_t * wasc68;
      if (wasc68 = lock(), wasc68) {
        res = xinfo(data, dest, max, wasc68->sc68, 0);
        unlock(wasc68);
      }
    } else if (disk = wasc68_cache_get(uri), disk) {
      res = xinfo(data, dest, max, 0, disk);
      wasc68_cache_release(disk, 0);
    }
  }
  return res;
}
