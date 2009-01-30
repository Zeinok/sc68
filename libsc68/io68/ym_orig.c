/*
 *                  sc68 - YM-2149 original engine
 *            Copyright (C) 2001-2009 Ben(jamin) Gerard
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

#include "ymemul.h"
#include <sc68/debugmsg68.h>

extern int ym_feature;		/* defined in ymemul.c */

#ifndef INTMSB
# define INTMSB (sizeof(int)*8-1)
#endif

#define YM_OUT_MSK(C,B,A) \
 (((((C)&0x1F)<<10))\
 |((((B)&0x1F)<< 5))\
 |((((A)&0x1F)    )))

const int ym_smsk_table[8] = {
  /* 000 */ YM_OUT_MSK(00,00,00),
  /* 001 */ YM_OUT_MSK(00,00,-1),
  /* 010 */ YM_OUT_MSK(00,-1,00),
  /* 011 */ YM_OUT_MSK(00,-1,-1),
  /* 100 */ YM_OUT_MSK(-1,00,00),
  /* 101 */ YM_OUT_MSK(-1,00,-1),
  /* 110 */ YM_OUT_MSK(-1,-1,00),
  /* 111 */ YM_OUT_MSK(-1,-1,-1)
};

/* static const int ym_out_msk[3] = {  */
/*  YM_OUT_MSK_A, YM_OUT_MSK_B, YM_OUT_MSK_C */
/* }; */


#if 0
static const struct filter_def_t {
  s64 A0,B0; /* A1 = -A0 */
} filter_def[8] =
{
  /* 0123456789ABCDEF     0123456789ABCDEF  cutoff/sampling */
  {0x0000ffb4522a469bLL,0x0000ff68a4548d37LL}, /* 23/62500 */
  {0x0000ff9a09ef0019LL,0x0000ff3413de0032LL}, /* 31/62500 */
  {0x0000ff72a7ba0de5LL,0x0000fee54f741bcbLL}, /* 43/62500 */
  {0x0000ff586d0271f6LL,0x0000feb0da04e3edLL}, /* 51/62500 */

  {0x0000ff94cbfc73a4LL,0x0000ff2997f8e748LL}, /* 23/44100 */
  {0x0000ff6f974da2b8LL,0x0000fedf2e9b4571LL}, /* 31/44100 */
  {0x0000ff37dc9e7a33LL,0x0000fe6fb93cf466LL}, /* 43/44100 */
  {0x0000ff12c309cd7aLL,0x0000fe2586139af5LL}  /* 51/44100 */
};
#endif


static int reset(ym_t * const ym, const cycle68_t ymcycle)
{
  ym_orig_t * const orig = &ym->emu.orig;

  /* Reset envelop generator */
  orig->env_bit            = 0;
  orig->env_ct             = 0;
  
  /* Reset noise generator */
  orig->noise_gen          = 1;
  orig->noise_ct           = 0;
  /* Reset tone generator */
  orig->voice_ctA          = 0;
  orig->voice_ctB          = 0;
  orig->voice_ctC          = 0;
  orig->levels             = 0;
  
  /* Reset filters */
  orig->hipass_inp1 = 0;
  orig->hipass_out1 = 0;
  orig->lopass_out1 = 0;

  return 0;
}

/* ,-----------------------------------------------------------------.
 * |                          Noise generator                        |
 * `-----------------------------------------------------------------'
 */

/* Perform noise generator for N ym-cycles
 *
 *   The noise generator will use the 16 Less Signifiant Bit of the
 *   32 output buffer for each entry. 16 Most Signifiant Bit are
 *   clear by this function.
 *
 *   **BEFORE** envelop_generator() and tone_generator().
 */
