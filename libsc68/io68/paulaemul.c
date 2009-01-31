/*
 *             sc68 - Paula emulator (Amiga soundchip)
 *             Copyright (C) 2001-2009 Benjamin Gerard
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
 * $Id$
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_CONFIG_OPTION68_H
# include <config_option68.h>
#else
# include <default_option68.h>
#endif

#include "io68/paulaemul.h"

/* #define PAULA_CALCUL_TABLE 1 */

#ifdef PAULA_CALCUL_TABLE
# include <math.h>
# include <stdio.h>
#endif

#define CT_FIX       ct_fix
#define PL_VOL_FIX   16
#define PL_VOL_MUL   (1<<PL_VOL_FIX)
#define PL_MIX_FIX   (PL_VOL_FIX+1+8-16) /*(PL_VOL_FIX+2+8-16) */

#ifdef PAULA_CALCUL_TABLE
# define LN_10_OVER_10 0.230258509299
# define PL_N_DECIBEL 65

static const s16 Tvol[] = {
  0, 1, 3, 4, 6, 7, 9, 10, 12, 13, 15, 16, 18, 20, 21, 23,
  25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 48, 50, 52, 55, 58,
  60, 63, 66, 69, 72, 75, 78, 82, 85, 89, 93, 97, 101, 105, 110,
  115, 120, 126, 132, 138, 145, 153, 161, 170, 181, 192, 206,
  221, 241, 266, 301, 361, 450
};
static unsigned int volume[65];

static unsigned int calc_volume(unsigned int vol, unsigned int mult)
{
  double r;

  if (!vol) {
    return 0;
  }
  r = (double) mult / exp((double) Tvol[64 - vol] / 100.0);
  return (unsigned int) r;
}

static void init_volume(void)
{
  int i;

  for (i = 0; i < 65; i++) {
    volume[i] = calc_volume(i, PL_VOL_MUL);
    fprintf(stderr, "XXX:%08x\n", volume[i]);
  }
}

#else

/* This table has been precalculated with the above piece of code. */
static const uint68_t volume[65] = {
  0x00000,0x006ec,0x00c9e,0x011e8,0x016fe,0x01c15,0x020a0,0x02588,
  0x029e5,0x02ec4,0x0332b,0x0376e,0x03c0c,0x04067,0x04462,0x0489d,
  0x04d1b,0x0510f,0x05537,0x05995,0x05d3d,0x0610b,0x06501,0x06920,
  0x06d6b,0x070c0,0x0755a,0x078ed,0x07c9b,0x08067,0x08450,0x08857,
  0x08c7e,0x08f55,0x093b2,0x09832,0x09b45,0x09e68,0x0a33b,0x0a687,
  0x0a9e4,0x0ad53,0x0b0d3,0x0b466,0x0b80b,0x0bbc3,0x0bf8e,0x0c36c,
  0x0c75f,0x0cb66,0x0cf82,0x0d198,0x0d5d4,0x0da26,0x0dc57,0x0e0ca,
  0x0e30d,0x0e7a3,0x0e9f7,0x0eeb1,0x0f117,0x0f5f6,0x0f86f,0x0fd73,
  0x10000,
};

static void init_volume(void) {}


#endif

static paula_parms_t default_parms = {
  PAULA_EMUL_SIMPLE,SAMPLING_RATE_DEF,PAULA_CLOCK_PAL
};

static int msw_first = 0; /* big/little endian compliance */

/* ,-----------------------------------------------------------------.
 * |			     Paula init                             |
 * `-----------------------------------------------------------------'
 */

static volatile u32 tmp = 0x1234;

int paula_init(paula_parms_t * const parms_data)
{
  paula_parms_t * parms = parms_data ? parms_data : &default_parms;

  /* Setup little/big endian swap */
  msw_first = !(*(u16 *)&tmp);

  /* Default emulation mode */
  if (parms->emul == PAULA_EMUL_DEFAULT) {
    parms->emul = default_parms.emul;
  }

  /* Default sampling rate */
  if (!parms->hz) {
    parms->hz = default_parms.hz;
  }

  /* Default clock mode */
  if (parms->clock == PAULA_CLOCK_DEFAULT) {
    parms->clock = default_parms.clock;
  }

  /* Init volume table. */
  init_volume();

  return 0;
}

void paula_shutdown()
{
}

/* ,-----------------------------------------------------------------.
 * |                      Paula Sampling Rate                        |
 * `-----------------------------------------------------------------'
 */

