/*
 *                     debug68 - sc68 debugger
 *
 *            Copyright (C) 1998-2009 Ben(jamin) Gerard
 *           <benjihan -4t- users.sourceforge -d0t- net>
 *
 * This  program is  free  software: you  can  redistribute it  and/or
 * modify  it under the  terms of  the GNU  General Public  License as
 * published by the Free Software  Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 *
 * You should have  received a copy of the  GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id: sc68.c 57 2009-01-31 18:24:54Z benjihan $
 */

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>

#include "sc68/sc68.h"
#include "sc68/error68.h"
#include "sc68/file68.h"
#include "sc68/alloc68.h"
#include "sc68/istream68.h"
#include "sc68/istream68_file.h"
#include "sc68/string68.h"
#include "sc68/rsc68.h"
#include "sc68/init68.h"
#include "sc68/msg68.h"

#include "debug68_error.h"
#include "debug68_cli.h"
#include "debug68_debugcom.h"
#include "debug68_io.h"

#include "emu68/struct68.h"
#include "emu68/emu68.h"
#include "emu68/ioplug68.h"
#include "emu68/mem68.h"
#include "desa68.h"




typedef struct option my_option_t;

/* sc68 */
void sc68_emulators(sc68_t * sc68, emu68_t  ** p_emu68, io68_t  *** p_ios68);
sc68_t   * sc68;
emu68_t  * emu68;
io68_t  ** ios68;

/* Global */
static char *lastload_fname=0;      /* filename used by loaddisk    */
static int quit;                    /* trigger quit at next command */
static int quit_code = 0;           /* exit code                    */
static int debugmode = 0;           /* running the debugger         */
static const char * lastcom = NULL; /* current running command      */

#if 0
/* Replay */
static u8   * replay        = NULL;   /* replay buffer              */
static int    replaysz;               /* replay size                */
#endif

static char * replayname    = NULL;   /* replay name                */
static char * forced_replay = NULL;   /* override predefined replay */


/* Disk & Music */
static disk68_t  * disk = NULL;
static music68_t * mus  = NULL;
static int track=-1;
static u32 time_ms=(u32)-1,memlow=(u32)-1,memhi=(u32)-1,frames=(u32)-1;
static int forced_a0=0, a0=0, playaddr;
static unsigned int cycleperpass;


/* Debug */
#ifdef DEBUG
# define def_VERB msg68_DEBUG
#else
# define def_VERB msg68_WARNING
#endif

#ifndef DEBUG68_O
# define DEBUG68_O 0
#endif

int debug68_feature = msg68_DEFAULT;

void dmsg_va(const char * fmt, va_list list)
{
  msg68_va(debug68_feature, fmt, list);
}

void dmsg(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  dmsg_va(fmt, list);
  va_end(list);
}

void warn_va(const char * fmt, va_list list)
{
  msg68_va(msg68_WARNING, fmt, list);
}

void warn(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  warn_va(fmt, list);
  va_end(list);
}

int derr_va(int code, const char * fmt, va_list list)
{
  msg68_error("%s: ", lastcom ? lastcom : "debug68");
  error68_va(fmt, list);
  return code;
}

int derr(int code, const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  code = derr_va(code, fmt, list);
  va_end(list);
  return code;
}

void dinf_va(const char * fmt, va_list list)
{
  msg68_va(msg68_INFO, fmt, list);
}

void dinf(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_INFO, fmt, list);
  va_end(list);
}

static void
sc68_debug_cb(const int bit, void *data, const char *fmt, va_list list)
{
  /* select output: always error output except for INFO messages */
  FILE * out = bit == msg68_INFO
    ? stdout
    : stderr
    ;
  vfprintf(out,fmt,list);
}

typedef int (command_fct_t)(int,char**);

typedef struct
{
  char *com,                 /* command name                        */
  *altcom,                   /* alternate command name ( shortcut ) */
  *use,                      /* usage strng                         */
  *help;                     /* help command string                 */
  command_fct_t *fnct;
} command_t;

/* Any declarations  */
static int settrack(int newtrack);
static int setreplay(char *newname);

/* Command fonctions declarations  */
static command_fct_t loaddisk_com, mergedisk_com, savedisk_com,
diskinfo_com, settrack_com, trackinfo_com, exit_com,
help_com, settime_com, findtime_com, debug_com, capitalize_com,
replay_com, author_com, composer_com, name_com, deltrack_com,
frq_com, loaddata_com, savedata_com, dup_com, address_com;

static command_t commands[] = {
  {
    "exit","x",NULL,
    "Exit debug68",exit_com
  },
  {
    "help","?","[command]",
    "Display command(s) help message",help_com
  },
  {
    "loaddisk","ld","disk-file-name",
    "Load a new SC68 file as current disk",loaddisk_com
  },
  {
    "mergedisk","md","disk-file-name",
    "Add a new SC68 file to current disk",
    mergedisk_com
  },
  {
    "savedisk","sd","[disk-file-name] [gzip-level]",
    "Save current disk with current track as default disk track,"
    " if no filename use last loaded disk",savedisk_com
  },
  {
    "diskinfo","di",NULL,
    "Display current disk info",diskinfo_com
  },
  {
    "settrack","t=","#",
    "Set current track number",
    settrack_com
  },
  {
    "trackinfo","ti","[#] default:current-track",
    "Display track info",trackinfo_com
  },
  {
    "settime","st","[ALL|* mm:sec] | [[mm:sec] [ [track mm:sec] ... ]]",
    "Set track(s) time",
    settime_com
  },
  {
    "findtime","ft","[ALL|* [mm:sec]] | [[mm:sec] track] ...]",
    "Search track(s) loop time within given duration",
    findtime_com
  },
  {
    "debug","db",NULL,
    "Enter debugger mode",debug_com
  },
  {
    "capitalize","cap","<ALL|*>",
    "Capitalize Disk,Title,Author",capitalize_com
  },
  {
    "replay","r=","[ALL|Track-list]  <NONE|NAME>",
    "Change external replay",replay_com
  },
  {
    "name","n=","[ALL|Track-list]  <NONE|NAME>",
    "Change disk or track name",name_com
  },
  {
    "author","a=","[Track-list]  <NONE|NAME>",
    "Change track author name",
    author_com
  },
  {
    "composer","c=","[Track-list]  <NONE|NAME>",
    "Change track composer name",composer_com
  },
  {
    "del","-","track-nmber",
    "Remove track from disk",deltrack_com
  },
  {
    "frq","f=","[track-list] replay-frq(hz)",
    "Change replay rate",frq_com
  },
  {
    "loaddata","ldat","data-file-name -d0=# -a0=hex -f=# -[AaYySs]",
    "Load a data file with track parms", loaddata_com
  },
  {
    "savedata","sdat","[track] data-file-name",
    "Save track data into file", savedata_com
  },
  {
    "dup","+","track",
    "Copy a track, set d0 to new track number", dup_com
  },
  {
    "address","a0=","[track] -|hexa-value",
    "Set load address (- is default)", address_com
  },
  {NULL}
};


static int print_usage(void)
{
  puts
    ("Usage: debug68 [OPTION] URI...\n"
     "\n"
     "  SC68 file toolbox - create and debug SC68 file\n"
     "\n"
     "Options:\n"
     "  -h --help           Display this message and exit\n"
     "  -V --version        Display sc68 version x.y.z and licence and exit\n"
     "  -v --verbose        Increase verbosity level\n"
     "  -q --quiet          Decrease verbosity level\n"
     "\n"
     "Copyright (C) 1998-2009 Benjamin Gerard\n"
     "Visit <" PACKAGE_URL ">\n"
     "Report bugs to <" PACKAGE_BUGREPORT ">");
  return 1;
}

/* Display version number. */
static int print_version(void)
{
  puts
    (PACKAGE_STRING "\n"
     "\n"
     "Copyright (C) 1998-2009 Benjamin Gerard.\n"
     "License GPLv3+ or later <http://gnu.org/licenses/gpl.html>\n"
     "This is free software: you are free to change and redistribute it.\n"
     "There is NO WARRANTY, to the extent permitted by law.\n"
     "\n"
     "Written by Benjamin Gerard <" PACKAGE_BUGREPORT ">"
      );
  return 0;
}

/* string to Hexa number */
int atox(const char *s)
{
  int r;
  if (!s) {
    return 0;
  }
  for (r=0; *s; s++) {
    if (*s>='0' && *s<='9') {
      r = r*16 + *s-'0';
    } else if (*s>='a' && *s<='f') {
      r = r*16 + *s-'a'+10;
    } else if (*s>='A' && *s<='F') {
      r = r*16 + *s-'A'+10;
    } else {
      return r;
    }
  }
  return r;
}

