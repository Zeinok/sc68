/*
 *                 sc68 - MicroWire - STE soundchip emulator
 *                  Copyright (C) 1999-2007 Benjamin Gerard
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */ 

/** @todo
 *
 * - Stereo !!
 * - Check overflow in mix routine.
 * - Verify STE / YM volume ratio
 * - Special case for not mixing in Db_alone.
 * - And in the YM emul, skip emulation !!! 
 *
 */

#ifdef HAVE_CONFIG_OPTION68_H
# include "config_option68.h"
#else
# include "default_option68.h"
#endif
#include "mwemul.h"

/* #define MW_CALCUL_TABLE 1 */

#define MW_N_DECIBEL 121

#define MW_MIX_FIX 10

#define MW_STE_MULT ((1<<MW_MIX_FIX)/4)
#define MW_YM_MULT  ((1<<MW_MIX_FIX)-MW_STE_MULT)

#define TOINTERNAL(N) ((mw->map[N]<<16)+(mw->map[(N)+2]<<8)+(mw->map[(N)+4]))


#ifndef MW_CALCUL_TABLE

static const int Db_alone[MW_N_DECIBEL] = {
  0x40000,0x32d64,0x28619,0x20137,0x197a9,0x143d1,0x10137,0xcc50,
  0xa24b,0x80e9,0x6666,0x5156,0x409c,0x3352,0x28c4,0x2061,
  0x19b8,0x146e,0x103a,0xce4,0xa3d,0x822,0x676,0x521,
  0x413,0x33c,0x292,0x20b,0x19f,0x14a,0x106,0xd0,
  0xa5,0x83,0x68,0x52,0x41,0x34,0x29,0x21,
  0x1a,0x14,0x10,0xd,0xa,0x8,0x6,0x5, 0x4,0x3,0x2,0x2,0x1,0x1,0x1,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0
};

static const int Db_mix[MW_N_DECIBEL] = {
  0x10000,0xcb59,0xa186,0x804d,0x65ea,0x50f4,0x404d,0x3314,
  0x2892,0x203a,0x1999,0x1455,0x1027,0xcd4,0xa31,0x818,
  0x66e,0x51b,0x40e,0x339,0x28f,0x208,0x19d,0x148,
  0x104,0xcf,0xa4,0x82,0x67,0x52,0x41,0x34,
  0x29,0x20,0x1a,0x14,0x10,0xd,0xa,0x8,
  0x6,0x5,0x4,0x3,0x2,0x2,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,
};

static const int Db_mix12[MW_N_DECIBEL] = {
  0x1027,0xcd4,0xa31,0x818,0x66e,0x51b,0x40e,0x339,
  0x28f,0x208,0x19d,0x148,0x104,0xcf,0xa4,0x82,
  0x67,0x52,0x41,0x34,0x29,0x20,0x1a,0x14,
  0x10,0xd,0xa,0x8,0x6,0x5,0x4,0x3,0x2,0x2,0x1,0x1,0x1,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0
};

static void init_volume(void) { }

#else

# include <math.h>   /* $$$ Calcul DB table */
# include <stdio.h>  /* $$$ For display table */

static int Db_alone[MW_N_DECIBEL];
static int Db_mix[MW_N_DECIBEL];
static int Db_mix12[MW_N_DECIBEL];

#define LN_10_OVER_10 0.230258509299

/*

 A,B signal intensity
 1.Db = 10*LOG( A/B ) = 10*LN(A/B)/LN(10)
 => A = B * EXP( Decibel*LN(10)/10 )
 => A = B * R
    with R=EXP( Decibel*LN(10)/10 )

 R1,R2 rate of 2 signal for D1,D2 in decibel
 A = B*R1*R2  <=> B*R3
 with R3 = rate for (D1+D2) decibel

*/

static u32 calc_volume(s32 decibel, u32 mult)
{
  double r;
  r = exp( (double)decibel*LN_10_OVER_10 );
  r *= (double)mult;
  return (u32)r;
}

static void init_volume(void)
{
  int i;

  fprintf(stderr, "\n");
  for(i=0; i<MW_N_DECIBEL; i++) {
    Db_alone[i] = calc_volume(-i,256<<MW_MIX_FIX);
    Db_mix[i] = calc_volume(-i,MW_STE_MULT*256);
    Db_mix12[i] = calc_volume(-i-12,MW_STE_MULT*256);
    fprintf(stderr, "AAA:%x\n", Db_alone[i]);
    fprintf(stderr, "BBB:%x\n", Db_mix[i]);
    fprintf(stderr, "CCC:%x\n", Db_mix12[i]);
  }
}

#endif

