/*
 * @file    mksc68_cmd_time.c
 * @brief   the "time" command
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-07-11 21:20:04 ben>
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

#include <sc68/file68.h>
#include <sc68/alloc68.h>
#include <sc68/istream68.h>
#include <sc68/sc68.h>
#include <emu68/emu68.h>
#include <emu68/excep68.h>
#include <io68/io68.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

#define MAX_TIME     (60 * 60 * 1000)   /*  */
#define SILENCE_TIME (5 * 1000)         /*  */
#define PASS_TIME    (3 * 60 * 1000)    /*  */

static const opt_t longopts[] = {
  { "help",       0, 0, 'h' },
  { "tracks",     1, 0, 't' },          /* track-list              */
  { "max-time",   1, 0, 'M' },          /* max search time         */
  { "pass-time",  1, 0, 'p' },          /* search pass time        */
  { "silent",     1, 0, 's' },          /* silent detection length */
  { 0,0,0,0 }
};


#ifndef EMU68_ATARIST_CLOCK
# define EMU68_ATARIST_CLOCK (8010613u&~3u)
#endif
static const unsigned int atarist_clk = EMU68_ATARIST_CLOCK;

static unsigned int cycle_per_frame(unsigned int hz, unsigned int clk)
{
  hz  = hz  ? hz  : 50u;
  clk = clk ? clk : atarist_clk;
  return
    ( hz % 50u == 0 && clk == atarist_clk)
    ? 160256u * 50u / hz
    : clk / hz
    ;
}

unsigned int fr2ms(unsigned int fr, unsigned int cpf_or_hz, unsigned int clk)
{
  uint64_t ms;
  unsigned cpf;
  clk = clk ? clk : atarist_clk;
  cpf = (cpf_or_hz <= 1000u)
    ? cycle_per_frame(cpf_or_hz, clk)
    : cpf_or_hz
    ;

  ms  = fr;                             /* total frames. */
  ms *= cpf;                            /* total cycles. */
  ms *= 1000u;                          /* result in ms  */
  ms += clk >> 1;
  ms /= clk;

  return (unsigned int) ms;
}

static unsigned int ms2fr(unsigned int ms, unsigned int cpf_or_hz, unsigned int clk)
{
  uint64_t fr;
  unsigned cpf;
  clk = clk ? clk : atarist_clk;
  cpf = (cpf_or_hz <= 1000u)
    ? cycle_per_frame(cpf_or_hz, clk)
    : cpf_or_hz
    ;

  fr   = ms;                             /* number of millisecond */
  fr  *= clk;                            /* number of millicycle  */
  cpf *= 1000u;
  fr  += cpf >> 1;
  fr  /= cpf;                            /* number of milliframe  */

  return (unsigned int) fr;
}

typedef struct
{
  io68_t   io;
  unsigned r;
  unsigned w;
  unsigned a;
  io68_t * pio;
} time_io_t;

static void time_rb(io68_t * pio) {
  time_io_t * ti = (time_io_t *)pio;
  ++ti->r;
  ++ti->a;
  ti->pio->r_byte(ti->pio);
}

static void time_rw(io68_t * pio) {
  time_io_t * ti = (time_io_t *)pio;
  ++ti->r;
  ++ti->a;
  ti->pio->r_word(ti->pio);
}

static void time_rl(io68_t * pio) {
  time_io_t * ti = (time_io_t *)pio;
  ++ti->r;
  ++ti->a;
  ti->pio->r_long(ti->pio);
}


static void time_wb(io68_t * pio) {
  time_io_t * ti = (time_io_t *)pio;
  ++ti->w;
  ++ti->a;
  ti->pio->w_byte(ti->pio);
}

static void time_ww(io68_t * pio) {
  time_io_t * ti = (time_io_t *)pio;
  ++ti->w;
  ++ti->a;
  ti->pio->w_word(ti->pio);
}

