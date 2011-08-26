/* $Id$ */

/* Time-stamp: <2009-10-17 06:02:10 ben>  */

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
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

#include "sc68/file68.h"
#include "sc68/alloc68.h"
#include "sc68/istream68.h"
#include "sc68/sc68.h"

#include "emu68/emu68.h"
#include "emu68/excep68.h"
#include "io68/io68.h"


#define MAX_TIME     (60 * 60 * 1000)   /*  */
#define SILENCE_TIME (5 * 1000)         /*  */
#define PASS_TIME    (3 * 60 * 1000)    /*  */

static const opt_t longopts[] = {
  { "tracks",     1, 0, 't' },          /* track-list              */
  { "max-time",   1, 0, 'M' },          /* max search time         */
  { "pass-time",  1, 0, 'p' },          /* search pass time        */
  { "silent",     1, 0, 's' },          /* silent detection length */
  { 0,0,0,0 }
};


static const unsigned int atarist_clk = (8010613u&~3u);

static unsigned int cycle_per_frame(unsigned int hz, unsigned int clk)
{
  if (!hz || hz == 50)
    return 160256;
  else if (hz == 60)
    ;
  return (clk ? clk : atarist_clk) / hz;
}

static unsigned int fr2ms(unsigned int fr, unsigned int cpf, unsigned int clk)
{
  uint64_t ms;
  ms =  fr;                                  /* total frames. */
  ms *= cpf ? cpf : cycle_per_frame(0, clk); /* total cycles. */
  ms *= 1000;
  ms /= clk ? clk : atarist_clk;
  return (unsigned int) ms;
}

static unsigned int ms2fr(unsigned int ms, unsigned int cpf, unsigned int clk)
{
  uint64_t fr;
  fr  = ms;                             /* number of millisecond */
  fr *= clk ? clk : atarist_clk;        /* number of millicycle  */
  fr /= 1000u;                          /* number of cycle       */
  fr /= cpf;                            /* number of frame       */
  return (unsigned int) fr;
}

typedef struct measureinfo_s measureinfo_t;

struct measureinfo_s {
  int           code;       /* return code */
  volatile int  isplaying;  /* 0:stoped 1:playing 2:init 3:shutdown */
  pthread_t     thread;     /* thread instance.  */

  int           track;      /* track to measure. */
  sc68_t      * sc68;       /* sc68 instance.    */
  emu68_t     * emu68;      /* emu68 instance.   */
  io68_t     ** ios68;      /* other chip.       */

  unsigned int sampling;                /* sampling rate (in hz) */
  unsigned int replayhz;                /* replay rate (in hz)   */
  unsigned int location;                /* memory start address  */

  unsigned int max_ms;                  /* maximum search time */
  unsigned int stp_ms;                  /* search depth increment */
  unsigned int sil_ms;                  /* length of silence */

  /* results */
  addr68_t minaddr;     /* lower memory location used by htis track */
  addr68_t maxaddr;     /* upper memory location used by htis track */
  unsigned frames;      /* last frame of the music.                 */

  unsigned curfrm;      /* current frame counter.                   */

  unsigned int ym:1;              /* YM used by this music          */
  unsigned int mw:1;              /* mw used by this music          */
  unsigned int ta:1;              /* MFP Timer-A used by this track */
  unsigned int tb:1;              /* MFP Timer-B used by this track */
  unsigned int tc:1;              /* MFP Timer-C used by this track */
  unsigned int td:1;              /* MFP Timer-D used by this track */
  unsigned int pl:1;              /* Amiga/Paulaused by this track  */

  struct {
    unsigned int cnt;
    unsigned int fst;
    unsigned int lst;
  } vector[260];

};

static measureinfo_t measureinfo;

enum timer_e {
  TIMER_D = 0X110>>2,
  TIMER_C = 0X114>>2,
  TIMER_B = 0X120>>2,
  TIMER_A = 0X134>>2,
};

