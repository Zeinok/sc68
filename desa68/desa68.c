/*
 * @file    de68.c
 * @brief   68000 disassembler
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "desa68.h"

#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif

/***********************************************************
 * Defines my types :                                      *
 * Should be OK without configure, since 32-bit and 16-bit *
 * are not really needed.                                  *
 **********************************************************/

#if defined(HAVE_STDINT_H)
#include <stdint.h>
typedef int8_t   s8;
typedef uint8_t  u8;
typedef int16_t  s16;
typedef uint16_t u16;
/* typedef int32_t  s32; */
/* typedef uint32_t u32; */
#else
typedef signed char    s8;
typedef unsigned char  u8;
typedef unsigned short u16;
typedef short          s16;
#endif

typedef int            s32;
typedef unsigned int   u32;

#define REG0(W)         (((W))&7)
#define REG9(W)         (((W)>>9)&7)
#define OPSZ(W)         (((W)>>6)&3)
#define LINE(W)         (((W)>>12)&15)
#define MODE3(W)        (((W)>>3)&7)
#define MODE6(W)        (((W)>>6)&7)

/******************************
 * Disassembler string tables *
 ******************************/

enum {
  MODE_DN=0,
  MODE_AN,
  MODE_iAN,
  MODE_ANp,
  MODE_pAN,
  MODE_dAN,
  MODE_dANXI,
  MODE_ABSW,
  MODE_ABSL,
  MODE_dPC,
  MODE_dPCXI,
  MODE_IMM
};

/* Hexa digit table */
static const char Thex[16] = {
  '0','1','2','3','4','5','6','7',
  '8','9','A','B','C','D','E','F'
};

/* Branch Condition Code String Tables */
static const u16 bcc_ascii[16] = {
  'RA','SR','HI','LS',
  'CC','CS','NE','EQ',
  'VC','VS','PL','MI',
  'GE','LT','GT','LE'
};

/* Scc Condition Code String Tables */
static const u16 scc_ascii[16] = {
  'T', 'F','HI','LS',
  'CC','CS','NE','EQ',
  'VC','VS','PL','MI',
  'GE','LT','GT','LE'
};

/* Dbcc Condition Code String Tables */
static const u16 dbcc_ascii[16] = {
  'RA', 'F','HI','LS',
  'CC','CS','NE','EQ',
  'VC','VS','PL','MI',
  'GE','LT','GT','LE'
};

/* static uint32_t regs[16] = { */
/*   'D0','D1','D2','D3','D4','D5','D6','D7', */
/*   'A0','A1','A2','A3','A4','A5','A6','A7', */
/*   'USP', 'CCR', 'SR', 'PC' */
/* }; */

/* ======================================================================
 * String functions *
 * ====================================================================== */

#define BIT6(W) ( ( (W) >> 6 ) & 1 )
#define BIT8(W) ( ( (W) >> 8 ) & 1 )

/* Add a char to disassembly string */
#define desa_char(D,C) (D)->strput((D),(C))

static void def_strput(desa68_t * d, int c)
{
  if (d->out < d->strmax)
    d->str[d->out++] = c;
  else if (d->str) {
    d->flags |= DESA68_ERR_OUT;
    if (d->strmax > 0)
      d->str[d->strmax-1] = 0;
  }
}

#if 0
static void def_strput(desa68_t * d, int c)
{
  unsigned int nxt;
  if (c != ' ')
    nxt = d->out + 1;
  else {
    const unsigned int tabs = 13;
    nxt = d->out + tabs - (d->out % tabs);
  }
  while (d->out < nxt) {
    if (d->out < d->strmax)
      d->str[d->out++] = c;
    else {
      if (d->str) {
        d->flags |= DESA68_ERR_OUT;
        if (d->strmax > 0)
          d->str[d->strmax-1] = 0;
      }
      nxt = d->out;
    }
  }
}
#endif

static inline void desa_space(desa68_t * d)
{
  desa_char(d,' ');
}

static inline void desa_comma(desa68_t * d)
{
  desa_char(d,',');
}

/* Add a string to disassembly string */
static void desa_str(desa68_t * d, char *str)
{
  char c;
  while (c=*str++, c)
    desa_char(d, c);
}

/* Add a string to disassembly string */
static void desa_ascii(desa68_t * d, unsigned int n)
{
  int shift;
  for (shift=24; shift>=0; shift-=8) {
    u8 c = (u8)(n >> shift);
    if (c)
      desa_char(d, c);
  }
}

/* Add a N-digit unsigned hexa number with header char
   to disassembly string */
static void desa_uhexacat(desa68_t * d, unsigned int n,
                          int ndigit, int header_char)
{
  int shf;
  desa_char(d,header_char);
  for (shf=(ndigit-1)*4; shf>=0; shf-=4) {
    desa_char(d,Thex[(n>>shf)&15] );
  }
}

/* Add a signifiant digit only unsigned hexa number
 * with heading '$' to disassembly string
 */
static void desa_usignifiant(desa68_t * d, unsigned int n)
{
  int shf;
  desa_char(d,'$');
  for (shf=(sizeof(int)*2-1)*4; shf>=0 && !(n>>shf); shf-=4)
    ;
  if (shf<0) shf=0;
  for (; shf>=0; shf-=4)
    desa_char(d,Thex[(n>>shf)&15] );
}

/* idem desa_usignifiant, but signed */
static void desa_signifiant(desa68_t * d, int n)
{
  if (n<0) {
    desa_char(d,'-');
    n = -n;
  }
  desa_usignifiant(d,n);
}

static int my_isfalse(desa68_t * d, int c)
{
  return 0;
}

static int my_isalnum(desa68_t * d, int c)
{
  return 0
    || (c>='a' && c<='z')
    || (c>='A' && c<='Z')
    || (c>='0' && c<='9');
}

static int my_isgraph(desa68_t * d, int c) {
  return c >= 32 && c < 127;
}

/* not really ASCII ... just a name */
static int my_isascii(desa68_t * d, int c)
{
  return
    c == '-' || c=='_' || c==' ' || c == '!' || c == '.' || c == '#'
    || (c>='a' && c<='z')
    || (c>='A' && c<='Z')
    || (c>='0' && c<='9');
}