static unsigned int ms_to_sec(unsigned int ms)
{
  return (ms+999u) / 1000u;
}

static const unsigned int atarist_clock = (8010613u&~3u);

static unsigned int frames_to_ms(unsigned int frames, unsigned int cpf)
{
  u64 ms;

  ms =  frames; /* total frames. */
  ms *= cpf;    /* total cycles. */
  ms *= 1000;
  ms /= atarist_clock;

  return (unsigned int) ms;
}

static unsigned int ms_to_frames(unsigned int ms, unsigned int cpf)
{
  u64 fr;

  fr =  ms;    /* total ms */
  fr *= atarist_clock;
  fr /= 1000u; /* total cycles */
  fr /= cpf;

  return (unsigned int) fr;
}

static unsigned int cycle_per_frame(unsigned int frq)
{
  if (!frq) {
    frq = 50;
  }
  if (frq == 50) {
    return 160256;
  } else if (frq == 60) {
    /* $$$ Can't remember the exact number of cycle for 60Hz VBL */
  }
  return atarist_clock / frq;
}

/* /\* load external replay file *\/ */
/* static u8 * load_external_replay(istream68_t * is, int *psize) */
/* { */
/*   int size; */
/*   u8 *buf = 0; */
/*   const char * reason = NULL; */
/*   const char * fname  = istream68_filename(is); */

/*   size = istream68_length(is); */

/*   dmsg("load_external_replay: '%s' %d bytes\n", fname, size); */
/*   if (size == -1) { */
/*     reason = "can't stat file size"; */
/*     goto error; */
/*   } */

/*   buf = alloc68(size); */
/*   if (!buf) { */
/*     reason = "memory allocation failure"; */
/*     goto error; */
/*   } */

/*   if (istream68_read(is, buf, size) != size) { */
/*     reason = "read error"; */
/*     goto error; */
/*   } */

/*   dmsg("load_external_replay: '%s' loaded\n", fname); */
/*   goto ok; */
/* error: */
/*   size = 0; */
/*   free68(buf); */
/*   buf = 0; */
/*   msg68_error("load_external_replay: '%s' (%s)\n", fname, reason); */
/* ok: */
/*   *psize = size; */
/*   return buf; */
/* } */

static char *check_a_name(char *name)
{
  int i;
  static char *list[][2] =
    {
      { "Max", "Jochen Hippel (Mad Max)" },
      { "Mad Max", "Jochen Hippel (Mad Max)" },

      { "Count Zero", "Nic Alderton (Count Zero)" },
      { "CZ", "Nic Alderton (Count Zero)" },
      { "Nick Alderton (Count Zero)", "Nic Alderton (Count Zero)" },
      { "C.Z.", "Nic Alderton (Count Zero)" },
      { "C.Z", "Nic Alderton (Count Zero)" },

      { "Big Alec", "Gunnar Gaubatz (Big Alec)" },
      { "B.A.", "Gunnar Gaubatz (Big Alec)" },
      { "B.A", "Gunnar Gaubatz (Big Alec)" },
      { "G. Gaubatz (Big Alec)", "Gunnar Gaubatz (Big Alec)" },

      { "Lap", "Emmanuel Larry (Lap)" },

      { "Docland", "Cyril Payet (Doclands)" },
      { "Doclands", "Cyril Payet (Doclands)" },
      { "Cyril Payet (Docland)", "Cyril Payet (Doclands)" },

      { "Jess", "J.S. Gerard (Jess)" },

      { "Lotus", "Laurens Tummers (Lotus)" },

      { "JMP","John M. Phillips" },
      { "J.M.P","John M. Phillips" },
      { "J.M.P.","John M. Phillips" },


      { 0, 0 }
    };

  if (name) {
    for(i=0; list[i][0]; i++) {
      if (!strcmp68(name,list[i][0])) {
        name = list[i][1];
        break;
      }
    }
  }
  return name;
}

/* Some stupid checks */
static int checknames(void)
{
  int i;
  if (!disk) {
    return 0;
  }
  for (i=0; i<disk->nb_six; i++) {
    disk->mus[i].author   = check_a_name(disk->mus[i].author);
    disk->mus[i].composer = check_a_name(disk->mus[i].composer);
  }
  return 0;
}

static int checkdisk(void)
{
  int err = !disk
    ? derr(-1, "no disk loaded")
    : 0
    ;
  return err;
}

static int checktrack(int track)
{
  for (;;) {
    if (checkdisk() < 0) {
      track = -1;
      break;
    }
    if (!disk->nb_six) {
      track = derr(-1,"disk has no track");
      break;
    }

    if (track < 0 || track >= disk->nb_six) {
      track = derr(-1,"file corrupted; track #%d out of range [0..%d]",
                   track, disk->nb_six-1);
      break;
    }
    if (disk->default_six < 0) {
      disk->default_six = 0;
    } else if (disk->default_six >= disk->nb_six) {
      disk->default_six = disk->nb_six-1;
    }
    break;
  }
  return track;
}

/* Display various track info depending on level
 *  level 1 : track No, music name
 *  level 2 : author, composer
 *  level 4 : IO-chip, replay rate , cycles/pass
 *  level 8 : Load address, External replay
 */
static int trackinfo(int trk, int infolevel)
{
  int err;
  music68_t *mus;

  if (err = checktrack(trk), err < 0) {
    return err;
  }
  if (err != trk) {
    return derr(-1,"track #%d out of range", trk+1);
  }
  mus = disk->mus + trk;

  /* level 1 : time, music name */
  if (infolevel & 1) {
    dinf("%c%-8s %-40s",
         (trk==track) ? '*' : ' ',
         strtime68(0, trk+1, mus->time_ms/1000),
         mus->name);
  }

  /* level 2 : authors, composer */
  if (infolevel & 2) {
    if (!strcmp68(mus->author,mus->composer)) {
      dinf(" (%s)",mus->author);
    } else {
      dinf(" (%s/%s)",mus->author,mus->composer);
    }
  }

  if (infolevel & 3) {
    dinf("\n");
  }

  /* level 4 : IO-chip, replay rate , cycles/pass */
  if (infolevel&4) {
    if (mus->hwflags.bit.ym) {
      dinf("  YM-2149 (Atari STF/E sound chip) required\n");
    }
    if (mus->hwflags.bit.ste) {
      dinf("  MicroWire (Atari STE sound chip) required\n");
    }
    if (mus->hwflags.bit.amiga) {
      dinf("  Paula (Amiga 1000/500/1200... sound chip) required\n");
    }
    dinf("  replay rate: %d Hz (%u cpf)\n",
         mus->frq, cycle_per_frame(mus->frq));
  }

  /* level 8 : Load address, External replay */
  if (infolevel&8) {
    if (!mus->replay) {
      if (mus->a0 != SC68_LOADADDR) {
        dinf("  Absolute position built-in replay @ $%X-%X (%d bytes)\n",
             mus->a0,mus->a0+mus->datasz-1,mus->datasz);
      } else {
        dinf("  Relocatable built-in replay (%d bytes)\n",
             mus->datasz);
      }
    } else {
      if (mus->a0 != SC68_LOADADDR) {
        dinf("  Absolute external replay <%s> @ $%X-%X (%d bytes)\n",
             mus->replay,
             mus->a0,mus->a0+mus->datasz-1,mus->datasz);
      } else {
        dinf("  Relocatable external replay <%s> (%d bytes)\n",
             mus->replay,
             mus->datasz);
      }
    }
  }
  return 0;
}

/************
 * DISK INFO *
 ************/
static int diskinfo(void)
{
  int err;
  int i;

  if (err=checkdisk(), err < 0) {
    return err;
  }

  dinf("\nDISK <%s>\n",disk->name);
  for (i = 0; i < disk->nb_six; i++) {
    if (err = trackinfo(i, 1 + 2), err < 0) {
      return err;
    }
  }
  dinf(" -- -----\n %s\n",
       strtime68(0, disk->nb_six, ms_to_sec(disk->time_ms)));
  return 0;
}

/*************
 * KILL DISK *
 *************/