static int noise_generator(ym_t * const ym, int ymcycles)
{
  ym_orig_t * const orig = &ym->emu.orig;
  int rem_cycles;
  int v, noise_gen, ct, per, msk;
  s32 * b;

  rem_cycles = ymcycles & 7;
  if(!(ymcycles >>= 3)) goto finish;

  /* All inits */
  ct        = orig->noise_ct;
  noise_gen = orig->noise_gen;
  per       = ym->reg.name.per_noise & 0x3F;
    
  /*per      |= !per;*/ /* $$$ Must verify if 0 and 1 really are the same */
  per     <<= 1;    /* because the noise generator base frequency is
		       master/16 but we have to match the envelop
		       generator frequency which is master/8.
		    */

  msk       = ym_smsk_table[7 & (ym->reg.name.ctl_mixer >> 3)];
  v         = (u16)(-(noise_gen & 1) | msk);
  b         = orig->noiptr;
  do {
    if (++ct >= per) {
      ct = 0;

      /* *** Based on MAME. Bit have been reversed for optimzation :) ***
       *
       *   The Random Number Generator of the 8910 is a 17-bit shift
       *   register. The input to the shift register is bit0 XOR bit2.
       *   bit0 is the output.
       */

      /* bit 17 := bit 0 ^ bit 2 */
      noise_gen |= ((noise_gen^(noise_gen>>2)) & 1)<<17;
      noise_gen >>= 1;
      v = (u16)(-(noise_gen & 1) | msk);
    }
    *b++ = v;
  } while (--ymcycles);

  /* Save value for next pass */
  orig->noiptr    = b;
  orig->noise_gen = noise_gen;
  orig->noise_ct  = ct;

 finish:
  /* return not mixed cycle */
  return rem_cycles;
}

/* Flush all noise registers write access and perform noise generation
 * until given cycle. The given ymcycle should/must be greater than the
 * latest write access cycle stamp.
 */
static void do_noise(ym_t * const ym, cycle68_t ymcycle)
{
  ym_orig_t * const orig = &ym->emu.orig;
  ym_waccess_t * access;
  ym_waccess_list_t * const regs = &ym->noi_regs;
  cycle68_t lastcycle;

  orig->noiptr = ym->outbuf;
  if (!ymcycle) {
    return;
  }

  for (access=regs->head, lastcycle=0; access; access=access->link) {
    int ymcycles = access->ymcycle-lastcycle;

    if (access->ymcycle > ymcycle) {
      TRACE68(ym_feature,"%s access reg %X out of frame!! (%u>%u %u)\n",
	      regs->name, access->reg, access->ymcycle, ymcycle,
	      access->ymcycle/ymcycle);
      break;
    }

    if (ymcycles) {
      lastcycle = access->ymcycle - noise_generator(ym,ymcycles);
    }
    ym->reg.index[access->reg] = access->val;
/*     TRACE68(ym_feature,"ReadBack %s #%X => %02X (%u)\n", */
/* 	    regs->name,access->reg,access->val,access->ymcycle); */
  }
  lastcycle = ymcycle - noise_generator(ym, ymcycle-lastcycle);
  regs->head = regs->tail = 0;
}

/* ,-----------------------------------------------------------------.
 * |                         Envelop generator                       |
 * `-----------------------------------------------------------------'
 */

#if YM_ENV_TABLE

#undef V
#define V(X) YM_OUT_MSK(X,X,X)

static const int env_uplo[64] = {
  V(000),V(001),V(002),V(003),V(004),V(005),V(006),V(007),
  V(010),V(011),V(012),V(013),V(014),V(015),V(016),V(017),
  V(020),V(021),V(022),V(023),V(024),V(025),V(026),V(027),
  V(030),V(031),V(032),V(033),V(034),V(035),V(036),V(037),

  V(000),V(000),V(000),V(000),V(000),V(000),V(000),V(000),
  V(000),V(000),V(000),V(000),V(000),V(000),V(000),V(000),
  V(000),V(000),V(000),V(000),V(000),V(000),V(000),V(000),
  V(000),V(000),V(000),V(000),V(000),V(000),V(000),V(000),
};

static const int env_uphi[64] = {
  V(000),V(001),V(002),V(003),V(004),V(005),V(006),V(007),
  V(010),V(011),V(012),V(013),V(014),V(015),V(016),V(017),
  V(020),V(021),V(022),V(023),V(024),V(025),V(026),V(027),
  V(030),V(031),V(032),V(033),V(034),V(035),V(036),V(037),

  V(037),V(037),V(037),V(037),V(037),V(037),V(037),V(037),
  V(037),V(037),V(037),V(037),V(037),V(037),V(037),V(037),
  V(037),V(037),V(037),V(037),V(037),V(037),V(037),V(037),
  V(037),V(037),V(037),V(037),V(037),V(037),V(037),V(037),
};