/* ======================================================================
 * Memory access
 * ====================================================================== */

static int def_memget(desa68_t * d, unsigned int addr)
{
  addr &= d->memmask;
  return (addr >= d->memsize)
    ? -1
    : (u8)d->mem[addr]
    ;
}

static int get_uB(desa68_t * d, unsigned int addr)
{
  int c = d->memget(d,addr);
  if (c < 0) {
    d->error |= DESA68_ERR_MEM;
    c = 0;
  }
  return c;
}

#if 0 /* unused */
static int get_sB(desa68_t * d, unsigned int addr)
{
  return (s8) get_uB(d,addr);
}
#endif

static int get_uW(desa68_t * d, unsigned int addr)
{
  if ( addr & 1 )
    d->error |= DESA68_ERR_ODD;
  return ( get_uB(d,addr) << 8 ) | get_uB(d,addr+1);
}

static int get_sW(desa68_t * d, unsigned int addr)
{
  return (s16) get_uW(d,addr);
}

/* Read next word and increment pc */
static int read_pc(desa68_t * d)
{
  d->_w = get_sW(d, d->pc);
  d->pc += 2;
  return d->_w;
}

static int immB(desa68_t * d)
{
  return (s8) read_pc(d);
}

static int immW(desa68_t * d)
{
  return read_pc(d);
}

static int immL(desa68_t * d)
{
  return (read_pc(d)<<16) | (u16)read_pc(d);
}

static int adrW(desa68_t * d)
{
  return immW(d);
}

static int adrL(desa68_t * d)
{
  return immL(d);
}

static int relPC(desa68_t * d)
{
  read_pc(d);
  return (d->pc + d->_w - 2) & d->memmask;
}

/* ======================================================================
 * General disassembly function
 * ====================================================================== */

static void set_ea(desa68_t *d, unsigned int v)
{
  /* if not true _ea has not been commited at some point. */
  assert (d->_ea == DESA68_INVALID_ADDR);
  d->_ea = v & d->memmask;
}

static void set_src_ref(desa68_t * d)
{
  if (d->_ea != DESA68_INVALID_ADDR) {
    d->sref = d->_ea;
    d->_ea = DESA68_INVALID_ADDR;
    d->regs |= 1 << DESA68_REG_SREF;
  }
}

static void set_dst_ref(desa68_t * d)
{
  if (d->_ea != DESA68_INVALID_ADDR) {
    d->dref = d->_ea;
    d->_ea = DESA68_INVALID_ADDR;
    d->regs |= 1 << DESA68_REG_DREF;
  }
}

static void set_both_ref(desa68_t * d)
{
  if (d->_ea != DESA68_INVALID_ADDR) {
    d->sref = d->dref = d->_ea;
    d->_ea = DESA68_INVALID_ADDR;
    d->regs |= (1 << DESA68_REG_SREF) | (1 << DESA68_REG_DREF);
  }
}

static void set_branch(desa68_t * d, int itype)
{
  set_both_ref(d);
  d->itype = itype;
}

static void set_interrupt(desa68_t * d, unsigned int vector)
{
  set_ea(d,vector);
  set_branch(d,DESA68_INT);
}

static void desa_op_DN(desa68_t * d, u8 reg)
{
  assert(reg < 8);
  desa_char(d,'D');
  desa_char(d,'0'+reg);
  d->regs |= 1 << (DESA68_REG_DN+reg);
}

static void desa_op_AN(desa68_t * d, u8 reg)
{
  assert(reg < 8);
  desa_char(d,'A');
  desa_char(d,'0'+reg);
  d->regs |= 1 << (DESA68_REG_AN+reg);
}

static void desa_op_RN(desa68_t * d, u8 reg)
{
  assert(reg < 16);
  desa_char(d,"DA"[reg>>3]);
  desa_char(d,'0'+(reg&7));
  d->regs |= 1 << (DESA68_REG_DN+reg);
}

static void desa_op_iAN(desa68_t * d, u8 reg)
{
  desa_char(d,'(');
  desa_op_AN(d,reg);
  desa_char(d,')');
}

static void desa_op_pAN(desa68_t * d, u8 reg)
{
  desa_char(d,'-');
  desa_op_iAN(d,reg);
}

static void desa_op_ANp(desa68_t * d, u8 reg)
{
  desa_op_iAN(d,reg);
  desa_char(d,'+');
}

static void desa_op_anyreg(desa68_t * d, u8 reg)
{
  unsigned int name;
  if (reg < DESA68_REG_AN)
    name = 'D0'+(reg-DESA68_REG_DN);
  else if (reg <= DESA68_REG_SP)
    name = 'A0'+(reg-DESA68_REG_AN);
  else

    switch (reg) {
    case DESA68_REG_USP: name = 'USP'; break;
    case DESA68_REG_CCR: name = 'CCR'; break;
    case DESA68_REG_SR:  name = 'SR';  break;
    case DESA68_REG_PC:  name = 'PC';  break;
    default:
      assert(!"invalid register");
      name = 'R?';
    }
  desa_ascii(d,name);
  d->regs |= 1 << reg;
}

static void desa_op_xi(desa68_t * d, u16 w)
{
  int r = (w >> 12) & 7;                /* register base */
  if (w & 0x8000)
    desa_op_AN(d,r);
  else
    desa_op_DN(d,r);
  desa_ascii(d, ('.'<<8) | "WL"[ 1 & (w >> 11) ]);
}

static void desa_opsz(desa68_t *d, int size)
{
//  assert(size >= 0 && size < 3);
  desa_char(d,'.');
  desa_char(d,"BWL"[size]);
  d->opsz = size;
}

static void desa_label(desa68_t * d, unsigned int v)
{
  desa_uhexacat(d, v, 6, 'L');
}

static int desa_is_symbol(desa68_t * d, unsigned int v2, unsigned int bit)
{
  int r =
    ((d->flags & DESA68_SYMBOL_FLAG)
     &&
     ( (bit < 5u && (d->flags & (DESA68_FORCESYMB_FLAG<<bit)))
       || (v2>=d->immsym_min && v2<d->immsym_max))
      );
  return r;
}