static void time_wl(io68_t * pio) {
  time_io_t * ti = (time_io_t *)pio;
  ++ti->w;
  ++ti->a;
  ti->pio->w_long(ti->pio);
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

  unsigned sampling;        /* sampling rate (in hz) */
  unsigned replayhz;        /* replay rate (in hz)   */
  unsigned location;        /* memory start address  */

  unsigned startfr;         /* starting frame (loop)  */
  unsigned startms;         /* corrsponding ms        */

  unsigned max_ms;          /* maximum search time    */
  unsigned stp_ms;          /* search depth increment */
  unsigned sil_ms;          /* length of silence      */

  /* results */
  addr68_t minaddr;     /* lower memory location used by htis track */
  addr68_t maxaddr;     /* upper memory location used by htis track */
  unsigned frames;      /* last frame of the music                  */
  unsigned timems;      /* corresponding time in ms                 */
  unsigned loopfr;      /* loop duration in frames                  */
  unsigned loopms;      /* correponding time in ms                  */
  unsigned curfrm;      /* current frame counter                    */
  unsigned ym:1;        /* YM used by this music                    */
  unsigned mw:1;        /* mw used by this music                    */
  unsigned ta:1;        /* MFP Timer-A used by this track           */
  unsigned tb:1;        /* MFP Timer-B used by this track           */
  unsigned tc:1;        /* MFP Timer-C used by this track           */
  unsigned td:1;        /* MFP Timer-D used by this track           */
  unsigned pl:1;        /* Amiga/Paulaused by this track            */
  time_io_t timeios[5]; /* hooked IOs                               */
  struct {
    unsigned cnt;
    unsigned fst;
    unsigned lst;
  } vector[260];

};

static measureinfo_t measureinfo;

enum {
  YM = 0,
  MW,
  SHIFTER,
  PAULA,
  MFP
};

enum timer_e {
  TIMER_A = 0X134 >> 2,
  TIMER_B = 0X120 >> 2,
  TIMER_C = 0X114 >> 2,
  TIMER_D = 0X110 >> 2
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
  case TIMER_A:         return "MFP timer-A";
  case TIMER_B:         return "MFP timer-B";
  case TIMER_C:         return "MFP timer-C";
  case TIMER_D:         return "MFP timer-D";

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
    mi->vector[vector].lst = mi->curfrm;
  }
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

/* Timers use */
static void access_timers(measureinfo_t * mi)
{
  mi->ta = mi->vector[TIMER_A].cnt > 0;
  mi->tb = mi->vector[TIMER_B].cnt > 0;
  mi->tc = mi->vector[TIMER_C].cnt > 0;
  mi->td = mi->vector[TIMER_D].cnt > 0;
}

/* IO chip access */
static void access_ios(measureinfo_t * mi)
{
  mi->ym = mi->timeios[YM].a    > 0;
  mi->pl = mi->timeios[PAULA].a > 0;
  mi->mw = mi->timeios[MW].a    > 0;
}

static int hook_ios(measureinfo_t * mi)
{
  int i;

  for (i=0; i<5; ++i) {
    io68_t    * pio = mi->ios68[i], * mio;
    int        line = (pio->addr_lo>>8) & 0xFF;
    time_io_t * ti  = mi->timeios+i;

    mio = mi->emu68->mapped_io[line];
    ti->pio = mio;                      /* legacy mapped io */
    ti->r = ti->w = 0;                  /* reset counters   */

    /* create new hooked io */
    snprintf(ti->io.name,sizeof(ti->io.name),"H: %s", mio->name);
    ti->io.addr_lo = pio->addr_lo;
    ti->io.addr_hi = pio->addr_hi;

    ti->io.r_byte = time_rb;            /* hook memory access handler */
    ti->io.r_word = time_rw;
    ti->io.r_long = time_rl;
    ti->io.w_byte = time_wb;
    ti->io.w_word = time_ww;
    ti->io.w_long = time_wl;
    ti->io.emu68  = mi->emu68;

    if ( mi->emu68->mapped_io[line] == pio )
      msgdbg("hook io #%d '%-20s' addr:$%06x-%06x line:%02x\n",
             i, ti->io.name, pio->addr_lo & 0xFFFFFF , pio->addr_hi & 0xFFFFFF, line);
    else
      msgdbg("hook ?? #%d '%-20s' addr:$%06x-%06x line:%02x\n",
             i, ti->io.name, mio->addr_lo & 0xFFFFFF , mio->addr_hi & 0xFFFFFF, line);

    mi->emu68->mapped_io[line] = &ti->io;
  }
  return 0;
}

