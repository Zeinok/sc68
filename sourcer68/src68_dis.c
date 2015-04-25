/*
 * @file    src68_dis.c
 * @brief   disassembler sourcer.
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
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

#include "src68_dis.h"
#include "src68_mbk.h"
#include "src68_msg.h"

#include <string.h>
#include <stdio.h>

#include <desa68.h>
typedef struct {
  desa68_t desa;
  uint_t  maxdepth;
  uint_t  addr;
  uint_t  flag;
  uint_t  mib;
  char    str[256];
  mbk_t  *mbk;
  int     result;

} dis_t;

static int memget(desa68_t * d, unsigned int addr)
{
  dis_t * dis = d->user;
  mbk_t * mbk = dis->mbk;
  int mib, old;

  mib = MIB_READ | ( (-(addr == dis->addr)) & MIB_EXEC );

  addr -= mbk->org;
  if (addr >= mbk->len)
    return -1;

  old  = mbk->mib[addr];
  mib |= old;
  mbk->mib[addr] = mib;
  dis->mib |= mib ^ old;                /* what's up doc ? */

  return mbk->mem[addr];
}

static const char hexa[] = "0123456789ABCDEF";
static void hexordot(char *s, int v)
{
  if (v >= 0 && v < 256) {
    s[0] = hexa[(v >> 4) & 15];
    s[1] = hexa[v & 15];
  } else
    s[0] = s[1] = '.';
}

static char * hexstr(const mbk_t * mbk, uint_t a, uint_t b)
{
  static char tmp[64];
  const int max = sizeof(tmp)-1;
  int i;
  a -= mbk->org;
  b -= mbk->org;
  for (i=0; a<b; ++a) {
    if (i>0 && i<max)
      tmp[i++] = '-';
    if (i+2<max) {
      hexordot(tmp+i, (a < mbk->len) ? mbk->mem[a] : -1);
      i += 2;
    }
  }
  tmp[i] = 0;
  return tmp;
}

static const char * regname(uint8_t reg)
{
  static int rol;
  static char tmp[64];

  if (reg < 16) {
    char *s = tmp + (rol++ & 15) * 4;
    s[0] = "DA"[reg>>3];
    s[1] = '0' + (reg&7);
    s[2] = 0;
    return s;
  } else  {
    switch (reg) {
    case DESA68_REG_USP: return "USP";
    case DESA68_REG_CCR: return "CCR";
    case DESA68_REG_SR:  return "SR";
    case DESA68_REG_PC:  return "PC";
    }
  }
  dmsg("invalid register -- %d\n", reg);
  assert(!"invalid register");
  return "?";
}

enum {
  BRK_NOT, BRK_RTS, BRK_JMP, BRK_JTB, BRK_EXE, BRK_OOR
};

static const char * itypestr(int itype)
{
  switch (itype) {
  case DESA68_DCW: return "DCW";
  case DESA68_INS: return "INS";
  case DESA68_BRA: return "BRA";
  case DESA68_BSR: return "BSR";
  case DESA68_RTS: return "RTS";
  case DESA68_INT: return "INT";
  case DESA68_NOP: return "NOP";
  }
  return "???";
}