static void desa_immL(desa68_t * d, int v, int pc)
{
  unsigned int v2 = v;
  const int bit = (pc == -1)
    ? 256
    : ( ((pc - d->_pc) & d->memmask) >> 1);

  desa_char(d,'#');
  if (desa_is_symbol(d, v, bit)) {
    desa_label(d,v2);
    /* $$$ TODO add potential reference address. */
  } else {
    if (d->ischar(d,(u8)v2) && d->ischar(d,(u8)(v2>>8)) &&
        d->ischar(d,(u8)(v2>>16)) && d->ischar(d,(u8)(v2>>24))) {
      desa_char (d,'\'');
      desa_ascii(d,(u32)v2);
      desa_char (d,'\'');
    } else {
      desa_signifiant(d,v);
    }
  }
}

static void desa_absL(desa68_t * d, int v, int pc)
{
  unsigned int v2 = v & 0xFFFFFF;
  const int bit = (pc == -1)
    ? 256
    : ( (pc - d->_pc) >> 1);

  if (desa_is_symbol(d, v, bit)) {
    desa_uhexacat(d, v2, 6, 'L');
  } else {
    desa_usignifiant(d, v);
  }
}

static void get_ea_2(desa68_t * d, u32 mode, u32 reg, u8 sz)
{
  int v;

  if (mode == MODE_ABSW)
    mode += reg;

  switch(mode ) {
  case MODE_DN:
    desa_op_DN(d, reg);
    break;
  case MODE_AN:
    desa_op_AN(d, reg);
    break;
  case MODE_pAN:
    desa_op_pAN(d,reg);
    break;
  case MODE_ANp:
    desa_op_ANp(d,reg);
    break;
  case MODE_dAN:
    read_pc(d);
    desa_signifiant(d,d->_w);
    /* continue on MODE_iAN */
  case MODE_iAN:
    desa_op_iAN(d,reg);
    break;
  case MODE_dANXI:
    v = read_pc(d);                       /* control word */
    desa_signifiant(d,(s8)(v>>8));
    desa_char(d,'(');
    desa_op_AN(d,reg);
    desa_comma(d);
    desa_op_xi(d,v);
    desa_char(d,')');
    break;
  case MODE_ABSW:
    set_ea(d, v = adrW(d));
    desa_absL(d, v, (d->pc-2) & 0xFFFFFF);
    desa_ascii(d,'.W');
    break;
  case MODE_ABSL:
    set_ea(d, v = adrL(d));
    desa_absL(d, v, (d->pc - 4) & 0xFFFFFF);
    if ( (s16)v == v )
      desa_ascii(d,'.L');
    break;
  case MODE_dPC:
    set_ea(d, v = relPC(d));
    if (d->flags & DESA68_SYMBOL_FLAG)
      desa_label(d, v);
    else
      desa_usignifiant(d, v);
    desa_char(d,'(');
    desa_op_anyreg(d, DESA68_REG_PC);
    desa_char(d,')');
    break;
  case MODE_dPCXI:
    set_ea(d, v = relPC(d));
    if (d->flags & DESA68_SYMBOL_FLAG)
      desa_label(d,v);
    else
      desa_usignifiant(d, v);
    desa_char(d,'(');
    desa_op_anyreg(d, DESA68_REG_PC);
    desa_comma(d);
    desa_op_xi(d, v);
    desa_char(d,')');
    break;
  case MODE_IMM:
    switch (sz) {
    case 1:
    case 'B':
      v = immB(d);
      desa_char(d,'#');
      desa_signifiant(d,v);
      break;
    case 2:
    case 'W':
      v = immW(d); /* should not be ok without cast */
      desa_char(d,'#');
      desa_signifiant(d,v);
      break;
    case 4:
    case 'L':
      v = (u32)immL(d);
      desa_immL(d, v, d->pc-4);
      break;
    default:
      desa_ascii(d,'#?');
      break;
    }
    break;

  default:
    desa_char(d,'?');
    desa_usignifiant(d, mode);
    desa_char(d,'?');
    break;
  }
}

/* Get move operand
 * bit : 0/6 -> src/dest
 * w   : current opcode
 */
static void get_ea_move(desa68_t * d, int bit, s32 w, u32 easz)
{
  int ea = (w>>bit) & 63;

  if (bit)
    get_ea_2(d, ea&7, ea>>3, easz);
  else if (bit==0)
    get_ea_2(d, ea>>3, ea&7, easz);
}

/* Used with ABCD, SBCD, ADDX, SUBX */
static void desa_ry_rx(desa68_t * d, int inst, int size)
{
  desa_ascii(d,inst);
  if (size != DESA68_SZ_IMPL)
    desa_opsz(d,size);                  /* ADDX */
  else
    d->opsz = DESA68_SZ_BYTE;           /* ABCD */
  desa_space(d);
  if (d->_mode3&1) {             /* -(Ay),-(Ax) */
    desa_op_pAN(d,d->_reg0);
    desa_comma(d);
    desa_op_pAN(d,d->_reg9);
  } else {                      /* Dy,Dx */
    desa_op_DN(d, d->_reg0);
    desa_comma(d);
    desa_op_DN(d, d->_reg9);
  }
  d->opsz = size;

}

static void desa_dn_ae(desa68_t * d, int name)
{
  desa_ascii(d,name);
  desa_opsz(d,d->_opsz);
  desa_space(d);
  /*  dn,<ae> */
  if (d->_w&0400) {
    desa_op_DN(d,d->_reg9);
    desa_comma(d);
    get_ea_2(d, d->_mode3, d->_reg0, d->_chsz);
    set_dst_ref(d);
  }

  /*  <ae>,dn */
  else {
    get_ea_2(d, d->_mode3, d->_reg0, d->_chsz);
    set_src_ref(d);
    desa_comma(d);
    desa_op_DN(d, d->_reg9);
  }
}


static void desa_dcw(desa68_t * d)
{
  desa_ascii(d,'DC.W');
  desa_space(d);
  desa_uhexacat(d,(u16)d->_w,4,'$');
  d->itype = DESA68_DCW;
  d->opsz  = DESA68_SZ_WORD;
}

