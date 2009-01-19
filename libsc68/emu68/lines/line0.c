/* line0.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

static void BTST_reg(emu68_t * const emu68, int b, int reg0)
{
  int68_t a = REG68.d[reg0];
  b &= 31;
  BTST(a,b);
  ADDCYCLE(2);
}

static void BTST_mem(emu68_t * const emu68, int b, int mode, int reg0)
{
  addr68_t addr = get_eab68[mode](emu68,reg0);
  int68_t a = read_B(addr);
  b &= 7;
  BTST(a,b);
  ADDCYCLE(0);
}

static void BCHG_reg(emu68_t * const emu68, int b, int reg0)
{
  int68_t a = REG68.d[reg0];
  b &= 31;
  BCHG(a,b);
  REG68.d[reg0] = a;
  ADDCYCLE(4);
}

static void BCHG_mem(emu68_t * const emu68, int b, int mode, int reg0)
{
  addr68_t addr = get_eab68[mode](emu68,reg0);
  int68_t a = read_B(addr);
  b &= 7;
  BCHG(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

static void BCLR_reg(emu68_t * const emu68, int b, int reg0)
{
  int68_t a = REG68.d[reg0];
  b &= 31;
  BCLR(a,b);
  REG68.d[reg0] = a;
  ADDCYCLE(6);
}

static void BCLR_mem(emu68_t * const emu68, int b, int mode, int reg0)
{
  addr68_t addr = get_eab68[mode](emu68,reg0);
  int68_t a = read_B(addr);
  b &= 7;
  BCLR(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

static void BSET_reg(emu68_t * const emu68, int b, int reg0)
{
  int68_t a = REG68.d[reg0];
  b &= 31;
  BSET(a,b);
  REG68.d[reg0] = a;
  ADDCYCLE(4);
}

static void BSET_mem(emu68_t * const emu68, int b, int mode, int reg0)
{
  addr68_t addr = get_eab68[mode](emu68,reg0);
  int68_t a = read_B(addr);
  b &= 7;
  BSET(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

static void l0_ill(emu68_t * const emu68, int reg0)
{
  reg0=reg0; ILLEGAL;
}

static void l0_ORb0(emu68_t * const emu68, int reg0)
{
  /* OR.B #imm,Dy */
  int68_t a,b,s;
  a = get_nextw()<<BYTE_SHIFT;
  b = REG68.d[reg0]<<BYTE_SHIFT;
  ORB(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

static void l0_ANDb0(emu68_t * const emu68, int reg0)
{
  /* AND.B #imm,Dy */
  int68_t a,b,s;
  a = get_nextw()<<BYTE_SHIFT;
  b = REG68.d[reg0]<<BYTE_SHIFT;
  ANDB(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

static void l0_EORb0(emu68_t * const emu68, int reg0)
{
  /* EOR.B #imm,Dy */
  int68_t a,b,s;
  a = get_nextw()<<BYTE_SHIFT;
  b = REG68.d[reg0]<<BYTE_SHIFT;
  EORB(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

static void l0_ADDb0(emu68_t * const emu68, int reg0)
{
  /* ADD.B #imm,Dy */
  int68_t a,b,s;
  a = get_nextw()<<BYTE_SHIFT;
  b = REG68.d[reg0]<<BYTE_SHIFT;
  ADDB(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

static void l0_SUBb0(emu68_t * const emu68, int reg0)
{
  /* SUB.B #imm,Dy */
  int68_t a,b,s;
  a = get_nextw()<<BYTE_SHIFT;
  b = REG68.d[reg0]<<BYTE_SHIFT;
  SUBB(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

static void l0_CMPb0(emu68_t * const emu68, int reg0)
{
  /* CMP.B #imm,Dy */
  int68_t a,b;
  a = get_nextw()<<BYTE_SHIFT;
  b = REG68.d[reg0]<<BYTE_SHIFT;
  CMPB(a,b);
}

static void l0_ORw0(emu68_t * const emu68, int reg0)
{
  /* OR.W #imm,Dy */
  int68_t a,b,s;
  a = get_nextw()<<WORD_SHIFT;
  b = REG68.d[reg0]<<WORD_SHIFT;
  ORW(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

static void l0_ANDw0(emu68_t * const emu68, int reg0)
{
  /* AND.W #imm,Dy */
  int68_t a,b,s;
  a = get_nextw()<<WORD_SHIFT;
  b = REG68.d[reg0]<<WORD_SHIFT;
  ANDW(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

static void l0_EORw0(emu68_t * const emu68, int reg0)
{
  /* EOR.W #imm,Dy */
  int68_t a,b,s;
  a = get_nextw()<<WORD_SHIFT;
  b = REG68.d[reg0]<<WORD_SHIFT;
  EORW(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

static void l0_ADDw0(emu68_t * const emu68, int reg0)
{
  /* ADD.W #imm,Dy */
  int68_t a,b,s;
  a = get_nextw()<<WORD_SHIFT;
  b = REG68.d[reg0]<<WORD_SHIFT;
  ADDW(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

static void l0_SUBw0(emu68_t * const emu68, int reg0)
{
  /* SUB.W #imm,Dy */
  int68_t a,b,s;
  a = get_nextw()<<WORD_SHIFT;
  b = REG68.d[reg0]<<WORD_SHIFT;
  SUBW(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

static void l0_CMPw0(emu68_t * const emu68, int reg0)
{
  /* CMP.W #imm,Dy */
  int68_t a,b;
  a = get_nextw()<<WORD_SHIFT;
  b = REG68.d[reg0]<<WORD_SHIFT;
  CMPW(a,b);
}

static void l0_ORl0(emu68_t * const emu68, int reg0)
{
  /* OR.L #imm,Dy */
  int68_t a,b,s;
  a = get_nextl()<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  ORL(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

static void l0_ANDl0(emu68_t * const emu68, int reg0)
{
  /* AND.L #imm,Dy */
  int68_t a,b,s;
  a = get_nextl()<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  ANDL(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

static void l0_EORl0(emu68_t * const emu68, int reg0)
{
  /* EOR.L #imm,Dy */
  int68_t a,b,s;
  a = get_nextl()<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  EORL(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

static void l0_ADDl0(emu68_t * const emu68, int reg0)
{
  /* ADD.L #imm,Dy */
  int68_t a,b,s;
  a = get_nextl()<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  ADDL(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

static void l0_SUBl0(emu68_t * const emu68, int reg0)
{
  /* SUB.L #imm,Dy */
  int68_t a,b,s;
  a = get_nextl()<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  SUBL(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

static void l0_CMPl0(emu68_t * const emu68, int reg0)
{
  /* CMP.L #imm,Dy */
  int68_t a,b;
  a = get_nextl()<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  CMPL(a,b);
}

static void l0_ORb2(emu68_t * const emu68, int reg0)
{
  /* OR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_ANDb2(emu68_t * const emu68, int reg0)
{
  /* AND.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_EORb2(emu68_t * const emu68, int reg0)
{
  /* EOR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_ADDb2(emu68_t * const emu68, int reg0)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_SUBb2(emu68_t * const emu68, int reg0)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_CMPb2(emu68_t * const emu68, int reg0)
{
  /* CMP.B #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  CMPB(a,b);
}

static void l0_ORw2(emu68_t * const emu68, int reg0)
{
  /* OR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_ANDw2(emu68_t * const emu68, int reg0)
{
  /* AND.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_EORw2(emu68_t * const emu68, int reg0)
{
  /* EOR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_ADDw2(emu68_t * const emu68, int reg0)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_SUBw2(emu68_t * const emu68, int reg0)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_CMPw2(emu68_t * const emu68, int reg0)
{
  /* CMP.W #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  CMPW(a,b);
}

static void l0_ORl2(emu68_t * const emu68, int reg0)
{
  /* OR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ANDl2(emu68_t * const emu68, int reg0)
{
  /* AND.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_EORl2(emu68_t * const emu68, int reg0)
{
  /* EOR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ADDl2(emu68_t * const emu68, int reg0)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_SUBl2(emu68_t * const emu68, int reg0)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_CMPl2(emu68_t * const emu68, int reg0)
{
  /* CMP.L #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  CMPL(a,b);
}

static void l0_ORb3(emu68_t * const emu68, int reg0)
{
  /* OR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_ANDb3(emu68_t * const emu68, int reg0)
{
  /* AND.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_EORb3(emu68_t * const emu68, int reg0)
{
  /* EOR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_ADDb3(emu68_t * const emu68, int reg0)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_SUBb3(emu68_t * const emu68, int reg0)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_CMPb3(emu68_t * const emu68, int reg0)
{
  /* CMP.B #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  CMPB(a,b);
}

static void l0_ORw3(emu68_t * const emu68, int reg0)
{
  /* OR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_ANDw3(emu68_t * const emu68, int reg0)
{
  /* AND.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_EORw3(emu68_t * const emu68, int reg0)
{
  /* EOR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_ADDw3(emu68_t * const emu68, int reg0)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_SUBw3(emu68_t * const emu68, int reg0)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_CMPw3(emu68_t * const emu68, int reg0)
{
  /* CMP.W #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  CMPW(a,b);
}

static void l0_ORl3(emu68_t * const emu68, int reg0)
{
  /* OR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ANDl3(emu68_t * const emu68, int reg0)
{
  /* AND.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_EORl3(emu68_t * const emu68, int reg0)
{
  /* EOR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ADDl3(emu68_t * const emu68, int reg0)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_SUBl3(emu68_t * const emu68, int reg0)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_CMPl3(emu68_t * const emu68, int reg0)
{
  /* CMP.L #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  CMPL(a,b);
}

static void l0_ORb4(emu68_t * const emu68, int reg0)
{
  /* OR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_ANDb4(emu68_t * const emu68, int reg0)
{
  /* AND.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_EORb4(emu68_t * const emu68, int reg0)
{
  /* EOR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_ADDb4(emu68_t * const emu68, int reg0)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_SUBb4(emu68_t * const emu68, int reg0)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_CMPb4(emu68_t * const emu68, int reg0)
{
  /* CMP.B #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  CMPB(a,b);
}

static void l0_ORw4(emu68_t * const emu68, int reg0)
{
  /* OR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_ANDw4(emu68_t * const emu68, int reg0)
{
  /* AND.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_EORw4(emu68_t * const emu68, int reg0)
{
  /* EOR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_ADDw4(emu68_t * const emu68, int reg0)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_SUBw4(emu68_t * const emu68, int reg0)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_CMPw4(emu68_t * const emu68, int reg0)
{
  /* CMP.W #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  CMPW(a,b);
}

static void l0_ORl4(emu68_t * const emu68, int reg0)
{
  /* OR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ANDl4(emu68_t * const emu68, int reg0)
{
  /* AND.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_EORl4(emu68_t * const emu68, int reg0)
{
  /* EOR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ADDl4(emu68_t * const emu68, int reg0)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_SUBl4(emu68_t * const emu68, int reg0)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_CMPl4(emu68_t * const emu68, int reg0)
{
  /* CMP.L #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  CMPL(a,b);
}

static void l0_ORb5(emu68_t * const emu68, int reg0)
{
  /* OR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_ANDb5(emu68_t * const emu68, int reg0)
{
  /* AND.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_EORb5(emu68_t * const emu68, int reg0)
{
  /* EOR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_ADDb5(emu68_t * const emu68, int reg0)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_SUBb5(emu68_t * const emu68, int reg0)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_CMPb5(emu68_t * const emu68, int reg0)
{
  /* CMP.B #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  CMPB(a,b);
}

static void l0_ORw5(emu68_t * const emu68, int reg0)
{
  /* OR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_ANDw5(emu68_t * const emu68, int reg0)
{
  /* AND.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_EORw5(emu68_t * const emu68, int reg0)
{
  /* EOR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_ADDw5(emu68_t * const emu68, int reg0)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_SUBw5(emu68_t * const emu68, int reg0)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_CMPw5(emu68_t * const emu68, int reg0)
{
  /* CMP.W #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  CMPW(a,b);
}

static void l0_ORl5(emu68_t * const emu68, int reg0)
{
  /* OR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ANDl5(emu68_t * const emu68, int reg0)
{
  /* AND.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_EORl5(emu68_t * const emu68, int reg0)
{
  /* EOR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ADDl5(emu68_t * const emu68, int reg0)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_SUBl5(emu68_t * const emu68, int reg0)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_CMPl5(emu68_t * const emu68, int reg0)
{
  /* CMP.L #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  CMPL(a,b);
}

static void l0_ORb6(emu68_t * const emu68, int reg0)
{
  /* OR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_ANDb6(emu68_t * const emu68, int reg0)
{
  /* AND.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_EORb6(emu68_t * const emu68, int reg0)
{
  /* EOR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_ADDb6(emu68_t * const emu68, int reg0)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_SUBb6(emu68_t * const emu68, int reg0)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_CMPb6(emu68_t * const emu68, int reg0)
{
  /* CMP.B #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  CMPB(a,b);
}

static void l0_ORw6(emu68_t * const emu68, int reg0)
{
  /* OR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_ANDw6(emu68_t * const emu68, int reg0)
{
  /* AND.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_EORw6(emu68_t * const emu68, int reg0)
{
  /* EOR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_ADDw6(emu68_t * const emu68, int reg0)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_SUBw6(emu68_t * const emu68, int reg0)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_CMPw6(emu68_t * const emu68, int reg0)
{
  /* CMP.W #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  CMPW(a,b);
}

static void l0_ORl6(emu68_t * const emu68, int reg0)
{
  /* OR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ANDl6(emu68_t * const emu68, int reg0)
{
  /* AND.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_EORl6(emu68_t * const emu68, int reg0)
{
  /* EOR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ADDl6(emu68_t * const emu68, int reg0)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_SUBl6(emu68_t * const emu68, int reg0)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_CMPl6(emu68_t * const emu68, int reg0)
{
  /* CMP.L #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  CMPL(a,b);
}

static void l0_ORb7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) {
    uint68_t a;
    a = get_nextw()&255;
    REG68.sr |= a;
  } else {
  /* OR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
  }
}

static void l0_ANDb7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) {
    uint68_t a;
    a = get_nextw()|0xFF00;
    REG68.sr &= a;
  } else {
  /* AND.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
  }
}

static void l0_EORb7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) {
    uint68_t a;
    a = get_nextw()&255;
    REG68.sr ^= a;
  } else {
  /* EOR.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
  }
}

static void l0_ADDb7(emu68_t * const emu68, int reg0)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_SUBb7(emu68_t * const emu68, int reg0)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

static void l0_CMPb7(emu68_t * const emu68, int reg0)
{
  /* CMP.B #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  CMPB(a,b);
}

static void l0_ORw7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) {
    uint68_t a;
    a = get_nextw();
    REG68.sr |= a;
  } else {
  /* OR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
  }
}

static void l0_ANDw7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) {
    uint68_t a;
    a = get_nextw();
    REG68.sr &= a;
  } else {
  /* AND.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
  }
}

static void l0_EORw7(emu68_t * const emu68, int reg0)
{
  if (reg0==4) {
    uint68_t a;
    a = get_nextw();
    REG68.sr ^= a;
  } else {
  /* EOR.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
  }
}

static void l0_ADDw7(emu68_t * const emu68, int reg0)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_SUBw7(emu68_t * const emu68, int reg0)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

static void l0_CMPw7(emu68_t * const emu68, int reg0)
{
  /* CMP.W #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextw()<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  CMPW(a,b);
}

static void l0_ORl7(emu68_t * const emu68, int reg0)
{
  /* OR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ANDl7(emu68_t * const emu68, int reg0)
{
  /* AND.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_EORl7(emu68_t * const emu68, int reg0)
{
  /* EOR.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_ADDl7(emu68_t * const emu68, int reg0)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_SUBl7(emu68_t * const emu68, int reg0)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

static void l0_CMPl7(emu68_t * const emu68, int reg0)
{
  /* CMP.L #imm,<Ae> */
  int68_t a,b;
  addr68_t addr;
  a = get_nextl()<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  CMPL(a,b);
}

static void (*const line0_imm[8][32])(emu68_t * const emu68, int) =
{
/* OR */
  {
  l0_ORb0,l0_ill,l0_ORb2,l0_ORb3,l0_ORb4,l0_ORb5,l0_ORb6,l0_ORb7,
  l0_ORw0,l0_ill,l0_ORw2,l0_ORw3,l0_ORw4,l0_ORw5,l0_ORw6,l0_ORw7,
  l0_ORl0,l0_ill,l0_ORl2,l0_ORl3,l0_ORl4,l0_ORl5,l0_ORl6,l0_ORl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* AND */
  {
  l0_ANDb0,l0_ill,l0_ANDb2,l0_ANDb3,l0_ANDb4,l0_ANDb5,l0_ANDb6,l0_ANDb7,
  l0_ANDw0,l0_ill,l0_ANDw2,l0_ANDw3,l0_ANDw4,l0_ANDw5,l0_ANDw6,l0_ANDw7,
  l0_ANDl0,l0_ill,l0_ANDl2,l0_ANDl3,l0_ANDl4,l0_ANDl5,l0_ANDl6,l0_ANDl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* SUB */
  {
  l0_SUBb0,l0_ill,l0_SUBb2,l0_SUBb3,l0_SUBb4,l0_SUBb5,l0_SUBb6,l0_SUBb7,
  l0_SUBw0,l0_ill,l0_SUBw2,l0_SUBw3,l0_SUBw4,l0_SUBw5,l0_SUBw6,l0_SUBw7,
  l0_SUBl0,l0_ill,l0_SUBl2,l0_SUBl3,l0_SUBl4,l0_SUBl5,l0_SUBl6,l0_SUBl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* ADD */
  {
  l0_ADDb0,l0_ill,l0_ADDb2,l0_ADDb3,l0_ADDb4,l0_ADDb5,l0_ADDb6,l0_ADDb7,
  l0_ADDw0,l0_ill,l0_ADDw2,l0_ADDw3,l0_ADDw4,l0_ADDw5,l0_ADDw6,l0_ADDw7,
  l0_ADDl0,l0_ill,l0_ADDl2,l0_ADDl3,l0_ADDl4,l0_ADDl5,l0_ADDl6,l0_ADDl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* ? */
  {
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* EOR */
  {
  l0_EORb0,l0_ill,l0_EORb2,l0_EORb3,l0_EORb4,l0_EORb5,l0_EORb6,l0_EORb7,
  l0_EORw0,l0_ill,l0_EORw2,l0_EORw3,l0_EORw4,l0_EORw5,l0_EORw6,l0_EORw7,
  l0_EORl0,l0_ill,l0_EORl2,l0_EORl3,l0_EORl4,l0_EORl5,l0_EORl6,l0_EORl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* CMP */
  {
  l0_CMPb0,l0_ill,l0_CMPb2,l0_CMPb3,l0_CMPb4,l0_CMPb5,l0_CMPb6,l0_CMPb7,
  l0_CMPw0,l0_ill,l0_CMPw2,l0_CMPw3,l0_CMPw4,l0_CMPw5,l0_CMPw6,l0_CMPw7,
  l0_CMPl0,l0_ill,l0_CMPl2,l0_CMPl3,l0_CMPl4,l0_CMPl5,l0_CMPl6,l0_CMPl7,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
/* ? */
  {
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,
  l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,l0_ill,

  },
};

DECL_LINE68(line000)
{
  if (reg9==4) {
    BTST_reg(emu68,get_nextw(),reg0);
  } else {
    line0_imm[reg9][0](emu68,reg0);
  }
}

DECL_LINE68(line001)
{
  if (reg9==4) {
    BTST_mem(emu68,get_nextw(),1,reg0);
  } else {
    line0_imm[reg9][1](emu68,reg0);
  }
}

DECL_LINE68(line002)
{
  if (reg9==4) {
    BTST_mem(emu68,get_nextw(),2,reg0);
  } else {
    line0_imm[reg9][2](emu68,reg0);
  }
}

DECL_LINE68(line003)
{
  if (reg9==4) {
    BTST_mem(emu68,get_nextw(),3,reg0);
  } else {
    line0_imm[reg9][3](emu68,reg0);
  }
}

DECL_LINE68(line004)
{
  if (reg9==4) {
    BTST_mem(emu68,get_nextw(),4,reg0);
  } else {
    line0_imm[reg9][4](emu68,reg0);
  }
}

DECL_LINE68(line005)
{
  if (reg9==4) {
    BTST_mem(emu68,get_nextw(),5,reg0);
  } else {
    line0_imm[reg9][5](emu68,reg0);
  }
}

DECL_LINE68(line006)
{
  if (reg9==4) {
    BTST_mem(emu68,get_nextw(),6,reg0);
  } else {
    line0_imm[reg9][6](emu68,reg0);
  }
}

DECL_LINE68(line007)
{
  if (reg9==4) {
    BTST_mem(emu68,get_nextw(),7,reg0);
  } else {
    line0_imm[reg9][7](emu68,reg0);
  }
}

DECL_LINE68(line008)
{
  if (reg9==4) {
    BCHG_reg(emu68,get_nextw(),reg0);
  } else {
    line0_imm[reg9][8](emu68,reg0);
  }
}

DECL_LINE68(line009)
{
  if (reg9==4) {
    BCHG_mem(emu68,get_nextw(),1,reg0);
  } else {
    line0_imm[reg9][9](emu68,reg0);
  }
}

DECL_LINE68(line00A)
{
  if (reg9==4) {
    BCHG_mem(emu68,get_nextw(),2,reg0);
  } else {
    line0_imm[reg9][10](emu68,reg0);
  }
}

DECL_LINE68(line00B)
{
  if (reg9==4) {
    BCHG_mem(emu68,get_nextw(),3,reg0);
  } else {
    line0_imm[reg9][11](emu68,reg0);
  }
}

DECL_LINE68(line00C)
{
  if (reg9==4) {
    BCHG_mem(emu68,get_nextw(),4,reg0);
  } else {
    line0_imm[reg9][12](emu68,reg0);
  }
}

DECL_LINE68(line00D)
{
  if (reg9==4) {
    BCHG_mem(emu68,get_nextw(),5,reg0);
  } else {
    line0_imm[reg9][13](emu68,reg0);
  }
}

DECL_LINE68(line00E)
{
  if (reg9==4) {
    BCHG_mem(emu68,get_nextw(),6,reg0);
  } else {
    line0_imm[reg9][14](emu68,reg0);
  }
}

DECL_LINE68(line00F)
{
  if (reg9==4) {
    BCHG_mem(emu68,get_nextw(),7,reg0);
  } else {
    line0_imm[reg9][15](emu68,reg0);
  }
}

DECL_LINE68(line010)
{
  if (reg9==4) {
    BCLR_reg(emu68,get_nextw(),reg0);
  } else {
    line0_imm[reg9][16](emu68,reg0);
  }
}

DECL_LINE68(line011)
{
  if (reg9==4) {
    BCLR_mem(emu68,get_nextw(),1,reg0);
  } else {
    line0_imm[reg9][17](emu68,reg0);
  }
}

DECL_LINE68(line012)
{
  if (reg9==4) {
    BCLR_mem(emu68,get_nextw(),2,reg0);
  } else {
    line0_imm[reg9][18](emu68,reg0);
  }
}

DECL_LINE68(line013)
{
  if (reg9==4) {
    BCLR_mem(emu68,get_nextw(),3,reg0);
  } else {
    line0_imm[reg9][19](emu68,reg0);
  }
}

DECL_LINE68(line014)
{
  if (reg9==4) {
    BCLR_mem(emu68,get_nextw(),4,reg0);
  } else {
    line0_imm[reg9][20](emu68,reg0);
  }
}

DECL_LINE68(line015)
{
  if (reg9==4) {
    BCLR_mem(emu68,get_nextw(),5,reg0);
  } else {
    line0_imm[reg9][21](emu68,reg0);
  }
}

DECL_LINE68(line016)
{
  if (reg9==4) {
    BCLR_mem(emu68,get_nextw(),6,reg0);
  } else {
    line0_imm[reg9][22](emu68,reg0);
  }
}

DECL_LINE68(line017)
{
  if (reg9==4) {
    BCLR_mem(emu68,get_nextw(),7,reg0);
  } else {
    line0_imm[reg9][23](emu68,reg0);
  }
}

DECL_LINE68(line018)
{
  if (reg9==4) {
    BSET_reg(emu68,get_nextw(),reg0);
  } else {
    line0_imm[reg9][24](emu68,reg0);
  }
}

DECL_LINE68(line019)
{
  if (reg9==4) {
    BSET_mem(emu68,get_nextw(),1,reg0);
  } else {
    line0_imm[reg9][25](emu68,reg0);
  }
}

DECL_LINE68(line01A)
{
  if (reg9==4) {
    BSET_mem(emu68,get_nextw(),2,reg0);
  } else {
    line0_imm[reg9][26](emu68,reg0);
  }
}

DECL_LINE68(line01B)
{
  if (reg9==4) {
    BSET_mem(emu68,get_nextw(),3,reg0);
  } else {
    line0_imm[reg9][27](emu68,reg0);
  }
}

DECL_LINE68(line01C)
{
  if (reg9==4) {
    BSET_mem(emu68,get_nextw(),4,reg0);
  } else {
    line0_imm[reg9][28](emu68,reg0);
  }
}

DECL_LINE68(line01D)
{
  if (reg9==4) {
    BSET_mem(emu68,get_nextw(),5,reg0);
  } else {
    line0_imm[reg9][29](emu68,reg0);
  }
}

DECL_LINE68(line01E)
{
  if (reg9==4) {
    BSET_mem(emu68,get_nextw(),6,reg0);
  } else {
    line0_imm[reg9][30](emu68,reg0);
  }
}

DECL_LINE68(line01F)
{
  if (reg9==4) {
    BSET_mem(emu68,get_nextw(),7,reg0);
  } else {
    line0_imm[reg9][31](emu68,reg0);
  }
}

DECL_LINE68(line020)
{
  /* BTST Dx,Dy */
  int68_t a,b;
  a = REG68.d[reg0];
  b = REG68.d[reg9]&31;
  BTST(a,b);
  ADDCYCLE(2);
}

DECL_LINE68(line021)
{
  /* MOVEP.W d(An),Dn */
  const addr68_t addr = REG68.a[reg0] + get_nextw();
  uint68_t a;
  a  =(u8)read_B(addr+0)<<8;
  a +=(u8)read_B(addr+2)<<0;
  REG68.d[reg9] = (REG68.d[reg9]&~0xFFFF) | a;
  ADDCYCLE(8);
}

DECL_LINE68(line022)
{
  /* BTST Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BTST(a,b);
  ADDCYCLE(0);
}

DECL_LINE68(line023)
{
  /* BTST Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BTST(a,b);
  ADDCYCLE(0);
}

DECL_LINE68(line024)
{
  /* BTST Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BTST(a,b);
  ADDCYCLE(0);
}

DECL_LINE68(line025)
{
  /* BTST Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BTST(a,b);
  ADDCYCLE(0);
}

DECL_LINE68(line026)
{
  /* BTST Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BTST(a,b);
  ADDCYCLE(0);
}

DECL_LINE68(line027)
{
  /* BTST Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BTST(a,b);
  ADDCYCLE(0);
}

DECL_LINE68(line028)
{
  /* BCHG Dx,Dy */
  int68_t a,b;
  a = REG68.d[reg0];
  b = REG68.d[reg9]&31;
  BCHG(a,b);
  REG68.d[reg0] = a;
  ADDCYCLE(4);
}

DECL_LINE68(line029)
{
  /* MOVEP.L d(An),Dn */
  const addr68_t addr = REG68.a[reg0] + get_nextw();
  uint68_t a;
  a  =(u8)read_B(addr+0)<<24;
  a +=(u8)read_B(addr+2)<<16;
  a +=(u8)read_B(addr+4)<<8;
  a +=(u8)read_B(addr+6)<<0;
  REG68.d[reg9] = a;
  ADDCYCLE(16);
}

DECL_LINE68(line02A)
{
  /* BCHG Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCHG(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line02B)
{
  /* BCHG Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCHG(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line02C)
{
  /* BCHG Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCHG(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line02D)
{
  /* BCHG Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCHG(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line02E)
{
  /* BCHG Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCHG(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line02F)
{
  /* BCHG Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCHG(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line030)
{
  /* BCLR Dx,Dy */
  int68_t a,b;
  a = REG68.d[reg0];
  b = REG68.d[reg9]&31;
  BCLR(a,b);
  REG68.d[reg0] = a;
  ADDCYCLE(6);
}

DECL_LINE68(line031)
{
  /* MOVEP.W Dn,d(An) */
  const addr68_t addr = REG68.a[reg0] + get_nextw();
  const uint68_t a = REG68.d[reg9];
  write_B(addr+0,a>>8);
  write_B(addr+2,a>>0);
  ADDCYCLE(8);
}

DECL_LINE68(line032)
{
  /* BCLR Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCLR(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line033)
{
  /* BCLR Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCLR(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line034)
{
  /* BCLR Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCLR(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line035)
{
  /* BCLR Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCLR(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line036)
{
  /* BCLR Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCLR(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line037)
{
  /* BCLR Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BCLR(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line038)
{
  /* BSET Dx,Dy */
  int68_t a,b;
  a = REG68.d[reg0];
  b = REG68.d[reg9]&31;
  BSET(a,b);
  REG68.d[reg0] = a;
  ADDCYCLE(4);
}

DECL_LINE68(line039)
{
  /* MOVEP.L Dn,d(An) */
  const addr68_t addr = REG68.a[reg0] + get_nextw();
  const uint68_t a = REG68.d[reg9];
  write_B(addr+0,a>>24);
  write_B(addr+2,a>>16);
  write_B(addr+4,a>>8);
  write_B(addr+6,a>>0);
  ADDCYCLE(16);
}

DECL_LINE68(line03A)
{
  /* BSET Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BSET(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line03B)
{
  /* BSET Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BSET(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line03C)
{
  /* BSET Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BSET(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line03D)
{
  /* BSET Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BSET(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line03E)
{
  /* BSET Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BSET(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

DECL_LINE68(line03F)
{
  /* BSET Dn,<Ae> */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  a = read_B(addr);
  b = REG68.d[reg9]&7;
  BSET(a,b);
  write_B(addr,a);
  ADDCYCLE(4);
}