static int killdisk(void)
{
  /* Clear 68K memory */
  emu68_memset(emu68,0,0,0);
  emu68_chkset(emu68,0,0,0);

  if (emu68) {
    if (emu68->mem) {
      dmsg("killdisk: clear 68K memory\n");
      memset(emu68->mem,0,emu68->memmsk+1);
    }
    if (emu68->chk) {
      dmsg("killdisk: clear 68K memory\n");
      memset(emu68->chk,0,emu68->memmsk+1);
    }
  }

  /* Reset replay buffer */
/*   dmsg("killdisk: free replay\n"); */
/*   free68(replay); */
/*   replay=0; */
/*   replaysz = 0; */
  free68(replayname);
  replayname = 0;
  forced_replay = 0;

  /* Free mem & CLear pointers */
  if (disk) {
    int i;
    dmsg("killdisk: free disk\n");

    free68(disk->name);
    for (i=0; i<disk->nb_six; ++i) {
      free68(disk->mus[i].replay);
      free68(disk->mus[i].name);
      free68(disk->mus[i].author);
      free68(disk->mus[i].composer);
      memset(disk->mus+i,0,sizeof(disk->mus[i]));
    }
    free68(disk);
    disk = 0;
  }
  mus = 0;
  track = -1;
  free68(lastload_fname);
  lastload_fname = 0;

  return 0;
}

/* compute disk total time */
static void totaltime(void)
{
  if (disk) {
    int i;
    for (i=0, disk->time_ms=0; i<disk->nb_six; ++i) {
      disk->time_ms += disk->mus[i].time_ms;
    }
    dmsg("totaltime: %ums for %d track\n", disk->time_ms, disk->nb_six);
  } else {
    dmsg("totaltime: no disk\n");
  }
}


/******************
 * LOAD A NEW DISK *
 ******************/
static int loaddisk(char *fname, int mergemode)
{
  disk68_t *newdisk = 0;

  dmsg("loaddisk: %s '%s'\n",
       mergemode ? "merging" : "loading", fname);

  if (!fname) {
    return derr(-1, "no disk to load");
  }

  /* Desactif merge if no disk */
  mergemode &= -( disk != NULL );

  /* Load SC68 file */
  if (newdisk = file68_load_url(fname), newdisk == NULL) {
    return derr(-1, "failed to load '%s'");
  }

  /* If not merge mode : Free previous disk */
  if (!mergemode) {
    killdisk();
    disk = newdisk;
    lastload_fname = strdup68(fname);
  } else {
    int i,j,nb = disk->nb_six + newdisk->nb_six;

    /* Clear 68K memory */
    memset(emu68->mem,0,emu68->memmsk+1);
    memset(emu68->chk,0,emu68->memmsk+1);

    if (nb > SC68_MAX_TRACK) {
      warn("too many musix; skipping %d track(s)\n",nb-SC68_MAX_TRACK);
      nb = SC68_MAX_TRACK;
    }
    for (i=disk->nb_six, j=0; i<nb; i++, j++) {
      disk->mus[i] = newdisk->mus[j];
      disk->nb_six = nb;
    }
    /* $$$ MEMORY LEAK HERE , can't free newdisk (in used) $$$ */
  }

  /* Auto transform very known names !!! */
  checknames();

  /* Set current track */
  if (settrack(disk->default_six)<0) {
    return derr(-1,"File corrupted : Default track out of disk bound.");
  }
  totaltime();

  return 0;
}

static int loaddata(char *fname, int d0, int a0, int frq, int flags,
                    char *mname, char *aname, char *rname)
{
  disk68_t * newdisk;
  istream68_t * is;
  int  flen,size;

  if (!fname) {
    return derr(-1,"NULL pointer as data file !!");
  }

  is = istream68_file_create(fname,1);
  if (!is) {
    return derr(-1,"Can't load data file : see previous");
  }
  if (istream68_open(is)) {
    istream68_destroy(is);
    return derr(-1,"Can't load data file : see previous");
  }
  flen = istream68_length(is);
  if (flen == -1) {
    istream68_destroy(is);
    return derr(-1,"Can't load data file : see previous");
  }

  printf("Load data file : %s (%d bytes)\n"
         " d0=%d a0=$%x frq=%d flags=%c%c%c (%x)\n",
         fname, flen, d0, a0, frq,
         'y'-((!!(flags&SC68_YM))<<5),
         's'-((!!(flags&SC68_STE))<<5),
         'a'-((!!(flags&SC68_AMIGA))<<5),
         flags);

  size = flen + sizeof(*newdisk);
  if (newdisk=(disk68_t *)alloc68(size), !newdisk) {
    istream68_destroy(is);
    return derr(-1,"Can't load data file : see previous");
  }
  memset(newdisk,0,size);

  if (istream68_read(is, newdisk->data, flen) != flen) {
    free68(newdisk);
    istream68_destroy(is);
    return derr(-1,"Can't load data file : see previous");
  }
  istream68_destroy(is);

  killdisk();
  disk = newdisk;

  /* At least 1 track for a valid disk */
  disk->nb_six    = 1;
  disk->mus[0].d0 = d0;
  disk->mus[0].a0 = a0;
  disk->mus[0].frq = frq;
  disk->mus[0].hwflags.all = 0;
  disk->mus[0].hwflags.bit.ym = !!(flags&SC68_YM);
  disk->mus[0].hwflags.bit.ste = !!(flags&SC68_STE);
  disk->mus[0].hwflags.bit.amiga = !!(flags&SC68_AMIGA);
  disk->mus[0].data = disk->data;
  disk->mus[0].datasz = flen;

  /* Name settup */
  disk->name = strdup68(mname);
  disk->mus[0].replay = strdup68(rname);
  disk->mus[0].author = strdup68(aname);
  disk->mus[0].composer = strdup68(disk->mus[0].author);

  /* Set current track */
  if (settrack(disk->default_six) < 0) {
    return derr(-1,"File corrupted : Default track out of disk bound.");
  }

  return 0;
}

/********************
 * SAVE CURRENT DISK *
 ********************/
static int savedisk(char *fname, int gzlevel)
{
  int err;
  if (err=checkdisk(), err<0) {
    return err;
  }
  if (!fname) {
    return derr(-1,"Missing filename");
  }
  if (err = checktrack(disk->default_six = track), err<0) {
    return err;
  }
  if (err=file68_save_url(fname, disk, gzlevel), !err) {
    printf("%s saved [gzip level:%d]\n",fname, gzlevel);
  }
  return err;
}

/* Kill previous external replay.
 * Load new one ( NULL is none ).
 */
static int setreplay(char *newname)
{
/*   istream68_t * is; */
  char * tmpname;

  /* Kill previous replay */
  free68(replayname);
  replayname = 0;

/*   free68(replay); */
/*   replay = 0; */
/*   replaysz = 0; */

  if (!newname) {
    return 0;
  }
  tmpname = strdup68(newname);
  if (!tmpname) {
    return derr(-1,"could not alloc replay name <%s>", newname);
  }

/*   is = rsc68_open(rsc68_replay, newname, 1, 0); */
/*   if (!is) { */
/*     free68(tmpname); */
/*     return derr(-1,"Could not find external replay <%s>", newname); */
/*   } */

/*   replay = load_external_replay(is, &replaysz); */
/*   istream68_destroy(is); */
/*   if (!replay) { */
/*     free68(tmpname); */
/*     derr(-1,"Failed to load external replay <%s>", newname); */
/*     return -1; */
/*   } */
  replayname = tmpname;
  return 0;
}

/* Set current track. */
static int settrack(int newtrack)
{
  int err;
  char *thisreplay;

  if (err=checktrack(newtrack), err<0) {
    return err;
  }

  track    = newtrack;
  mus      = disk->mus+track;
  playaddr = a0 = forced_a0 ? forced_a0 : mus->a0;
  time_ms  = mus->time_ms;
  memlow   = (u32)-1;
  memhi    = (u32)-1;
  if (mus->frq==0) mus->frq = 50;
  cycleperpass = cycle_per_frame(mus->frq);
  thisreplay = (forced_replay) ? forced_replay : mus->replay ;
  if (err=setreplay(thisreplay), err<0) {
    return derr(-1,"Failed loading track %d external replay <%s>\n",
                             track,thisreplay);
  }
  return 0;
}

static inline const char * ok_int(const int const err) {
  return !err ? "success" : "failure";
}

#if 0
/*  68K First time init;
 */
static int init68K(int size)
{
  int err;
  emu68_init_t emu68_parms;
  /*   io68_init_t io68_parms; */

  sc68_debug(sc68,"init_emu68() {\n");

  /* Initialize emu68 */
  sc68_debug(sc68,"init_emu68() : Initialize emu68 (68k emulator)\n");
  emu68_parms.alloc = sc68_alloc;
  emu68_parms.free  = sc68_free;
  err = emu68_init(&emu68_parms);
  sc68_debug(sc68,"init_emu68() : emu68 library init %s\n", ok_int(err));
  if (err) {
    goto error;
  }

  /* Initialize chipset */
  /*   sc68_debug(sc68,"init_emu68() : Initialise io68 (chipset)\n"); */
  /*   memset(&io68_parms, 0, sizeof(io68_parms)); */
  /*   err = io68_init(&io68_parms); */
  /*   sc68_debug(sc68,"init_emu68() : io68 library init %s\n", ok_int(err)); */

error:
  sc68_debug(sc68,"} init_emu68() => [%s]\n", ok_int(err));
  return err;
}
#endif


