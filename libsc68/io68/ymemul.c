/*
 *                     sc68 - YM-2149 emulator
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
#include <sc68/option68.h>

#include <string.h>

#ifndef BREAKPOINT68
# define BREAKPOINT68 while(0)
#endif

#ifndef DEBUG_YM_O
# define DEBUG_YM_O 0
#endif
int ym_feature = debugmsg68_DEFAULT;

int ym_default_chans = 7;

#include "ym_linear_table.c"
#include "ym_atarist_table.c"


/** 3 channels output table.
 *  Using a table for non linear mixing.
 */
static s16 ymout5[32*32*32];
static inline
void access_list_reset(ym_waccess_list_t * const access_list,
		       const char * name,
		       const cycle68_t ymcycle)
{
  if (!name) name = "und";
  access_list->name[0] = name[0];
  access_list->name[1] = name[1];
  access_list->name[2] = name[2];
  access_list->name[3] = 0;
  access_list->head = access_list->tail = 0;
}

static void access_list_add(ym_t * const ym,
			    ym_waccess_list_t * const access_list,
			    const int reg, const int val,
			    const cycle68_t ymcycle)
{
  ym_waccess_t * free_access = ym->waccess_nxt;
  
  if (free_access >= ym->waccess+ym->waccess_max) {
    /* No more free entries. */

    /* $$$ TODO: realloc buffer, reloc all lists ... */
    TRACE68(ym_feature,"access_list_add(%s,%d,%02X) OVERFLOW",
	    access_list->name,reg,val);
    return;
  }
  ym->waccess_nxt = free_access+1;

  free_access->ymcycle = ymcycle;
  free_access->reg     = reg;
  free_access->val     = val;
  free_access->link    = 0;

  if (access_list->tail) {
    access_list->tail->link = free_access;
  } else {
    access_list->head = free_access;
  }
  access_list->tail = free_access;
  
/*   TRACE68(ym_feature,"WriteBack %s #%X <= %02X (%u)\n", */
/* 	  access_list->name,reg,val,ymcycle); */
}

static void access_adjust_cycle(ym_waccess_list_t * const access_list,
				const cycle68_t ymcycles)
{
  ym_waccess_t * access;
  /* access_list->last_cycle -= ymcycles; */
  for (access = access_list->head; access; access = access->link) {
    access->ymcycle -= ymcycles;
  }
}


/******************************************************
*                  Yamaha reset                       *
******************************************************/

int ym_reset(ym_t * const ym, const cycle68_t ymcycle)
{
  int ret = -1;

  if (ym) {
    int i;

    /* Run emulator specific reset callback. */
    if (ym->cb_reset) {
      ym->cb_reset(ym,ymcycle);
    }

    /* Clear internal registers. */
    for (i=0; i<sizeof(ym->reg.index)/sizeof(*ym->reg.index); ++i) {
      ym->reg.index[i] = 0;
    }

    /* Reset control register */
    ym->ctrl               = 0;

    /* Reset mixer: tone & noise off for all channels */
    ym->reg.name.ctl_mixer = 077;

    /* Reset envelop generator */
    ym->reg.name.env_shape = 0x0A;

    /* Reset access lists */
    access_list_reset(&ym->ton_regs, "Ton", ymcycle); 
    access_list_reset(&ym->noi_regs, "Noi", ymcycle); 
    access_list_reset(&ym->env_regs, "Env", ymcycle); 

    /* Copy registers to shadow */
    for (i=0; i<sizeof(ym->reg.index)/sizeof(*ym->reg.index); ++i) {
      ym->shadow.index[i] = ym->reg.index[i];
    }

    ret = 0;
  }

  return ret;
}

/******************************************************
*                  Yamaha init                        *
******************************************************/

/* -DYM_EMUL=YM_EMUL_BLEP choose BLEP as default engine */
#ifndef YM_EMUL
# define YM_EMUL YM_EMUL_ORIG
#endif