static const int env_upup[64] = {
  V(000),V(001),V(002),V(003),V(004),V(005),V(006),V(007),
  V(010),V(011),V(012),V(013),V(014),V(015),V(016),V(017),
  V(020),V(021),V(022),V(023),V(024),V(025),V(026),V(027),
  V(030),V(031),V(032),V(033),V(034),V(035),V(036),V(037),

  V(000),V(001),V(002),V(003),V(004),V(005),V(006),V(007),
  V(010),V(011),V(012),V(013),V(014),V(015),V(016),V(017),
  V(020),V(021),V(022),V(023),V(024),V(025),V(026),V(027),
  V(030),V(031),V(032),V(033),V(034),V(035),V(036),V(037)
};

static const int env_updw[64] = {
  V(000),V(001),V(002),V(003),V(004),V(005),V(006),V(007),
  V(010),V(011),V(012),V(013),V(014),V(015),V(016),V(017),
  V(020),V(021),V(022),V(023),V(024),V(025),V(026),V(027),
  V(030),V(031),V(032),V(033),V(034),V(035),V(036),V(037),

  V(037),V(036),V(035),V(034),V(033),V(032),V(031),V(030),
  V(027),V(026),V(025),V(024),V(023),V(022),V(021),V(020),
  V(017),V(016),V(015),V(014),V(013),V(012),V(011),V(010),
  V(007),V(006),V(005),V(004),V(003),V(002),V(001),V(000)
};

static const int env_dwlo[64] = {
  V(037),V(036),V(035),V(034),V(033),V(032),V(031),V(030),
  V(027),V(026),V(025),V(024),V(023),V(022),V(021),V(020),
  V(017),V(016),V(015),V(014),V(013),V(012),V(011),V(010),
  V(007),V(006),V(005),V(004),V(003),V(002),V(001),V(000),

  V(000),V(000),V(000),V(000),V(000),V(000),V(000),V(000),
  V(000),V(000),V(000),V(000),V(000),V(000),V(000),V(000),
  V(000),V(000),V(000),V(000),V(000),V(000),V(000),V(000),
  V(000),V(000),V(000),V(000),V(000),V(000),V(000),V(000),
};

static const int env_dwhi[64] = {
  V(037),V(036),V(035),V(034),V(033),V(032),V(031),V(030),
  V(027),V(026),V(025),V(024),V(023),V(022),V(021),V(020),
  V(017),V(016),V(015),V(014),V(013),V(012),V(011),V(010),
  V(007),V(006),V(005),V(004),V(003),V(002),V(001),V(000),

  V(037),V(037),V(037),V(037),V(037),V(037),V(037),V(037),
  V(037),V(037),V(037),V(037),V(037),V(037),V(037),V(037),
  V(037),V(037),V(037),V(037),V(037),V(037),V(037),V(037),
  V(037),V(037),V(037),V(037),V(037),V(037),V(037),V(037),
};

static const int env_dwup[64] = {
  V(037),V(036),V(035),V(034),V(033),V(032),V(031),V(030),
  V(027),V(026),V(025),V(024),V(023),V(022),V(021),V(020),
  V(017),V(016),V(015),V(014),V(013),V(012),V(011),V(010),
  V(007),V(006),V(005),V(004),V(003),V(002),V(001),V(000),

  V(000),V(001),V(002),V(003),V(004),V(005),V(006),V(007),
  V(010),V(011),V(012),V(013),V(014),V(015),V(016),V(017),
  V(020),V(021),V(022),V(023),V(024),V(025),V(026),V(027),
  V(030),V(031),V(032),V(033),V(034),V(035),V(036),V(037)
};

