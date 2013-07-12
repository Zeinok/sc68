/*
 * @file    mksc68_cmd_play.c
 * @brief   the "play" command
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-07-12 22:34:18 ben>
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
#include "config.h"
#endif

#include "mksc68_cmd.h"
#include "mksc68_dsk.h"
#include "mksc68_msg.h"
#include "mksc68_opt.h"
#include "mksc68_str.h"
#include "mksc68_gdb.h"

#include <sc68/file68.h>
#include <sc68/alloc68.h>
#include <sc68/istream68.h>
#include <sc68/sc68.h>
#include <emu68/emu68.h>
#include <emu68/excep68.h>
#include <io68/io68.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>

extern void sc68_emulators(sc68_t *, emu68_t **, io68_t ***);

static const opt_t longopts[] = {
  { "help",     0, 0, 'h' },            /* help             */
  { "loop",     1, 0, 'l' },            /* loopplay        */
  { "seek",     1, 0, 's' },            /* seek to position */
  { "to",       1, 0, 't' },            /* stop time        */
  { "fg",       0, 0, 'f' },            /* foreground play  */
  { "debug",    0, 0, 'd' },            /* gdb debug mode   */
  { 0,0,0,0 }
};

typedef struct {
  volatile int  isplaying;
  volatile int  debug;
  sc68_t      * sc68;       /* sc68 instance.    */
  emu68_t     * emu68;      /* emu68 instance.   */
  io68_t     ** ios68;      /* other chip.       */
  gdb_t       * gdb;        /* gdb instance      */
  istream68_t * out;        /* audio out stream  */
  pthread_t     thread;     /* play thread       */
  int           track;
  int           loop;
  int           code;
} playinfo_t;

static playinfo_t playinfo;             /* unique playinfo */

static void play_info(playinfo_t * pi)
{
  sc68_music_info_t info;
  if (!sc68_music_info(pi->sc68,&info,-1,0)) {
    int i, len = 11;
    msginf("%-*s : %d/%d\n",  len, "Track",   info.trk.track, info.tracks);
    msginf("%-*s : %s\n",     len, "Album",   info.album);
    msginf("%-*s : %s\n",     len, "Title",   info.title);
    msginf("%-*s : %s\n",     len, "Artist",  info.artist);
    msginf("%-*s : %s\n",     len, "Hardware",info.trk.hw);
    msginf("%-*s : %u:%02u\n",len, "Start time",
          info.start_ms/60000u, (info.start_ms/1000u)%60u);
    msginf("%-*s : %s\n",len, "Duration", info.trk.time);

    if (info.dsk.tags) {
      msginf("Disk tags:\n");
      for (i=2; i<info.dsk.tags; ++i)
        msginf("%c%-*s : %s\n",
               toupper((int) *info.dsk.tag[i].key),
               len-1,
               info.dsk.tag[i].key+1,
               info.dsk.tag[i].val);
    }

    if (info.trk.tags) {
      msginf("Track tags:\n");
      for (i=2; i<info.trk.tags; ++i)
        msginf("%c%-*s : %s\n",
               toupper((int) *info.trk.tag[i].key),
               len-1,
               info.trk.tag[i].key+1,
               info.trk.tag[i].val);
    }


  }
}

static int play_hdl(emu68_t* const emu68, int vector, void * cookie)
{
  playinfo_t * pi = cookie;
  assert(pi == &playinfo);

  switch (gdb_event(pi->gdb, vector, emu68)) {

  case RUN_EXIT:
    return 1;

  case RUN_STOP:
    assert(!"gdb status is stopped but we are not stopped !!!");

  case RUN_SKIP:
    emu68_set_handler(emu68,0);
    gdb_destroy(pi->gdb);
    pi->gdb = 0;

  case RUN_CONT:
    return 0;

  default:
    assert(!"unhandled or invalid gdb status");
    return 0;
  }

  return -1;
}