static int unhook_ios(measureinfo_t * mi)
{
  int i;

  for (i=0; i<5; ++i) {
    io68_t    * pio = mi->ios68[i];
    int        line = (pio->addr_lo>>8) & 0xFF;
    time_io_t * ti  = mi->timeios+i;

    msgdbg("unhook io #%d '%s' addr:$%06x-%06x line:%02x, counts:%u (%u/%u)\n",
           i, ti->io.name, ti->io.addr_lo & 0xFFFFFF , ti->io.addr_hi & 0xFFFFFF,
           line, ti->r+ti->w, ti->r, ti->w);
    if (mi->emu68->mapped_io[line] != &ti->io) {
      io68_t * mio = mi->emu68->mapped_io[line];
      io68_t * tio = &ti->io;

      msgerr("%p != %p\n"
             "i:%d\n"
             "line:%02x\n"
             "pio: %p '%s' addr:$%06x-%06x\n"
             "tio: %p '%s' addr:$%06x-%06x\n"
             "mio: %p '%s' addr:$%06x-%06x\n"
             ,
             mi->emu68->mapped_io[line], &ti->io,
             i,
             line,
             pio, pio->name, pio->addr_lo, pio->addr_hi,
             tio, tio->name, tio->addr_lo, tio->addr_hi,
             mio, mio->name, mio->addr_lo, mio->addr_hi );
    }

    assert(mi->emu68->mapped_io[line] == &ti->io);
    mi->emu68->mapped_io[line] = ti->pio;
  }
  return 0;
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

  if (sc68_play(mi->sc68, mi->track, SC68_INF_LOOP) < 0)
    return;

  if (sc68_process(mi->sc68,0,0) == SC68_ERROR)
    return;

  if (hook_ios(mi))
    return;

  sampling = sc68_sampling_rate(mi->sc68, SC68_SPR_QUERY);
  if (sampling <= 0)
    return;
  mi->sampling = sampling;

  mi->code = 0;
}