static const int env_dwdw[64] = {
  V(037),V(036),V(035),V(034),V(033),V(032),V(031),V(030),
  V(027),V(026),V(025),V(024),V(023),V(022),V(021),V(020),
  V(017),V(016),V(015),V(014),V(013),V(012),V(011),V(010),
  V(007),V(006),V(005),V(004),V(003),V(002),V(001),V(000),

  V(037),V(036),V(035),V(034),V(033),V(032),V(031),V(030),
  V(027),V(026),V(025),V(024),V(023),V(022),V(021),V(020),
  V(017),V(016),V(015),V(014),V(013),V(012),V(011),V(010),
  V(007),V(006),V(005),V(004),V(003),V(002),V(001),V(000)
};

#undef V

static  const int * waveforms[16] = {
  /*0 \_ */ env_dwlo,
  /*1 \_ */ env_dwlo,
  /*2 \_ */ env_dwlo,
  /*3 \_ */ env_dwlo,
  /*4 /_ */ env_uplo,
  /*5 /_ */ env_uplo,
  /*6 /_ */ env_uplo,
  /*7 /_ */ env_uplo,
  /*8 \\ */ env_dwdw,
  /*9 \_ */ env_dwlo,
  /*A \/ */ env_dwup,
  /*B \- */ env_dwhi,
  /*C // */ env_upup,
  /*D /- */ env_uphi,
  /*E /\ */ env_updw,
  /*F /_ */ env_uplo
};

#endif

/* Perform envelop generator for N ym-cycles
 *
 *   The envelop generator will use the 16 Lost Signifiant Bit of the
 *   32 output buffer for each entry. 16 Mores Signifiant Bit are used
 *   by noise generator and have already been setted by
 *   envelop_generator() function calls so the value must be
 *   preserved.
 *
 *   **AFTER** noise_generator() and **BEFORE** tone_generator().
 *
 *   shape format : [CONTinue | ATTack | ALTernate | HOLD]
 */

static int envelop_generator(ym_t * const ym, int ymcycles)
#if YM_ENV_TABLE
{
  ym_orig_t * const orig = &ym->emu.orig;
  int rem_cycle = ymcycles & 7;

  if((ymcycles >>= 3)) {
    const int shape = ym->reg.name.env_shape & 15;
    const int * const waveform = waveforms[shape];
    /* Do Not Repeat is NOT ([CONT] AND NOT [HOLD]) */
    const int dnr = 1 & ~((shape>>3)&~shape);
    s32 *b  = orig->envptr;
    int ct  = orig->env_ct;
    int bit = orig->env_bit;
    int per = ym->reg.name.per_env_lo | (ym->reg.name.per_env_hi<<8);

    do {
      int t = ++ct >= per;
      bit += t;
      ct &= ~-t;
      bit |= -((bit >> 6) & dnr);
      bit &= 63;
      *b++ |= waveform[bit]<<16;
    } while (--ymcycles);
 
    /* Save value for next pass */
    orig->envptr     = b;
    orig->env_ct     = ct;
    orig->env_bit    = bit;
  }
  return rem_cycle;
}
#else /* #if YM_ENV_TABLE */
{
  unsigned int ncycle = ymcycles;
  int rem_cycle;
  int *b;
  int ct, per;
  unsigned int bit, bitstp, restp;
  unsigned int cont, recont;
  unsigned int alt, altalt;
  int shape;

#ifdef _DEBUG
  if (ncycle < 0) {
    BREAKPOINT68;
    return 0;
  }
#endif

  rem_cycle = ncycle & 7;
  if(!(ncycle >>= 3)) return rem_cycle;

  b       = ym->envptr;

  /* period */
  ct      = ym->env_ct;
  per     = ym->reg.index[YM_ENVL] | (ym->reg.index[YM_ENVH]<<8);
  per     |= !per;
  shape   = ym->reg.index[YM_ENVTYPE];

  /* bit */
  bit     = ym->env_bit;
  bitstp  = ym->env_bitstp;
  restp   = (shape & 1) ^ 1;

  /* continue */
  cont    = ym->env_cont;
  recont  = (-((shape>>3) & 0x1)) & 0x1F;

  /* alternate */
  alt     = ym->env_alt;
  altalt  = (-((shape ^ (shape>>1)) & 0x1)) & 0x1F;

  do {
    int n;

    n = per - ct;
    if (n <= 0) {
      int prev_bit;
      ct = 0;
      n = per;
      prev_bit = bit;
      bit += bitstp;
      if ((bit^prev_bit) & 32) {
        bitstp = restp;
        cont = recont;
        alt ^= altalt;
      }

/*       if ((bit ^ alt) & cont) */
/*       TRACE68(ym_feature,"v:%02x\n",(bit ^ alt) & cont); */

    }

    /* 5 bit version */
    int v = (bit ^ alt) & cont;
    v |= v<<5;
    v |= v<<5;
    v <<= 16;

    if (n > ncycle) {
      n = ncycle;
    }
    ncycle -= n;
    ct += n;

#ifdef _DEBUG
    if (n<=0) {
      BREAKPOINT68;
      break;
    }
#endif

    do {
      *b++ |= v;
    } while (--n);

  } while (ncycle);

  ym->envptr     = b;
  ym->env_ct     = ct;
  ym->env_bit    = bit;
  ym->env_bitstp = bitstp;
  ym->env_cont   = cont;
  ym->env_alt    = alt;

  return rem_cycle;
}

