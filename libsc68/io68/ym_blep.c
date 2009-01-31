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
65536,65536,65536,65536,65536,65535,65536,65536,65536,65535,65536,65535,65535,
65535,65535,65535,65534,65534,65533,65533,65533,65531,65531,65530,65529,65528,65526,
65525,65523,65521,65518,65516,65513,65509,65506,65501,65497,65491,65486,65478,65472,
65464,65454,65445,65435,65422,65409,65396,65380,65363,65344,65325,65302,65280,65253,
65226,65196,65164,65129,65091,65051,65007,64962,64911,64858,64801,64740,64674,64606,
64531,64453,64369,64281,64186,64087,63980,63869,63751,63626,63494,63356,63209,63055,
62893,62724,62544,62358,62161,61956,61741,61516,61281,61036,60781,60514,60236,59947,
59647,59335,59010,58674,58326,57964,57590,57202,56803,56389,55962,55522,55068,54601,
54119,53623,53115,52591,52054,51504,50939,50360,49768,49162,48543,47910,47264,46604,
45933,45249,44552,43843,43122,42390,41647,40894,40129,39355,38571,37778,36978,36168,
35352,34528,33697,32862,32021,31175,30326,29472,28617,27760,26900,26042,25182,24324,
23467,22613,21762,20915,20072,19234,18404,17580,16763,15955,15155,14367,13588,12821,
12066,11324,10594,9879,9179,8494,7824,7172,6536,5919,5318,4736,4174,3631,3108,2604,
2121,1660,1218,798,400,23,-332,-666,-978,-1268,-1536,-1783,-2009,-2212,-2396,-2557,
-2698,-2820,-2921,-3001,-3064,-3108,-3132,-3139,-3129,-3103,-3059,-3001,-2928,-2840,
-2739,-2626,-2499,-2362,-2214,-2056,-1889,-1714,-1531,-1342,-1147,-946,-742,-533,
-323,-110,104,319,532,745,957,1164,1370,1570,1768,1959,2145,2324,2496,2662,2818,
2968,3108,3239,3360,3472,3573,3665,3745,3816,3875,3923,3960,3987,4002,4006,3999,
3982,3954,3915,3866,3807,3739,3661,3573,3478,3373,3261,3141,3014,2880,2740,2595,
2444,2289,2129,1965,1800,1631,1461,1288,1115,943,769,597,426,258,90,-73,-234,-390,
-543,-691,-833,-971,-1102,-1228,-1346,-1458,-1564,-1660,-1751,-1833,-1907,-1974,
-2033,-2082,-2124,-2158,-2182,-2200,-2209,-2210,-2202,-2188,-2164,-2135,-2097,-2052,
-2001,-1943,-1880,-1809,-1734,-1653,-1567,-1478,-1383,-1284,-1184,-1079,-973,-863,
-753,-641,-528,-415,-301,-189,-76,34,144,252,357,459,560,656,748,838,923,1003,1079,
1150,1217,1277,1332,1383,1427,1466,1499,1526,1548,1564,1573,1577,1576,1568,1556,
1538,1515,1486,1452,1415,1372,1325,1274,1219,1160,1099,1034,966,896,824,749,673,597,
517,439,360,280,200,122,44,-33,-110,-183,-256,-326,-394,-460,-522,-583,-639,-693,
-744,-790,-833,-872,-907,-939,-967,-990,-1009,-1024,-1036,-1042,-1046,-1045,-1040,
-1032,-1019,-1003,-984,-961,-934,-906,-873,-838,-800,-761,-718,-673,-628,-579,-531,
-480,-428,-377,-323,-270,-217,-163,-110,-58,-6,46,96,144,193,238,283,325,366,405,
441,475,507,536,563,587,608,627,642,657,666,674,680,681,681,679,672,665,654,641,
626,609,590,568,545,521,494,467,437,407,375,344,310,277,242,208,174,139,104,71,36,
2,-30,-62,-94,-124,-154,-181,-209,-234,-258,-281,-302,-322,-340,-356,-371,-383,
-395,-403,-412,-417,-421,-423,-424,-423,-420,-415,-410,-402,-393,-382,-371,-359,
-344,-329,-313,-295,-278,-260,-240,-220,-200,-179,-158,-137,-116,-94,-73,-52,-31,
-10,9,29,49,67,85,102,118,134,149,162,175,187,197,208,216,223,230,236,240,243,245,
246,247,246,243,241,237,232,227,221,213,205,197,188,178,168,158,146,134,123,111,99,
87,74,62,49,37,25,13,2,-11,-21,-32,-42,-53,-62,-71,-80,-87,-95,-102,-108,-114,
-119,-123,-127,-130,-133,-134,-136,-137,-137,-136,-135,-134,-131,-129,-126,-122,
-119,-113,-110,-104,-98,-93,-87,-81,-75,-68,-62,-55,-48,-41,-35,-27,-22,-14,-8,-2,4,
11,16,21,27,32,37,41,45,49,53,56,58,61,64,65,66,68,68,69,70,68,69,68,66,66,64,62,
60,57,55,53,50,47,44,40,38,34,31,27,24,20,17,13,10,7,3,0,-3,-6,-9,-12,-14,-17,-19,
-21,-23,-25,-27,-29,-29,-31,-32,-33,-33,-34,-34,-34,-34,-34,-34,-33,-32,-32,-31,
-30,-29,-27,-26,-25,-24,-22,-20,-19,-17,-15,-14,-12,-10,-9,-7,-5,-4,-2,0,1,3,4,5,6,
8,9,10,11,12,12,13,14,15,14,16,15,16,16,15,16,16,15,15,15,15,14,13,13,12,12,11,
11,9,9,8,8,6,6,5,5,3,3,2,1,0,0,-1,-1,-3,-2,-4,-3,-5,-4,-5,-6,-5,-6,-7,-6,-7,-6,-7,
-7,-7,-7,-6,-7,-7,-6,-6,-6,-6,-5,-6,-5,-4,-5,-4,-3,-4,-3,-2,-3,-2,-1,-2,-1,0,-1,0,
0,1,1,1,1,1,2,2,2,2,2,2,3,2,3,3,2,3,3,2,3,3,2,3,2,2,3,2,2,2,1,2,1,2,1,1,1,1,1,0,
1,0,0,0,0,0,0,0,-1,0,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,
-1,-1,0,-1,0,-1,0,-1,0,0,0,-1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,
0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
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
                volume |= ((*(&ym->reg.name.vol_a + i) & LEVEL_LEVEL_MASK) << 1) | 1;
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
                (((orig->noise_state ^ (orig->noise_state >> 2)) & 1) << 16);
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
    return hz;
}

int ym_blep_setup(ym_t * const ym) {
    ym->cb_cleanup       = cleanup;
    ym->cb_reset         = reset;
    ym->cb_run           = run;
    ym->cb_buffersize    = buffersize;
    ym->cb_sampling_rate = sampling_rate;
    return 0;
}