static int set_clock(paula_t * const paula, int clock_type, uint68_t f)
{
  u64 tmp;
  const int ct_fix = paula->ct_fix;

  paula->hz = f;
  paula->clock = clock_type;
  tmp = (clock_type == PAULA_CLOCK_PAL ? PAULA_PAL_FRQ : PAULA_NTSC_FRQ);
  tmp <<= CT_FIX;
  tmp /= f;
  paula->dividand = (uint68_t) tmp;
  return f;
}

uint68_t paula_sampling_rate(paula_t * const paula, uint68_t f)
{
  uint68_t hz = default_parms.hz;
  if (paula) {
    hz = !f
      ? paula->hz
      : set_clock(paula, paula->clock, f);
  }
  return hz;
}

int paula_set_emulation(paula_t * const paula, int emul)
{
  switch (emul) {
  case PAULA_EMUL_SIMPLE:
  case PAULA_EMUL_LINEAR:
    break;
  default:
    emul = default_parms.emul;
    break;
  }
  if (paula) paula->emul = emul;
  return emul;
}

int paula_clock(paula_t * const paula, int clock)
{
  switch (clock) {
  case PAULA_CLOCK_PAL:
  case PAULA_CLOCK_NTSC:
    break;
  default:
    clock = default_parms.clock;
    break;
  }
  if (paula) {
    set_clock(paula, clock,paula->hz);
  }
  return clock;
}

/* ,-----------------------------------------------------------------.
 * |			     paula reset                             |
 * `-----------------------------------------------------------------'
 */

int paula_reset(paula_t * const paula)
{
  int i;

  /* reset shadow registers */
  for (i=0; i<sizeof(paula->map); i++) {
    paula->map[i] = 0;
  }

  /* reset voices */
  for (i=0; i<4; i++) {
    paula->voice[i].adr = 0;
    paula->voice[i].start = 0;
    paula->voice[i].end = 2;
  }

  /* Reset DMACON and INTENA/REQ to something that
   * seems acceptable to me.
   */
  paula->dmacon = 1 << 9;  /* DMA general activate, DMA audio desactivate. */
  paula->intreq = 0;       /* No interrupt request.                        */
  paula->intena = 1 << 14; /* Master interrupt enable, audio int disable.  */
  paula->adkcon = 0;       /* No modulation.                               */

  return 0;
}

void paula_cleanup(paula_t * const paula) {}

int paula_setup(paula_t * const paula,
		paula_setup_t * const setup)
{
  
  if (!paula || !setup || !setup->mem) {
    return -1;
  }


  /* Default emulation mode */
  if (setup->parms.emul == PAULA_EMUL_DEFAULT) {
    setup->parms.emul = default_parms.emul;
  }

  /* Default sampling rate */
  if (!setup->parms.hz) {
    setup->parms.hz = default_parms.hz;
  }

  /* Default clock mode */
  if (setup->parms.clock == PAULA_CLOCK_DEFAULT) {
    setup->parms.clock = default_parms.clock;
  }

  paula->mem     = setup->mem;
  paula->log2mem = setup->log2mem;
  paula->ct_fix  = (sizeof(uint68_t)<<3)-paula->log2mem;

  paula_reset(paula);
  set_clock(paula, setup->parms.clock, setup->parms.hz);

  return 0;
}

#if 0
static void poll_irq(paula_t * const paula, unsigned int N)
{
  u8 *p = paula->map + PAULA_VOICE(N);
  paulav_t * w = paula->voice+N;

  /* Reload internal when interrupt is DENIED */
  if (
      (paula->intreq
       |
       ~((paula->intena << (8*sizeof(int)-1-14) >> (8*sizeof(int)-1))
         & paula->intena)) & (1 << (N + 7))) {
    unsigned int a,l;

    /* Get sample pointer. */
    a = (((p[1] << 16) | (p[2] << 8) | p[3]) & 0x7FFFE) << PAULA_CT_FIX;
    w->adr = w->start = a;
    /* Get length */
    l = (p[4] << 8) | p[5];
    l |= (!l) << 16;
    l <<= (1 + PAULA_CT_FIX);
    w->end = a + l;
  }
  paula->intreq |= 1 << (N + 7);
}
#endif

/* Mix with laudio channel data (1 char instead of 2) */