/* -DYM_VOL_TABLE=YM_VOL_LINEAR choose linear volume table */
#ifndef YM_VOL_TABLE
# define YM_VOL_TABLE  YM_VOL_ATARIST
#endif

static ym_parms_t default_parms =  {
  /* emul     */ YM_EMUL,
  /* voltable */ YM_VOL_TABLE,
  /* hz       */ SAMPLING_RATE_DEF,
  /* clock    */ YM_CLOCK_ATARIST,
  /* outlevel */ 0x80,
};

int ym_default_engine(int emul)
{
  switch (emul) {
  case YM_EMUL_DEFAULT:		/* Get current value. */
    emul = default_parms.emul;
    break;

  default:			/* Invalid values */
    debugmsg68_error("ym-2149: unknown ym-engine (%d)\n",emul);
    emul = default_parms.emul;
  case YM_EMUL_ORIG:
  case YM_EMUL_BLEP:
    default_parms.emul = emul;
    debugmsg68_info("ym-2149: switch default engine to *%s*\n",
		    emul==YM_EMUL_ORIG ? "SC68 ORIGINAL":"BLEP SYSNTHESIS");
    break;
  }

  return emul;
}

/* Max output level for volume tables. */
static const int ym_output_level = 0xEFFF;

/* command line options option */
static const char prefix[] = "sc68-";
static const char engcat[] = "ym-2149";
static option68_t opts[] = {
  { option68_STR, prefix, "ym-engine", engcat,
    "set ym-2149 engine [orig|blep]" },
  { option68_STR, prefix, "ym-voltable", engcat,
    "set ym-2149 volume table [atari|linear]" },
  { option68_INT, prefix, "ym-chans", engcat,
    "set ym-2149 active channel [bit-0:A ... bit-2:C]" }
};

int ym_init(ym_parms_t * const parms)
{
  int argc = 0;
  char ** argv = 0;
  option68_t * opt;

  /* Debug */
  ym_feature = debugmsg68_feature("ym","ym-2149 emulator",DEBUG_YM_O);

  /* Set default parameters. */
  if (parms) {
    if (parms->emul)     default_parms.emul     = parms->emul;
    if (parms->voltable) default_parms.voltable = parms->voltable;
    if (parms->hz)       default_parms.hz       = parms->hz;
    if (parms->clock)    default_parms.clock    = parms->clock;
    if (parms->outlevel) default_parms.outlevel = parms->outlevel;
    if (parms->argc) {
      argc = *parms->argc;
      argv = parms->argv;
    }
  }

  /* ym- Options */
  option68_append(opts,sizeof(opts)/sizeof(*opts));
  argc = option68_parse(argc,argv,0);

  /* --sc68-ym-engine */
  opt = option68_get("ym-engine",1);
  if (opt) {
    int k;
    if (!strcmp(opt->val.str,"orig")) {
      k = YM_EMUL_ORIG;
    } else if (!strcmp(opt->val.str,"blep")) {
      k = YM_EMUL_BLEP;
    } else {
      k = YM_EMUL_DEFAULT;
    }
    ym_default_engine(k);
  }

  /* --sc68-ym-voltable */
  opt = option68_get("ym-voltable",1);
  if (opt) {
    if (!strcmp(opt->val.str,"linear")) {
      default_parms.voltable = YM_VOL_LINEAR;
    } else if (!strcmp(opt->val.str,"atari")) {
      default_parms.voltable = YM_VOL_ATARIST;
    }
  }

  /* --sc68-ym-chans */
  opt = option68_get("ym-chans",1);
  if (opt) {
    ym_default_chans = opt->val.num & 7;
  }

  /* Set volume table (unique for all instance) */
  switch (default_parms.voltable) {
  case YM_VOL_LINEAR:
    ym_create_5bit_linear_table(ymout5, ym_output_level);
    break;
  case YM_VOL_DEFAULT:
  case YM_VOL_ATARIST:
  default:
    ym_create_5bit_atarist_table(ymout5, ym_output_level);
  }

  /* Process ym-orig options */
  argc = ym_orig_options(argc, argv);

  /* Save remaining cli options */
  if (parms && parms->argc) {
    *parms->argc =  argc;
  }

  return 0;
}

