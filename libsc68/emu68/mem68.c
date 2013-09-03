/*
 * @file    emu68/mem68.c
 * @brief   68K memory and IO access
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-09-03 23:39:02 ben>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* generated config include */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "mem68.h"
#include "assert68.h"

/** Test for direct memory access or IO quick table access */
#define ISIO68(ADDR) ((ADDR)&0x800000)

/* Set memory access check flags. */
static void chkframe(emu68_t * const emu68, addr68_t addr, const int flags)
{
  int oldchk;

  assert( ! ISIO68(addr) );
  addr &= MEMMSK68;
  oldchk = emu68->chk[addr];
  if ( ( oldchk & flags ) != flags ) {
    emu68->framechk |= flags;
    emu68->chk[addr] = oldchk|flags;
  }
}

static void chkframe_b(emu68_t * const emu68, const int flags)
{
  chkframe(emu68, emu68->bus_addr, flags);
}

static void chkframe_w(emu68_t * const emu68, const int flags)
{
  chkframe(emu68, emu68->bus_addr+0, flags);
  chkframe(emu68, emu68->bus_addr+1, flags);
}

static void chkframe_l(emu68_t * const emu68, const int flags)
{
  chkframe(emu68, emu68->bus_addr+0, flags);
  chkframe(emu68, emu68->bus_addr+1, flags);
  chkframe(emu68, emu68->bus_addr+2, flags);
  chkframe(emu68, emu68->bus_addr+3, flags);
}


/* ,--------------------------------------------------------.
 * |                                                        |
 * |            68000 memory access functions               |
 * |                                                        |
 * |   These functions don't check address/bus access nor   |
 * |   and privilege access violation.                      |
 * |                                                        |
 * `--------------------------------------------------------'
 */


/* ,--------------------------------------------------------.
 * |                      Dummy IO                          |
 * `--------------------------------------------------------'
 */

static void memchk_rb(io68_t * const io) {
  emu68_t * const emu68 = io->emu68;
  const addr68_t addr = emu68->bus_addr;

  emu68->bus_data = emu68->mem[addr&MEMMSK68];
  chkframe_b(emu68, EMU68_R);
}

static void memchk_rw(io68_t * const io) {
  emu68_t * const emu68 = io->emu68;
  const u8 * const mem = emu68->mem+(emu68->bus_addr&MEMMSK68);
  emu68->bus_data = (mem[0]<<8) + mem[1];
  chkframe_w(emu68, EMU68_R);
}

static void memchk_rl(io68_t * const io) {
  emu68_t * const emu68 = io->emu68;
  const u8 * const mem = emu68->mem+(emu68->bus_addr&MEMMSK68);
  emu68->bus_data = (mem[0]<<24) + (mem[1]<<16) + (mem[2]<<8) + mem[3];
  chkframe_l(emu68, EMU68_R);
}

static void memchk_wb(io68_t * const io) {
  emu68_t * const emu68 = io->emu68;
  const addr68_t addr = emu68->bus_addr;
  emu68->mem[addr&MEMMSK68] = emu68->bus_data;
  chkframe_b(emu68, EMU68_W);
}

static void memchk_ww(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  u8 * mem = emu68->mem + (emu68->bus_addr&MEMMSK68);
  int68_t v = emu68->bus_data;
  mem[1] = v; v>>=8; mem[0] = v;
  chkframe_w(emu68, EMU68_W);
}

static void memchk_wl(io68_t * const io)
{
  emu68_t * const emu68 = io->emu68;
  u8 * mem = emu68->mem + (emu68->bus_addr&MEMMSK68);
  int68_t v = emu68->bus_data;
  mem[3] = v; v>>=8; mem[2] = v; v>>=8; mem[1] = v; v>>=8; mem[0] = v;
  chkframe_l(emu68, EMU68_W);
}

static void dummy_ra(io68_t * const io)
{
  assert(!"read access to dummy IO");
}

static void dummy_wa(io68_t * const io)
{
  assert(!"write access to dummy IO");
}

static const io68_t dummy_io = {
  0,"Fault",0,0,
  dummy_ra,dummy_ra,dummy_ra,
  dummy_wa,dummy_wa,dummy_wa,
};

static int  memchk_reset(io68_t * const io) { return 0; }
static void memchk_destroy(io68_t * const io) { emu68_free(io); }

static const io68_t memory_io = {
  0,"RAM",0,0,
  memchk_rb,memchk_rw,memchk_rl,
  memchk_wb,memchk_ww,memchk_wl,
  0,0,0,memchk_reset,memchk_destroy
};

io68_t * mem68_io(void)
{
  io68_t * io = emu68_alloc(sizeof(memory_io));
  if (io)
    *io = memory_io;
  return io;
}


/* ,--------------------------------------------------------.
 * |                   Read functions                       |
 * `--------------------------------------------------------'
 */

