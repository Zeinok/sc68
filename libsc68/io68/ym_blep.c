/*
 *               sc68 - YM-2149 blep synthesis engine
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

#include <stdio.h>
#include <string.h>

enum {
    MIN_EVENT_INTERVAL = 8,
    TONE_HI_MASK = 0xf,
    NOISE_MASK = 0x1f,
    CTL_ENV_MASK = 0xf,
    LEVEL_M_MASK = 0x10,
    LEVEL_LEVEL_MASK = 0xf,

    BLEP_SIZE = 512,
    BLEP_SCALE = 17,
};

const static int32_t sine_integral[BLEP_SIZE] = {
131072,131057,131042,131026,131010,130993,130975,130956,130936,130914,130890,
130865,130837,130807,130773,130737,130698,130654,130607,130555,130499,130437,130369,
130296,130216,130129,130035,129933,129823,129704,129575,129437,129288,129128,128957,
128773,128577,128367,128144,127906,127654,127385,127100,126799,126480,126142,125787,
125411,125016,124601,124164,123706,123226,122722,122196,121646,121071,120472,119848,
119197,118521,117818,117089,116332,115548,114736,113896,113028,112131,111206,110252,
109270,108259,107219,106150,105053,103928,102774,101592,100383,99146,97882,96592,
95276,93933,92566,91175,89759,88321,86860,85377,83874,82351,80809,79248,77671,76078,
74470,72848,71213,69566,67910,66244,64571,62890,61205,59516,57824,56131,54438,52747,
51059,49375,47697,46026,44364,42712,41072,39444,37830,36232,34651,33089,31546,30024,
28524,27047,25595,24169,22770,21398,20056,18744,17463,16213,14997,13814,12665,11552,
10474,9432,8427,7460,6530,5638,4785,3970,3194,2457,1759,1100,479,-102,-646,-1151,
-1618,-2048,-2440,-2796,-3116,-3401,-3651,-3867,-4049,-4199,-4318,-4406,-4464,-4493,
-4494,-4469,-4418,-4342,-4244,-4122,-3980,-3818,-3637,-3439,-3225,-2995,-2752,-2497,
-2230,-1953,-1668,-1375,-1076,-771,-463,-152,160,473,785,1096,1404,1708,2007,2301,
2588,2868,3140,3403,3656,3899,4131,4351,4559,4755,4938,5107,5262,5403,5530,5643,
5741,5824,5892,5945,5983,6007,6016,6011,5991,5958,5911,5850,5776,5690,5591,5480,
5359,5226,5083,4930,4768,4598,4419,4234,4041,3842,3638,3429,3216,2999,2779,2558,
2334,2110,1885,1661,1438,1216,996,779,566,355,150,-51,-247,-437,-621,-799,-970,
-1134,-1290,-1439,-1579,-1712,-1836,-1951,-2058,-2156,-2245,-2325,-2396,-2459,-2512,
-2556,-2592,-2619,-2638,-2648,-2650,-2643,-2629,-2608,-2579,-2543,-2500,-2451,-2396,
-2335,-2268,-2197,-2120,-2039,-1954,-1865,-1773,-1678,-1581,-1481,-1379,-1276,-1171,
-1066,-961,-855,-750,-645,-541,-439,-338,-238,-141,-46,46,136,223,306,386,463,536,
605,670,731,788,841,890,934,975,1011,1042,1069,1093,1112,1126,1137,1144,1147,1146,
1142,1134,1123,1108,1090,1070,1047,1021,993,962,929,895,859,821,782,741,700,658,615,
572,529,485,441,398,355,312,270,229,189,149,111,74,38,3,-30,-61,-91,-119,-146,-171,
-194,-215,-235,-253,-269,-283,-295,-306,-315,-322,-328,-332,-334,-335,-334,-332,
-329,-324,-318,-311,-303,-294,-283,-273,-261,-248,-235,-222,-208,-194,-179,-164,
-149,-134,-119,-104,-89,-74,-60,-46,-32,-18,-5,7,19,31,41,52,61,70,78,86,92,98,104,
109,113,116,118,120,122,122,122,122,121,119,117,115,112,108,105,101,96,92,87,82,76,
71,66,60,54,49,43,38,32,27,22,17,12,7,2,-2,-6,-10,-13,-17,-19,-22,-24,-26,-28,-30,
-31,-31,-32,-32,-32,-32,-31,-30,-29,-28,-26,-25,-23,-21,-19,-17,-15,-13,-11,-9,-7,
-5,-4,-3,-1,-1,0,0,
};

/* these are the envelopes. First 32 values are used for first iteration, the next 64
 * describe how the envelope loops. I know this is pretty lame, but I wanted as
 * simple implementation as possible. */