static void timemeasure_run(measureinfo_t * mi)
{
  char   str[256];
  int8_t buf[32 << 2];
  const int slice = sizeof(buf) >> 2;
  int code, n;
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

  cpf = cycle_per_frame( mi->replayhz, mi->emu68->clock );

  frm_cnt = 0;
  if (mi->startfr) {
    mi->startms = fr2ms(mi->startfr, cpf, mi->emu68->clock);
    msgdbg("time-measure: now measuring loop, skipping %u frames (%s)\n",
           mi->startfr, str_timefmt(str+0x00,0x20, mi->startms));
    while (n = slice,
           code = sc68_process(mi->sc68,buf,&n),
           code != SC68_ERROR ) {
      if (code & SC68_IDLE)
        continue;
      if (code & (SC68_CHANGE|SC68_END))
        msgwrn("sc68_process() returns an wrong return code (%x) at frame %u (%s)\n",
               code, frm_cnt, str_timefmt(str,32,fr2ms(frm_cnt, cpf, mi->emu68->clock)));
      assert( (code & (SC68_CHANGE|SC68_END)) == 0 );
      if (++frm_cnt == mi->startfr)
        break;
    }
    emu68_chkset(mi->emu68, 0, 0, 0);   /* reset memory access flags */
    msgdbg("time-measure: %u frames skipped\n", frm_cnt);
  }

  sil_len = 0;
  sil_val = 20 * slice * 2;
  sil_frm = sil_nop;

  sil_max = ms2fr(mi->sil_ms, cpf, mi->emu68->clock);
  frm_max = frm_cnt + ms2fr(mi->max_ms, cpf, mi->emu68->clock);
  frm_stp = ms2fr(mi->stp_ms, cpf, mi->emu68->clock);
  frm_lim = frm_cnt + frm_stp;
  upd_frm = sil_nop;
  updated = 0;

  msgdbg("time search:\n"
         " max: %u fr, %s\n"
         " stp: %u fr, %s\n"
         " sil: %u fr, %s\n",
         (unsigned) frm_max, str_timefmt(str+0x00,0x20,mi->max_ms),
         (unsigned) frm_stp, str_timefmt(str+0x20,0x20,mi->stp_ms),
         (unsigned) sil_max, str_timefmt(str+0x40,0x20,mi->sil_ms));

  while (n = slice,
         code = sc68_process(mi->sc68,buf,&n),
         code != SC68_ERROR ) {

    if (code & (SC68_CHANGE|SC68_END))
      msgwrn("sc68_process() returns an wrong return code (%x) at frame %u (%s)\n",
             code, frm_cnt, str_timefmt(str,32,fr2ms(frm_cnt, cpf, mi->emu68->clock)));
    assert( (code & (SC68_CHANGE|SC68_END)) == 0 );

    /* Compute the sum of deltas for silence detection */
    for (acu=i=0; i<n; ++i) {
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
          msgdbg("silence length: %u frames (%s) at frame %u (%s)\n",
                 sil_len, str_timefmt(str+0x00,0x20,fr2ms(sil_len, cpf, mi->emu68->clock)),
                 sil_frm, str_timefmt(str+0x00,0x20,fr2ms(sil_frm, cpf, mi->emu68->clock)));
        }
      }
      sil_frm = sil_nop;                /* Not silent slice */
    } else if ( sil_frm == sil_nop )
      sil_frm = frm_cnt;

    if ( sil_max > 0 && sil_frm != sil_nop && frm_cnt - sil_frm > sil_max ) {
      /* silence detected !!! */
      msgdbg("silence detected at frame %u (%s)\n",
             (unsigned) sil_frm,
             str_timefmt(str+0x00,0x20,fr2ms(sil_frm, cpf, mi->emu68->clock)));
      mi->frames = sil_frm;
      mi->loopfr = ~0;
      break;
    }

    /* If no emulation pass has been run by sc68_process() we are done here */
    if (code & SC68_IDLE) continue;

    if (mi->emu68->framechk) {
      /* something as change here */
      upd_frm = frm_cnt;
      updated = 1;
      /* msgdbg("update detected at frame %d (%s)\n", */
      /*        (unsigned) upd_frm, */
      /*        str_timefmt(str+0x00,0x200,fr2ms(upd_frm, cpf, mi->emu68->clock))); */
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
        msgdbg("end of track detected at frame %u (%s)\n",
               upd_frm,
               str_timefmt(str+0x00,0x20,fr2ms(upd_frm, cpf, mi->emu68->clock)));
        mi->frames = upd_frm;
        break;
      } else {
        msgdbg("unable to detect end of track at frame %u (%s)\n",
               frm_lim,
               str_timefmt(str+0x00,0x20,fr2ms(frm_lim, cpf, mi->emu68->clock)));
        frm_lim += frm_stp;
        updated = 0;
      }
    }
  }

  /* Don't want to do this while computing loops */
  if (!mi->startms) {
    /* memory access */
    access_range(mi);
    access_timers(mi);
    access_ios(mi);

    /* Display interrupt vectors */
    for (i=0; i<sizeof(mi->vector)/sizeof(*mi->vector); ++i)
      if (mi->vector[i].cnt)
        msginf("vector #%03d \"%s\" triggered %d times fist:%u last:%u\n",
               i, vectorname(i),
               mi->vector[i].cnt, mi->vector[i].fst, mi->vector[i].lst);
  }

  if (mi->frames) {
    unsigned frames, timems;

    frames = mi->frames - mi->startfr;
    timems = fr2ms(frames, cpf, mi->emu68->clock);

    msgdbg("this pass -- %u fr (%s)\n",
           frames, str_timefmt(str, 32, timems));

    if (!mi->startfr) {
      mi->frames = frames;
      mi->timems = timems;
      if (mi->loopfr == ~0) {
        msgdbg("silence was detected, set loop length to 0\n");
        mi->loopfr = mi->loopms = 0;
      } else {
        mi->loopfr = mi->frames;
        mi->loopms = mi->timems;
      }
    } else {
      msgdbg("2nd pass set time and loop to %ufr (%s)\n",
             mi->startfr, str_timefmt(str, 32, mi->startms));
      mi->frames = mi->startfr;
      mi->timems = mi->startms;
      mi->loopfr = frames;
      mi->loopms = timems;
    }
    msgdbg("set time: %ufr (%s) -- loop: %u fr (%s)\n",
           mi->frames, str_timefmt(str, 32, mi->timems),
           mi->loopfr, str_timefmt(str, 32, mi->loopms) );
    mi->code   = 0;
  }

}