/**************
 *
 *   LINE 0 :
 *   -Immediat operations
 *   -SR & CCR operations
 *   -Bit operations
 *   -Movep
 *
 ***************/

static int check_desa_bitop(desa68_t * d)
{
  static u32 fn[] = { 'BTST', 'BCHG', 'BCLR', 'BSET'};
  int modemsk = 00775;
  unsigned int inst;

  if (!(modemsk&(1<<d->_adrm0)))
    return 0;

  inst = fn[d->_opsz];
  if (!(d->_w&0400)) {
    /* B... #x,<AE>*/
    if ((d->_w&0xF00)!=0x800)
      return 0;
    desa_ascii(d,inst);
    desa_space(d);
    desa_char(d,'#');
    read_pc(d);
    desa_usignifiant(d, (u8)d->_w);
  } else {
    /* B... dn,<AE>*/
    desa_ascii(d,inst);
    desa_space(d);
    desa_op_DN(d, d->_reg9);
  }
  desa_comma(d);
  get_ea_2(d, d->_mode3, d->_reg0, 'B');
  d->opsz = d->_mode3 ? DESA68_SZ_BYTE : DESA68_SZ_LONG;
  return 1;
}

static int check_desa_IMM_SR(desa68_t * d)
{
  u32 modemsk = 1 + (1<<2) + (1<<10);
  int mode = (d->_w>>8)&15, inst='ORI';

  if ((d->_w&0677)!=0074 || !(modemsk&(1<<mode)))
    return 0;

  switch(mode) {
  case 0xA: /* EORI */
    inst = 'EORI';
    break;
#if 0
  case 0x0: /* ORI */
    inst = 'ORI';
    break;
#endif
  case 0x2: /* ANDI */
    inst = 'ANDI';
    break;
  }
  desa_ascii(d,inst);
  desa_space(d);
  get_ea_2(d, MODE_ABSW, MODE_IMM-MODE_ABSW, 'W');
  set_src_ref(d);
  desa_comma(d);
  desa_op_anyreg(d, (d->_mode6&1) ? DESA68_REG_SR : DESA68_REG_CCR);
  d->opsz = DESA68_SZ_WORD;
  return 1;
}

static int check_desa_movep(desa68_t * d)
{
  if ((d->_w & 0470) != 0410 )
    return 0;

  desa_str(d,"MOVEP");
  desa_opsz(d,1+(d->_opsz&1));
  desa_space(d);

  if (!(d->_w&(1<<7))) {
    /* MOVEP.? d(Ax),Dy */
    get_ea_2(d, MODE_dAN, d->_reg0, 0);
    desa_comma(d);
    desa_op_DN(d,d->_reg9);
  } else {
    /* MOVEP.? Dx,d(Ay) */
    desa_op_DN(d,d->_reg9);
    desa_comma(d);
    get_ea_2(d, MODE_dAN, d->_reg0, 0);
  }
  return 1;
}

static int check_desa_imm_op(desa68_t * d)
{
  static u32 fn[8] = {
    'ORI' , 'ANDI', 'SUBI', 'ADDI',
    '???I', 'EORI', 'CMPI', '???I'
  };
  const int modemsk = 0x6F;

  if ((d->_w&0x100) || !(modemsk&(1<<d->_reg9)) || d->_opsz==3)
    return 0;

  desa_ascii(d,fn[d->_reg9]);
  desa_opsz(d,d->_opsz);
  desa_space(d);
  get_ea_2(d, MODE_ABSW, MODE_IMM-MODE_ABSW, d->_chsz);
  set_src_ref(d);
  desa_comma(d);
  get_ea_2(d, d->_mode3, d->_reg0, d->_chsz);
  set_dst_ref(d);
  return 1;
}

static void desa_line0(desa68_t * d)
{
  if (!check_desa_movep(d)  &&
      !check_desa_bitop(d)  &&
      !check_desa_IMM_SR(d) &&
      !check_desa_imm_op(d) )
    desa_dcw(d);
}

/* General move(a) disassemble
 */
static void desa_move(desa68_t * d)
{
  static u8 mvsz[4] = {
    DESA68_SZ_UDEF, DESA68_SZ_BYTE, DESA68_SZ_LONG, DESA68_SZ_WORD
  };
  s32 w = d->_w;
  int movsz  = (u8)('WLB?' >> ((w&(3<<12))>>(12-3)));
  desa_ascii(d,'MOVE');
  if (d->_adrm6==MODE_AN)
    desa_char(d,'A');                   /* MOVEA */
  desa_opsz(d,mvsz[3 & (w>>12)]);
  desa_space(d);
  get_ea_move(d,0,w,movsz);
  set_src_ref(d);
  desa_comma(d);
  get_ea_move(d,6,w,movsz);
  set_dst_ref(d);
}

/* Check and disassemble a valid move
 * return TRUE if valid move
 */
static int check_desa_move(desa68_t * d)
{
  u32 srcmsk = 0xFFF, dstmsk = 0x1FF;
  /* Remove An source & destination addressing mode for byte access */
  if (d->_line==0x1) {
    srcmsk &= ~(1<<MODE_AN);
    dstmsk &= ~(1<<MODE_AN);
  }
  if ((srcmsk&(1<<d->_adrm0)) && (dstmsk&(1<<d->_adrm6)) ) {
    desa_move(d);
    return 1;
  }
  return 0;
}

/**************
 *
 *   LINE 1 :
 *   -MOVE.B
 *   LINE 2 :
 *   -MOVE.L
 *   LINE 3 :
 *   -MOVE.W
 *
 ***************/

static void desa_li123(desa68_t * d)
{
  if (!check_desa_move(d))
    desa_dcw(d);
}

/**************
 *
 *   LINE 4 :
 *   -Other instructions
 *
 ***************/

static void get_movemsub(desa68_t * d, u8 i, u8 j)
{
  assert(j <= DESA68_REG_SP);
  assert(i <= j);
  desa_op_anyreg(d, i);
  if ( i != j ) {
    desa_char(d,'-');
    desa_op_anyreg(d, j);
  }
}