static const char * vectorname(int vector)
{
  static char tmp[64];


  switch (vector) {
  case HWBREAK_VECTOR:  return "hardware breakpoint";
  case HWTRACE_VECTOR:  return "hardware trace";
  case HWHALT_VECTOR:   return "processor halted";

  case RESET_SP_VECTOR:
  case RESET_PC_VECTOR: return "reset";

  case BUSERR_VECTOR:   return "bus error";
  case ADRERR_VECTOR:   return "address error";
  case ILLEGAL_VECTOR:  return "illegal";
  case DIVIDE_VECTOR:   return "divide by zero";
  case CHK_VECTOR:      return "chk";
  case TRAPV_VECTOR:    return "trapv";

  case PRIVV_VECTOR:    return "privilege violation";
  case TRACE_VECTOR:    return "trace";
  case LINEA_VECTOR:    return "line-A";
  case LINEF_VECTOR:    return "line-f";
  case SPURIOUS_VECTOR: return "spurious interrupt";

    /* Not always true, depends on MFP VR register bits 4-7 */
  case TIMER_D:         return "MFP timer-D";
  case TIMER_C:         return "MFP timer-C";
  case TIMER_B:         return "MFP timer-B";
  case TIMER_A:         return "MFP timer-A";

  default:
    if (vector >= TRAP_VECTOR(0) && vector <= TRAP_VECTOR(15)) {
      sprintf(tmp,"trap #%d",vector-TRAP_VECTOR_0);
    } else if (vector >= AUTO_VECTOR(0) && vector <= AUTO_VECTOR(7)) {
      sprintf(tmp,"auto vector #%d",vector-AUTO_VECTOR(0));
    } else {
      sprintf(tmp,"unknown vector #%d",vector);
    }
  }

  return tmp;
}


static int timemeasure_hdl(emu68_t* const emu68, int vector, void * cookie)
{
  measureinfo_t * mi = cookie;
  assert(mi == &measureinfo);
  if (vector >= 0 && vector < sizeof(mi->vector)/sizeof(*mi->vector)) {
    if (!mi->vector[vector].cnt ++)
      mi->vector[vector].fst = mi->curfrm;
    else
      mi->vector[vector].lst = mi->curfrm;
  }

/*   msgdbg("exception: vector=%02X\n",vector); */
  return 0;                             /* 0:continue in normal mode */
}

extern void sc68_emulators(sc68_t *, emu68_t **, io68_t ***);

/* Find memory access range */
static void access_range(measureinfo_t * mi)
{
  unsigned i,j;
  unsigned stack;
  unsigned start = mi->location & mi->emu68->memmsk;
  unsigned endsp = ( mi->emu68->reg.a[7] - 1 ) & mi->emu68->memmsk;

  struct {
    unsigned min, max;
  } mod[4];

  for ( stack = endsp;
        stack >= start && ( mi->emu68->chk[stack] & ( EMU68_R | EMU68_W ) );
        --stack)
    ;
  if (stack != endsp)
    msginf("S access range: 0x%06X .. 0x%06X\n",
           stack, mi->emu68->reg.a[7]& mi->emu68->memmsk);
  else
    msginf("S access range: none\n");

  for (j=0; j<4; ++j) {
    if (j < 3) {
      mod[j].min = stack;
      mod[j].max = start;
      for ( i = start; i < stack ; i++ ) {
        if ( mi->emu68->chk[i] & ( EMU68_R << j ) ) {
          if ( i < mod[j].min ) mod[j].min = i;
          if ( i > mod[j].max ) mod[j].max = i;
        }
      }
      if (!j) {
        mod[3].min = mod[j].min;
        mod[3].max = mod[j].max;
      } else {
        if ( mod[j].min < mod[3].min ) mod[3].min = mod[j].min;
        if ( mod[j].max > mod[3].max ) mod[3].max = mod[j].max;
      }
    }
    if ( mod[j].min < mod[j].max )
      msginf("%c access range: 0x%06X .. 0x%06X (%d bytes)\n",
             j["RWXA"],mod[j].min, mod[j].max, mod[j].max-mod[j].min+1);
    else
      msginf("%c access range: none\n", j["RWXA"]);
  }

  mi->minaddr = mod[3].min;
  mi->maxaddr = mod[3].max;

  if (0) {
    for (i=0 ; i<mi->emu68->memmsk+1; ++i) {
      if (!(i&15))
        printf ("%06x ",i);
      printf ("%c%c%c%c",
              (mi->emu68->chk[i]&EMU68_R)?'R':'.',
              (mi->emu68->chk[i]&EMU68_W)?'W':'.',
              (mi->emu68->chk[i]&EMU68_X)?'X':'.',
              (i&15)==15 ? '\n' : ' ');
    }
  }


}