void ym_shutdown(void)
{
}



/* ,-----------------------------------------------------------------.
 * |                         Run emulation                           |
 * `-----------------------------------------------------------------'
 */
int ym_run(ym_t * const ym, s32 * output, const cycle68_t ymcycles)
{
  if (!ymcycles) {
    return 0;
  }

  /*
   * Currently ymcycles must be multiple of 32.
   * 32 cycles will generate 4 samples @ 250000hz. It helps to
   * simplify the first filter code (half-level buzz trick).
   */
  if ( (ymcycles&31) || !output)  {
    return -1;
  }
  
  return ym->cb_run(ym,output,ymcycles);
}



/* ,-----------------------------------------------------------------.
 * |                         Write YM register                       |
 * `-----------------------------------------------------------------'
 */

void ym_writereg(ym_t * const ym,
		 const int val, const cycle68_t ymcycle)
{
  const int reg = ym->ctrl;

  if (reg >= 0 && reg < 16) {

    /*TRACE68(ym_feature,"write #%X = %02X (%u)\n",reg,(int)(u8)val,ymcycle); */

    ym->shadow.index[reg] = val;
    
    switch(reg) {
      /* Tone generator related registers. */
    case YM_PERL(0): case YM_PERH(0):
    case YM_PERL(1): case YM_PERH(1):
    case YM_PERL(2): case YM_PERH(2):
    case YM_VOL(0): case YM_VOL(1): case YM_VOL(2):
      access_list_add(ym, &ym->ton_regs, reg, val, ymcycle);
      break;
      
      /* Envelop generator related registers. */
    case YM_ENVL: case YM_ENVH: case YM_ENVTYPE:
      access_list_add(ym, &ym->env_regs, reg, val, ymcycle);
      break;
      
      /* Reg 7 modifies both noise &ym-> tone generators */
    case YM_MIXER:
      access_list_add(ym, &ym->ton_regs, reg, val, ymcycle);
      /* Noise generator related registers. */
    case YM_NOISE:
      access_list_add(ym, &ym->noi_regs, reg, val, ymcycle);
      break;
      
    default:
      break;
    }
  }
}

/* ,-----------------------------------------------------------------.
 * |                  Adjust YM-2149 cycle counters                  |
 * `-----------------------------------------------------------------'
 */
void ym_adjust_cycle(ym_t * const ym, const cycle68_t ymcycles)
{
  if (ym) {
    access_adjust_cycle(&ym->ton_regs, ymcycles);
    access_adjust_cycle(&ym->noi_regs, ymcycles);
    access_adjust_cycle(&ym->env_regs, ymcycles);
  }
}

/* ,-----------------------------------------------------------------.
 * |                  Yamaha get activated voices                    |
 * `-----------------------------------------------------------------'
 */

extern const int ym_smsk_table[]; 	/* declared in ym_orig.c */
int ym_active_channels(ym_t * const ym, const int off, const int on)
{
  int v = 0;
  if (ym) {
    const int voice_mute = ym->voice_mute;
    v = (voice_mute&1) | ((voice_mute>>5)&2) | ((voice_mute>>10)&4);
    v = ((v&~off)|on)&7;
    ym->voice_mute = ym_smsk_table[v];
    debugmsg68_debug("ym-2149: active channels -- %c%c%c\n",
		     (v&1)?'A':'.', (v&1)?'B':'.', (v&1)?'B':'.');
  }
  return v;
}

/* ,-----------------------------------------------------------------.
 * |                        Output sampling rate                     |
 * `-----------------------------------------------------------------'
 */