/* Set music time ( if newtrack out of bound , use default )
 */
static int settime(int newtrack, unsigned newtime_ms, unsigned newframe)
{
  unsigned newtime;

  if (newtrack=checktrack(newtrack), newtrack<0)
    return newtrack;

  newtime = ms_to_sec(newtime_ms);
  printf("Set track #%02d time to %02d:%02d (%d frames)\n",
         newtrack+1,newtime/60,newtime%60, newframe);
  disk->mus[newtrack].frames  = newframe;
  disk->mus[newtrack].time_ms = newtime_ms;

  /* Rebuild total */
  totaltime();
  return 0;
}

#if 0
static int prepare_reg68(void)
{
  int err;
  addr68_t A0=a0;

  if (err = checktrack(track), err < 0) {
    return err;
  }

  dinf("Prepare 68000:\n"
       " - Disk [%s]\n"
       " - Track #%02d [%s]\n"
       " - Replay [%s]\n"
       " - Frq:%uHz Cpf:%u\n"
       , disk->name, track+1, mus->name,
       replayname ? replayname : "built-in",
       mus->frq, cycleperpass);

  /* Must be call because of some _DEBUG test */
  emu68_reset(emu68);

  /* Clear 68 K mem */
  emu68_memset(emu68, 0, 0, 0);
  emu68_poke(emu68,0,0x4e);
  emu68_poke(emu68,1,0x73);

  if (setreplay(mus->replay) < 0) {
    return derr(-1,"Failed loading external replay <%s>\n",mus->replay);
  }

  if (replay) {
    u32 data;
    if (emu68_memput(emu68,A0,replay,replaysz)<0) {
      derr(emu68_error_get());
      return
        derr(-1,"Failed copying external replay into 68K memory");
    }
    printf(" - External replay <%s> loaded @$%X-%X\n",
           replayname, A0, A0+replaysz-1);
    data = A0 + ((replaysz+1)&-2);
    /* Check memory overflow */

    if (EMU68_memvalid(data,mus->datasz+1024)) {
      EMU68error_get(); /* Pop error message !!! */
      data = A0 - ((mus->datasz+1025)&-2);
      if (data<0x1000) {
        return derr(-1,"No room for copying data into 68K memory");
      }
    }
    A0 = data;
  }

  /* Copy Data into 68K memory */
  if (EMU68_memput(A0,(u8*)mus->data,mus->datasz)) {
    return derr(EMU68error_get());
  }

  /* $$$ NOTE: Absolute/Indepenadnt test is broken */
  printf(" - %s position %s loaded @$%X-%X\n",
         (a0 == SC68_LOADADDR) ? "Independant" : "Absolute",
         replay ? "music data" : " music replay and data",
         A0,A0+mus->datasz-1);

  /* 68K registers init */
  REG68.pc   = playaddr;
  REG68.sr   = 0x2300;
  REG68.d[0] = mus->d0;
  REG68.d[1] = !mus->hwflags.bit.ste;
  REG68.d[2] = mus->datasz;
  REG68.a[0] = A0;
  REG68.a[7] = REG68.memsz-16;
  REG68.cycle = 0;
  memset(REG68.chk,0,REG68.memsz);

  printf("68000 Ready!\n");



#if 0
  u32 A0=a0;
  int err;
  io68_t *io;
  int i;

  printf("Prepare 68000:\n"
         " - Disk [%s]\n"
         " - Track #%02d [%s]\n"
         " - Replay [%s]\n"
         " - Frq:%uHz Cpf:%u\n"
         , disk->name, track+1, mus->name,
         replayname ? replayname : "built-in",
         mus->frq, cycleperpass);

  /* Must be call because of some _DEBUG test */
  emu68_reset(emu68);

  /* Clear 68 K mem */
  emu68_memset(emu68, 0, 0, 0);

  emu68_poke(emu68,0,0x4e);
  emu68_poke(emu68,1,0x73);
  emu68_poke(emu68,0x41a,0);         /* Zound Dragger */
  emu68_poke(emu68,0x41b,0x10);      /* Zound Dragger */
  emu68_poke(emu68,TRAP_VECTOR(14)+0,(u8) (TRAP_14_ADDR >> 24));
  emu68_poke(emu68,TRAP_VECTOR(14)+1,(u8) (TRAP_14_ADDR >> 16));
  emu68_poke(emu68,TRAP_VECTOR(14)+2,(u8) (TRAP_14_ADDR >>  8));
  emu68_poke(emu68,TRAP_VECTOR(14)+3,(u8) (TRAP_14_ADDR));
  emu68_memput(emu68, TRAP_14_ADDR, trap14, sizeof(trap14));

  /*$$$*/
  /*   EMU68ioplug(&paula_io); */
  /*   EMU68ioplug(&ym_io); */
  /*   EMU68ioplug(&mw_io); */
  /*   EMU68ioplug(&sh_io); */

  /* Display IO list */
  /*   for(i=0, io=REG68.iohead; io; io=io->next, i++) { */
  /*     printf(" - io #%d/%d : '%s' %x-%x\n", */
  /*         i+1,REG68.nio,io->name,io->addr_low,io->addr_high); */
  /*   } */

  if (setreplay(mus->replay) < 0) {
    return derr(-1,"Failed loading external replay <%s>\n",mus->replay);
  }

  if (replay) {
    u32 data;
    if (emu68_memput(emu68,A0,replay,replaysz)<0) {
      derr(emu68_error_get());
      return
        derr(-1,"Failed copying external replay into 68K memory");
    }
    printf(" - External replay <%s> loaded @$%X-%X\n",
           replayname, A0, A0+replaysz-1);
    data = A0 + ((replaysz+1)&-2);
    /* Check memory overflow */

    if (EMU68_memvalid(data,mus->datasz+1024)) {
      EMU68error_get(); /* Pop error message !!! */
      data = A0 - ((mus->datasz+1025)&-2);
      if (data<0x1000) {
        return derr(-1,"No room for copying data into 68K memory");
      }
    }
    A0 = data;
  }

  /* Copy Data into 68K memory */
  if (EMU68_memput(A0,(u8*)mus->data,mus->datasz)) {
    return derr(EMU68error_get());
  }

  /* $$$ NOTE: Absolute/Indepenadnt test is broken */
  printf(" - %s position %s loaded @$%X-%X\n",
         (a0 == SC68_LOADADDR) ? "Independant" : "Absolute",
         replay ? "music data" : " music replay and data",
         A0,A0+mus->datasz-1);

  /* 68K registers init */
  REG68.pc   = playaddr;
  REG68.sr   = 0x2300;
  REG68.d[0] = mus->d0;
  REG68.d[1] = !mus->hwflags.bit.ste;
  REG68.d[2] = mus->datasz;
  REG68.a[0] = A0;
  REG68.a[7] = REG68.memsz-16;
  REG68.cycle = 0;
  memset(REG68.chk,0,REG68.memsz);

  printf("68000 Ready!\n");
#endif
  return 0;
}

#endif

