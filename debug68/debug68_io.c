/*
 *                   debug68 - fake IO emulation                      
 *
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
 * $Id: sc68.c 57 2009-01-31 18:24:54Z benjihan $
 */

#include "debug68_io.h"
#include "emu68/ioplug68.h"

#define DIO(NAME,DESC,LO,HI)					\
  {								\
    {								\
      0,DESC,LO,HI,						\
	any_readB,any_readW,any_readL,				\
	any_writeB,any_writeW,any_writeL,			\
	any_int,any_nextint,any_adjust,any_reset,any_destroy,	\
	0							\
	},							\
      0,NAME##_read, NAME##_write, NAME##_reset			\
	}


/***********************/
/* Common to all chips */
/***********************/

static int68_t dummy_read(debug68_io_t * dio, addr68_t addr) {return 0;}
static void dummy_write(debug68_io_t * dio, addr68_t addr, int68_t val) {}
static int dummy_reset(debug68_io_t * dio) { return 0; }

static void any_readB(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  addr68_t addr = emu68->bus_addr;
  debug68_io_t * const dio = (debug68_io_t *) io;
  dio->access |= 1;
  emu68->bus_data = !dio->read ? 0 : dio->read(dio,addr&0xFF);
}

static void any_readW(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  addr68_t addr = emu68->bus_addr;
  debug68_io_t * const dio = (debug68_io_t *) io;
  int68_t a0, a1;
  dio->access |= 1;
  a0 = dio->read(dio,(addr+0)&255) & 255;
  a1 = dio->read(dio,(addr+1)&255) & 255;
  emu68->bus_data = (a0<<8) + a1;
}

static void any_readL(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  addr68_t addr = emu68->bus_addr;
  debug68_io_t * const dio = (debug68_io_t *) io;
  int68_t a0, a1, a2, a3;
  dio->access |= 1;
  a0 = dio->read(dio,(addr+0)&255) & 255;
  a1 = dio->read(dio,(addr+1)&255) & 255;
  a2 = dio->read(dio,(addr+2)&255) & 255;
  a3 = dio->read(dio,(addr+3)&255) & 255;
  emu68->bus_data = (a0<<24) + (a1<<16) + (a2<<8) + a3;
}

static void
any_writeB(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  debug68_io_t * const dio = (debug68_io_t *) io;
  addr68_t addr = emu68->bus_addr;
  int68_t data  = emu68->bus_data;
  dio->access |= 2;
  dio->write(dio,addr&255,data&255);
}

static void
any_writeW(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  debug68_io_t * const dio = (debug68_io_t *) io;
  addr68_t addr = emu68->bus_addr;
  int68_t  data = emu68->bus_data;
  dio->access |= 2;
  dio->write(dio,(addr+0)&255,(data>>8)&255);
  dio->write(dio,(addr+1)&255,(data>>0)&255);
}

static void
any_writeL(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  debug68_io_t * const dio = (debug68_io_t *) io;
  addr68_t addr = emu68->bus_addr;
  int68_t  data = emu68->bus_data;
  dio->access |= 2;
  dio->write(dio,(addr+0)&255,(data>>24)&255);
  dio->write(dio,(addr+1)&255,(data>>16)&255);
  dio->write(dio,(addr+2)&255,(data>> 8)&255);
  dio->write(dio,(addr+3)&255,(data>> 0)&255);
}

static interrupt68_t *any_int(io68_t * const io, const cycle68_t cycle) {
  return 0;
}

static cycle68_t any_nextint(io68_t * const io, const cycle68_t cycle) {
  return IO68_NO_INT;
}

static void any_adjust(io68_t * const io, const cycle68_t sub) {}

static int any_reset(io68_t * io) {
  debug68_io_t * const dio = (debug68_io_t *) io;
  dio->access = 0;
  return dio->reset(dio);
}

static void any_destroy(io68_t * const io) {}

/* ----------------------------------------------------------------------
   Paula Part
   ---------------------------------------------------------------------- */

static int68_t pl_read(debug68_io_t * dio, addr68_t addr);
#define pl_write dummy_write
static int pl_reset(debug68_io_t * dio);

struct pl_io_t {
  debug68_io_t io;
  u8 vposr;
} pl_io = {
  DIO(pl,"AMIGA Paula",0xFFDFF000,0xFFDFF0DF),
  0
};

