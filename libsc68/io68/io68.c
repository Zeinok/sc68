/*
 *                 sc68 - IO plugin initialization
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
 */

/* $Id$ */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "io68.h"

static volatile int isinit_paula;
static volatile int isinit_ym;
static volatile int isinit_mw;
static volatile int isinit_mfp;
static volatile int isinit_shifter;

int io68_init(io68_init_t * const parms)
{
  int err = 0;

  /* Copy argc and argv where applicable. */
  if (parms) {
    parms->mw.argc = parms->ym.argc = parms->paula.argc = parms->argc;
    parms->mw.argv = parms->ym.argv = parms->paula.argv = parms->argv;
  }

  if (!isinit_paula) {
    isinit_paula = !paulaio_init(parms?&parms->paula:0);
  }
  if (!isinit_ym) {
    isinit_ym = !ymio_init(parms?&parms->ym:0);
  }
  if (!isinit_mw) {
    isinit_mw = !mwio_init(parms?&parms->mw:0);
  }
  if (!isinit_mfp) {
    isinit_mfp = !mfpio_init();
  }
  if (!isinit_shifter) {
    isinit_shifter = !shifterio_init();
  }

  err = -!(1
           & isinit_paula
           & isinit_ym
           & isinit_mw
           & isinit_mfp
           & isinit_shifter
    );

  return err;
}

void io68_destroy(io68_t * const io)
{
  if (io && io->destroy) {
    io->destroy(io);
  }
}

void io68_shutdown(void)
{
  if (isinit_paula) {
    paulaio_shutdown();
    isinit_paula = 0;
  }
  if (isinit_ym) {
    ymio_shutdown();
    isinit_ym = 0;
  }
  if (isinit_mw) {
    mwio_shutdown();
    isinit_mw = 0;
  }
  if (isinit_mfp) {
    mfpio_shutdown();
    isinit_mfp = 0;
  }
  if (isinit_shifter) {
    shifterio_shutdown();
    isinit_shifter = 0;
  }
}