static void r_dis_pass(dis_t * dis, int depth)
{
  char rts; uint_t save_mib;

  dmsg(";;; ENTER $%08X (%d)\n", dis->addr, depth);

  if (dis->maxdepth && depth > dis->maxdepth) {
    wmsg(dis->addr,"skip (level %u is too deep)\n", depth);
    return;
  }

  save_mib = dis->mib;
  dis->mib = 0;
  dis->desa.pc = dis->addr;

  for (rts=BRK_NOT; rts == BRK_NOT; ) {
    unsigned int off = dis->desa.pc - dis->mbk->org;

    /* dmsg("<<<< %08x $%08x $%02x $%02x \n", */
    /*      dis->desa.pc,off,dis->mbk->mib[off], dis->mib); */

    if ( off < dis->mbk->len && (dis->mbk->mib[off] & MIB_EXEC)) {
      rts = BRK_EXE;
      break;
    }

    if (off >= dis->mbk->len) {
      rts = BRK_OOR;
      break;
    }

    dis->addr = dis->desa.pc;
    desa68(&dis->desa);

    if (1) {
      int r;
      dmsg("\n"
           "type: %s $%04X\n", itypestr(dis->desa.itype), dis->desa._opw );
      dmsg("src-ref: %c %08x\n",
           "?BWL-"[1+(signed char)dis->desa.sref.type],
           dis->desa.sref.addr);
      dmsg("dst-ref: %c %08x\n",
           "?BWL-"[1+(signed char)dis->desa.dref.type],
           dis->desa.dref.addr);
      dmsg("regs:");
      for (r=0; r<DESA68_REG_LAST; ++r)
        if (dis->desa.regs & (1<<r))
          dmsg(" %s", regname(r));
      dmsg("\n");

    }


    /* dmsg(">>>> %08x $%08x $%02x $%02x \n", */
    /*      dis->desa.pc_org,off,dis->mbk->mib[off], dis->mib); */

    /* dmsg(";;; itype:%02x err:%02x flags:%08x w:$%04X org:%x-%x (%d)\n", */
    /*      dis->desa.itype, dis->desa.error, dis->desa.flags, */
    /*      (uint16_t) dis->desa.w, */
    /*      dis->desa.pc_org, dis->desa.pc, dis->desa.pc-dis->desa.pc_org); */

    if (dis->desa.error)
      wmsg(dis->addr, "disassembler error (ignored) -- %d\n",
           dis->desa.error);

    if (dis->desa.itype == DESA68_DCW) {
      wmsg(dis->addr, "invalid opcode -- %04x\n", dis->desa._w);
      break;
    }

    dmsg("$%08x%*s%-*s ;; %s\n", dis->addr,
         /* (depth+1)*2 */2, "",
         32-(depth+1)*2,dis->str,
         hexstr(dis->mbk,dis->addr,dis->desa.pc));

    switch (dis->desa.itype) {

    case DESA68_RTS:
      rts = BRK_RTS;
      break;

    case DESA68_INT: case DESA68_NOP:
      break;

    case DESA68_INS:
    {
      int i;
      for (i = 0; i < 2; ++i) {
        mbk_t * mbk = dis->mbk;
        struct desa68_ref * ref = !i ? &dis->desa.sref : &dis->desa.dref;
        uint_t adr = ref->addr, off = ref->addr - mbk->org, end = off;
        int mib = MIB_ADDR, old;

        if (ref->type == DESA68_OP_NDEF) /* set ? */
          continue;
        switch (ref->type) {
        case DESA68_OP_A: break;
        case DESA68_OP_B: case DESA68_OP_W: case DESA68_OP_L:
          if ( dis->desa.regs & ( 1 << DESA68_REG_PC ) ) {
            /* d8(pc,rn) mode: it's an address not an access */
          } else if ( (off & 1) && ref->type != DESA68_OP_B ) {
            /* odd address for word or long access. */
            wmsg(adr, "%s reference on odd address (+$%x)\n",
                 ref->type == DESA68_OP_W ? "word" : "long", off);
          } else {
            const int log2 = ref->type - DESA68_OP_B;
            end = off + ( 1 << log2 ) - 1;
            mib = MIB_BYTE << log2;
          }
          break;
        default:
          assert(!"invalid reference type");
          continue;
        }
        if (end < off || off >= mbk->len) /* out of range ? */
          continue;
        old = mbk->mib[off];
        mib |= old;
        mbk->mib[off] = mib;
        dis->mib |= mib ^ old;          /* what's up doc ? */

        if (mib ^ old)
          dmsg("tag $%06x = $%02x + $%02x\n", adr, old, mib ^ old);
      }
    } break;

    case DESA68_BRA:
      if (dis->desa.dref.type == DESA68_OP_A) {
        if (! (dis->desa.regs & (1<<DESA68_REG_PC) )) {
          dis->desa.pc = dis->desa.dref.addr;
          dmsg(";;; . . . . . . . . . . . . . . . . . . . . .\n");
          break;
        } else {
          /* jmp table : $$$ TODO */
        }
      }
      rts = BRK_JMP;                    /* unknown jump */
      break;

    case DESA68_BSR:
      if (dis->desa.dref.type == DESA68_OP_A) {
        uint_t save_pc = dis->desa.pc;
        if (! (dis->desa.regs & (1<<DESA68_REG_PC) )) {
          dis->addr = dis->desa.dref.addr;
          r_dis_pass(dis,depth+1);
        } else {
          /* jsr table : $$$ TODO */
        }
        dis->desa.pc = save_pc;
      }
      break;

    default:
      assert(!"unknowm instruction type");
      wmsg(dis->addr,"unknowm instruction type -- %d [$%x]\n",
           dis->desa.itype);
      break;
    }
  } while (!rts);

  switch (rts) {
  /* case BRK_NOT: */
  /*   dmsg(";;; ?????????????????????????????????????????\n"); break; */
  case BRK_RTS:
    dmsg(";;; -----------------------------------------\n"); break;
  case BRK_JMP:
    dmsg(";;; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); break;
  case BRK_EXE:
    dmsg(";;; .........................................\n"); break;
  case BRK_OOR:
    dmsg(";;; =========================================\n"); break;
  default:
    assert(!"invalid rts");
  }

  dis->mib = save_mib;
}


int dis_pass(uint_t entry, mbk_t * mbk)
{
  dis_t dis;
  memset(&dis, 0, sizeof(dis));

  // TEMP
  dis.desa.flags = DESA68_SYMBOL_FLAG | DESA68_GRAPH_FLAG;

  dis.addr = entry;
  dis.mbk = mbk;
  dis.desa.user = &dis;
  dis.desa.memget = memget;
  dis.desa.memorg = mbk->org;
  dis.desa.memlen = mbk->len;

  // TEMP
  dis.desa.immsym_min = mbk->org;
  dis.desa.immsym_max = mbk->len + dis.desa.immsym_min;

  dis.desa.str = dis.str;
  dis.desa.strmax = sizeof(dis.str);
  dis.maxdepth = 64;
  dis.result = 0;

  r_dis_pass(&dis, 0);
  return dis.result;
}