static void get_movemreg(desa68_t * d, unsigned int v, unsigned int rev)
{
  int i,j,p=0;

  assert(v < 0x10000);
  assert(rev == 0 || rev == 15);

  for (i=0 ; i<16; i++) {
    for (; i<16 && (v&(1<<(i^rev)))==0; i++);
    if (i==16) break;
    j = i;
    for (; i<16 && (v&(1<<(i^rev))); i++)
      d->regs |= 1<<(DESA68_REG_DN+i);
    if (p) desa_char(d,'/');
    get_movemsub(d,j,(i-1));
    p = 1;
  }
}

static int desa_check_imp(desa68_t * d,
                          unsigned int name, int mskmode)
{
  if ((d->_w&0400) || !(mskmode&(1<<d->_adrm0)))
    return 0;
  desa_ascii(d,name);
  desa_opsz(d,d->_opsz);
  desa_space(d);
  get_ea_2(d, d->_mode3, d->_reg0, d->_chsz);
  set_both_ref(d);
  return 1;
}

static int check_desa_lea(desa68_t * d)
{
  int modemsk = 03744;
  if ((d->_w&0700) != 0700 || !(modemsk&(1<<d->_adrm0)))
    return 0;
  desa_ascii(d,'LEA');
  desa_space(d);
  get_ea_2(d, d->_mode3, d->_reg0, 0);
  set_src_ref(d);
  desa_comma(d);
  desa_op_AN(d,d->_reg9);
  d->opsz = DESA68_SZ_LONG;
  return 1;
}

static int check_desa_chk(desa68_t * d)
{
  int modemsk = 07775;
  if ((d->_w&0700) != 0600 || !(modemsk&(1<<d->_adrm0)))
    return 0;
  desa_ascii(d,'CHK ');
  get_ea_2(d, d->_mode3, d->_reg0, 0);
  set_src_ref(d);
  desa_comma(d);
  desa_op_DN(d, d->_reg9);
  set_interrupt(d, 0x18);
  desa_opsz(d,DESA68_SZ_IMPL);
  return 1;
}

static int check_desa_ext(desa68_t * d)
{
  if ((d->_w&07670)!=04200)
    return 0;
  desa_ascii(d,'EXT');
  desa_opsz(d,1+(1&(d->_w>>6)));
  desa_space(d);
  desa_op_DN(d,d->_reg0);
  return 1;
}

static int check_desa_movem(desa68_t * d)
{
  int modemsk, regmsk;
  int mode3, reg0, w;
  if ((d->_w&05600)!=04200)
    return 0;
  modemsk = !(d->_w&02000) ? 00764 : 03754;
  if (!(modemsk&(1<<d->_adrm0)))
    return 0;

  desa_str(d,"MOVEM");
  desa_opsz(d,1 + (1 & (d->_w>>6)));
  desa_space(d);

  w = d->_w;
  mode3 = d->_mode3;
  reg0 = d->_reg0;
  regmsk = (u16) immW(d);

  if (w&02000) {
    /*  -> reg */
    get_ea_2(d, mode3, reg0, 0);
    set_src_ref(d);
    desa_comma(d);
    get_movemreg(d,regmsk,0);
  } else {
    /* -> mem */
    get_movemreg(d, regmsk, (mode3==MODE_pAN) ? 15 : 0);
    desa_comma(d);
    get_ea_2(d, mode3, reg0, 0);
    set_dst_ref(d);
  }
  return 1;
}

static int check_desa_jmp(desa68_t * d)
{
  int modemsk = 03744;
  if ((d->_w&07600) != 07200) {
    return 0;
  }
  /* JMP / JSR */
  if (modemsk & (1<<d->_adrm0)) {
    int type;
    desa_char(d,'J');
    if (d->_opsz==2) {
      type = DESA68_BSR;
      desa_ascii(d,'SR');
    } else {
      type = DESA68_BRA;
      desa_ascii(d,'MP');
    }
    d->opsz = DESA68_SZ_IMPL;
    desa_space(d);
    get_ea_2(d, d->_mode3, d->_reg0, d->_chsz);
    set_branch(d, type);
    return 1;
  }

  /* invalid JSR / JMP address mode */
  /* $$$ Not sure for a DC.W here */
  desa_dcw(d);
  return 1;
  /*return 0;*/
}

static int check_desa_line4_mode3(desa68_t * d)
{
  if (d->_mode6 != 3) {
    return 0;
  }

  switch(d->_reg9) {

  case 0:                               /* MOVE FROM SR */
    if (!(00775 & (1<<d->_adrm0)))
      break;
    desa_ascii(d,'MOVE');
    desa_space(d);
    desa_op_anyreg(d, DESA68_REG_SR);
    desa_comma(d);
    get_ea_2(d, d->_mode3, d->_reg0, 'W');
    set_dst_ref(d);
    d->opsz = DESA68_SZ_WORD;
    return 1;

  case 2:                               /* MOVE TO CCR */
    if (!(07775 & (1<<d->_adrm0)))
      break;
    desa_ascii(d,'MOVE');
    desa_space(d);
    get_ea_2(d, d->_mode3, d->_reg0, 'B');
    set_src_ref(d);
    desa_comma(d);
    desa_op_anyreg(d,DESA68_REG_CCR);
    d->opsz = DESA68_SZ_WORD;           /* Word indeed */
    return 1;

  case 3:                               /* MOVE TO SR */
    if (!(07775 & (1<<d->_adrm0)))
      break;
    desa_ascii(d,'MOVE');
    desa_space(d);
    get_ea_2(d, d->_mode3, d->_reg0, 'W');
    set_src_ref(d);
    desa_comma(d);
    desa_op_anyreg(d,DESA68_REG_SR);
    d->opsz = DESA68_SZ_WORD;
    return 1;

  case 5:                               /* TAS */
    d->_chsz = 0;
    if (desa_check_imp(d, 'TAS', 00775)) {
      d->opsz = DESA68_SZ_BYTE;
      return 1;
    } else if (d->_w == 0x4AFC) {
      desa_str(d,"ILLEGAL");
      set_interrupt(d, 0x10);
      d->opsz = DESA68_SZ_IMPL;
      return 1;
    }
  }
  return 0;
}