static mw_parms_t default_parms = {
  MW_EMUL_LINEAR, SAMPLING_RATE_DEF
};

/* ,-----------------------------------------------------------------.
 * |                     Set replay frequency                        |
 * `-----------------------------------------------------------------'
 */
uint68_t mw_sampling_rate(mw_t * const mw, uint68_t f)
{
  if (f) {
    if (f < SAMPLING_RATE_MIN) {
      f = SAMPLING_RATE_MIN;
    } else if (f > SAMPLING_RATE_MAX) {
      f = SAMPLING_RATE_MAX;
    }
    mw->hz = f;
  }
  return mw->hz;
}

/* ,-----------------------------------------------------------------.
 * | Set master volume   0=-80 Db, 40=0 Db                           |
 * | Set left volume     0=-40 Db, 40=0 Db                           |
 * | Set right volume    0=-40 Db, 40=0 Db                           |
 * | Set high frequency  0=-12 Db, 40=12 Db                          |
 * | Set low  frequency  0=-12 Db, 40=12 Db                          |
 * | Set mixer type : 0=-12 Db 1=YM+STE 2=STE only 3=reserved        |
 * `-----------------------------------------------------------------'
 */

void mw_set_lmc_mixer(mw_t * const mw, int n)
{
  static const int * table[] = { Db_mix12, Db_mix, Db_alone };
  n &= 3;
  mw->lmc.mixer = n;
  if (n != 3) {
    mw->db_conv = table[n];
  }
}

void mw_set_lmc_master(mw_t * const mw, int n)
{
  if (n<0)  n = 0;
  if (n>40) n = 40;
  mw->lmc.master = -80+2*n;
}

void mw_set_lmc_left(mw_t * const mw, int n)
{
  if (n<0)  n = 0;
  if (n>20) n = 20;
  mw->lmc.left = -40+2*n;
  mw->lmc.lr = (mw->lmc.left+mw->lmc.right)>>1;
}

void mw_set_lmc_right(mw_t * const mw, int n)
{
  if (n<0)  n = 0;
  if (n>20) n = 20;
  mw->lmc.right = -40+2*n;
  mw->lmc.lr = (mw->lmc.left+mw->lmc.right)>>1;
}

void mw_set_lmc_high(mw_t * const mw, int n)
{
  if (n<0)  n = 0;
  if (n>12) n = 12;
  mw->lmc.high = -12+2*n;
}

void mw_set_lmc_low(mw_t * const mw, int n)
{
  if (n<0)  n = 0;
  if (n>12) n = 12;
  mw->lmc.low = -12+2*n;
}

/* ,-----------------------------------------------------------------.
 * |                         Micro-Wire reset                        |
 * `-----------------------------------------------------------------'
 */

static void lmc_reset(mw_t * const mw)
{
  mw_set_lmc_mixer(mw,1);
  mw_set_lmc_master(mw,40);
  mw_set_lmc_left(mw,20);
  mw_set_lmc_right(mw,20);
  mw_set_lmc_high(mw,0);
  mw_set_lmc_low(mw,0);
}

int mw_reset(mw_t * const mw)
{
  int i;

  for (i=0; i<sizeof(mw->map); i++) {
    mw->map[i] = 0;
  }
  mw->ct = mw->end = 0;
  lmc_reset(mw);

  return 0;
}

void mw_cleanup(mw_t * const mw) {}

int mw_setup(mw_t * const mw,
	     mw_setup_t * const setup)
{
  
  if (!mw || !setup || !setup->mem) {
    return -1;
  }
  
  /* Default emulation mode */
  if (setup->parms.emul == MW_EMUL_DEFAULT) {
    setup->parms.emul = default_parms.emul;
  }

  /* Default sampling rate */
  if (!setup->parms.hz) {
    setup->parms.hz = default_parms.hz;
  }

  mw->mem     = setup->mem;
  mw->log2mem = setup->log2mem;
  mw->ct_fix  = (sizeof(uint68_t)<<3)-mw->log2mem;

  mw_reset(mw);

  return 0;
}

/* ,-----------------------------------------------------------------.
 * |                    Micro-Wire initialization                    |
 * `-----------------------------------------------------------------'
 */

int mw_init(mw_parms_t * const parms_data)
{
  mw_parms_t * parms = parms_data ? parms_data : &default_parms;

  /* Default emulation mode */
  if (parms->emul == MW_EMUL_DEFAULT) {
    parms->emul = default_parms.emul;
  }

  /* Default sampling rate */
  if (!parms->hz) {
    parms->hz = default_parms.hz;
  }

  /* Init volume table */
  init_volume();

  return 0;
}

void mw_shutdown(void) {}