/* Memory & Time checker */
static int find_time(uint68_t nbsec,
                     uint68_t *pt_frm,
                     uint68_t *pt_sec,
                     addr68_t *pt_memmin,
                     addr68_t *pt_memmax)
{
  int err;
  uint68_t lastframe, nbframe;
  uint68_t nb_ms;
  uint68_t min,max,i,pass;
  uint68_t maxsec = 60*60;

  if (err = sc68_open(sc68, disk), err < 0) {
    return err;
  }
  if (err = sc68_play(sc68, track, 0), err < 0) {
    return err;
  }
  if (err = sc68_process(sc68, 0, 0), err < 0) {
    return err;
  }

  /* default time: 3 minutes */
  if (!nbsec) {
    nbsec = 3*60;
  }
  nbframe = ms_to_frames(nbsec*1000, cycleperpass);

  lastframe=0;
  i=0;
  pass=0;

  do {
    int lf;
    ++pass;
    nbsec = ms_to_sec(frames_to_ms(nbframe,cycleperpass));
    if (nbsec >= maxsec) {
      warn("reach maximum time: aborting!\n");
      lastframe=0;
      break;
    }
    printf("Pass #%d: [%s] (%7u frames, %u cpf). Processing ... ", pass,
           strtime68(0,track+1, nbsec), nbframe-i, cycleperpass);
    fflush(stdout);
    /* Run emulation code for given time */
    emu68->framechk = 0;
    for(lf=0; i<nbframe; i++) {
      emu68_level_and_interrupt(emu68,cycleperpass);
      if (emu68->framechk) {
        lastframe = i;
        lf++;
      }
    }
    printf("[%7d], %u\n", lf, lastframe);
    if (!lf) {
      break;
    }
    nb_ms = frames_to_ms(lastframe,cycleperpass);
    nbsec = ms_to_sec(nb_ms);
    printf("Last frame: %u [%s]\n", lastframe,
           strtime68(0, -1, nbsec));
    nbframe *= 2;
  } while(1);

  nb_ms = frames_to_ms(lastframe, cycleperpass);
  nbsec = nb_ms/1000u;

  /* Retrieve memory access bound */
  min = emu68->memmsk+1-1024;
  max = 0;
  for(i=0x4000; i<emu68->memmsk+1-1024; i++) {
    if (emu68->chk[i]) {
      if (i<min) min=i;
      if (i>max) max=i;
    }
  }

  if (pt_memmin) *pt_memmin = min;
  if (pt_memmax) *pt_memmax = max;
  if (pt_frm)    *pt_frm = lastframe;
  if (pt_sec)    *pt_sec = nb_ms;

  printf("Last frame       : %u [%s]\n", lastframe, strtime68(0,-1, nbsec));
  printf("Memory hit       : [$%x-$%x] (%d bytes)\n", min,max,max-min);
  printf("Paula access     : %c%c\n",
         (pl_dio->access & 1) ? 'R' : '-',
         (pl_dio->access & 2) ? 'W' : '-');
  printf("YM-2149 access   : %c%c\n",
         (ym_dio->access & 1) ? 'R' : '-',
         (ym_dio->access & 2) ? 'W' : '-');
  printf("MicroWire access : %c%c\n",
         (mw_dio->access & 1) ? 'R' : '-',
         (mw_dio->access & 2) ? 'W' : '-');
  printf("shifter access   : %c%c\n",
         (sh_dio->access & 1) ? 'R' : '-',
         (sh_dio->access & 2) ? 'W' : '-');

  if (!(pl_dio->access|ym_dio->access|mw_dio->access|sh_dio->access)) {
    printf("No hardware access detected !!!\n");
  } else if (pl_dio->access &&
             (ym_dio->access|mw_dio->access|sh_dio->access)) {
    printf("Both Atari and Amiga hardware detected !!!\n");
  } else {
    int old;
    old = mus->hwflags.bit.amiga;
    mus->hwflags.bit.amiga = !!pl_dio->access;
    if (old != mus->hwflags.bit.amiga) {
      printf("%s PAULA hardware\n", old ? "CLEAR" : "FORCE");
    }

    old = mus->hwflags.bit.ym;
    mus->hwflags.bit.ym = !!(ym_dio->access|sh_dio->access);
    if (old != mus->hwflags.bit.ym) {
      printf("%s YM-2149 hardware\n", old ? "CLEAR" : "FORCE");
    }

    old = mus->hwflags.bit.ste;
    mus->hwflags.bit.ste = !!mw_dio->access;
    if (old != mus->hwflags.bit.ste) {
      printf("%s STE hardware\n", old ? "CLEAR" : "FORCE");
    }

  }

  return 0;
}

static char * prompt(void)
{
  static char tmp[1024];

  if (!debugmode) {
    if (!disk) {
      return "No Disk>";
    } else {
      sprintf(tmp, "%s (#%d)>",disk->name,track+1);
      return tmp;
    }
  }  else {
    if (!disk) {
      return "Nothing to debug ('x' to exit debug mode)>";
    } else {
      return debug68_prompt(disk->name,track+1);
    }
  }
}

/*
 *
 *
 *
 */
static int interactive(void)
{
  char **a;
  int na, err, i;
  debug68_cli_t cli;
  memset(&cli,0,sizeof(cli));

  while (!quit) {
    int hascom = debug68_cli_read(prompt(), &cli) > 0;
    if (!hascom && !debugmode) {
      /* Skip empty cli except for debugger mode */
      continue;
    }
    na = cli.argc;
    a  = cli.coms;

    for (i=0; i<na; ++i) {
      dmsg("%s: #%d '%s'\n",debugmode?"debugger":"interactive", i, a[i]);
    }

    /* Debugger mode */
    if (debugmode) {
      err = debug68_newcom(na,a);
      if (err = debug68_newcom(na,a), err < 0) {
      } else if (err > 0) {
        debugmode = 0;
      }
      continue;
    }

    /* Find prefixed test mode (conditionnal command execution) */
    switch (a[0][0]) {
    case '=':    case '!':    case '>':    case '<':
    {
      /* Perform test */
      char * s;
      int not = 0, res = 0, err = 0, c;

      for (s=a[0]; !err && ((c=*s++),c); ) {
        switch (c) {
        case '!': not = !not; break;
        case '=': res = res || (quit_code == 0); break;
        case '>': res = quit_code > 0; break;
        case '<': res = quit_code < 0; break;
        default:
          err = 1;
          derr(0, a[0], "syntax error in test operand");
        }
      }
      if (err || !(res ^ not) || --na <= 0) {
        continue;
      }
      a++;
    } break;
    }

    lastcom = a[0];
    for(i=0; commands[i].com; i++) {
      if ( ! strcmp68(commands[i].com,    a[0]) ||
          ! strcmp68(commands[i].altcom, a[0])) {
        dmsg("running command %s\n", commands[i].com);
        lastcom = commands[i].com;
        quit_code = (*commands[i].fnct)(na,a);
        dmsg("%s: -> [%d]\n", commands[i].com, quit_code);
        break;
      }
    }
    if (!commands[i].com) {
      quit_code = derr(-1, "unknown command");
    }
  }
  dmsg("%s: -> [%d]\n",debugmode?"debugger":"interactive", quit_code);

  return quit_code;
}

static int opt_vers, opt_help, opt_verb = def_VERB;

int main(int argc, char *argv[])
{
  int err = -1, i, j;
  sc68_init_t   init68;
  sc68_create_t create68;
  const my_option_t longopts[] = {
    {"help",       0, 0, 'h'},
    {"version",    0, 0, 'V'},
    {"version",    0, 0, 'V'},
    {"verbose",    0, 0, 'v'},
    {"quiet",      0, 0, 'q'},
    {0,0,0,0}
  };
  char shortopts[(sizeof(longopts)/sizeof(*longopts))*3];

  /* Build short options list from long */
  for (i=j=0; longopts[i].val; ++i) {
    shortopts[j++] = longopts[i].val;
    switch (longopts[i].has_arg) {
    case 2: shortopts[j++] = ':';
    case 1: shortopts[j++] = ':'; break;
    }
  }
  shortopts[j++] = 0;

  argv[0] = "debug68";

  /* Initialize sc68 api. */
  memset(&init68, 0, sizeof(init68));
  init68.argc = argc;
  init68.argv = argv;
  init68.msg_handler = sc68_debug_cb;
  init68.msg_cookie  = 0;

  if (sc68_init(&init68)) {
    return -1;
  }
  argc = init68.argc;
  argv = init68.argv;

  while (1) {
    int longindex;
    int val =
      getopt_long(argc, argv, shortopts, longopts, &longindex);

    switch (val) {
    case  -1: break;                /* Scan finish */
    case 'h': opt_help = 1; break;  /* --help      */
    case 'V': opt_vers = 1; break;  /* --version   */
    case 'v': opt_verb++;   break;  /* --verbose   */
    case 'q': opt_verb--;   break;  /* --quiet     */
    case '?':                       /* Unknown or missing parameter */
      goto error;
    default:
      msg68_error("%s: unexpected getopt return value (%d)\n", argv[0], val);
      goto error;
    }
    if (val == -1) break;
  };
  i = optind;

  if (opt_help) {
    return print_usage();
  }

  if (opt_vers) {
    return print_version();
  }

  if (opt_verb < msg68_CRITICAL) opt_verb = msg68_CRITICAL;
  if (opt_verb > msg68_TRACE)    opt_verb = msg68_TRACE;
  msg68_feature_level(opt_verb);
  debug68_feature = msg68_feature("debug68","sc68 toolbox",DEBUG68_O);
  if (debug68_feature == -1) debug68_feature = msg68_DEFAULT;

  memset(&create68,0,sizeof(create68));
  create68.log2mem     = 19;
  create68.emu68_debug = 1;
  sc68 = sc68_create(&create68);
  if (!sc68) {
    goto error;
  }
  sc68_emulators(sc68, &emu68, &ios68);
  dmsg("emulators: emu68 %p\n", emu68);
  dmsg("emulators: io68 %p '%s'\n", ios68[0], ios68[0]->name);
  dmsg("emulators: io68 %p '%s'\n", ios68[1], ios68[1]->name);
  dmsg("emulators: io68 %p '%s'\n", ios68[2], ios68[2]->name);
  dmsg("emulators: io68 %p '%s'\n", ios68[3], ios68[3]->name);
  dmsg("emulators: io68 %p '%s'\n", ios68[4], ios68[4]->name);

  for (; i<argc; ++i) {
    dmsg("cli: loaddisk '%s'\n", argv[i]);
    err = loaddisk(argv[i],1);
    if (err) goto error;
  }

  diskinfo();
  err = interactive();

error:
  sc68_destroy(sc68);
  sc68_shutdown();
  return err;
}