static void desa_line4(desa68_t * d)
{
  switch (d->_mode6) {

  case 7:
    if (check_desa_lea(d)) return;
    break;
  case 5:
    if (check_desa_chk(d)) return;
    break;
  case 2:
  case 3:
    if (check_desa_ext(d)) return;
    else if (check_desa_movem(d)) return;
    else if (check_desa_jmp(d)) return;
    if (check_desa_line4_mode3(d)) return;

  default:

    switch(d->_reg9) {
    case 0:                             /* NEGX */
      if (desa_check_imp(d, 'NEGX', 00775))
        return;
      break;

    case 1:                             /* CLR */
      if (desa_check_imp(d, 'CLR', 00775))
        return;
      break;

    case 2:                             /* NEG */
      if (desa_check_imp(d, 'NEG', 00775))
        return;
      break;

    case 3:                             /* NOT */
      if (desa_check_imp(d, 'NOT', 00775))
        return;
      break;

    case 4:
      if (d->_mode6==0) {
        /* NBCD */
        d->_chsz = 0;
        if (desa_check_imp(d, 'NBCD', 00775))
          return;
      } else if (d->_mode6==1) {
        if (d->_mode3 == MODE_DN) {
          /* SWAP */
          desa_ascii(d,'SWAP');
          desa_space(d);
          desa_op_DN(d, d->_reg0);
          d->opsz = DESA68_SZ_LONG;
          return;
        } else {
          /* PEA */
          d->_chsz = 0;
          if (desa_check_imp(d, 'PEA', 0x7E4))
            return;
        }
      }
      break;

    case 5:                             /* TST */
      if (desa_check_imp(d, 'TST', 00775))
        return;
      break;

    case 6:
      break;

    case 7:
      if (d->_mode6 == 1) {
        /* FUNKY LINE 4 */
        switch(d->_mode3) {
        case 0:
        case 1: {
          const int num = d->_w & 0xF;
          desa_ascii(d,'TRAP');
          desa_space(d);
          desa_ascii(d,'#$');
          desa_char(d, Thex[num]);
          set_interrupt(d,0x80 + (num<<2));
          d->opsz = DESA68_SZ_IMPL;
        } return;
        case 2:
          desa_ascii(d,'LINK');
          desa_space(d);
          desa_op_AN(d,d->_reg0);
          desa_comma(d);
          get_ea_2(d, MODE_ABSW, MODE_IMM-MODE_ABSW, 'W');
          d->regs |= 1 << DESA68_REG_SP;
          d->opsz = DESA68_SZ_IMPL;
          return;
        case 3:
          desa_ascii(d,'UNLK');
          desa_space(d);
          desa_op_AN(d,d->_reg0);
          d->regs |= 1 << DESA68_REG_SP;
          d->opsz = DESA68_SZ_IMPL;
          return;
        case 4:                         /* MOVE An,USP */
          desa_ascii(d,'MOVE');
          desa_space(d);
          desa_op_AN(d,d->_reg0);
          desa_comma(d);
          desa_op_anyreg(d,DESA68_REG_USP);
          d->opsz = DESA68_SZ_LONG;
          return;

        case 5:                         /* MOVE USP,Sn */
          desa_ascii(d,'MOVE');
          desa_space(d);
          desa_op_anyreg(d,DESA68_REG_USP);
          desa_comma(d);
          desa_op_AN(d,d->_reg0);
          d->opsz = DESA68_SZ_LONG;
          return;

        case 6: {
          /* FUNKY LINE 4 MODE 6 (4E */
          static char *str[8] = { /* $4E70 - $4E77 */
            /* 0      1      2      3     4    5      6      7 */
            "RESET","NOP","STOP ","RTE", "?","RTS","TRAPV","RTR"
          };
          if (d->_reg0 == 4)
            break;

          if ((d->_reg0 & 1))
            d->itype = (d->_reg0 == 1) ? DESA68_NOP : DESA68_RTS;

          desa_str(d, str[d->_reg0]);
          if (d->_reg0 == 2)
            get_ea_2(d,MODE_IMM,0,'W');
          d->opsz = DESA68_SZ_IMPL;
        } return;
        }
      }
    }
  }
  desa_dcw(d);
}


/**************
 *
 *   LINE 5 :
 *   -ADDQ
 *   -SUBQ
 *   -Scc
 *   -Dcc
 *
 ***************/

static void desa_addq_subq(desa68_t * d)
{
  int v;

  v = d->_reg9;
  v += (!v) << 3;
  desa_ascii(d,(d->_w&0400) ? 'SUBQ':'ADDQ');
  desa_opsz(d,d->_opsz);
  desa_space(d);
  desa_ascii(d,'#0' + v);
  desa_comma(d);
  get_ea_2(d, d->_mode3, d->_reg0, d->_chsz);
  set_dst_ref(d);
}

static void desa_Dcc(desa68_t * d)
{
  int v;

  desa_ascii(d,('DB'<<16) + dbcc_ascii[(d->_w>>8)&15]);
  desa_space(d);
  desa_op_DN(d,d->_reg0);
  desa_comma(d);
  set_ea(d, v = relPC(d));
  if (d->flags & DESA68_SYMBOL_FLAG)
    desa_label(d, v);
  else
    desa_usignifiant(d, v);
  set_branch(d,DESA68_BSR);
  d->opsz = DESA68_SZ_IMPL;
}

static void desa_Scc(desa68_t * d)
{
  desa_char(d,'S');
  desa_ascii(d,scc_ascii[(d->_w>>8)&15]);
  desa_space(d);
  get_ea_2(d, d->_mode3, d->_reg0, d->_chsz);
  d->opsz = DESA68_SZ_BYTE;
  set_both_ref(d);
}

static void desa_line5(desa68_t * d)
{
  if ( (d->_w&0370) == 0310 ) {
    desa_Dcc(d);
  } else if (d->_opsz == 3) {
    desa_Scc(d);
  } else {
    desa_addq_subq(d);
  }
}

/**************
 *
 *   LINE 6 :
 *   -Bcc
 *
 * Format 0110 COND OFFSET08 [OFFSET16 if OFFSET08==0]
 * !!! COND=0001(false) => BSR
 ***************/

