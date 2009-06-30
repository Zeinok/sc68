/*
 *            sc68 - MicroWire IO plugin (STE soundchip)
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

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "mw_io.h"
#include "mwemul.h"
#include "emu68/struct68.h"

#define TOINTERNAL(N)                                                   \
  ((mwio->mw.map[N]<<16)+(mwio->mw.map[(N)+2]<<8)+(mwio->mw.map[(N)+4]))

typedef struct {
  io68_t io;
  mw_t mw;
} mw_io68_t;


static int68_t _mw_readB(mw_io68_t * const mwio, addr68_t const addr)
{
  int i = addr & 0x3F;

  /* Not Micro-Wire Ctrl/Data */
  if (i < MW_DATA) {
    /* Even line */
    if (!(i&1)) {
      return 0;
    }

    /* Sample counter */
    if(i>=MW_CTH && i<=MW_CTL) {
      i -= MW_CTH;           /*  0, 2, 4 */
      i  = 16 - (i<<2);      /* 16, 8, 0 */
      i += mwio->mw.ct_fix;
      return (u8)(mwio->mw.ct>>i);
    }
  }

  /* Micro-Wire Ctrl/Data */
  return mwio->mw.map[i];
}

static void mwio_readB(io68_t * const io)
{
  io->emu68->bus_data =
    _mw_readB((mw_io68_t *)io, io->emu68->bus_addr);
}

static void mwio_readW(io68_t * const io)
{
  io->emu68->bus_data = 0
    |(_mw_readB((mw_io68_t *)io, io->emu68->bus_addr+0)<<8)
    |(_mw_readB((mw_io68_t *)io, io->emu68->bus_addr+1)   )
    ;
}

static void mwio_readL(io68_t * const io)
{
  io->emu68->bus_data = 0
    |(_mw_readB((mw_io68_t *)io, io->emu68->bus_addr+0)<<24)
    |(_mw_readB((mw_io68_t *)io, io->emu68->bus_addr+1)<<16)
    |(_mw_readB((mw_io68_t *)io, io->emu68->bus_addr+2)<< 8)
    |(_mw_readB((mw_io68_t *)io, io->emu68->bus_addr+3)    )
    ;
}

static void mw_command(mw_io68_t * const mwio, int n)
{
  int c = n & 0700;

  n -= c;
  switch(c) {
  case 0000:
    mw_set_lmc_mixer(&mwio->mw, n&3);
    break;
  case 0100:
    mw_set_lmc_low(&mwio->mw, n&15);
    break;
  case 0200:
    mw_set_lmc_high(&mwio->mw, n&15);
    break;
  case 0300:
    mw_set_lmc_master(&mwio->mw, n&63);
    break;
  case 0400:
    mw_set_lmc_right(&mwio->mw, n&31);
    break;
  case 0500:
    mw_set_lmc_left(&mwio->mw, n&31);
    break;
  }
}

static void _mw_writeB(mw_io68_t * const mwio, addr68_t addr, int68_t v)
{
  addr &= 0x3F;

  /* Not Micro-Wire Ctrl/Data */
  if (addr<MW_DATA) {

    /* Even line */
    if (!(addr&1)) {
      return;
    }

    /* ACTIV register */
    if (addr==MW_ACTI) {
      v &= 3;

      /* Reload internal counter
       * $$$ Should we do this whatever the value or
       *     only if dma is started ?
       */
      mwio->mw.ct  = TOINTERNAL(MW_BASH) << mwio->mw.ct_fix;
      mwio->mw.end = TOINTERNAL(MW_ENDH) << mwio->mw.ct_fix;

      mwio->mw.map[MW_ACTI] = v;
      return;
    }

    /* Sample counter (RO) */
    if(addr>=MW_CTH && addr<=MW_CTL) {
      return;
    }

    mwio->mw.map[addr] = v;
    return;
  }

  /* Micro-Wire Data */
  if (addr < MW_CTRL) {
    uint68_t ctrl,data;
    mwio->mw.map[addr] = v;

    /* Write in high : skip */
    if (addr == MW_DATA) {
      return;
    }

    /* Write in low : process */
    ctrl = (mwio->mw.map[MW_CTRL]<<8) + mwio->mw.map[MW_CTRL+1];
    data = (mwio->mw.map[MW_DATA]<<8) + mwio->mw.map[MW_DATA+1];

    /* Find first address */
    for(; ctrl && (ctrl&0xC000)!=0xC000 ; ctrl<<=1, data<<=1)
      ;
    if (((data>>14) & 3) != 2) {
      return;
    }

    for (ctrl<<=2, data<<=2; ctrl && (ctrl&0xFF80) != 0xFF80;
         ctrl<<=1, data<<=1)
      ;
    if (ctrl) {
      mw_command(mwio, (data>>7) & 0x1ff);
    }
    return;
  }

  /* Micro-Wire Ctrl */
  mwio->mw.map[addr] = v;
}