/* display single command help string */
static void dcom(command_t *com)
{
  printf("%s",com->com);
  if (com->altcom!=NULL) printf(" (%s)",com->altcom);
  if (com->help  !=NULL) printf(" : %s",com->help);
  if (com->use   !=NULL) printf("\n  usage: %s %s",com->com,com->use);
  printf("\n");
}

/* display help on given command */
static int help_on(char *name)
{
  int i;
  for (i=0; commands[i].com != NULL; i++) {
    if (!strcmp68(commands[i].com,name) ||
        !strcmp68(commands[i].altcom,name)) {
      dcom(commands+i);
      return 0;
    }
  }
  return -1;
}

static int help_com(int na, char **a)
{
  int i,j;

  if (na==1) {
    /* no args: display all commands help */
    for (i=0; commands[i].com; i++) {
      dcom(commands+i);
    }
  } else {
    /* display help on all given commands */
    for (j=1; j<na;j++) {
      if (help_on(a[j]) < 0) {
        msg68_error("%s: unknown command\n", a[j]);
      }
    }
  }
  return 0;
}

static int exit_com(int na, char **a)
{
  int code = quit_code;
  quit = 1;
  if (na > 1) {
    code = atoi(a[1]);
  }
  return code;
}

static int loaddisk_com(int na, char **a)
{
  if (na<2) {
    return help_on("ld");
  }
  return loaddisk(a[1],0) || diskinfo();
}

static int mergedisk_com(int na, char **a)
{
  if (na<2) {
    return help_on("md");
  }
  return loaddisk(a[1],1) || diskinfo();
}

static int savedisk_com(int na, char **a)
{
  int gz=0;

  if (na > 2) {
    int c = a[2][0];
    if (c>='0' && c<='9' && !a[2][1]) {
      gz = c - '0';
    } else {
      msg68_error("savedisk: bad compression level; must be in [0..9]\n");
      return -1;
    }
  }

  if (na < 2) {
    if (!lastload_fname || ! lastload_fname[0]) {
      return help_on("sd");
    } else {
      return savedisk(lastload_fname, gz);
    }
  }
  return savedisk(a[1], gz);
}

static int diskinfo_com(int na, char **a)
{
  return diskinfo();
}

static int settrack_com(int na, char **a)
{
  if (na<2) {
    return help_on("t=");
  }
  return settrack(atoi(a[1])-1) || trackinfo(track,-1);
}

static int trackinfo_com(int na, char **a)
{
  int i,err=0;

  if (na<2) {
    return trackinfo(track,-1);
  }

  for (i=1; i<na; i++) {
    int t;
    t = atoi(a[i]);
    err |= trackinfo(t-1, -1);
  }
  return err;
}

/* read mm:sec time format
 * Returns:
 * >=0  time in seconds
 * -1   Not a time format (missing ':')
 * -2   invalid digit
 */
static int read_time_format(char *a)
{
  char * deuxpoint,* s;
  int min=0,sec=0;

  if (deuxpoint=strchr(a,':'), !deuxpoint) {
    return -1;
  }

  for(s=a; s<deuxpoint; s++) {
    if (!isdigit(*s)) {
      return -2;
    } else {
      min = min*10 + *s-'0';
    }
  }

  for(++s; *s; s++) {
    if (!isdigit(*s)) {
      return -2;
    } else {
      sec = sec*10 + *s-'0';
    }
  }

  return min*60 + sec;
}

/* return 'perfect' unsigned decimal or -1
 */
static int read_unsigned(char *a)
{
  int n;
  for(n=0; *a; a++) {
    if (!isdigit(*a)) {
      return -1;
    }
    n = n * 10 + *a - '0';
  }
  return n;
}

static int settime_com(int na, char **a)
{
  int i,err,all;
  int newtime=-1,newtrack=track;

  /* settime */
  if (na<2) {
    return help_on("st");
  }

  /* settime ALL|* mm:sec */
  all = !strcmp68(a[1],"ALL") || !strcmp68(a[1],"*");
  if (all)
  {
    if (na!=3)
      return derr(-1,"bad parmeters : settime ALL|* mm:sec");
    if (newtime=read_time_format(a[2]), newtime<0)
      return derr(-1,"found %s, expected time format mm:sec",a[2]);
    for(newtrack=0; newtrack>=0 && checktrack(newtrack)==newtrack; newtrack++)
      if (err=settime(newtrack,newtime,-1), err<0)
        return err;
    return 0;
  }

  /* settime [mm:sec] [ [track mm:sec] ... ] */
  for(i=1; i<na; i++)
  {
    int gettime,gettrack;
    gettrack = read_unsigned(a[i]);
    gettime  = read_time_format(a[i]);
    if (gettrack>0)
      newtrack = gettrack-1;
    else if (gettime>=0)
      newtime = gettime;
    else
      return derr(-1,"Found %s, expected track number or time mm:sec format\n",a[i]);

    if (newtime>=0)
    {
      if (newtrack<0)
        return derr(-1,"Found %s, expected track number\n",a[i]);
      else if (err=settime(newtrack,newtime,-1), err<0)
        return err;
      newtrack = newtime = -1;
    }
  }

  if (newtrack>=0)
    return derr(-1,"Missing time for track #%02d\n",newtrack+1);

  return 0;
}

static int findtime_com(int na, char **a)
{
  int i,err=0,all;
  int newtime=0;
  int savetrack=track,newtrack=track;

  if (na <= 1) {
    /* findtime (current-track, default-time) */
    if (err = find_time(0, &frames, &time_ms, &memlow, &memhi), !err) {
      settime(track,time_ms,frames);
    }
    return err;
  }

  /* findtime  ALL|* [mm:sec] */
  all = !strcmp68(a[1],"ALL") || !strcmp68(a[1],"*");
  if (all) {
    if (na==3) {
      if (newtime=read_time_format(a[2]), newtime<0) {
        return derr(-1,"bad parmeters : findtime ALL|* [mm:sec]");
      }
    } else if (na!=2) {
      return derr(-1,"too many parmeters : findtime ALL|* [mm:sec]");
    }
    for(newtrack=0;
        newtrack>=0 && checktrack(newtrack)==newtrack; newtrack++) {
      if (err=settrack(newtrack), err<0) {
        break;
      }
      if (err=find_time(newtime, &frames, &time_ms, &memlow, &memhi), err<0) {
        break;
      }
      settime(newtrack, time_ms, frames);
    }
    newtrack = -1;
  } else {
    /* findtime [[mm:sec] track] ... */
    for(i=1; i<na; i++) {
      int gettime,gettrack;

      gettrack = read_unsigned(a[i]);
      gettime  = read_time_format(a[i]);

      if (gettrack > 0) {
        newtrack = gettrack-1;
      } else if (gettime>=0) {
        newtime = gettime;
      } else {
        err=derr(-1,"Found %s, expected track number or"
                              " time mm:sec format\n",a[i]);
        break;
      }

      if (newtrack<0) {
        err=derr(-1,"Found %s, expected track number\n",a[i]);
        break;
      } else if (err=settrack(newtrack), err<0) {
        break;
      } else {
        err=find_time(newtime, &frames, &time_ms, &memlow, &memhi);
        if (err < 0) {
          break;
        }
        settime(newtrack, time_ms, frames);
        newtrack = -1;
      }
    }
  }

  /* Restore the current (default) track */
  settrack(savetrack);
  return err;
}

static int debug_com(int na, char **a)
{
  int err;
  debugmode = 0;
  if (err=checkdisk(), err<0)
    return derr(-1,"nothing to debug");

/*   if (err=prepare_reg68(), err<0) */
/*     return err; */
/*   if (err=debug68_entering(emu68), err<0) */
/*     return err; */
  debugmode = 1;
  return 0;
}


static char *killspace(char *s)
{
  while(isspace(*s)) s++;
  return s;
}

