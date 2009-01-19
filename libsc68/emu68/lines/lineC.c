/* lineC.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

DECL_LINE68(lineC00)
{
  /* AND.B Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ANDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC01)
{
  /*ILLEGAL in EXG*/
  ILLEGAL;
}

DECL_LINE68(lineC02)
{
  /* AND.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ANDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC03)
{
  /* AND.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ANDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC04)
{
  /* AND.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ANDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC05)
{
  /* AND.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ANDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC06)
{
  /* AND.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ANDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC07)
{
  /* AND.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ANDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC08)
{
  /* AND.W Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ANDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC09)
{
  /* EXG Ax,Ay */
  EXG(REG68.a[reg9],REG68.a[reg0]);
}

DECL_LINE68(lineC0A)
{
  /* AND.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ANDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC0B)
{
  /* AND.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ANDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC0C)
{
  /* AND.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ANDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC0D)
{
  /* AND.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ANDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC0E)
{
  /* AND.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ANDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC0F)
{
  /* AND.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ANDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC10)
{
  /* AND.L Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ANDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC11)
{
  /* EXG Dx,Ay */
  EXG(REG68.d[reg9],REG68.a[reg0]);
}

DECL_LINE68(lineC12)
{
  /* AND.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[2](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ANDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC13)
{
  /* AND.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[3](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ANDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC14)
{
  /* AND.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[4](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ANDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC15)
{
  /* AND.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[5](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ANDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC16)
{
  /* AND.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[6](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ANDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC17)
{
  /* AND.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[7](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ANDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC18)
{
  /* MULU.W Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC19)
{
  /* MULU.W Ax,Dy */
  int68_t a,b,s;
  a = REG68.a[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC1A)
{
  /* MULU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC1B)
{
  /* MULU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC1C)
{
  /* MULU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC1D)
{
  /* MULU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC1E)
{
  /* MULU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC1F)
{
  /* MULU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC20)
{
  /* ABCD Dy,Dx */
  reg0=reg0; reg9=reg9;
}

DECL_LINE68(lineC21)
{
  /* ABCD -(Ay),-(Ax) */
  REG68.a[reg0] -= 2;
  REG68.a[reg9] -= 2;
}

DECL_LINE68(lineC22)
{
  /* AND.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC23)
{
  /* AND.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC24)
{
  /* AND.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC25)
{
  /* AND.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC26)
{
  /* AND.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC27)
{
  /* AND.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ANDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineC28)
{
  /* EXG Dx,Dy */
  EXG(REG68.d[reg9],REG68.d[reg0]);
}

DECL_LINE68(lineC29)
{
  /* EXG Ax,Ay */
  EXG(REG68.a[reg9],REG68.a[reg0]);
}

DECL_LINE68(lineC2A)
{
  /* AND.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC2B)
{
  /* AND.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC2C)
{
  /* AND.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC2D)
{
  /* AND.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC2E)
{
  /* AND.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC2F)
{
  /* AND.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ANDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineC30)
{
  /* AND.L Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg9]<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  ANDL(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC31)
{
  /* EXG Dx,Ay */
  EXG(REG68.d[reg9],REG68.a[reg0]);
}

DECL_LINE68(lineC32)
{
  /* AND.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC33)
{
  /* AND.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC34)
{
  /* AND.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC35)
{
  /* AND.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC36)
{
  /* AND.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC37)
{
  /* AND.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ANDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineC38)
{
  /* MULS.W Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC39)
{
  /* MULS.W Ax,Dy */
  int68_t a,b,s;
  a = REG68.a[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC3A)
{
  /* MULS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC3B)
{
  /* MULS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC3C)
{
  /* MULS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC3D)
{
  /* MULS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC3E)
{
  /* MULS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineC3F)
{
  /* MULS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  MULSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