static const uint8_t envelopes[16][32+64] = {
{ 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
{ 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
{ 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
{ 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
{ 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, 
  31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, },
{ 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
{ 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, 
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, },
{ 31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, 
  31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31, },
{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, },
{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 
  31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31, },
{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 
  31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, },
{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
};

extern int ym_feature;		/* defined in ymemul.c */

static void ym2149_mix_output(ym_t * const ym, cycle68_t cycles)
{
    ym_blep_t *orig = &ym->emu.blep;

    int i, output;
    int volume = 0;

    for (i = 2; i >= 0; i -= 1) {
        volume <<= 5; 
        if (((ym->reg.name.ctl_mixer & (1<<i)) || orig->tonegen[i].flip_flop)
            && ((ym->reg.name.ctl_mixer & (1<<(i+3))) || (orig->noise_state & 1))) {
            if (*(&ym->reg.name.vol_a + i) & LEVEL_M_MASK)
                /* Envelope reg */
                volume |= orig->env_output;
            else
                /* Level reg */
                volume |= (*(&ym->reg.name.vol_a + i) & LEVEL_LEVEL_MASK) << 1;
        }
    }

    output = ym->ymout5[volume] >> 1;

    /* Age the bleps -- this advances the blep phases for output code */
    for (i = 0; i < orig->active_bleps; i += 1) {
        orig->blepstate[i].age += cycles;
        if (orig->blepstate[i].age >= BLEP_SIZE) {
            orig->active_bleps = i;
            break;
        }
    }

    if (output != orig->global_output_level) {
        /* Start a new blep: level is the difference, age is 0 clocks. */
        if (orig->active_bleps > MAX_BLEPS - 1) {
            TRACE68(ym_feature, "Active blep list truncated; sound distorted");
            orig->active_bleps = MAX_BLEPS - 1;
        }
        /* Make room for new blep */
        memmove(&orig->blepstate[1], &orig->blepstate[0], sizeof(orig->blepstate[0]) * orig->active_bleps);
        /* Update state to account for the new blep */
        orig->active_bleps += 1;
        orig->blepstate[0].age = cycles;
        orig->blepstate[0].level = output - orig->global_output_level;
        orig->global_output_level = output;
    }
}

static void ym2149_clock(ym_t * const ym, cycle68_t cycles)
{
    ym_blep_t *orig = &ym->emu.blep;

    int i, tone_event[3], env_event, noise_event;
    
    /* refresh all event count variables from registers */
    for (i = 0; i < 3; i += 1) {
        tone_event[i] = ym->reg.index[i * 2] | ((ym->reg.index[i * 2 + 1] & TONE_HI_MASK) << 8);
        if (tone_event[i] == 0)
            tone_event[i] = 1;
        tone_event[i] = tone_event[i] * 8 + 1;
    }

    noise_event = ym->reg.name.per_noise & NOISE_MASK;
    if (noise_event == 0)
        noise_event = 1;
    noise_event = noise_event * 16 + 1;

    env_event = ym->reg.name.per_env_lo | (ym->reg.name.per_env_hi << 8);
    if (env_event == 0)
        env_event = 1;
    env_event += 8 * env_event + 1;

    while (cycles) {
        int iter = cycles;

        /* establish length of period without state change */
        for (i = 0; i < 3; i ++) {
            if (iter > tone_event[i] - orig->tonegen[i].count)
                iter = tone_event[i] - orig->tonegen[i].count;
        }

        if (iter > noise_event - orig->noise_count)
            iter = noise_event - orig->noise_count;

        if (iter > env_event - orig->env_count)
            iter = env_event - orig->env_count;

        if (iter < 0)
            iter = 0;

        /* produce pulse waveform for iter clocks */
        ym2149_mix_output(ym, iter);

        /* clock subsystems forward */
        for (i = 0; i < 3; i ++) {
            orig->tonegen[i].count += iter;
            if (orig->tonegen[i].count >= tone_event[i]) {
                orig->tonegen[i].flip_flop = ~orig->tonegen[i].flip_flop;
                orig->tonegen[i].count = 0;
            }
        }

        orig->noise_count += iter;
        if (orig->noise_count >= noise_event) {
            orig->noise_state = 
                (orig->noise_state >> 1) |
                (((orig->noise_state ^ (orig->noise_state >> 2)) & 1) << 17);
            orig->noise_count = 0;
        }

        orig->env_count += iter;
        if (orig->env_count >= env_event) {
            int envtype = ym->reg.name.env_shape & CTL_ENV_MASK;

            orig->env_output = envelopes[envtype][orig->env_state];
            orig->env_state += 1;
            if (orig->env_state == 96)
                orig->env_state = 32;
            orig->env_count = 0;
        }

        cycles -= iter;
    }
}

static int ym2149_output(ym_t * const ym)
{
    ym_blep_t *orig = &ym->emu.blep;
    int i;

    int output = orig->global_output_level << BLEP_SCALE;
    for (i = 0; i < orig->active_bleps; i ++)
        output -= sine_integral[orig->blepstate[i].age] * orig->blepstate[i].level;
    output >>= 16;
    return output;
}

static int highpass(ym_t * const ym, int output)
{
    ym_blep_t *orig = &ym->emu.blep;
    
    orig->hp = (orig->hp * 255 + (output << 4)) >> 8;
    output -= orig->hp >> 4;

    if (output > 32767)
        output = 32767;
    if (output < -32768)
        output = -32768;

    return output;
}

/* run output synthesis for some clocks */
static int mix_to_buffer(ym_t * const ym, cycle68_t cycles, s32 *output)
{
    ym_blep_t *orig = &ym->emu.blep;

    int len = 0;
    while (cycles) {
        cycle68_t iter = cycles;
        int makesample = 0;
        if (iter >= orig->cycles_to_next_sample >> 8) {
            iter = orig->cycles_to_next_sample >> 8;
            makesample = 1;
        }

        /* Simulate ym2149 for iter clocks */
        ym2149_clock(ym, iter);
        cycles -= iter;
        orig->cycles_to_next_sample -= iter << 8;

        /* Generate output */
        if (makesample) {
            output[len ++] = highpass(ym, ym2149_output(ym));
            orig->cycles_to_next_sample = orig->cycles_per_sample;
        }
    }
    return len;
}

static void cleanup(ym_t * const ym) {}

static int reset(ym_t * const ym, const cycle68_t ymcycle)
{
    ym_blep_t *orig = &ym->emu.blep;
    int tmp = orig->cycles_per_sample;
    memset(orig, 0, sizeof(ym_blep_t));
    orig->cycles_per_sample = tmp;
    orig->noise_state = 1;
    return 0;
}

/* mix for ymcycles cycles. */
static int run(ym_t * const ym, s32 * output, const cycle68_t ymcycles)
{
    ym_blep_t *orig = &ym->emu.blep;

    int len = 0;

    /* Walk  the static list of allocated events */
    int currcycle = 0;
    ym_waccess_t *access;
    for (access = ym->waccess; access != ym->waccess_nxt; access ++) {
        if (access->ymcycle > ymcycles) {
            TRACE68(ym_feature, "access reg %X out of frame: (%u > %u)\n",
                    access->reg, access->ymcycle, ymcycles);
        }

        /* mix up to this cycle, update state */
        len += mix_to_buffer(ym, access->ymcycle - currcycle, output + len);
        /* reset envelope on write */
        if (&ym->reg.index[access->reg] == &ym->reg.name.env_shape)
            orig->env_state = 0;
        ym->reg.index[access->reg] = access->val;
        currcycle = access->ymcycle;
    }

    /* Reset all access lists, even the unused ones... */
    ym_waccess_list_t * const regs_n = &ym->noi_regs;
    ym_waccess_list_t * const regs_e = &ym->env_regs;
    ym_waccess_list_t * const regs_t = &ym->ton_regs;
    regs_n->head = regs_n->tail = regs_e->head = regs_e->tail = regs_t->head = regs_t->tail = 0;

    /* Set free ptr to start of list */
    ym->waccess_nxt = ym->waccess;
    return len;
}

/* Get required length of buffer at run(s32 *output) (number of frames). */
static uint68_t buffersize(const ym_t const * ym, const cycle68_t ymcycles)
{
    return 2048; /* suffices to up to 96 kHz */
}

static uint68_t sampling_rate(ym_t * const ym, const uint68_t hz)
{
    ym_blep_t *orig = &ym->emu.blep;
    orig->cycles_per_sample = (ym->clock << 8) / hz;
    return 0;
}

int ym_blep_setup(ym_t * const ym) {
    //interpolate_volumetable(volumetable);

    ym->cb_cleanup       = cleanup;
    ym->cb_reset         = reset;
    ym->cb_run           = run;
    ym->cb_buffersize    = buffersize;
    ym->cb_sampling_rate = sampling_rate;
    return 0;
}


