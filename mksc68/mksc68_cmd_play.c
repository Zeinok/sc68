/* $Id$ */

/* Time-stamp: <2009-10-07 18:07:53 ben>  */

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "mksc68_cmd.h"
#include "mksc68_dsk.h"
#include "mksc68_msg.h"
#include "mksc68_opt.h"
#include "mksc68_str.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "sc68/file68.h"
#include "sc68/alloc68.h"
#include "sc68/istream68.h"
#include "sc68/sc68.h"


static const opt_t longopts[] = {
  {"seek",     1,0,'s'},                  /* seek to position */
  {"to",       1,0,'t'},                  /* stop time        */
  {"fg",       0,0,'f'},                  /* foreground play  */
  {0,0,0,0}
};

typedef struct {
  volatile int  isplaying;
  sc68_t      * sc68;
  istream68_t * out;
  pthread_t     thread;
  int           track;
  int           loop;
  int           code;
} playinfo_t;

static playinfo_t playinfo;             /* unique playinfo */

static void play_info(playinfo_t * pi)
{
  sc68_music_info_t info;
  if (!sc68_music_info(pi->sc68,&info,-1,0)) {
    msginf("Track      : %d/%d\n",info.track, info.tracks);
    msginf("Title      : %s\n",info.title);
    msginf("Author     : %s\n",info.author);
    msginf("Composer   : %s\n",info.composer);
    msginf("Ripper     : %s\n",info.ripper);
    msginf("Converter  : %s\n",info.converter);
    msginf("Replay     : %s\n",info.replay);
    msginf("Hardware   : %s\n",info.hwname);
    msginf("Start time : %u:%02u\n",
           info.start_ms/60000u, (info.start_ms/1000u)%60u);
    msginf("Duration   : %s\n", info.time);
  }
}

static void play_init(playinfo_t * pi)
{
  const char * outname = "audio://default";
  sc68_create_t create68;

  pi->isplaying = 2;
  pi->code      = -1;
  pi->out       = sc68_stream_create(outname, 2);
  if (!pi->out) return;
  if (istream68_open(pi->out)) return;

  memset(&create68,0,sizeof(create68));
  create68.name = "mksc68-play";
  if ( !(pi->sc68 = sc68_create(&create68) ) ) return;
  if ( sc68_open(pi->sc68, (sc68_disk_t) dsk_get_disk() ) ) return;
  if ( sc68_play(pi->sc68, pi->track, pi->loop) < 0 ) return;
  pi->code = -(sc68_process(pi->sc68, 0, 0) == SC68_ERROR);
}

static void play_run(playinfo_t * pi)
{
  char buffer[512 * 4];
  int loop = 0, code;

  pi->isplaying = 1;
  pi->code      = 1;
  for ( ;; ) {
    code = sc68_process(pi->sc68, buffer, sizeof(buffer) >> 2);
    if (code == SC68_ERROR)
      break;
    if (code & SC68_LOOP)
      ++loop;
    if (code & SC68_CHANGE)
      break;
    if (istream68_write(pi->out, buffer, sizeof(buffer)) != sizeof(buffer)) {
      code = -1;
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
  }
  return err;
}

int dsk_play(int trk, int loop, int start, int len, int bg)
{
  int err = -1;
/*   int tracks = dsk_get_tracks(); */

  msgdbg("play track:%d loop:%d start:%d length:%d bg:%d\n",
         trk, loop, start, len, bg);

  if (playinfo.isplaying) {
    msgdbg("already playing: stop resqueted\n");
    dsk_stop();
    msgdbg("stop completed\n");
  }

  memset(&playinfo,0,sizeof(playinfo));
  playinfo.track = trk;
  playinfo.loop  = 1;
  if ( pthread_create(&playinfo.thread, 0, play_thread, &playinfo) ) {
    msgdbg("play: error creating play thread\n"); goto error;
  }

  if (!bg) {
    msgdbg("play: waiting for play thread to end\n");
    pthread_join(playinfo.thread, 0);
    err = playinfo.code;
    msgdbg("play: play thread released with code %d\n", err);
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
  int ret = -1, i, track, tracks, bg = 1;
  const char * str;
  int seek_mode, seek, dur_mode, duration;

  opt_create_short(shortopts, longopts);

  while (1) {
    int longindex;
    int val =
      getopt_long(argc, argv, shortopts, longopts, &longindex);

    switch (val) {
    case  -1: break;                    /* Scan finish */

    case 'f':                           /* --fg        */
      bg = 0; break;

    case 's':                           /* --seek      */
      str = optarg;
      if (*str == '+' || *str == '-')
        seek_mode = *str++;
      if (str_time_stamp(&str, &seek)) {
        msgerr("invalid seek value \"%s\"\n", optarg);
        goto error;
      }
      break;

    case 't':                           /* --to        */
      str = optarg;
      if (*str == '+' || *str == '-')
        dur_mode = *str++;
      if (str_time_stamp(&str, &duration)) {
        msgerr("invalid duration value \"%s\"\n", optarg);
        goto error;
      }
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

  ret =  dsk_play(track, 1, seek, duration, bg);

error:
  return ret;
}

cmd_t cmd_play = {
  /* run */ run_play,
  /* com */ "play",
  /* alt */ 0,
  /* use */ "[OPTION ...] [--] [TRACK]",
  /* des */ "play a track",
  /* hlp */
  "Play a track.\n"
  "\n"
  "OPTION\n"
  "   -s --seek=POS     Seek to this position.\n"
  "   -t --to=POS       End position.\n"
  "   -f --fg           Foreground play.\n"
  "\n"
  "POS := [+|-]ms | [+|-]mm:ss[:ms]\n"
  "  '+' is relative to start position.\n"
  "  '-' is relative to end position.\n"
  "  no prefix is absolute time posiiton.\n"
  "  The value can be express by either a single integer in milliseconds\n"
  "  or by a mm:ss,ms form where the \",ms\" part is optionnal.\n"
};

static
int run_stop(cmd_t * cmd, int argc, char ** argv)
{
  if (argc > 1)
    msgwrn("%d extra parameters ignored\n", argc-1);
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
  "The `stop' command stops a background play as started by `play -b'."
};
