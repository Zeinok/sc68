/* lineB.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

DECL_LINE68(lineB00)
{
  /* CMP.B Dx,Dy */
  int68_t a,b;
  a = REG68.d[reg0]<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  CMPB(a,b);
}

DECL_LINE68(lineB01)
{
  /* CMP.B Ax,Dy */
  int68_t a,b;
  a = REG68.a[reg0]<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  CMPB(a,b);
}

DECL_LINE68(lineB02)
{
  /* CMP.B <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  CMPB(a,b);
}

DECL_LINE68(lineB03)
{
  /* CMP.B <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  CMPB(a,b);
}

DECL_LINE68(lineB04)
{
  /* CMP.B <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  CMPB(a,b);
}

DECL_LINE68(lineB05)
{
  /* CMP.B <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  CMPB(a,b);
}

DECL_LINE68(lineB06)
{
  /* CMP.B <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  CMPB(a,b);
}

DECL_LINE68(lineB07)
{
  /* CMP.B <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  CMPB(a,b);
}

DECL_LINE68(lineB08)
{
  /* CMP.W Dx,Dy */
  int68_t a,b;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CMPW(a,b);
}

DECL_LINE68(lineB09)
{
  /* CMP.W Ax,Dy */
  int68_t a,b;
  a = REG68.a[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CMPW(a,b);
}

DECL_LINE68(lineB0A)
{
  /* CMP.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CMPW(a,b);
}

DECL_LINE68(lineB0B)
{
  /* CMP.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CMPW(a,b);
}

DECL_LINE68(lineB0C)
{
  /* CMP.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CMPW(a,b);
}

DECL_LINE68(lineB0D)
{
  /* CMP.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CMPW(a,b);
}

DECL_LINE68(lineB0E)
{
  /* CMP.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CMPW(a,b);
}

DECL_LINE68(lineB0F)
{
  /* CMP.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CMPW(a,b);
}

DECL_LINE68(lineB10)
{
  /* CMP.L Dx,Dy */
  int68_t a,b;
  a = REG68.d[reg0]<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  CMPL(a,b);
}

DECL_LINE68(lineB11)
{
  /* CMP.L Ax,Dy */
  int68_t a,b;
  a = REG68.a[reg0]<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  CMPL(a,b);
}

DECL_LINE68(lineB12)
{
  /* CMP.L <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[2](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  CMPL(a,b);
}

DECL_LINE68(lineB13)
{
  /* CMP.L <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[3](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  CMPL(a,b);
}

DECL_LINE68(lineB14)
{
  /* CMP.L <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[4](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  CMPL(a,b);
}

DECL_LINE68(lineB15)
{
  /* CMP.L <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[5](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  CMPL(a,b);
}

DECL_LINE68(lineB16)
{
  /* CMP.L <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[6](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  CMPL(a,b);
}

DECL_LINE68(lineB17)
{
  /* CMP.L <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[7](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  CMPL(a,b);
}

DECL_LINE68(lineB18)
{
  /* CMP.W Dx,Ay */
  int68_t a,b;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.a[reg9];
  CMPAW(a,b);
}

DECL_LINE68(lineB19)
{
  /* CMP.W Ax,Ay */
  int68_t a,b;
  a = REG68.a[reg0]<<WORD_SHIFT;
  b = REG68.a[reg9];
  CMPAW(a,b);
}

DECL_LINE68(lineB1A)
{
  /* CMP.W <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  CMPAW(a,b);
}

DECL_LINE68(lineB1B)
{
  /* CMP.W <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  CMPAW(a,b);
}

DECL_LINE68(lineB1C)
{
  /* CMP.W <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  CMPAW(a,b);
}

DECL_LINE68(lineB1D)
{
  /* CMP.W <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  CMPAW(a,b);
}

DECL_LINE68(lineB1E)
{
  /* CMP.W <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  CMPAW(a,b);
}

DECL_LINE68(lineB1F)
{
  /* CMP.W <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  CMPAW(a,b);
}

DECL_LINE68(lineB20)
{
  /* EOR.B Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  b = REG68.d[reg0]<<BYTE_SHIFT;
  EORB(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineB21)
{
  /* CMPM.B (Ay)+,(Ax)+ */
  int68_t x,y;
  y = read_B(REG68.a[reg0]-=1);
  x = read_B(REG68.a[reg9]-=1);
  CMPB(y,x);
}

DECL_LINE68(lineB22)
{
  /* EOR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineB23)
{
  /* EOR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineB24)
{
  /* EOR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineB25)
{
  /* EOR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineB26)
{
  /* EOR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineB27)
{
  /* EOR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  EORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineB28)
{
  /* EOR.W Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg9]<<WORD_SHIFT;
  b = REG68.d[reg0]<<WORD_SHIFT;
  EORW(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineB29)
{
  /* CMPM.W (Ay)+,(Ax)+ */
  int68_t x,y;
  y = read_W(REG68.a[reg0]-=2);
  x = read_W(REG68.a[reg9]-=2);
  CMPW(y,x);
}

DECL_LINE68(lineB2A)
{
  /* EOR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineB2B)
{
  /* EOR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineB2C)
{
  /* EOR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineB2D)
{
  /* EOR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineB2E)
{
  /* EOR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineB2F)
{
  /* EOR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  EORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineB30)
{
  /* EOR.L Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg9]<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  EORL(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineB31)
{
  /* CMPM.L (Ay)+,(Ax)+ */
  int68_t x,y;
  y = read_L(REG68.a[reg0]-=4);
  x = read_L(REG68.a[reg9]-=4);
  CMPL(y,x);
}

DECL_LINE68(lineB32)
{
  /* EOR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineB33)
{
  /* EOR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineB34)
{
  /* EOR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineB35)
{
  /* EOR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineB36)
{
  /* EOR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineB37)
{
  /* EOR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  EORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineB38)
{
  /* CMP.L Dx,Ay */
  int68_t a,b;
  a = REG68.d[reg0]<<LONG_SHIFT;
  b = REG68.a[reg9];
  CMPAL(a,b);
}

DECL_LINE68(lineB39)
{
  /* CMP.L Ax,Ay */
  int68_t a,b;
  a = REG68.a[reg0]<<LONG_SHIFT;
  b = REG68.a[reg9];
  CMPAL(a,b);
}

DECL_LINE68(lineB3A)
{
  /* CMP.L <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[2](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  CMPAL(a,b);
}

DECL_LINE68(lineB3B)
{
  /* CMP.L <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[3](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  CMPAL(a,b);
}

DECL_LINE68(lineB3C)
{
  /* CMP.L <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[4](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  CMPAL(a,b);
}

DECL_LINE68(lineB3D)
{
  /* CMP.L <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[5](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  CMPAL(a,b);
}

DECL_LINE68(lineB3E)
{
  /* CMP.L <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[6](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  CMPAL(a,b);
}

DECL_LINE68(lineB3F)
{
  /* CMP.L <Ae>,An */
  int68_t a,b;
  addr68_t addr;
  addr = get_eal68[7](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  CMPAL(a,b);
}