static void play_init(playinfo_t * pi)
{
  const char * outname = pi->debug ? "null://default" : "audio://default";
  /* const char * outname = "audio://default"; */
  sc68_create_t create68;

  pi->isplaying = 2;
  pi->code      = -1;
  pi->out       = sc68_stream_create(outname, 2);
  pi->gdb       = 0;

  if (!pi->out) return;
  if (istream68_open(pi->out)) return;

  memset(&create68,0,sizeof(create68));
  create68.name = pi->debug ? "mksc68-debug" : "mksc68-play";
  create68.emu68_debug = !!pi->debug;
  if ( !(pi->sc68 = sc68_create(&create68) ) ) return;
  if (pi->debug) {
    sc68_emulators(pi->sc68, &pi->emu68, &pi->ios68);
    emu68_set_handler(pi->emu68, play_hdl);
    emu68_set_cookie(pi->emu68, pi);
    pi->gdb = gdb_create();
  }

  if ( sc68_open(pi->sc68, (sc68_disk_t) dsk_get_disk() ) ) return;
  if ( sc68_play(pi->sc68, pi->track, pi->loop) < 0 ) return;
  pi->code = -(sc68_process(pi->sc68, 0, 0) == SC68_ERROR);
  if (!pi->code)
    play_info(pi);
}

static void play_run(playinfo_t * pi)
{
  char buffer[512 * 4];
  int loop = 0, code;

  pi->isplaying = 1;
  pi->code      = 1;
  for ( ;; ) {
    int n = sizeof(buffer) >> 2;
    code = sc68_process(pi->sc68, buffer, &n);
    if (code & (SC68_END|SC68_CHANGE))
      break;
    if (code & SC68_LOOP)
      ++loop;
    if (istream68_write(pi->out, buffer, (n << 2)) != (n << 2)) {
      code = SC68_ERROR;
      break;
    }
  }
  pi->code = -(code == SC68_ERROR);
}

static void play_end(playinfo_t * pi)
{
  pi->isplaying = 3;
  istream68_close(pi->out);
  pi->out = 0;
  gdb_destroy(pi->gdb);
  pi->gdb = 0;
  sc68_destroy(pi->sc68);
  pi->sc68 = 0;
  pi->isplaying = 0;
}

static void * play_thread(void * userdata)
{
  playinfo_t * pi = (playinfo_t *) userdata;
  assert( pi == &playinfo );
  play_init(pi);
  if (!pi->code) play_run(pi);
  play_end(pi);
  return pi;
}

int dsk_stop(void)
{
  int err = -1;

  if (playinfo.isplaying) {
    if (playinfo.sc68) {
      msgdbg("stop: sending stop signal to %s\n",sc68_name(playinfo.sc68));
      sc68_stop(playinfo.sc68);
    } else {
      pthread_cancel(playinfo.thread);
    }
    pthread_join(playinfo.thread, 0);
    err = playinfo.code;
  } else {
    msgnot("not playing\n");
  }
  return err;
}

int dsk_playing(void)
{
  return (playinfo.isplaying == 1) + (!!playinfo.debug << 1);
}

struct dsk_play_s {
  int track, loop, start, len, bg, debug;
};


/* int trk, int loop, int start, int len, int bg, int dbg */
int dsk_play(dsk_play_t * p)
{
  int err = -1;

  msgdbg("play [track:%d loop:%d start:%d length:%d bg:%d dbg:%d]\n",
         p->track, p->loop, p->start, p->len, p->bg, p->debug);

  if (playinfo.isplaying) {
    dsk_stop();
  }

  memset(&playinfo,0,sizeof(playinfo));
  playinfo.track = p->track;
  playinfo.loop  = p->loop;
  playinfo.debug = p->debug;
  if ( pthread_create(&playinfo.thread, 0, play_thread, &playinfo) ) {
    msgerr("error creating play thread\n");
    goto error;
  }

  if (!p->bg) {
    pthread_join(playinfo.thread, 0);
    err = playinfo.code;
  } else {
    msginf("play: playing in background (use `stop' command to stop)\n");
    err = 0;
  }

error:
  return err;
}