static void mwio_writeB(io68_t * const io)
{
  _mw_writeB((mw_io68_t *)io,
             io->emu68->bus_addr, io->emu68->bus_data);
}

static void mwio_writeW(io68_t * const io)
{
  _mw_writeB((mw_io68_t *)io,
             io->emu68->bus_addr+0, io->emu68->bus_data>>8);
  _mw_writeB((mw_io68_t *)io,
             io->emu68->bus_addr+1, io->emu68->bus_data   );
}

static void mwio_writeL(io68_t * const io)
{
  _mw_writeB((mw_io68_t *)io,
             io->emu68->bus_addr+0, io->emu68->bus_data>>24);
  _mw_writeB((mw_io68_t *)io,
             io->emu68->bus_addr+1, io->emu68->bus_data>>16);
  _mw_writeB((mw_io68_t *)io,
             io->emu68->bus_addr+2, io->emu68->bus_data>> 8);
  _mw_writeB((mw_io68_t *)io,
             io->emu68->bus_addr+3, io->emu68->bus_data    );
}

static interrupt68_t * mwio_interrupt(io68_t * const io, cycle68_t cycle)
{
  return 0;
}

static cycle68_t mwio_next_interrupt(io68_t * const io, cycle68_t cycle)
{
  return IO68_NO_INT;
}

static void mwio_adjust_cycle(io68_t * const io, cycle68_t cycle)
{
}

static int mwio_reset(io68_t * const io)
{
  return mw_reset(&((mw_io68_t *)io)->mw);
}

static void mwio_destroy(io68_t * const io)
{
  if (io) {
    mw_cleanup(&((mw_io68_t *)io)->mw);
  }
}

static io68_t mw_io = {
  0,
  "STE-MicroWire",
  0xFFFF8900, 0xFFFF8925,
  mwio_readB,  mwio_readW,  mwio_readL,
  mwio_writeB, mwio_writeW, mwio_writeL,
  mwio_interrupt, mwio_next_interrupt,
  mwio_adjust_cycle,
  mwio_reset,
  mwio_destroy,
};

int mwio_init(mw_parms_t * const parms)
{
  return mw_init(parms);
}

void mwio_shutdown(void)
{
  mw_shutdown();
}

io68_t * mwio_create(emu68_t * const emu68, mw_parms_t * const parms)
{
  mw_io68_t * mwio = 0;

  if (emu68) {
    mwio = emu68_alloc(sizeof(*mwio));
    if (mwio) {
      mw_setup_t setup;
      if (parms) {
        setup.parms = *parms;
      } else {
        setup.parms.emul  = MW_EMUL_DEFAULT;
        setup.parms.hz    = 0;
      }
      setup.mem     = emu68->mem;
      setup.log2mem = emu68->log2mem;
      mwio->io = mw_io;
      mw_setup(&mwio->mw, &setup);
    }
  }
  return &mwio->io;
}

uint68_t mwio_sampling_rate(io68_t * const io, uint68_t sampling_rate)
{
  return io
    ? mw_sampling_rate(&((mw_io68_t*)io)->mw,sampling_rate)
    : sampling_rate
    ;
}

mw_t * mwio_emulator(io68_t * const io)
{
  return io
    ? &((mw_io68_t*)io)->mw
    : 0
    ;
}