static void desa_line6(desa68_t * d)
{
  s32 a;

  int cond = (d->_w>>8) & 0xF;

  desa_ascii(d,('B'<<16) + bcc_ascii[cond]);

  /* Bcc.S */
  if (d->_w & 255) {
    desa_ascii(d,'.S');
    a = (s32)(s8)d->_w;
    a += (s32)d->pc;
    d->opsz = DESA68_SZ_BYTE;
  }
  /* Bcc.W */
  else {
    desa_ascii(d,'.W');
    a = relPC(d);
    d->opsz = DESA68_SZ_WORD;

  }
  desa_space(d);
  desa_absL(d, a, -1);
  set_ea(d,a);
  set_branch(d, !cond ? DESA68_BRA : DESA68_BSR);

}

/**************
 *
 *   LINE 7 :
 *   -MOVEQ
 *
 * Format : 01111 REG 0 XXXXXXXX
 *
 ***************/
static void desa_line7(desa68_t * d)
{
  if (d->_w & 0400) {
    desa_dcw(d);
  } else {
    int v = (int)(s8)d->_w;
    desa_str(d,"MOVEQ");
    desa_space(d);
    desa_char(d,'#');
    desa_signifiant(d,v);
    desa_comma(d);
    desa_op_DN(d,d->_reg9);
    d->opsz = DESA68_SZ_LONG; /* DESA68_SZ_IMPL; */
  }
}


/**************
 *
 *   LINE B :
 *   -CMP
 *   -CMPM
 *   -EOR
 *
 ***************/

static int check_desa_cmpa(desa68_t * d)
{
  int modemsk = 07777;

  if ( d->_opsz != 3 || !(modemsk & (1<<d->_adrm0)) )
    return 0;
  desa_ascii(d,'CMPA');
  desa_opsz(d,1+(1 & (d->_w >> 8)));
  desa_space(d);
  get_ea_2(d,d->_mode3, d->_reg0, d->_chsz);
  set_src_ref(d);
  desa_comma(d);
  desa_op_AN(d,d->_reg9);
  return 1;
}

static int check_desa_eor_cmp(desa68_t * d)
{
  int modemsk, inst;

  if (d->_opsz == 3)
    return 0;

  /* EOR */
  else if (d->_w & 0400) {
    modemsk = 00775;
    inst = 'EOR';
  }

  /* CMP */
  else {
    modemsk = (!d->_opsz) ? 07775 : 07777;
    inst = 'CMP';
  }

  if ( ! (modemsk & (1<<d->_adrm0) ) )
    return 0;

  desa_dn_ae(d,inst);
  return 1;
}

static int check_desa_cmpm(desa68_t * d)
{
  if ((d->_w&0470) != 0410)
    return 0;
  desa_ascii(d,'CMPM');
  desa_space(d);
  desa_op_ANp(d, d->_reg0);
  desa_comma(d);
  desa_op_ANp(d, d->_reg9);
  return 1;
}

static void desa_lineB(desa68_t * d)
{
  if (!check_desa_cmpa(d) &&
      !check_desa_eor_cmp(d) &&
      !check_desa_cmpm(d))
    desa_dcw(d);
}

/**************
 *
 *   LINE 8 :
 *   -OR
 *   -SBCD
 *   -DIVU
 *
 *
 *   LINE C :
 *   -EXG
 *   -MULS,MULU
 *   -ABCD
 *   -AND
 *
 ***************/

static int check_desa_exg(desa68_t * d)
{
  unsigned int reg;
  switch(d->_w&0770) {
  case 0500:
    reg = 0x0000;
    break;
  case 0510:
    reg = 0x0808;
    break;
  case 0610:
    reg = 0x0008;
    break;
  default:
    return 0;
  }
  desa_ascii(d,'EXG');
  desa_space(d);
  desa_op_RN(d, d->_reg9 + (reg>>8));
  desa_comma(d);
  desa_op_RN(d, d->_reg0 + (reg&8));
  return 1;
}

static int check_desa_mul_div(desa68_t * d)
{
  int modemsk = 0xFFD;
  if ( (d->_w&0300) != 0300 || !( modemsk & (1<<d->_adrm0) ) )
    return 0;
  desa_ascii(d,(d->_line==0xC) ? 'MUL' : 'DIV');
  desa_char(d,(d->_w&0x100) ? 'S' : 'U');
  desa_space(d);
  get_ea_2(d, d->_mode3, d->_reg0, 'W');
  set_src_ref(d);
  desa_comma(d);
  desa_op_DN(d,d->_reg9);
  d->opsz = DESA68_SZ_LONG;
  return 1;
}

static int check_desa_abcd_sbcd(desa68_t * d)
{
  if ( (d->_w&0x1f0) != 0x100 )
    return 0;
  desa_ry_rx(d,(d->_line==0xC) ? 'ABCD' : 'SBCD', DESA68_SZ_IMPL);
  return 1;
}

static int check_desa_and_or(desa68_t * d)
{
  int modemsk = !(d->_w&0400)? 0xFFD : 0x1FC;
  if ( ! (modemsk & (1<<d->_adrm0) ) )
    return 0;
  desa_dn_ae(d, (d->_line==0xC) ? 'AND' : 'OR');
  return 1;
}

static void desa_lin8C(desa68_t * d)
{
  if (!check_desa_abcd_sbcd(d) &&
      !check_desa_mul_div(d) &&
      !check_desa_exg(d) &&
      !check_desa_and_or(d))
    desa_dcw(d);
}

/**************
 *
 *   LINE 9 :
 *   -SUB, SUBX, SUBA
 *
 *   LINE D :
 *   -ADD, ADDX, ADDA
 *
 **************/

static int check_desa_addx_subx(desa68_t * d)
{
  if ( (d->_w&0460) != 0400 )
    return 0;
  desa_ry_rx(d,(d->_line==0xD) ? 'ADDX' : 'SUBX', d->_opsz);
  return 1;
}

static int check_desa_adda_suba(desa68_t * d)
{
  if (d->_opsz != 3)
    return 0;

  desa_ascii(d,(d->_line==0xD) ? 'ADDA' : 'SUBA');
  desa_opsz(d,DESA68_SZ_WORD+BIT8(d->_w));
  desa_space(d);
  get_ea_2(d, d->_mode3, d->_reg0, d->_chsz);
  set_src_ref(d);
  desa_comma(d);
  desa_op_AN(d, d->_reg9);
  return 1;
}