#endif /* #if YM_ENV_TABLE */


/*
 * Flush all envelop registers write access and perform envelop
 * generation until given cycle. The given ymcycle should/must be
 * greater than the latest write access cycle stamp.
 */
static void do_envelop(ym_t * const ym, cycle68_t ymcycle)
{
  ym_orig_t * const orig = &ym->emu.orig;
  ym_waccess_t * access;
  ym_waccess_list_t * const regs = &ym->env_regs;

  cycle68_t lastcycle;

  orig->envptr = ym->outbuf;
  if (!ymcycle) {
    return;
  }

  for (access=regs->head, lastcycle=0; access; access=access->link) {
    int ymcycles = access->ymcycle-lastcycle;

    if (access->ymcycle > ymcycle) {
      TRACE68(ym_feature,"%s access reg %X out of frame!! (%u>%u %u)\n",
	      regs->name, access->reg, access->ymcycle, ymcycle,
	      access->ymcycle/ymcycle);
      break;
    }

    if (ymcycles) {
      lastcycle = access->ymcycle - envelop_generator(ym,ymcycles);
    }

    ym->reg.index[access->reg] = access->val;
    if(access->reg == YM_ENVTYPE) {
#if YM_ENV_TABLE
      orig->env_bit = 0;
      orig->env_ct  = 0; /* $$$ Needs to be verifed. It seems cleaner. */
#else
      int shape = access->val & 15;
      /* Alternate mask start value depend on ATTack bit */
      orig->env_alt    = (~((shape << (INTMSB-2)) >> INTMSB)) & 0x1F;
      orig->env_bit    = 0;
      orig->env_bitstp = 1;
      orig->env_cont   = 0x1f;
      orig->env_ct = 0;
#endif

  /* $$$ Not sure, that this counter is resetted, but it seems
         cleaner. */



    }
/*     if (access->reg >= 8 && access->reg <= 10) { */
/*       TRACE68(ym_feature,"ReadBack %s #%X => %02X (%u)\n", */
/* 	      regs->name,access->reg,access->val,access->ymcycle); */
/*     } */
  }
  /* lastcycle = ymcycle -  */envelop_generator(ym, ymcycle-lastcycle);
  regs->head = regs->tail = 0;
}

/* ,-----------------------------------------------------------------.
 * |                  Tone generator and mixer                       |
 * `-----------------------------------------------------------------'
 */