static void timemeasure_end(measureinfo_t * mi)
{
  mi->isplaying = 3;
  if (mi->sc68) {
    sc68_t * sc68 = mi->sc68;
    unhook_ios(mi);
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
  if (!mi->code)
    timemeasure_run(mi);
  timemeasure_end(mi);

  if (!mi->code && mi->loopfr) {
    unsigned startfr = mi->frames;
    timemeasure_init(mi);
    if (!mi->code) {
      mi->startfr = startfr;
      timemeasure_run(mi);
    }
    timemeasure_end(mi);
  }

  return mi;
}


static
int time_measure(measureinfo_t * mi, int trk,
                 int stp_ms, int max_ms, int sil_ms)
{
  int ret = -1, err;
  int tracks = dsk_get_tracks();
  struct timespec ts;
  ts.tv_sec  = 180;
  ts.tv_nsec = 0;

  msgdbg("time_measure() trk:%d, stp:%dms, max:%dms sil:%dms, time-out:%d\"\n",
         trk, stp_ms, max_ms, sil_ms, ts.tv_sec);

  assert(mi);
  assert(trk > 0 && trk <= tracks);

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

#ifdef HAVE_PTHREAD_TIMEDJOIN_NP
  msgdbg("time-measure thread created ... Waiting for %d seconds\n", ts.tv_sec);
  ts.tv_sec += time(0);
  err = pthread_timedjoin_np(mi->thread, 0, &ts);
  if (err == ETIMEDOUT) {
    msgdbg("time-measure thread time-out, canceling\n");
    err = pthread_cancel(mi->thread);
  }
#else
  msgdbg("time-measure thread created ... Waiting for %d seconds\n", ts.tv_sec);
  err = pthread_join(mi->thread, 0);
#endif
  msgdbg("time-measure thread ended with: %d, %d\n", err, mi->code);
  ret = err ? -1 : mi->code;
  if (!ret) {
    disk68_t  * d = dsk_get_disk();
    music68_t * m = d->mus+trk-1;
    char      s1[32],s2[32],s3[32],s4[32];
    unsigned int total_fr, total_ms;


    m->first_fr = mi->frames;
    m->first_ms = mi->timems;
    m->loops    = 1;
    m->loops_fr = mi->loopfr;
    m->loops_ms = mi->loopms;
    /* m-> */total_fr = m->first_fr + (m->loops-1) * m->loops_fr;
    /* m-> */total_ms = fr2ms(/* m-> */total_fr, m->frq, 0);
    m->has.time = 1;
    m->has.loop = 1;

    m->hwflags.all           = 0;
    m->hwflags.bit.ym        = mi->ym;
    m->hwflags.bit.ste       = mi->mw;
    m->hwflags.bit.amiga     = mi->pl;
    m->hwflags.bit.stechoice = 0;
    m->hwflags.bit.timers    = 1;
    m->hwflags.bit.timera    = mi->ta;
    m->hwflags.bit.timerb    = mi->tb;
    m->hwflags.bit.timerc    = mi->tc;
    m->hwflags.bit.timerd    = mi->td;

    /* mi->minaddr; */
    /* mi->maxaddr; */

    msginf("%02u - %s + %d x %s => %s [%s]\n",
           trk,
           str_timefmt(s1,sizeof(s1),m->first_ms),
           m->loops-1,
           m->loops_ms ? str_timefmt(s2,sizeof(s2),m->loops_ms) : "no loop",
           str_timefmt(s3,sizeof(s3),/* m-> */total_ms),
           str_hardware(s4,sizeof(s4),m->hwflags.all)
      );
  }
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
    case 'h':                           /* --help */
      help(argv[0]); return 0;

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
  if (i < argc)
    msgwrn("%d extra parameters ignored\n", argc-i);

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
    while (!ret && str_tracklist(&tracklist, &a, &b) > 0 ) {
      msgdbg("time: %d to %d\n",a,b);
      for (; !ret && a <= b; ++a) {
        ret = time_measure(&measureinfo, a, stp_ms, max_ms, sil_ms);
        msgdbg("time #%d -> %d\n",a,ret);
      }
    }
  }
  dsk_validate();

error:
  return ret;
}

cmd_t cmd_time = {
  /* run */ run_time,
  /* com */ "time",
  /* alt */ 0,
  /* use */ "[opts]",
  /* des */ "Autodetect track duration",
  /* hlp */
  "The `time' command run sc68 music emulator in a special way that allows\n"
  "to autodetect track duration.\n"
  "\n"
  "OPTIONS\n"
  "   -t --tracks=TRACKS  List of tracks (default is current track).\n"
  "   -s --silent=MS      Duration for silent detection (0:disable).\n"
  "   -M --max-time=MS    Maximum time.\n"
  "   -p --pass-time=MS   Search pass duration."
};
