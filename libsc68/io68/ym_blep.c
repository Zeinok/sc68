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

#include <assert.h>
#include <stdio.h>
#include <string.h>

enum {
    TONE_HI_MASK = 0xf,
    NOISE_MASK = 0x1f,
    CTL_ENV_MASK = 0xf,
    LEVEL_M_MASK = 0x10,
    LEVEL_LEVEL_MASK = 0xf,

    BLEP_SIZE = 1024,
    BLEP_SCALE = 16,
    
    MAX_MIXBUF = 2048,
};

const static s32 sine_integral[BLEP_SIZE] = {
65536,65536,65536,65536,65536,65536,65536,65535,65535,65535,65535,65535,65534,
65534,65534,65533,65533,65532,65531,65530,65529,65528,65527,65525,65523,65521,65519,
65516,65513,65509,65505,65501,65496,65490,65484,65477,65469,65460,65451,65440,65428,
65416,65401,65386,65369,65350,65330,65308,65284,65258,65230,65200,65166,65131,65092,
65051,65006,64958,64907,64852,64792,64729,64662,64590,64513,64431,64344,64251,64153,
64049,63939,63822,63699,63568,63430,63285,63132,62971,62802,62624,62437,62241,62036,
61821,61596,61361,61115,60859,60591,60313,60022,59721,59407,59081,58742,58391,58027,
57650,57260,56857,56440,56010,55566,55108,54636,54150,53650,53136,52609,52067,51510,
50940,50356,49758,49147,48522,47883,47231,46566,45888,45197,44494,43779,43052,42314,
41565,40805,40034,39254,38465,37666,36860,36045,35223,34394,33559,32719,31873,31023,
30170,29313,28454,27593,26732,25870,25009,24149,23291,22436,21584,20736,19894,19057,
18227,17404,16590,15784,14987,14201,13426,12663,11912,11174,10450,9740,9045,8366,
7704,7058,6429,5818,5226,4652,4098,3563,3048,2554,2080,1627,1195,785,396,28,-317,
-641,-944,-1224,-1483,-1721,-1937,-2132,-2305,-2458,-2591,-2704,-2796,-2870,-2924,
-2960,-2978,-2979,-2963,-2930,-2881,-2818,-2740,-2648,-2543,-2426,-2297,-2157,-2008,
-1848,-1680,-1505,-1322,-1133,-939,-740,-537,-332,-123,86,296,506,716,924,1129,1332,
1531,1726,1916,2100,2279,2450,2615,2772,2921,3062,3193,3316,3429,3532,3624,3707,
3779,3840,3891,3930,3959,3977,3984,3980,3965,3940,3904,3858,3803,3737,3662,3578,
3485,3383,3274,3157,3032,2901,2764,2621,2472,2319,2162,2000,1836,1669,1500,1329,
1157,985,812,641,470,301,135,-29,-190,-347,-501,-649,-793,-932,-1064,-1191,-1311,
-1425,-1532,-1631,-1724,-1808,-1885,-1953,-2014,-2067,-2111,-2147,-2175,-2194,-2206,
-2209,-2204,-2192,-2171,-2144,-2108,-2066,-2017,-1961,-1899,-1831,-1757,-1677,-1593,
-1504,-1411,-1314,-1213,-1110,-1003,-894,-784,-672,-559,-445,-331,-217,-104,8,118,
227,334,438,540,638,733,824,911,994,1072,1146,1214,1278,1336,1388,1435,1476,1512,
1542,1565,1583,1595,1602,1602,1597,1586,1570,1548,1521,1489,1452,1410,1364,1314,
1259,1201,1139,1074,1006,936,863,787,710,632,552,472,391,309,228,147,66,-13,-91,
-168,-244,-317,-388,-456,-522,-585,-645,-702,-755,-805,-851,-894,-932,-966,-997,
-1023,-1045,-1062,-1076,-1085,-1091,-1092,-1089,-1082,-1071,-1056,-1037,-1015,-990,
-961,-929,-894,-856,-815,-772,-727,-680,-631,-580,-528,-474,-420,-365,-309,-253,
-197,-141,-85,-30,24,78,130,181,231,279,325,370,412,452,489,524,557,587,614,639,661,
679,695,708,719,726,730,732,730,726,719,710,698,683,666,647,626,602,577,550,521,
490,458,425,391,356,320,284,247,209,172,134,97,59,22,-14,-50,-85,-119,-152,-183,
-214,-243,-271,-297,-322,-345,-366,-386,-403,-419,-433,-445,-455,-463,-469,-473,
-476,-476,-475,-471,-466,-460,-451,-441,-430,-416,-402,-386,-369,-351,-332,-312,
-291,-269,-247,-224,-201,-177,-153,-129,-105,-81,-57,-34,-10,13,35,57,78,99,118,137,
155,172,188,202,216,229,240,250,259,267,273,279,283,285,287,287,287,285,282,277,
272,266,259,251,242,232,222,211,199,187,174,160,147,133,118,104,89,75,60,46,31,17,
3,-11,-24,-37,-50,-62,-74,-85,-96,-105,-115,-123,-131,-138,-145,-151,-156,-160,
-163,-166,-168,-170,-170,-170,-169,-168,-166,-163,-159,-156,-151,-146,-141,-135,
-128,-122,-114,-107,-99,-92,-84,-75,-67,-59,-50,-42,-34,-25,-17,-9,-1,7,14,21,28,35,
41,47,53,58,63,67,71,75,78,81,83,85,87,88,89,89,89,88,87,86,84,83,80,78,75,72,69,
65,61,57,53,49,45,41,36,32,27,23,18,14,9,5,1,-3,-7,-11,-15,-18,-22,-25,-28,-31,
-33,-36,-38,-40,-41,-43,-44,-45,-46,-46,-46,-46,-46,-46,-45,-45,-44,-43,-42,-40,
-39,-37,-35,-33,-31,-29,-27,-25,-23,-20,-18,-16,-13,-11,-9,-6,-4,-2,0,2,4,6,8,10,
12,13,15,16,17,18,19,20,21,21,22,22,23,23,23,23,23,22,22,22,21,20,20,19,18,17,16,
15,14,13,12,11,10,9,7,6,5,4,3,2,1,0,-1,-2,-3,-4,-5,-6,-6,-7,-8,-8,-9,-9,-10,-10,
-10,-10,-10,-11,-11,-11,-11,-10,-10,-10,-10,-9,-9,-9,-8,-8,-8,-7,-7,-6,-6,-5,-5,-4,
-3,-3,-2,-2,-1,-1,0,0,1,1,1,2,2,3,3,3,3,4,4,4,4,4,4,4,5,5,5,5,4,4,4,4,4,4,4,4,3,3,
3,3,3,2,2,2,2,1,1,1,1,1,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,
-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
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
        tone_event[i] = tone_event[i] << 3;
    }

    noise_event = ym->reg.name.per_noise & NOISE_MASK;
    if (noise_event == 0)
        noise_event = 1;
    noise_event = noise_event << 4;

    env_event = ym->reg.name.per_env_lo | (ym->reg.name.per_env_hi << 8);
    if (env_event == 0)
        env_event = 1;
    env_event = env_event << 3;

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

            orig->env_output = envelopes[envtype][orig->env_state ++];
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
static int mix_to_buffer(ym_t * const ym, int cycles, s32 *output)
{
    ym_blep_t *orig = &ym->emu.blep;

    assert(cycles >= 0);

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
            assert(len < MAX_MIXBUF);
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
        ym->reg.index[access->reg] = access->val;
        /* reset envelope on write */
        if (&ym->reg.index[access->reg] == &ym->reg.name.env_shape)
            orig->env_state = 0;
        currcycle = access->ymcycle;
    }

    /* mix stuff outside writes */
    len += mix_to_buffer(ym, ymcycles - currcycle, output + len);

    /* Reset all access lists. */
    ym_waccess_list_t * const regs_n = &ym->noi_regs;
    ym_waccess_list_t * const regs_e = &ym->env_regs;
    ym_waccess_list_t * const regs_t = &ym->ton_regs;
    regs_n->head = regs_n->tail = regs_e->head = regs_e->tail = regs_t->head = regs_t->tail = 0;

    /* Set free ptr to start of list */
    ym->waccess = ym->static_waccess;
    ym->waccess_nxt = ym->waccess;
    return len;
}

/* Get required length of buffer at run(s32 *output) (number of frames). */
static uint68_t buffersize(const ym_t const * ym, const cycle68_t ymcycles)
{
    return MAX_MIXBUF;
}

static uint68_t sampling_rate(ym_t * const ym, const uint68_t hz)
{
    ym_blep_t *orig = &ym->emu.blep;
    orig->cycles_per_sample = (ym->clock << 8) / hz;
    return 0;
}

int ym_blep_setup(ym_t * const ym) {
    ym->cb_cleanup       = cleanup;
    ym->cb_reset         = reset;
    ym->cb_run           = run;
    ym->cb_buffersize    = buffersize;
    ym->cb_sampling_rate = sampling_rate;
    return 0;
}