void mem68_read_b(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;

  if (ISIO68(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->r_byte(io);
  } else if (!emu68->memio) {
    emu68->bus_data = emu68->mem[addr&MEMMSK68];
  } else {
    emu68->memio->r_byte(emu68->memio);
  }
}

void mem68_read_w(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;
  if (ISIO68(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->r_word(io);
  } else if (!emu68->memio) {
    const u8 * const mem = emu68->mem+(addr&MEMMSK68);
    emu68->bus_data = (mem[0]<<8) + mem[1];
  } else {
    emu68->memio->r_word(emu68->memio);
  }
}

void mem68_read_l(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;
  if (ISIO68(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->r_long(io);
  } else if (!emu68->memio) {
    const u8 * const mem = emu68->mem+(addr&MEMMSK68);
    emu68->bus_data = (mem[0]<<24) + (mem[1]<<16) + (mem[2]<<8) + mem[3];
  } else {
    emu68->memio->r_long(emu68->memio);
  }
}

/* ,--------------------------------------------------------.
 * |                   Write functions                      |
 * `--------------------------------------------------------'
 */

void mem68_write_b(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;
  if (ISIO68(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->w_byte(io);
  } else if (!emu68->memio) {
    emu68->mem[addr&MEMMSK68] = emu68->bus_data;
  } else {
    emu68->memio->w_byte(emu68->memio);
  }
}

void mem68_write_w(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;
  if (ISIO68(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->w_word(io);
  } else if (!emu68->memio) {
    u8 * mem = emu68->mem + (addr&MEMMSK68);
    int68_t v = emu68->bus_data;
    mem[1] = v; v>>=8; mem[0] = v;
  } else {
    emu68->memio->w_word(emu68->memio);
  }
}

void mem68_write_l(emu68_t * const emu68)
{
  const addr68_t addr = emu68->bus_addr;
  if (ISIO68(addr)) {
    io68_t * const io = emu68->mapped_io[(u8)((addr)>>8)];
    io->w_long(io);
  } else if (!emu68->memio) {
    u8 * mem = emu68->mem + (addr&MEMMSK68);
    int68_t v = emu68->bus_data;
    mem[3] = v; v>>=8; mem[2] = v; v>>=8; mem[1] = v; v>>=8; mem[0] = v;
  } else {
    emu68->memio->w_long(emu68->memio);
  }
}


/* Read 68000 (PC)+ word
 * - This version assume PC is in 68000 memory
 *   as long I don't try to make 68000 execute
 *   @ IO address, I assume it is not possible !
 */

int68_t mem68_nextw(emu68_t * const emu68)
{
  int68_t v;
  u8 *mem;

  assert( ! ISIO68(REG68.pc) );

  mem = emu68->mem + ( REG68.pc & MEMMSK68 );
  v  = (int68_t)(s8)mem[0]<<8;
  v |=              mem[1];

  if ( emu68->chk ) {
    chkframe(emu68, REG68.pc+0, EMU68_R);
    chkframe(emu68, REG68.pc+1, EMU68_R);
  }
  REG68.pc += 2;
  return v;
}

int68_t mem68_nextl(emu68_t * const emu68)
{
  int68_t v;
  u8 *mem;

  assert( ! ISIO68(REG68.pc) );

  mem = emu68->mem + ( REG68.pc & MEMMSK68 );
  v  = (int68_t)(s8)mem[0]<<24;
  v |=              mem[1]<<16;
  v |=              mem[2]<< 8;
  v |=              mem[3];
  if ( emu68->chk ) {
    chkframe(emu68, REG68.pc+0, EMU68_R);
    chkframe(emu68, REG68.pc+1, EMU68_R);
    chkframe(emu68, REG68.pc+2, EMU68_R);
    chkframe(emu68, REG68.pc+3, EMU68_R);
  }
  REG68.pc += 4;
  return v;
}

void mem68_pushw(emu68_t * const emu68, const int68_t v)
{
  REG68.a[7] -= 2;
  write_W(REG68.a[7],v);
}

void mem68_pushl(emu68_t * const emu68, const int68_t v)
{
  REG68.a[7] -= 4;
  write_L(REG68.a[7],v);
}

int68_t mem68_popl(emu68_t * const emu68)
{
  int68_t v = (s32)read_L(REG68.a[7]);
  REG68.a[7] += 4;
  return v;
}

int68_t mem68_popw(emu68_t * const emu68)
{
  s32 v = read_W(REG68.a[7]);
  REG68.a[7] += 2;
  return v;
}

void emu68_pushl(emu68_t * const emu68, int68_t val)
{
  if (emu68)
    mem68_pushl(emu68, val);
}

void emu68_pushw(emu68_t * const emu68, int68_t val)
{
  if (emu68)
    mem68_pushw(emu68, val);
}


int68_t emu68_popl(emu68_t * const emu68)
{
  return emu68
    ? mem68_popl(emu68)
    : -1
    ;
}

int68_t emu68_popw(emu68_t * const emu68)
{
  return emu68
    ? mem68_popw(emu68)
    : -1
    ;
}

/* Init memory quick acces table for SC68
 */
void emu68_mem_init(emu68_t * const emu68)
{
  emu68_mem_reset(emu68);
}

void emu68_mem_destroy(emu68_t * const emu68)
{
  if (emu68 && emu68->memio) {
    if (emu68->memio->destroy)
      emu68->memio->destroy(emu68->memio);
    else
      free(emu68->memio);
    emu68->memio = 0;
  }
}

/* Reset memory quick acces table
 */
void emu68_mem_reset(emu68_t * const emu68)
{
  if (emu68) {
    int i;
    for(i=0; i<256; i++)
      emu68->mapped_io[i] = (io68_t *)&dummy_io;
  }
}

/* Reset memory acces control area to normal state :
 */
void emu68_mem_reset_area(emu68_t * const emu68, u8 area)
{
  emu68->mapped_io[area] = (io68_t *)&dummy_io;
}