uint68_t ym_sampling_rate(ym_t * const ym, const uint68_t hz)
{
  uint68_t ret = 0;
  if (ym) {
    if (!hz) {
      /* Get current sampling rate */
      ret = ym->hz;
    } else {
      /* Clips in range [SAMPLING_RATE_MIN..SAMPLING_RATE_MAX] */
      ret = hz;
      if (ret < SAMPLING_RATE_MIN) ret = SAMPLING_RATE_MIN;
      if (ret > SAMPLING_RATE_MAX) ret = SAMPLING_RATE_MAX;
      if (ym->cb_sampling_rate) {
	/* engine sampling rate callback */
	ret = ym->cb_sampling_rate(ym,ret);
      }
      ym->hz = ret;
      debugmsg68_debug("ym-2149: rate -- %d -> %d\n", hz, ret);
    }
  }
  
  return ret;
}

/* ,-----------------------------------------------------------------.
 * |                  Setup / Cleanup emulator instance              |
 * `-----------------------------------------------------------------'
 */

int ym_setup(ym_t * const ym, ym_parms_t * const parms)
{
  ym_parms_t * const p = parms ? parms : &default_parms;
  int err = -1;

  /* engine */
  if (p->emul == YM_EMUL_DEFAULT) {
    p->emul = default_parms.emul;
  }

  /* sampling rate */
  if (p->hz == 0) {
    p->hz = default_parms.hz;
  }

  /* clock */
  switch (p->clock) {
  case YM_CLOCK_ATARIST:
    break;
  case YM_CLOCK_DEFAULT:
  default:
    p->clock = default_parms.clock;
  }

  /* output level */
  if (p->outlevel == 0) {
    p->outlevel = default_parms.outlevel;
  }
  if (p->outlevel <= 0) {
    p->outlevel = 0;
  } else if (p->outlevel >= 0x100) {
    p->outlevel = 0x100;
  }
  p->outlevel >>= 2;

  debugmsg68_debug("ym-2149: setup -- engine:%d rate:%d clock:%d level:%d\n",
		   p->emul,p->hz,p->clock,p->outlevel);

  if (ym) {
    ym->ymout5      = ymout5;
    ym->waccess_max = sizeof(ym->static_waccess)/sizeof(*ym->static_waccess);
    ym->waccess     = ym->static_waccess;
    ym->waccess_nxt = ym->waccess;
    ym->clock       = p->clock;
    ym->outlevel    = p->outlevel;
    ym->voice_mute  = ym_smsk_table[7 & ym_default_chans];
    /* clearing sampling rate callback ensure requested rate to be in
       valid range. */
    ym->cb_sampling_rate = 0;
    ym_sampling_rate(ym, p->hz);

    switch (p->emul) {
    case YM_EMUL_ORIG:
      debugmsg68_info("ym-2149: select *SC68 ORIGINAL* engine\n");
      err = ym_orig_setup(ym);
      break;

    case YM_EMUL_BLEP:
      debugmsg68_info("ym-2149: select *BLEP SYSNTHESIS* engine\n");
      err = ym_blep_setup(ym);
      break;
      
    default:
      debugmsg68_critical("ym-2149: invalid ym engine (%d)\n", p->emul);
      err = -1;
    }
    /* at this point specific sampling rate callback can be call */
    ym_sampling_rate(ym, ym->hz);
  }

  return err ? err : ym_reset(ym, 0);
}


/** Destroy an Yamaha-2149 emulator instance.
 */
void ym_cleanup(ym_t * const ym)
{
  debugmsg68_debug("ym-2149: cleanup\n");
  if (ym && ym->cb_cleanup) {
    ym->cb_cleanup(ym);
  }
}

/** Get required output buffer size.
 */
uint68_t ym_buffersize(const ym_t const * ym, const cycle68_t ymcycles)
{
  return ym->cb_buffersize(ym,ymcycles);
}