static int myisspace(int c)
{
  return isspace(c) || c=='-';
}

static char *nextword(char *s)
{
  int prev;
  if (!*s) return s;
  for(prev=*s++; *s; prev=*s++)
  {
    if (myisspace(prev) && !myisspace(*s))
      return s;
  }
  return s;
}

static void cap_word(char *s)
{
  int c;
  char *saves = s;
  if (s==NULL) return;
  printf("%s => ",s);
  for(; *s; s++) if (*s=='_') *s=' ';
  s=killspace(saves);
  for(c=*s; c; s=nextword(s), c=*s)
  {
    int c2=toupper(c); /* Do not remove becoz of     */
    if (c2 != c) *s = c2; /* static in checknames() !!! */
  }
  printf("%s\n",saves);
}

static int capitalize_com(int na, char **a)
{
  int err,i;

  /* capitilize */
  if (na<2)
    return help_on("cap");

  if (err=checkdisk(), err<0)
    return err;

  /* capitilize <ALL|*> */
  if (!strcmp68(a[1],"ALL") || !strcmp68(a[1],"*"))
  {
    cap_word(disk->name);
    for(i=0; i<disk->nb_six;i++)
    {
      cap_word(disk->mus[i].name);
      cap_word(disk->mus[i].author);
      cap_word(disk->mus[i].composer);
    }
    return 0;
  }
  return derr
    (-1,"This command is implemented in mode ALL only !! sorry :-)");
}

/* Sorry but these commands alloc memory but never freed :-)
 */
static int replay_com(int na, char **a)
{
  char *r;
  int err,i;

  /* replay */
  if (na<2)
    return help_on("r=");

  /* Get name */
  if (!strcmp68(a[na-1],"NONE"))
    r = NULL;
  else if (r=(char*)alloc68(strlen(a[na-1])+1), r==NULL)
    return derr(-1,"Can't alloc new replay name");
  else strcpy(r,a[na-1]);

  /* replay NAME|NONE */
  if (na==2)
  {
    if (err=checktrack(track), err<0)
      return err;
    mus->replay = r;
    return trackinfo(track,11);
  }

  if (!strcmp68(a[1],"ALL") || !strcmp68(a[1],"*"))
  {
    if (err=checkdisk(), err<0)
      return err;
    for(i=0; i<disk->nb_six; i++)
    {
      disk->mus[i].replay = r;
      trackinfo(i,11);
    }
    return 0;
  }

  for(i=1; i<na-1; i++)
  {
    int t;
    if (!isdigit(a[i][0]))
      return derr(-1,"<%s> is not a track number",a[i]);
    t=atoi(a[i])-1;
    if (err=checktrack(t), err<0)
      return err;
    if (err==t)
      disk->mus[t].replay = r;
    trackinfo(t,11);
  }
  return 0;
}

static int name_com(int na, char **a)
{
  char *r;
  int err,i;

  /* name */
  if (na<2)
    return help_on("n=");

  /* Get name */
  if (!strcmp68(a[na-1],"NONE"))
    r = NULL;
  else if (r=(char*)alloc68(strlen(a[na-1])+1), r==NULL)
    return derr(-1,"Can't alloc new name");
  else strcpy(r,a[na-1]);

  /* replay NAME|NONE : disk name */
  if (na==2)
  {
    if (err=checkdisk(), err<0)
      return err;
    if (r==NULL)
      return derr(-1,"Can't set disk name to NONE");
    disk->name = r;
    return diskinfo();
  }

  if (!strcmp68(a[1],"ALL"))
  {
    if (err=checkdisk(), err<0)
      return err;
    if (r!=NULL) disk->name = r;
    for(i=0; i<disk->nb_six; i++)
    {
      disk->mus[i].name = r==NULL ? disk->name : r;
    }
    diskinfo();
    return 0;
  }

  for(i=1; i<na-1; i++)
  {
    int t;
    if (!isdigit(a[i][0]))
      return derr(-1,"<%s> is not a track number",a[i]);
    t=atoi(a[i])-1;
    if (err=checktrack(t), err<0)
      return err;
    if (err==t)
      disk->mus[t].name = (r==NULL) ? disk->name : r;
    trackinfo(t,3);
  }
  return 0;
}

static int author_com(int na, char **a)
{
  char *r;
  int err,i;

  /* author */
  if (na<2)
    return help_on("a=");

  /* Get author name */
  if (!strcmp68(a[na-1],"NONE"))
    r = NULL;
  else if (r=(char*)alloc68(strlen(a[na-1])+1), r==NULL)
    return derr(-1,"Can't alloc new name");
  else strcpy(r,a[na-1]);

  if (na==2)
  {
    if (err=checkdisk(), err<0)
      return err;
    for(i=0; i<disk->nb_six; i++)
    {
      disk->mus[i].author = r;
    }
    diskinfo();
    return 0;
  }

  for(i=1; i<na-1; i++)
  {
    int t;
    if (!isdigit(a[i][0]))
      return derr(-1,"<%s> is not a track number",a[i]);
    t=atoi(a[i])-1;
    if (err=checktrack(t), err<0)
      return err;
    if (err==t)
      disk->mus[t].author = r;
    trackinfo(t,3);
  }

  return 0;
}

static int composer_com(int na, char **a)
{
  char *r;
  int err,i;

  /* composer */
  if (na<2)
    return help_on("c=");

  /* Get composer name */
  if (!strcmp68(a[na-1],"NONE"))
    r = NULL;
  else if (r=(char*)alloc68(strlen(a[na-1])+1), r==NULL)
    return derr(-1,"Can't alloc new name");
  else strcpy(r,a[na-1]);

  if (na==2)
  {
    if (err=checkdisk(), err<0)
      return err;
    for(i=0; i<disk->nb_six; i++)
    {
      disk->mus[i].composer = r;
    }
    diskinfo();
    return 0;
  }

  for(i=1; i<na-1; i++)
  {
    int t;
    if (!isdigit(a[i][0]))
      return derr(-1,"<%s> is not a track number",a[i]);
    t=atoi(a[i])-1;
    if (err=checktrack(t), err<0)
      return err;
    if (err==t)
      disk->mus[t].composer = r;
    trackinfo(t,3);
  }

  return 0;
}

static int deltrack_com(int na, char **a)
{
  int err,t;
  /* deltrack */
  if (na!=2)
    return help_on("del");

  /* deltrack # */
  if (!isdigit(a[1][0]))
    return derr(-1,"<%s> is not a track number",a[1]);
  t=atoi(a[1])-1;
  if (err=checktrack(t), err<0)
    return err;
  if (err==t)
  {
    int i;
    for(i=t; i<disk->nb_six-1; i++)
      disk->mus[i] = disk->mus[i+1];
    disk->nb_six--;
  }
  totaltime();
  return diskinfo();
}

static int frq_com(int na, char **a)
{
  int err,i,rate=-1, all;

  /* frq */
  if (na < 2) {
    return help_on("f=");
  }

  if (err=checkdisk(), err<0) {
    return err;
  }

  /* Get frq rate */
  if (isdigit(a[na-1][0])) {
    rate = atoi(a[na-1]);
  }
  if (rate<0 || rate>20000) {
    return derr(-1,"Bad or missing rate (in hz)");
  }

  all = (na == 2)
    || ((na == 3) && (!strcmp68(a[1],"ALL") || !strcmp68(a[1],"*")));

  if (all) {
    for(i=0; i<disk->nb_six; i++) {
      disk->mus[i].frq = rate;
    }
    diskinfo();
  } else {
    for(i=1; i<na-1; i++) {
      int t;
      if (!isdigit(a[i][0])) {
        err =  derr(-1,"<%s> is not a track number",a[i]);
        break;
      }
      t=atoi(a[i])-1;
      if (err=checktrack(t), err<0) {
        break;
      }
      if (err==t) {
        disk->mus[t].frq = rate;
      }
      trackinfo(t,7);
    }
  }
  if (mus) {
    cycleperpass = cycle_per_frame(mus->frq);
  }
  return -(err<0);
}

static int savedata_com(int na, char **a)
{
  int err;
  int t=track;
  int namei = 1;
  istream68_t * os;
  if (na<2)
    return help_on("sdat");
  if (err=checkdisk(), err<0)
    return err;
  if (na>2)
  {
    t = atoi(a[1]) - 1;
    namei = 2;
  }
  if (err=checktrack(t), err<0)
    return err;
  os = istream68_file_create(a[namei],2);
  if (!os) return -1;
  if (!istream68_open(os)) {
    istream68_destroy(os);
    return -1;
  }
  printf("saving track #%d data (%p,%d)\n",
         t+1,disk->mus[t].data,disk->mus[t].datasz);
  if (disk->mus[t].data!=NULL && disk->mus[t].datasz > 0)
    err =
      istream68_write(os, disk->mus[t].data, disk->mus[t].datasz)
      != disk->mus[t].datasz;
  else
    err = derr(-1,"No dat in this track!!!");
  istream68_destroy(os);
  return -!!err;
}

