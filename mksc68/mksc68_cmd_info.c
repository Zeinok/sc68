/*
 * @file    mksc68_cmd_info.c
 * @brief   the "info" command
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2011 Benjamin Gerard
 *
 * Time-stamp: <2011-11-03 13:34:17 ben>
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

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "mksc68_cmd.h"
#include "mksc68_dsk.h"
#include "mksc68_msg.h"
#include "mksc68_opt.h"
#include "mksc68_tag.h"
#include "mksc68_str.h"

#include <sc68/file68.h>

#include <string.h>
#include <stdio.h>
#include <ctype.h>

static const opt_t longopts[] = {
  { "help",       0, 0, 'h' },
  { "disk",       0, 0, 'd' },
  { "all",        0, 0, 'a' },
  { "long",       0, 0, 'l' },
  { "tracks",     1, 0, 't' },
  { 0,0,0,0 }
};

static void pinf(const char * label, const char * value)
{
  const int ll = 11;                    /* label length */
  if (label && *label && value)
    msginf("%c%-*s : %s\n", toupper(label[0]), ll-1, label+1, value);
}

static void iinf(const char * label, int value)
{
  char tmp[32];
  sprintf(tmp, "%d", value);
  pinf(label, tmp);
}

static void tinf(const char * label, unsigned int ms)
{
  char tmp[64];
  pinf(label, str_timefmt(tmp, sizeof(tmp), ms));
}

static void catflag(char * tmp, int bit, const char * label) {
  if (bit) {
    if (*tmp)
      strcat(tmp,",");
    strcat(tmp,label);
  }
}

static void hinf(const char * label, hwflags68_t hw)
{
  char tmp[256]
    ;
  *tmp = 0;
  if (!hw.all)
    strcpy(tmp,"NONE");
  else {
    catflag(tmp, hw.bit.ym,    "YM");
    catflag(tmp, hw.bit.ste,   "STE");
    catflag(tmp, hw.bit.amiga, "AGA");
    if (hw.bit.timers) {
      catflag(tmp, hw.bit.timera, "TA");
      catflag(tmp, hw.bit.timerb, "TB");
      catflag(tmp, hw.bit.timerc, "TC");
      catflag(tmp, hw.bit.timerd, "TD");
    } else
      catflag(tmp, 1, "T?");
  }
  pinf(label, tmp);
}


static
int run_info(cmd_t * cmd, int argc, char ** argv)
{
  char shortopts[(sizeof(longopts)/sizeof(*longopts))*3];
  int ret = -1, i, longinfo = 0, disktarget = 0, alltarget = 0, spec = 0, a, b;
  const char * tracklist = 0;
  disk68_t * dsk;

  opt_create_short(shortopts, longopts);
  while (1) {
    int longindex;
    int val
      = getopt_long(argc, argv, shortopts, longopts, &longindex);

    switch (val) {
    case  -1: break;                    /* Scan finish */

    case 'h':                           /* --help */
      help(argv[0]); return 0;

    case 'd':
      spec = disktarget = 1;
      break;

    case 't':                           /* --track=    */
      if (tracklist) {
        msgerr("multiple track-list not allowed.\n");
        goto error;
      } else {
        tracklist = optarg;
        while ( ret = str_tracklist(&tracklist, &a, &b), ret > 0 ) {
          msgdbg("track interval %d-%d\n",a,b);
        }
        tracklist = optarg;
        if (ret)
          goto error;
        ret = -1;
        spec = 1;
      }
      break;

    case 'a':
      spec = alltarget = 1;
      break;

    case 'l':
      longinfo = 1;
      break;

    case '?':                       /* Unknown or missing parameter */
      goto error;

    default:
      msgerr("unexpected getopt return value (%d)\n", val);
      goto error;
    }
    if (val == -1) break;
  }
  i = optind;

  if (!spec || alltarget) {
    disktarget = 1;
    tracklist = "1-0";
  }

  if (dsk = dsk_get_disk(), !dsk) {
    msgerr("no disk");
    goto error;
  }

  if (disktarget) {
    pinf("ALBUM",    tag_get(0,"title"));
    pinf("ARTIST",   tag_get(0,"artist"));
    iinf("TRACKS",   dsk->nb_mus);
    iinf("DEFAULT",  dsk->def_mus);
    hinf("HARDWARE", dsk->hwflags);
    tinf("TOTAL",    dsk->time_ms);
  }

  if (tracklist) {
    while ( str_tracklist(&tracklist, &a, &b) > 0 ) {
      int t;
      for (t = a; t <= b; ++t) {
        /* display track info */
        music68_t * mus = dsk->mus + t - 1;
        iinf("track",    t);
        pinf("title",    tag_get(t,"title"));
        pinf("artist",   tag_get(t,"artist"));
        pinf("replay",   mus->replay);
        hinf("hardware", mus->hwflags);
        tinf("length",   mus->first_ms);
        iinf("loops",    mus->loops);
        tinf("loop-len", mus->loops_ms);
        tinf("total",    mus->total_ms);
      }
    }
  }
  ret = 0;

error:
  return ret;
}

cmd_t cmd_info = {
  /* run */ run_info,
  /* com */ "info",
  /* alt */ "inf",
  /* use */ "[opts]",
  /* des */ "display album info",
  /* hlp */

  "The `info' command displays album informations.\n"
  "Use -d, -a and -t to select what to display.\n"
  "Use -l to select how to display how to display it.\n"
  "\n"
  "OPTIONS\n"
  "   -l --long        use verbose info format.\n"
  "   -d --disk        display disk info.\n"
  "   -a --all         display full info.\n"
  "   -t\n"
  "   --tracks=TRACKS  select tracks tag.\n"
  "\n"
  "TRACKS := N[-N][,N[-N]]* N:=[0-9]+ (0 is the last track)"
};