/* Rescale n sample of b with f ( << MW_MIX_FIX ) */
static void rescale(s32 * b, int68_t f, int n)
{
  if (!f) {
    do { *b++ = 0; } while (--n);
  } else if (f != (1<<MW_MIX_FIX)) {
    do {
      int68_t v;
      v = ((*b)*f) >> MW_MIX_FIX;
      *b++ = ((u32)(u16)v) + (v<<16);
    } while (--n);
  }
}

/* --- Rescale n sample of b with r ( << mw_MIX_FIX ) --- */
static void no_mix_ste(mw_t * const mw, s32 * b, int n)
{
  rescale(b, (mw->db_conv == Db_alone) ? 0 : MW_YM_MULT, n);
}

static void mix_ste(mw_t * const mw, s32 *b, int n)
{
  addr68_t base, end2;
  addr68_t ct, end, stp;
  const int vl = mw->db_conv[0/*mw->lmc.master+mw->lmc.left*/];
  const int vr = mw->db_conv[0/*mw->lmc.master+mw->lmc.right*/];
  const int loop = mw->map[MW_ACTI] & 2;
  const int mono = (mw->map[MW_MODE]>>7) & 1;
  const uint68_t frq = 50066u >> ((mw->map[MW_MODE]&3)^3);
  const int68_t ym_mult = (mw->db_conv == Db_alone) ? 0 : MW_YM_MULT;
  const int ct_fix = mw->ct_fix;
  const s8 * spl = (const s8 *)mw->mem;

  /* Get internal register for sample base and sample end
   * $$$ ??? what if base > end2 ??? 
   */
  base = TOINTERNAL(MW_BASH) << ct_fix;
  end2 = TOINTERNAL(MW_ENDH) << ct_fix;

  /* Get current sample counter and end */
  ct  = mw->ct;
  end = mw->end;

  if (ct >= end) {
    if (!loop) {
      goto out;
    } else {
      uint68_t overflow = ct-end;
      uint68_t length   = end-base;
      ct  = base;
      if (length) {
	ct += overflow>length ? overflow%length : overflow;
      }
      end = end2;
    }
  }
  /* Calculate sample step.
   * Stereo trick : Advance 2 times faster, take care of word alignment later.
   */
  stp = (frq << (ct_fix+1-mono)) / mw->hz;

  if (mono) {
    /* mix mono */
    do {
      int68_t v, ym;

      ym = (*b) * ym_mult;
      v = spl[(int)(ct>>ct_fix)];
      *b++ =
	(
	 (u16)((v*vl + ym) >> MW_MIX_FIX)
	 +
	 (((v*vr + ym)>>MW_MIX_FIX)<<16)
	 );

      ct += stp;
      if (ct >= end) {
	if (!loop) {
	  --n;
	  break;
	} else {
	  uint68_t overflow = ct-end;
	  uint68_t length   = end-base;
	  ct  = base;
	  if (length) {
	    ct += overflow>length ? overflow%length : overflow;
	  }
	  end = end2;
	}
      }
    } while(--n);

  } else {
    /* mix stereo */
    do {
      int68_t l,r,ym;
      int68_t addr;

      ym = (*b) * ym_mult;
      addr = (ct >> ct_fix) & ~1;
      l = spl[addr+0];
      r = spl[addr+1];
      *b++ =
	(
	 (u16)((l*vl + ym)>>MW_MIX_FIX)
	 +
	 (((r*vr + ym)>>MW_MIX_FIX)<<16)
	 );

      ct += stp;
      if (ct >= end) {
	if (!loop) {
	  --n;
	  break;
	} else {
	  uint68_t overflow = ct-end;
	  uint68_t length   = end-base;
	  ct  = base;
	  if (length) {
	    ct += overflow>length ? overflow%length : overflow;
	  }
	  end = end2;
	}
      }
    } while(--n);
  }

 out:
  if (!loop && ct >= end) {
    mw->map[MW_ACTI] = 0;
    ct  = base;
    end = end2;
  }
  mw->ct  = ct;
  mw->end = end;

  /* Finish the buffer */
  if (n>0) {
    no_mix_ste(mw,b,n);
  }
}

/* ,-----------------------------------------------------------------.
 * |                      Micro-Wire process                         |
 * `-----------------------------------------------------------------'
 */
/* void mw_mix(mw_t * const mw, u32 *b, int n) */
void mw_mix(mw_t * const mw, s32 *b, int n)
{
  if (n<=0) {
    return;
  }

  if (!(mw->map[MW_ACTI]&1)) {
    /* Micro-Wire desactivated */
    no_mix_ste(mw,b,n);
  } else {
    /* Micro-Wire activated */
    mix_ste(mw,b,n);
  }
}
