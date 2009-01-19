/* lineD.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

DECL_LINE68(lineD00)
{
  /* ADD.B Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ADDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD01)
{
  /* ADD.B Ax,Dy */
  int68_t a,b,s;
  a = REG68.a[reg0]<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ADDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD02)
{
  /* ADD.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ADDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD03)
{
  /* ADD.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ADDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD04)
{
  /* ADD.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ADDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD05)
{
  /* ADD.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ADDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD06)
{
  /* ADD.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ADDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD07)
{
  /* ADD.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ADDB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD08)
{
  /* ADD.W Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ADDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD09)
{
  /* ADD.W Ax,Dy */
  int68_t a,b,s;
  a = REG68.a[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ADDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD0A)
{
  /* ADD.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ADDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD0B)
{
  /* ADD.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ADDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD0C)
{
  /* ADD.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ADDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD0D)
{
  /* ADD.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ADDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD0E)
{
  /* ADD.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ADDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD0F)
{
  /* ADD.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ADDW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD10)
{
  /* ADD.L Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ADDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD11)
{
  /* ADD.L Ax,Dy */
  int68_t a,b,s;
  a = REG68.a[reg0]<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ADDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD12)
{
  /* ADD.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[2](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ADDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD13)
{
  /* ADD.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[3](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ADDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD14)
{
  /* ADD.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[4](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ADDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD15)
{
  /* ADD.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[5](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ADDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD16)
{
  /* ADD.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[6](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ADDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD17)
{
  /* ADD.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[7](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ADDL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD18)
{
  /* ADD.W Dx,Ay */
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.a[reg9];
  ADDAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD19)
{
  /* ADD.W Ax,Ay */
  int68_t a,b,s;
  a = REG68.a[reg0]<<WORD_SHIFT;
  b = REG68.a[reg9];
  ADDAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD1A)
{
  /* ADD.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  ADDAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD1B)
{
  /* ADD.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  ADDAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD1C)
{
  /* ADD.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  ADDAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD1D)
{
  /* ADD.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  ADDAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD1E)
{
  /* ADD.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  ADDAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD1F)
{
  /* ADD.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  ADDAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD20)
{
  int68_t a,b,s;
  a = REG68.d[reg0]<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ADDXB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD21)
{
  int68_t a,b,s;
  a = read_B(REG68.a[reg0]-=1)<<BYTE_SHIFT;
  b = read_B(REG68.a[reg9]-=1)<<BYTE_SHIFT;
  ADDXB(s,a,b);
  write_B(REG68.a[reg9],(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD22)
{
  /* ADD.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD23)
{
  /* ADD.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD24)
{
  /* ADD.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD25)
{
  /* ADD.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD26)
{
  /* ADD.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD27)
{
  /* ADD.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(lineD28)
{
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ADDXW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD29)
{
  int68_t a,b,s;
  a = read_W(REG68.a[reg0]-=2)<<WORD_SHIFT;
  b = read_W(REG68.a[reg9]-=2)<<WORD_SHIFT;
  ADDXW(s,a,b);
  write_W(REG68.a[reg9],(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD2A)
{
  /* ADD.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD2B)
{
  /* ADD.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD2C)
{
  /* ADD.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD2D)
{
  /* ADD.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD2E)
{
  /* ADD.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD2F)
{
  /* ADD.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(lineD30)
{
  int68_t a,b,s;
  a = REG68.d[reg0]<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ADDXL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD31)
{
  int68_t a,b,s;
  a = read_L(REG68.a[reg0]-=4)<<LONG_SHIFT;
  b = read_L(REG68.a[reg9]-=4)<<LONG_SHIFT;
  ADDXL(s,a,b);
  write_L(REG68.a[reg9],(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD32)
{
  /* ADD.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD33)
{
  /* ADD.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD34)
{
  /* ADD.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD35)
{
  /* ADD.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD36)
{
  /* ADD.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD37)
{
  /* ADD.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(lineD38)
{
  /* ADD.L Dx,Ay */
  int68_t a,b,s;
  a = REG68.d[reg0]<<LONG_SHIFT;
  b = REG68.a[reg9];
  ADDAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD39)
{
  /* ADD.L Ax,Ay */
  int68_t a,b,s;
  a = REG68.a[reg0]<<LONG_SHIFT;
  b = REG68.a[reg9];
  ADDAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD3A)
{
  /* ADD.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[2](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  ADDAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD3B)
{
  /* ADD.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[3](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  ADDAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD3C)
{
  /* ADD.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[4](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  ADDAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD3D)
{
  /* ADD.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[5](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  ADDAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD3E)
{
  /* ADD.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[6](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  ADDAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(lineD3F)
{
  /* ADD.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[7](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  ADDAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