static int check_desa_add_sub(desa68_t * d)
{
  int modemsk = !(d->_w&0400) ? 07777 : 00774;

  if (d->_opsz == 0) /* Fordib An for byte access size */
    modemsk &= ~(1<<MODE_AN);

  if ( ! (modemsk & (1<<d->_adrm0) ) )
    return 0;
  desa_dn_ae (d,d->_line==0xD ? 'ADD' : 'SUB');
  return 1;
}

static void desa_lin9D(desa68_t * d)
{
  if (!check_desa_adda_suba(d) &&
      !check_desa_addx_subx(d) &&
      !check_desa_add_sub(d))
    desa_dcw(d);
}

/**************
 *
 *   LINE E :
 *   -Shifting
 *
 * Format Reg: 1110 VAL D SZ I TY RG0
 * Format Mem: 1110 0TY D 11 MODRG0
 ***************/

static void desa_lineE(desa68_t * d)
{
  static u16 shf_ascii[4] = { 'AS', 'LS', 'RO', 'RO' };

  /* Memory */
  if (d->_opsz == 3) {
    const int modemsk = 00774;
    int type = d->_reg9;
    if (!(modemsk & (1<<d->_adrm0)) || (type&4)) {
      desa_dcw(d);
      return;
    }
    desa_ascii(d,shf_ascii[type]);
    if (type==2) desa_char(d,'X');
    desa_char (d, (d->_w&0400) ? 'L' :'R');
    desa_space(d);
    get_ea_2(d, d->_mode3, d->_reg0, 0);
    set_both_ref(d);
  }

  /* Register */
  else {
    int type = (d->_w>>3)&3;
    desa_ascii (d,shf_ascii[type]);
    if (type==2) desa_char(d,'X');
    desa_char (d, (d->_w&0400) ? 'L' :'R');
    desa_opsz(d,d->_opsz);
    desa_space(d);
    /* dn,dn */
    if (d->_w&(1<<5))
      desa_op_DN(d,d->_reg9);
    /* #x,dn */
    else {
      int v = 1 + ((d->_reg9-1)&7);
      desa_ascii(d,'#0'+v);
    }
    desa_comma(d);
    desa_op_DN(d,d->_reg0);
  }
}

/**************
 *
 *   LINE A : dc.w $Axxx
 *   - lineA instruction emulator
 *   LINE F : dc.w $Fxxx
 *   - lineF instruction emulator
 *
 * Format : LINE XXXX XXXX XXXX
 *
 ***************/

static void desa_linAF(desa68_t * d)
{
  int vector = d->_line - 0xA;
  desa_dcw(d);                          /* set itype */
  set_ea(d, 0x40 + ( (!!vector) <<2 ));
  set_branch(d, DESA68_INT);            /* restore valid itype */
  d->opsz = DESA68_SZ_IMPL;
}

static void (*desa_line[16])(desa68_t *) =
{
  desa_line0, desa_li123, desa_li123, desa_li123,
  desa_line4, desa_line5, desa_line6, desa_line7,
  desa_lin8C, desa_lin9D, desa_linAF, desa_lineB,
  desa_lin8C, desa_lin9D, desa_lineE, desa_linAF,
};

void desa68(desa68_t * d)
{
  /* Clean slate. */
  d->_ea   = DESA68_INVALID_ADDR;
  /* d->branch = DESA68_INVALID_ADDR; */
  d->sref  = DESA68_INVALID_ADDR;
  d->dref  = DESA68_INVALID_ADDR;
  d->regs  = 0;                /* clear used registers. */
  d->opsz  = DESA68_SZ_UDEF;   /* operand size */
  d->itype = DESA68_INS;       /* assume valid generic instruction */
  d->error = 0;                /* clear error flags */

  /* Setup 68k memory access. */
  if (!d->memget)
    d->memget = def_memget;
  if (!d->memmask)
    d->memmask = (1<<24)-1;

  /* Setup output string writer. */
  if (!d->strput)
    d->strput = def_strput;
  if (!d->str)
    d->strmax = 0;
  d->out = 0;

  /* Setup the ASCII disassembly */
  switch(d->flags & DESA68_GRAPH_FLAG) {
  case 0:
    d->ischar = my_isfalse; break;
  case DESA68_ASCII_FLAG:
    d->ischar = my_isascii; break;
  case DESA68_ALNUM_FLAG:
    d->ischar = my_isalnum; break;
  case DESA68_GRAPH_FLAG:
    d->ischar = my_isgraph; break;
  }

  /* Setup instruction decoder */
  d->_pc = d->pc &= d->memmask;
  read_pc(d);

  d->_reg0  = REG0(d->_w);
  d->_reg9  = REG9(d->_w);
  d->_mode3 = MODE3(d->_w);
  d->_mode6 = MODE6(d->_w);
  d->_line  = LINE(d->_w);
  d->_opsz  = OPSZ(d->_w);
  d->_adrm0 = d->_mode3 + ((d->_mode3==MODE_ABSW)? d->_reg0 : 0);
  d->_adrm6 = d->_mode6 + ((d->_mode6==MODE_ABSW)? d->_reg9 : 0);
  d->_chsz  = ('.'<<8) | (u8)('?LWB'>>(d->_opsz*8));

  (desa_line[d->_line])(d);
  desa_char(d,0);

  /* Restore current status to caller struct */
  d->pc &= d->memmask;

  /* if not true we forgot to commit _ea at some point. */
  assert(d->_ea == DESA68_INVALID_ADDR);

  /* if not true we forgot to set opsz or set it wrong. */
  /* assert(d->opsz != DESA68_SZ_UDEF); */
}

int desa68_version(void)
{
#ifndef PACKAGE_VERNUM
# define PACKAGE_VERNUM 0
#endif
  return PACKAGE_VERNUM;
}

const char * desa68_versionstr(void)
{
#ifndef PACKAGE_STRING
# define PACKAGE_STRING "desa68 n/a"
#endif
  return PACKAGE_STRING;
}