static void timemeasure_init(measureinfo_t * mi)
{
  sc68_create_t create68;
  disk68_t * disk;
  int sampling;

  mi->isplaying = 2;
  mi->code      = -1;

  memset(&create68,0,sizeof(create68));
  create68.name        = "mksc68-time";
  create68.emu68_debug = 1;
  mi->sc68 = sc68_create(&create68);
  if (!mi->sc68)
    return;

  sc68_emulators(mi->sc68, &mi->emu68, &mi->ios68);
  emu68_set_handler(mi->emu68, timemeasure_hdl);
  emu68_set_cookie(mi->emu68, mi);

  disk = dsk_get_disk();
  if (sc68_open(mi->sc68, disk) < 0)
    return;
  mi->replayhz = disk->mus[mi->track-1].frq;
  mi->location = disk->mus[mi->track-1].a0;

  if (sc68_play(mi->sc68, mi->track, 0) < 0)
    return;

  if (sc68_process(mi->sc68,0,0) == SC68_ERROR)
    return;

  sampling = sc68_sampling_rate(mi->sc68, SC68_SPR_QUERY);
  if (sampling <= 0)
    return;
  mi->sampling = sampling;

  mi->code = 0;
}



static void timemeasure_run(measureinfo_t * mi)
{
  int32_t buf[32];
  const int slice = sizeof(buf)/sizeof(*buf);
  int code;
  int i,lst=0;
  unsigned acu;
  unsigned frm_cnt;           /* frame counter                      */
  unsigned frm_max;           /* maximum frame to try to detect end */
  unsigned frm_lim;           /* frame limit for this pass          */
  unsigned frm_stp;           /* frame limit increment              */
  unsigned upd_frm;           /* last updated frame                 */
  unsigned cpf;               /* 68k cycle per frame                */
  unsigned sil_val;           /* pcm value                          */
  unsigned sil_len;
  unsigned sil_frm;           /* first silent frame                 */
  unsigned sil_nop = ~0;      /* special value for no silent        */
  unsigned sil_max;           /* frames to concider real silence    */
  int      updated;           /* has been updated this pass ?       */

  mi->isplaying = 1;
  mi->code      = -1;


  msgdbg("time-measure: run [track:%d max:%d stp:%d sil:%d spr:%u hz:%u]\n",
         mi->track, mi->max_ms, mi->stp_ms, mi->sil_ms,
         mi->sampling, mi->replayhz);

  frm_cnt = 0;
  sil_len = 0;
  sil_val = 20 * slice * 2;
  sil_frm = sil_nop;

  cpf = cycle_per_frame( mi->replayhz, mi->emu68->clock );

  sil_max = ms2fr(mi->sil_ms, cpf, mi->emu68->clock);
  frm_max = ms2fr(mi->max_ms, cpf, mi->emu68->clock);
  frm_stp = ms2fr(mi->stp_ms, cpf, mi->emu68->clock);
  frm_lim = frm_stp;
  upd_frm = sil_nop;
  updated = 0;

  msgdbg("time search:\n"
         " max: %u fr, %u ms\n"
         " stp: %u fr, %u ms\n"
         " sil: %u fr, %u ms\n",
         (unsigned) frm_max, mi->max_ms,
         (unsigned) frm_stp, mi->stp_ms,
         (unsigned) sil_max, mi->sil_ms);

  while ( code = sc68_process(mi->sc68,buf,slice),
          code != SC68_ERROR ) {

    /* Compute the sum of deltas for silence detection */
    for (acu=i=0; i<slice; ++i) {
      const int val
        = ( (int)(s16)buf[i] ) + ( (int)(s16)(buf[i]>>16) );
      const int dif = val - lst;
      acu += (dif >= 0) ? dif : -dif;
      lst = val;
    }

    if ( acu >= sil_val ) {
      if ( sil_frm != sil_nop ) {
        unsigned int sil_lll = frm_cnt - sil_frm;
        if (sil_lll > sil_len ) {
          sil_len = sil_lll;
          msgdbg("silence length: %u frames (%u ms) at frame %u (%u ms)\n",
                 sil_len, fr2ms(sil_len, cpf, mi->emu68->clock),
                 sil_frm, fr2ms(sil_frm, cpf, mi->emu68->clock) );
        }
      }
      sil_frm = sil_nop;                /* Not silent slice */
    } else if ( sil_frm == sil_nop )
      sil_frm = frm_cnt;

    if ( sil_frm != sil_nop && frm_cnt - sil_frm > sil_max ) {
      /* silence detected !!! */
      msgdbg("silence detected at frame %u (%u ms)\n",
             (unsigned) sil_frm, fr2ms(sil_frm, cpf, mi->emu68->clock));
      mi->frames = sil_frm;
      break;
    }

    /* If no emulation pass has been run by sc68_process() we are done here */
    if (code & SC68_IDLE) continue;

    if (mi->emu68->framechk) {
      /* something as change here */
      upd_frm = frm_cnt;
      updated = 1;
      msgdbg("update detected at frame %d (%u ms)\n",
             (unsigned) upd_frm, fr2ms(upd_frm, cpf, mi->emu68->clock));
    }

    /* That was a new frame. Let's do the memory access checking trick. */
    mi->curfrm = ++frm_cnt;

    if ( frm_cnt > frm_max ) {
      msgerr("reach max limit (%ufr %ums)\n", frm_max, mi->max_ms);
      break;
    }

    if ( frm_cnt > frm_lim ) {
      if ( !updated && upd_frm != sil_nop ) {
        /* found something */
        msgdbg("end of track detected at frame %u (%u ms)\n",
               upd_frm,
               fr2ms(upd_frm, cpf, mi->emu68->clock));
        mi->frames = upd_frm;
        break;
      } else {
        msgdbg("unable to detect end of track at frame %u (%u ms)\n",
               frm_lim, fr2ms(frm_lim, cpf, mi->emu68->clock));
        frm_lim += frm_stp;
        updated = 0;
      }
    }
  }

  /* memory access */
  access_range(mi);

  /* timers use */
  mi->ta = mi->vector[TIMER_A].cnt > 0;
  mi->tb = mi->vector[TIMER_B].cnt > 0;
  mi->tc = mi->vector[TIMER_C].cnt > 0;
  mi->td = mi->vector[TIMER_D].cnt > 0;

  /* Display interrupt vectors */
  for (i=0; i<sizeof(mi->vector)/sizeof(*mi->vector); ++i)
    if (mi->vector[i].cnt)
      msginf("vector #%03d \"%s\" triggered %d times fist:%u last:%u\n",
             i, vectorname(i),
             mi->vector[i].cnt, mi->vector[i].fst, mi->vector[i].lst);

  if (mi->frames)
    mi->code = 0;

}