static int tone_generator(ym_t  * const ym, int ymcycles)
{
  ym_orig_t * const orig = &ym->emu.orig;

  int ctA,  ctB,  ctC;
  int perA, perB, perC;
  int smsk, emsk, vols;

  s32 * b;
  int rem_cycles, v;
  int levels;
  int mute;

  rem_cycles = ymcycles & 7;
  ymcycles >>= 3;
  if(!ymcycles) goto finish;

  /* init buffer address */
  b = orig->tonptr;

  mute = ym->voice_mute & YM_OUT_MSK_ALL;
  smsk = ym_smsk_table[7 & ym->reg.name.ctl_mixer];

  /* 3 voices buzz or lvl mask */
  emsk = vols = 0;

  v = ym->reg.name.vol_a & 0x1F;
  if(v&0x10) emsk |= YM_OUT_MSK_A;
  else       vols |= (v<<1)+1;

  v = ym->reg.name.vol_b & 0x1F;
  if(v&0x10) emsk |= YM_OUT_MSK_B;
  else       vols |= (v<<6)+(1<<5);

  v = ym->reg.name.vol_c & 0x1F;
  if(v&0x10) emsk |= YM_OUT_MSK_C;
  else       vols |= (v<<11)+(1<<10);

  /* Mixer steps & couters */
  ctA = orig->voice_ctA;
  ctB = orig->voice_ctB;
  ctC = orig->voice_ctC;

  perA = ym->reg.name.per_a_lo | ((ym->reg.name.per_a_hi&0xF)<<8);
  perB = ym->reg.name.per_b_lo | ((ym->reg.name.per_b_hi&0xF)<<8);
  perC = ym->reg.name.per_c_lo | ((ym->reg.name.per_c_hi&0xF)<<8);

  levels = orig->levels;
  do {
    int sq;

    sq = -(++ctA >= perA);
    levels ^= YM_OUT_MSK_A & sq;
    ctA &= ~sq;

    sq = -(++ctB >= perB);
    levels ^= YM_OUT_MSK_B & sq;
    ctB &= ~sq;

    sq = -(++ctC >= perC);
    levels ^= YM_OUT_MSK_C & sq;
    ctC &= ~sq;

    sq = levels;
    sq |= smsk;
    {
      unsigned int eo = *b; /* EEEENNNN */
      sq &= eo; eo >>= 16;
      sq &= (eo&emsk) | vols;
    }

    sq &= mute;

    sq = (int) ym->ymout5[sq];
   
    *b++ = sq;
  
  } while (--ymcycles);

  /* Save value for next pass */
  orig->tonptr    = b;
  orig->voice_ctA = ctA;
  orig->voice_ctB = ctB;
  orig->voice_ctC = ctC;
  orig->levels    = levels;
 finish:
  return rem_cycles;
}

/*
 * Flush all tone registers write access and perform tone generation
 * and mixer until given cycle. The given ymcycle should/must be
 * greater than the latest write access cycle stamp.
 */
static void do_tone_and_mixer(ym_t * const ym, cycle68_t ymcycle)
{
  ym_orig_t * const orig = &ym->emu.orig;

  ym_waccess_t * access;
  ym_waccess_list_t * const regs = &ym->ton_regs;
  cycle68_t lastcycle;

  orig->tonptr = ym->outbuf;
  if (!ymcycle) {
    return;
  }

  for (access=regs->head, lastcycle=0; access; access=access->link) {
    const int ymcycles = access->ymcycle - lastcycle;

    if (access->ymcycle > ymcycle) {
      TRACE68(ym_feature,"%s access reg %X out of frame!! (%u>%u %u)\n",
	      regs->name, access->reg, access->ymcycle, ymcycle,
	      access->ymcycle/ymcycle);
      break;
    }

    if (ymcycles) {
      lastcycle = access->ymcycle - tone_generator(ym, ymcycles);
    }
    ym->reg.index[access->reg] = access->val;
    
/*     if (access->reg >= 8 && access->reg <= 10) { */
/*       TRACE68(ym_feature,"ReadBack %s #%X => %02X (%u)\n", */
/* 	      regs->name,access->reg,access->val,access->ymcycle); */
/*     } */

  }
  /* lastcycle = ymcycle -  */tone_generator(ym, ymcycle-lastcycle);
  regs->head = regs->tail = 0;
}

/* ,-----------------------------------------------------------------.
 * |                         Run emulation                           |
 * `-----------------------------------------------------------------'
 */