static
int run_play(cmd_t * cmd, int argc, char ** argv)
{
  char shortopts[(sizeof(longopts)/sizeof(*longopts))*3];
  int ret = -1, i, loop = SC68_DEF_LOOP, track, tracks, bg = 1, debug = 0;
  const char * str;
  int seek_mode = '?', seek = 0, dur_mode = '?', duration = 0;
  dsk_play_t params;

  opt_create_short(shortopts, longopts);

  while (1) {
    int longindex;
    int val =
      getopt_long(argc, argv, shortopts, longopts, &longindex);

    switch (val) {
    case  -1: break;                    /* Scan finish */

    case 'h':                           /* --help */
      help(argv[0]); return 0;

    case 'l':                           /* --loop      */
      str = optarg;
      if (!strcmp(str,"inf"))
        loop = SC68_INF_LOOP;
      else if (isdigit((int)*str))
        loop = strtol(str,0,0);
      break;

    case 'f':                           /* --fg        */
      bg = 0; break;

    case 's':                           /* --seek      */
      str = optarg;
      if (*str == '+' || *str == '-')
        seek_mode = *str++; /* $$$ TODO ? */
      seek_mode = seek_mode;
      if (str_time_stamp(&str, &seek)) {
        msgerr("invalid seek value \"%s\"\n", optarg);
        goto error;
      }
      break;

    case 't':                           /* --to        */
      str = optarg;
      if (*str == '+' || *str == '-')
        dur_mode = *str++; /* $$$ TODO ? */
      dur_mode = dur_mode;
      if (str_time_stamp(&str, &duration)) {
        msgerr("invalid duration value \"%s\"\n", optarg);
        goto error;
      }
      break;

    case 'd':
      debug = 1; break;

    case '?':                       /* Unknown or missing parameter */
      goto error;
    default:
      msgerr("unexpected getopt return value (%d)\n", val);
      goto error;
    }
    if (val == -1) break;
  }
  i = optind;

  if (!dsk_has_disk()) {
    msgerr("no disk loaded\n");
    goto error;
  }
  if (tracks = dsk_get_tracks(), tracks <= 0) {
    msgerr("disk has no track\n");
    goto error;
  }
  track = dsk_trk_get_current();

  if (i < argc) {
    int t;
    errno = 0;
    t = strtol(argv[i++],0,0);
    if (errno) {
      msgerr("invalid track number \"%s\"\n",argv[--i]);
      goto error;
    }
    track = t;
  }

  if (!track)
    track = dsk_trk_get_default();
  if (track < 0 || track > tracks) {
    msgerr("track number #%d out of range [1..%d]\n", track, tracks);
    goto error;
  }

  if (i < argc)
    msgwrn("%d extra parameters ignored\n", argc-i);


  memset(&params,0,sizeof(params));
  params.track = track;
  params.loop  = loop;
  params.start = seek;
  params.len   = duration;
  params.bg    = bg;
  params.debug = debug;
  ret =  dsk_play(&params);

error:
  return ret;
}

cmd_t cmd_play = {
  /* run */ run_play,
  /* com */ "play",
  /* alt */ 0,
  /* use */ "[opts] [track]",
  /* des */ "play a track",
  /* hlp */
  "The `play' command plays a track.\n"
  "\n"
  "OPTIONS\n"
  "   -l --loop=N       Number of loop (-1:inf 0:def)\n"
  "   -s --seek=POS     Seek to this position.\n"
  "   -t --to=POS       End position.\n"
  "   -f --fg           Foreground play.\n"
  "   -d --debug        Debug via gdb.\n"
  "\n"
  "POS := [+|-]ms | [+|-]mm:ss[:ms]\n"
  "  '+' is relative to start position.\n"
  "  '-' is relative to end position.\n"
  "  no prefix is absolute time posiiton.\n"
  "  The value can be express by either a single integer in milliseconds\n"
  "  or by a mm:ss,ms form where the \",ms\" part is optionnal."
};

static
int run_stop(cmd_t * cmd, int argc, char ** argv)
{
  if (argc > 1) {
    int i;
    for (i=1; i<argc; ++i)
      if (!strcmp(argv[i],"--help") || !strcmp(argv[i],"-h")) {
        help(argv[0]);
        return 0;
      }
    msgwrn("%d extra parameters ignored\n", argc-i);
  }
  return
    dsk_stop();
}

cmd_t cmd_stop = {
  /* run */ run_stop,
  /* com */ "stop",
  /* alt */ 0,
  /* use */ 0,
  /* des */ "stop a background play",
  /* hlp */
  "The `stop' command stops a background music started by the `play' command."
};