static void timemeasure_end(measureinfo_t * mi)
{
  mi->isplaying = 3;
  if (mi->sc68) {
    sc68_t * sc68 = mi->sc68;
    mi->sc68  = 0;
    mi->emu68 = 0;
    mi->ios68 = 0;
    sc68_destroy(sc68);
  }
  mi->isplaying = 0;
}

static void * time_thread(void * userdata)
{
  measureinfo_t * mi = (measureinfo_t *) userdata;

  assert( mi == &measureinfo );

  timemeasure_init(mi);
  if (!mi->code) timemeasure_run(mi);
  timemeasure_end(mi);

  return mi;
}


static
int time_measure(measureinfo_t * mi, int trk,
                 int stp_ms, int max_ms, int sil_ms )
{
  int ret = -1;
  int tracks = dsk_get_tracks();

  assert(mi);
  assert(trk > 0 && trk <= tracks);


  msgdbg("time_measure() trk:%d, stp:%d, max:%d sil:%d\n",
         trk, stp_ms, max_ms, sil_ms );

  if (trk <= 0 || trk > tracks) {
    msgerr("track #%02 out of range\n", trk);
    goto error;
  }

  if (mi->isplaying) {
    msgerr("busy\n");
    goto error;
  }

  memset(mi,0,sizeof(*mi));
  mi->stp_ms = stp_ms;
  mi->max_ms = max_ms;
  mi->sil_ms = sil_ms;
  mi->track  = trk;

  if ( pthread_create(&mi->thread, 0, time_thread, mi) ) {
    msgerr("failed to create time thread\n");
    goto error;
  }
  msgdbg("time-measure thread created ... Waiting\n");
  pthread_join(mi->thread, 0);
  msgdbg("time-measure thread ended with: %d\n", mi->code);
  ret = mi->code;
  memset(mi,0,sizeof(*mi));

error:
  return ret;
}