/* Transform 250000Hz buffer to current sampling rate.
 *
 * In order to emulate envelop tone half level trick, the function
 * works by block of 4 PCM which are averaged. Since number of PCM in
 * source buffer may not be a multiple of 4, the funcion uses a filter
 * accumulator.
 *
 * The accumulator has been disable. See ym_run().
 */
static void filter(ym_t * const ym)
{
  ym_orig_t * const orig = &ym->emu.orig;

  int n;
  s32 * src, * dst;

  n = orig->tonptr - ym->outbuf;
  if (n <= 0) return;

  src = ym->outbuf;
  dst = src/*  + (filter_cnt = n&3) */;
  n >>= 2; /* Number of block */

  if (n > 0) {
    const int outlevel = ym->outlevel;
    int h_i1 = orig->hipass_inp1;
    int h_o1 = orig->hipass_out1;
    int l_o1 = orig->lopass_out1;
    int m = n;
    int hasclip = 0;

    do {
      int i0,o0;

      /* Empirical filter from 250Khz to 62.5Khz.
       * It allows to emulate half level buzz sounds.
       */
      i0  = *src++;
      i0 += *src++; i0 += *src++; i0 += *src++;
      i0 >>= 2; /* i0 => 16bit */

      if (1) {
	o0 = i0;
	goto apply_volume;
      }

      /*
       * Recursive single pole lowpass filter.
       *
       * Harcoded for a 22.5Khz cutoff frequency with a 62.5Kz sampling rate.
       *
       *   o[N] = i[N] * A + output[N-1] * B
       *
       *   X = exp(-2.0 * pi * Fc)
       *   A = 1 - X = 1 - B
       *   B = X
       *   Fc = cutoff freq / sample rate
       *
       */
      {
	const int B = 0x0DF2A3a9; /* 31 bit */
	const int A = 0x720d5c57; /* 31 bit */

	s64 tmp0 =
	  ((i0<<15) * (s64) A) +
	  (l_o1 * (s64) B);       /* tmp0 => 62 bit */
	l_o1 = tmp0>>31;
	/* i0 => 31 bit */
      }

      /* Recursive single pole high-pass filter
       *
       * Harcoded for a 23Hz cutoff with a 62.5Kz sampling rate.
       *
       *   o[N] = A0 * i[N] + A1 * i[N-1] + B1 * o[N-1]
       *        = A0 * i[N] - A0 * i[N-1] + B1 * o[N-1]
       *        = A0 * ( i[N] - i[N-1] )  + B1 * o[N-1]
       *   X  = exp(-2.0 * pi * Fc)
       *   A0 = (1 + X) / 2
       *   A1 = -(1 + X) / 2 = -A0
       *   B1 = X
       *   Fc = cutoff freq / sample rate
       *
       */
      {
	const int A0 = 0x7FDA2915; /* 31 bit */
	const int B1 = 0x7FB4522A; /* 31 bit */

	s64 tmp0 =
	  ((l_o1 - h_i1) * (s64) A0) +
	  (h_o1 * (s64) B1);
	h_i1 = l_o1;
	h_o1 = tmp0 >> 31;
	o0 = h_o1 >> 15;
      }

    apply_volume:
      o0 = (o0 * outlevel) >> 6;

      if (1) {
	static int min, max;
	if (o0 < min) {
	  TRACE68(ym_feature,"min: %6d -> %6d\n",min,o0);
	  min = o0;
	}
	if (o0 > max) {
	  TRACE68(ym_feature,"max: %6d -> %6d\n",max,o0);
	  max = o0;
	}
      }

      /* Clipping methods. */
      if (0) {
	int tmp;
	o0 += 0x8000; /* Switch to unsigned range [0..65536]
		       *
		       * CLIP-MIN:
		       *  - condition :  o0 < 0
		       *  - effects   :  o0 = 0
		       *  - logic     : o0 & -(o0>=0)
		       *           or   o0 & ~(o0>>INT_MSB)
		       *
		       * CLIP-MAX:
		       *  - condition :  o0 > 65535
		       *          <=>    o0-65535 > 0
		       *          <=>    o0-65535 > 0
		       *          <=>    65535-o0 < 0
		       *  - effects   :  o0 = 65535
		       *  - logic     : o0 | -(o0>65535)
		       *           or   o0 | ((65535-o0)>>INTMSB)&0xFFFF
		       *  - haxxx       o0 | (o0<<(INTMSB-16)>>INTMSB)&0xFFFF
		       *                works only for clipping < 0x20000
		       */

	/* Clip MIN */
	tmp = (o0>>INTMSB);
	hasclip |= tmp;
	o0 &= ~tmp;

	/* Clip MAX */
	tmp = (o0<<(INTMSB-16))>>INTMSB;
	hasclip |= tmp;
	o0 |= tmp;

	o0 &= 0xFFFF; /* ensure in range [0..65536]           */
	o0 -= 0x8000; /* restore signed range [-32768..32767] */
      } else if (1) {
	/* Traditionnal method */
	if (o0 < -32768) {
	  hasclip = 1;
	  o0 = -32768;
	} else if (o0 > 32767) {
	  hasclip = 1;
	  o0 = 32767;
	}
      }


      /* store: */
      *dst++ = o0;

    } while (--m);

    orig->hipass_inp1 =  h_i1;
    orig->hipass_out1 =  h_o1;
    orig->lopass_out1 =  l_o1;

    /* $$$ dynamic output level ? May be not that good an idea ! */
    ym->outlevel = outlevel - !!hasclip;
    if (hasclip) {
      TRACE68(ym_feature,"has clip: new level: %02x\n",ym->outlevel);
    }

    /* Copy remaining sample in the beginning of buffer. */
    dst = ym->outbuf;
    /* if (filter_cnt & 1) { *dst++ = *src++; } */
    /* if (filter_cnt & 2) { *dst++ = *src++; *dst++ = *src++; } */
    src = dst;


    /* Rougth resampling. Should be ok thanks to lowpass filter.
     * Current sampling rate is clock / 8 (prediv) / 4 (filter)
     */
    {
      unsigned int stp, ct, end;
      ct  = 0;
      stp = (ym->clock<<(16-3-2)) / ym->hz;
      end = n << 16;
      do {
	*dst++ = src[(int)(ct>>16)];
      } while ((ct += stp) < end);
    }
  }
  ym->outptr = dst;
}