static int68_t pl_read(debug68_io_t * dio, addr68_t addr)
{
  struct pl_io_t * pl = (struct pl_io_t *) dio;
  if (addr == 6) {
    /* Big Cheat : Increment vertical raster pos at each read */
    return pl->vposr++;
  }
  return 0;
}


static int pl_reset(debug68_io_t * dio)
{
  struct pl_io_t * pl = (struct pl_io_t *) dio;
  pl->vposr = 0;
  return 0;
}

/* ----------------------------------------------------------------------
   YM-2149 Part
   ---------------------------------------------------------------------- */

static int68_t ym_read(debug68_io_t * dio, addr68_t addr);
static void ym_writeB(debug68_io_t * dio, addr68_t addr, int68_t v);
static int ym_reset(debug68_io_t * dio);


struct ym_io_t {
  debug68_io_t io;
  int cur;
  u8 reg[256];
} ym_io = {
  DIO(pl,"YM-2149",0xFFFF8800,0xFFFF88FF)
};

static int68_t ym_read(debug68_io_t * dio, addr68_t addr)
{
  struct ym_io_t * ym = (struct ym_io_t *) dio;
  if (addr&3) return 0;
  return ym->reg[ym->cur&255];
}

static void ym_writeB(debug68_io_t * dio, addr68_t addr, int68_t v)
{
  struct ym_io_t * ym = (struct ym_io_t *) dio;
  addr &= 2;
  if(!addr) ym->cur = v&255;	 /* control register          */
  else ym->reg[ym->cur&255] = v; /* data register [ control ] */
}

static int ym_reset(debug68_io_t * dio)
{
  struct ym_io_t * ym = (struct ym_io_t *) dio;
  int i;
  for (i=0; i<sizeof(ym->reg); ++i) {
    ym->reg[i] = 0;
  }
  ym->cur = 0;
  return 0;
}


/* ----------------------------------------------------------------------
   Microwire and LMC Part
   ---------------------------------------------------------------------- */

#define mw_read  dummy_read
#define mw_write dummy_write
#define mw_reset dummy_reset

struct mw_io_t {
  debug68_io_t io;
} mw_io = {
  DIO(mw,"STE-MicroWire", 0xFFFF8900, 0xFFFF8925)
};

/* ----------------------------------------------------------------------
   Shifter Part
   ---------------------------------------------------------------------- */

static int68_t sh_read(debug68_io_t *, addr68_t);
static void    sh_write(debug68_io_t *, addr68_t, int68_t);
static int     sh_reset(debug68_io_t *);

struct sh_io_t {
  debug68_io_t io;
  u8 data_0a;
  u8 data_60;
} sh_io = {
  DIO(sh,"Shifter",0xFFFF8200, 0xFFFF82FF)
};

static int sh_reset(debug68_io_t * dio)
{
  struct sh_io_t * sh = (struct sh_io_t *) dio;
  sh->data_0a = 0xfe;
  sh->data_60 = 0x00;
  return 0;
}

static int68_t sh_read(debug68_io_t * dio, addr68_t addr)
{
  struct sh_io_t * sh = (struct sh_io_t *) dio;
  switch (addr & 255) {
  case 0x0a: return sh->data_0a;
  case 0x60: return sh->data_60;
  }
  return 0;
}


static void sh_write(debug68_io_t * dio, addr68_t addr, int68_t data)
{
  struct sh_io_t * sh = (struct sh_io_t *) dio;
  switch (addr & 255) {
  case 0x0a: sh->data_0a = data; break;
  case 0x60: sh->data_60 = data; break;
  }
}
  


int debug68_io_plug(emu68_t * emu68) {
  if (!emu68) return -1;
  emu68_ioplug(emu68, &ym_dio->io);
  emu68_ioplug(emu68, &sh_dio->io);
  emu68_ioplug(emu68, &mw_dio->io);
  emu68_ioplug(emu68, &pl_dio->io);
  return 0;
}

void debug68_io_unplug(emu68_t * emu68) {
  if (!emu68) return;
  emu68_ioplug_unplug_all(emu68);
}


/* Exported io  */
debug68_io_t * mw_dio = &mw_io.io;
debug68_io_t * pl_dio = &pl_io.io;
debug68_io_t * sh_dio = &sh_io.io;
debug68_io_t * ym_dio = &ym_io.io;