static
int run_time(cmd_t * cmd, int argc, char ** argv)
{
  char shortopts[(sizeof(longopts)/sizeof(*longopts))*3];
  int ret = -1, i, track, tracks;
  const char * tracklist = 0;
  int /* seek_mode = 0,seek, dur_mode, duration, */ a, b;

  int max_ms = MAX_TIME, sil_ms = SILENCE_TIME, stp_ms = PASS_TIME;


  opt_create_short(shortopts, longopts);

  while (1) {
    int longindex;
    int val =
      getopt_long(argc, argv, shortopts, longopts, &longindex);

    switch (val) {
    case  -1: break;                    /* Scan finish */

    case 't':                           /* --tracks    */
      if (tracklist) {
        msgerr("multiple track-list not allowed.\n");
        goto error;
      }
      tracklist = optarg;
      /* check track-list syntax */
      while ( ret = str_tracklist(&tracklist, &a, &b), ret > 0 ) {
        msgdbg("track interval %d-%d\n",a,b);
      }
      if (ret)
        goto error;
      tracklist = optarg;
      break;

    case 'M':                           /* --max-time  */
      if (str_time_stamp((const char**)&optarg, &max_ms))
        goto error;
      break;
    case 's':                           /* --silent    */
      if (str_time_stamp((const char**)&optarg, &sil_ms))
        goto error;
      break;
    case 'p':                           /* --pass-time */
      if (str_time_stamp((const char**)&optarg, &stp_ms))
        goto error;
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

  if (!tracklist) {
    ret = time_measure(&measureinfo, track, stp_ms, max_ms, sil_ms);
  } else {
    ret = 0;
    while (!ret && str_tracklist(&tracklist, &a, &b) > 0 )
      for (track = a; !ret && track <= b; ++track)
        ret = time_measure(&measureinfo, a, stp_ms, max_ms, sil_ms);
  }

error:
  return ret;
}

cmd_t cmd_time = {
  /* run */ run_time,
  /* com */ "time",
  /* alt */ 0,
  /* use */ "[OPTION ...]]",
  /* des */ "Autodetect track duration",
  /* hlp */
  "Autodetect and update tracks duration.\n"
  "\n"
  "OPTION\n"
  "   -t --tracks=TRACKS  List of tracks (def:current).\n"
  "   -s --silent=MS      Duration for silent detection (0:disable).\n"
  "   -M --max-time=MS    Maximum time.\n"
  "   -p --pass-time=MS   Search pass duration.\n"
  "\n"
};