static
int run(ym_t * const ym, s32 * output, const cycle68_t ymcycles)
{
  ym_orig_t * const orig = &ym->emu.orig;
  ym->outbuf = ym->outptr = output;

  do_noise(ym,ymcycles);
  do_envelop(ym,ymcycles);
  do_tone_and_mixer(ym,ymcycles);

  ym->waccess = ym->static_waccess;
  ym->waccess_nxt = ym->waccess;

  if (0) {
    int i;
    TRACE68(ym_feature,"pre-filter:");
    for (i=0; i<orig->tonptr - ym->outbuf; ++i) {
      TRACE68(ym_feature,"%d ",output[i]);
    }
    TRACE68(ym_feature,"->%d\n",ym->outptr - ym->outbuf);
  }

  filter(ym);

  if (0) {
    int i;
    TRACE68(ym_feature,"post-filter:");
    for (i=0; i<ym->outptr - ym->outbuf; ++i) {
      TRACE68(ym_feature,"%d ",output[i]);
    }
    TRACE68(ym_feature,"->%d\n",ym->outptr - ym->outbuf);
  }
  
  /* TRACE68(ym_feature,"Generated PCM:%d -> %d \n",ym->tonptr - ym->outbuf, ym->outptr - ym->outbuf); */

  return ym->outptr - ym->outbuf;
}


static
uint68_t buffersize(const ym_t const * ym, const cycle68_t ymcycles)
{
  return ((ymcycles+7u) >> 3);
}

static
void cleanup(ym_t * const ym)
{
}

int ym_orig_setup(ym_t * const ym)
{
  /* ym_orig_t * const orig = &ym->emu.orig; */
  int err = 0;

  /* fill callback functions */
  ym->cb_cleanup       = cleanup;
  ym->cb_reset         = reset;
  ym->cb_run           = run;
  ym->cb_buffersize    = buffersize;
  ym->cb_sampling_rate = (void*)0;

  return err;
}