static void mix_one(paula_t * const paula, 
		    int N, const int shift,
		    s32 * b, int n)
{
  paulav_t * const w = paula->voice+N;
  u8 * const p = paula->map+PAULA_VOICE(N);
  const u8 * const mem = paula->mem;
  const int ct_fix = paula->ct_fix;
  int last;
  unsigned int stp, vol, per, readr, reend, end;
  int hasloop = 0;
  u32 adr;
  s16 *b2 = (s16 *)b + shift;
  const int imask = -(paula->emul == PAULA_EMUL_LINEAR);



  hasloop = 0;
  vol = p[9] & 127;
  if (vol >= 64) {
    vol = 64;
  }
  vol = volume[vol];

  per = (p[6]<<8) + p[7];
  if (!per) per = 1;
  stp = paula->dividand / per;

// /* audio int enable for this voice */
// if(paula_intena & ((paula_intena&~paula_intreq)<<(14-7-N)) & (1<<14) )
// {
//  /* Audio interrupt enable for this voice : No internal reload */
//  readr = v->start;
//  reend = v->end;
// }
// else

  {
    /* Audio irq disable for this voice :
     * Internal will be reload at end of block
     */
    readr = (((p[1] << 16) | (p[2] << 8) | p[3])) << CT_FIX;
    reend = ((p[4] << 8) | p[5]);
    reend |= (!reend)<<16;
    reend <<= (1 + CT_FIX);
    reend += readr;
  }

  if (reend < readr) {
    /* ??? */
    return;
  }

  adr = w->adr;
  end = w->end;

  if (end < adr) {
    return;
  }

  /* mix stereo */
  do {
    int v, ov;
    int low;

    low = adr & ((1 << CT_FIX) - 1) & imask;

    if (adr >= end) {
      unsigned int relen = reend - readr;
      hasloop = 1;
      adr = readr + adr - end;
      end = reend;
      while (adr >= end) {
        adr -= relen;
      }
    }

    {
      int i = (int)(adr >> CT_FIX);
      last = (int)(s8)mem[i++];
      v = last;

      if (( (u32)i << CT_FIX) >= end) {
        i = (int)(readr >> CT_FIX);
      }
      ov = (int)(s8)mem[i];
    }

    /* Linear interpol */
    v = (ov * low + v * ((1 << CT_FIX) - low)) >> CT_FIX;
    v *= vol;
    v >>= PL_MIX_FIX;
    *b2 += v;
    b2 += 2;
    adr += stp;
  } while (--n);

  if (adr >= end) {
    unsigned int relen = reend - readr;

    hasloop = 1;
    adr = readr + adr - end;
    end = reend;
    while (adr >= end)
      adr -= relen;
  }

  last &= 0xFF;
  p[0xA] = last + (last << 8);
  w->adr = adr;
  if (hasloop) {
    w->start = readr;
    w->end = end;
  }
}

/* ,-----------------------------------------------------------------.
 * |			    Paula process                            |
 * `-----------------------------------------------------------------'
 */

static void clear_buffer(s32 * b, int n)
{
  const s32 v = 0;
  if (n & 1) { *b++ = v; }
  if (n & 2) { *b++ = v; *b++ = v; }
  if (n >>= 2, n) do {
      *b++ = v; *b++ = v; *b++ = v; *b++ = v;
  } while (--n);
}

void paula_mix(paula_t * const paula, s32 * splbuf, int n)
{

  if (n>0) {
    int i;
    clear_buffer(splbuf, n);
    for (i=0; i<4; i++) {
      const int right = (i^msw_first)&1;
      if ((paula->dmacon >> 9) & (paula->dmacon >> i) & 1) {
	mix_one(paula, i, right, splbuf, n);
      }
    }
  }
    
  /* HaxXx: assuming next mix is next frame reset beam V/H position. */
  paula->vhpos = 0;
}

int paula_configure(paula_t * const paula, paula_parms_t * const parms)
{
  int err = -1;
  if (paula) {
    err = 0;
    if (parms) {
      uint68_t newclock = parms->clock ? parms->clock : paula->clock; 
      uint68_t  newhz   = parms->hz    ? parms->hz    : paula->hz;

      if (newclock != paula->clock || newhz != paula->hz) {
	set_clock(paula, newclock, newhz);
      }

      if (parms->emul != PAULA_EMUL_DEFAULT &&
	  parms->emul != paula->emul) {
	paula_set_emulation(paula,parms->emul);
      }
    }
  }
  return err;
}
