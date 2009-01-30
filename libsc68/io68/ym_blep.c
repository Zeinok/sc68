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

#include <string.h>

extern int ym_feature;		/* defined in ymemul.c */

static void ym2149_clock(ym_t * const ym, cycle68_t cycles) {
}

static int ym2149_output() {
    return 0;
}

/* run output synthesis for some clocks */
static int ym_mix_to_buffer(ym_t * const ym, cycle68_t cycles, s32 *output) {
    int len = 0;
    int cycles_to_next_sample = 40 << 8;
    int cycles_per_sample = 40 << 8;
    while (cycles) {
        cycle68_t iter = cycles;
        int makesample = 0;
        if (iter >= cycles_to_next_sample >> 16) {
            iter = cycles_to_next_sample >> 16;
            makesample = 1;
        }

        /* Simulate ym2149 for iter clocks */
        ym2149_clock(ym, iter);
        cycles -= iter;
        cycles_to_next_sample -= iter << 16;

        /* Generate output */
        if (makesample) {
            output[len] = ym2149_output();
            cycles_to_next_sample = cycles_per_sample;
            len ++;
        }
    }
    return len;
}

static void cleanup(ym_t * const ym) {}

static int reset(ym_t * const ym, const cycle68_t ymcycle)
{
    //ym2149_reset();
    return 0;
}

/* mix for ymcycles cycles. */
static int run(ym_t * const ym, s32 * output, const cycle68_t ymcycles)
{
    /* Walk  the static list of allocated events */
    int currcycle = 0;
    ym_waccess_t *access;
    for (access = ym->waccess; access != ym->waccess_nxt; access ++) {
        if (access->ymcycle > ymcycles) {
            TRACE68(ym_feature, "access reg %X out of frame: (%u > %u)\n",
                    access->reg, access->ymcycle, ymcycles);
        }

        /* mix up to this cycle, update state */
        output += ym_mix_to_buffer(ym, access->ymcycle - currcycle, output);
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
    return 0;
}

/* Get required length of buffer at run(s32 *output) (number of frames). */
static uint68_t buffersize(const ym_t const * ym, const cycle68_t ymcycles)
{
    return 2048; /* suffices to up to 96 kHz */
}

int ym_blep_setup(ym_t * const ym) {
    ym->cb_cleanup       = cleanup;
    ym->cb_reset         = reset;
    ym->cb_run           = run;
    ym->cb_buffersize    = buffersize;
    ym->cb_sampling_rate = (void*)0;
    return 0;
}