typedef struct
{
  char *mname;
  char **rname;
  int *flags;
  char **aname;
  int *a0;
  int *frq;
} replay_parm_t;

static int check_ext_list(replay_parm_t *rparm)
{
  if (!strcmp68(rparm->mname,"FC13") ||
     !strcmp68(rparm->mname,"FC14") ||
     !strcmp68(rparm->mname,"FC") ) {
    *rparm->rname = "fc13_14";
    *rparm->flags = SC68_AMIGA;
    return 1;
  } else if (!strcmp68(rparm->mname,  "PUMA") ||
            !strcmp68(rparm->mname,  "PUM") ) {
    *rparm->rname = "puma";
    *rparm->flags = SC68_AMIGA;
    return 2;
  } else if (!strcmp68(rparm->mname,  "MON") ) {
    *rparm->rname = "MON";
    *rparm->flags = SC68_AMIGA;
    *rparm->aname = "Maniacs Of Noise";
    return 3;
  } else if (!strcmp68(rparm->mname,  "DW") ) {
    *rparm->rname = "DWaga";
    *rparm->flags = SC68_AMIGA;
    *rparm->aname = "David Whittaker";
    return 4;
  } else if (!strcmp68(rparm->mname,  "BP") ||
             !strcmp68(rparm->mname,  "BP3") ) {
    *rparm->rname = "bs22";
    *rparm->flags = SC68_AMIGA;
    return 5;
  } else if (!strcmp68(rparm->mname,  "JAM") ) {
    *rparm->rname = "JamCrackerPro";
    *rparm->flags = SC68_AMIGA;
    return 6;
  } else if (!strcmp68(rparm->mname,  "DMU") ) {
    *rparm->rname = "DMU";
    *rparm->flags = SC68_AMIGA;
    *rparm->a0 = 0x74f6;
    return 6;
  } else if (!strcmp68(rparm->mname,  "TCB") ) {
    *rparm->rname = "TCBtracker";
    *rparm->flags = SC68_YM;
    return 7;
  } else if (!strcmp68(rparm->mname,  "MM2") ) {
    *rparm->rname = "musmon2";
    *rparm->flags = SC68_YM;
    *rparm->frq = 200;
    return 8;
  } else if (!strcmp68(rparm->mname,  "SSD1") ) {
    *rparm->rname = "ssd1";
    *rparm->flags = SC68_YM;
    return 9;
  } else if (!strcmp68(rparm->mname,  "ELOF") ) {
    *rparm->rname = "blipp079";
    *rparm->flags = SC68_YM;
    return 10;
  } else if (!strcmp68(rparm->mname,  "MS27") ) {
    *rparm->rname = "tao_ms27";
    *rparm->flags = SC68_YM;
    *rparm->aname = "Tao";
    *rparm->frq   = 200;
    return 11;
  } else if (!strcmp68(rparm->mname,  "MS25") ) {
    *rparm->rname = "tao_ms25";
    *rparm->flags = SC68_YM;
    *rparm->aname = "Tao";
    return 11;
  } else if (!strcmp68(rparm->mname,  "MS21") ) {
    *rparm->rname = "tao_ms211";
    *rparm->flags = SC68_YM;
    *rparm->aname = "Tao";
    return 12;
  } else if (!strcmp68(rparm->mname,  "YM") ) {
    *rparm->flags = SC68_YM;
    return 256;
  } else {
    return 0;
  }
}

static int loaddata_com(int na, char **a)
{
  char *fname=NULL,*rname=NULL,*aname=NULL,*mname=NULL,*s;
  char tmp[1024];
  int i;
  int d0=1,a0=SC68_LOADADDR,frq=50,flags=SC68_YM;
  replay_parm_t rparm;
  if (na<2)
    return help_on("ldat");

  /* Find filename in arg list */
  for(i=1, fname=NULL; i<na; i++) {
    if (a[i][0] != '-') {
      if (!fname) {
        fname = a[i];
      } else {
        return derr(-1,"Invalid argument [%s] :Only one file name",a[i]);
      }
    }
  }

  if (!fname) {
    return derr(-1,"Loaddata failed : Missing filename\n");
  }

  /* Get all possible info from filename */

  /* Kill path (posix) : get music-name from filename */
  strncpy(tmp,fname,sizeof(tmp));
  tmp[sizeof(tmp)-1] = 0;
  if (mname=strrchr(tmp,'/'), !mname) {
    mname = tmp;
  } else {
    mname = mname+1;
  }

  rparm.a0 = &a0;
  rparm.aname = &aname;
  rparm.flags = &flags;
  rparm.frq = &frq;
  rparm.mname = mname;
  rparm.rname = &rname;

  /* Kill prefix extension (Amigaaaaa) */
  if (s=strchr(mname,'.'), (s!=NULL && (s-mname)<=4))
  {
    *s = 0;
    if (check_ext_list(&rparm))
      mname = s+1;
    else
      *s = '.';
  }

  /* Kill ext */
  if (s=strrchr(mname,'.'), s!=NULL)
  {
    rparm.mname = s+1;
    if (check_ext_list(&rparm))
      *s = 0;
    /* Assume ext for len<=4 & no digit in first lettter
       (avoid to remove hydra.0.1) */
    else if (!isdigit(s[1]) && strlen(s+1)<=4)
      *s = 0;
  }

  /* Remove _ by white space */
  for(s=strchr(mname,'_'); s!=NULL; s=strchr(s,'_')) *s= ' ';

  /* Command line parsing : get command args */
  for(i=1; i<na; i++) {
    rparm.mname = a[i]+1;
    if (a[i][0]=='-' && !check_ext_list(&rparm)) {
      if (toupper(a[i][1])=='D' && a[i][2]=='0'  && a[i][3]=='=')
        d0 = atoi(a[i]+4);
      else if (toupper(a[i][1])=='A' && a[i][2]=='0'  && a[i][3]=='=')
        a0 = atox(a[i]+4);
      else if (toupper(a[i][1])=='F' && a[i][2]=='=')
        frq = atoi(a[i]+3);
      else {
        int j;
        for(j=1; a[i][j]; j++) {
          if (a[i][j]=='y') flags &= ~SC68_YM;
          else if (a[i][j]=='Y') flags |= SC68_YM;
          else if (a[i][j]=='a') flags &= ~SC68_AMIGA;
          else if (a[i][j]=='A') flags |= SC68_AMIGA;
          else if (a[i][j]=='s') flags &= ~SC68_STE;
          else if (a[i][j]=='S') flags |= SC68_STE;
        }
      }
    }
  }

  if (loaddata(fname,d0,a0,frq,flags,mname,aname,rname))
    return derr(-1,"loaddata : Failed");
  return diskinfo();
}

static int dup_com(int na, char **a)
{
  int err,t;

  if (na < 2)
    return help_on("+");

  if (err = checkdisk(), err < 0)
    return err;
  
  t = atoi(a[1]) - 1;                   /* track to duplicate */

  if (err = checktrack(t), err != t)
    return err;

  if (disk->nb_six == SC68_MAX_TRACK)
    return derr(-1,"too many tracks");

  disk->mus[disk->nb_six] = disk->mus[t];
  disk->mus[disk->nb_six].d0++;
  ++disk->nb_six;

  return diskinfo();
}


static int address_com(int na, char **a)
{
  int a0 = SC68_LOADADDR, track = 0, err, i;
  char *s;

  if (na < 2 || na > 3) {
    return help_on("address");
  }

  if (err = checkdisk(), err < 0) {
    return err;
  }

  if (na == 3) {
    s = a[2];
    track = atoi(a[1]);
  } else {
    s = a[1];
  }

  if (*s == '$') {
    ++s;
  }
  if (isxdigit(*s)) {
    a0 = atox(s);
  }

  if (!track) {
    /* for all tracks */
    for(i=0; i<disk->nb_six; i++) {
      disk->mus[i].a0 = a0;
    }
    diskinfo();
  } else if (track > 0 && track <= disk->nb_six) {
    /* for given track */
    --track;
    disk->mus[track].a0 = a0;
    trackinfo(track, -1);
  } else {
    /* out of range */
    return derr(-1,"track #%d out of disk range [%d..%d]",
                track,1,disk->nb_six);
  }
  return 0;
}
