/* line9.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

DECL_LINE68(line900)
{
  /* SUB.B Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  SUBB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line901)
{
  /* SUB.B Ax,Dy */
  int68_t a,b,s;
  a = REG68.a[reg0]<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  SUBB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line902)
{
  /* SUB.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  SUBB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line903)
{
  /* SUB.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  SUBB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line904)
{
  /* SUB.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  SUBB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line905)
{
  /* SUB.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  SUBB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line906)
{
  /* SUB.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  SUBB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line907)
{
  /* SUB.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  SUBB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line908)
{
  /* SUB.W Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  SUBW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line909)
{
  /* SUB.W Ax,Dy */
  int68_t a,b,s;
  a = REG68.a[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  SUBW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line90A)
{
  /* SUB.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  SUBW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line90B)
{
  /* SUB.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  SUBW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line90C)
{
  /* SUB.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  SUBW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line90D)
{
  /* SUB.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  SUBW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line90E)
{
  /* SUB.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  SUBW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line90F)
{
  /* SUB.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  SUBW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line910)
{
  /* SUB.L Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  SUBL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line911)
{
  /* SUB.L Ax,Dy */
  int68_t a,b,s;
  a = REG68.a[reg0]<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  SUBL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line912)
{
  /* SUB.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[2](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  SUBL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line913)
{
  /* SUB.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[3](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  SUBL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line914)
{
  /* SUB.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[4](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  SUBL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line915)
{
  /* SUB.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[5](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  SUBL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line916)
{
  /* SUB.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[6](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  SUBL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line917)
{
  /* SUB.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[7](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  SUBL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line918)
{
  /* SUB.W Dx,Ay */
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.a[reg9];
  SUBAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line919)
{
  /* SUB.W Ax,Ay */
  int68_t a,b,s;
  a = REG68.a[reg0]<<WORD_SHIFT;
  b = REG68.a[reg9];
  SUBAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line91A)
{
  /* SUB.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  SUBAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line91B)
{
  /* SUB.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  SUBAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line91C)
{
  /* SUB.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  SUBAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line91D)
{
  /* SUB.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  SUBAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line91E)
{
  /* SUB.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  SUBAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line91F)
{
  /* SUB.W <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.a[reg9];
  SUBAW(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line920)
{
  int68_t a,b,s;
  a = REG68.d[reg0]<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  SUBXB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line921)
{
  int68_t a,b,s;
  a = read_B(REG68.a[reg0]-=1)<<BYTE_SHIFT;
  b = read_B(REG68.a[reg9]-=1)<<BYTE_SHIFT;
  SUBXB(s,a,b);
  write_B(REG68.a[reg9],(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line922)
{
  /* SUB.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line923)
{
  /* SUB.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line924)
{
  /* SUB.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line925)
{
  /* SUB.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line926)
{
  /* SUB.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line927)
{
  /* SUB.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line928)
{
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  SUBXW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line929)
{
  int68_t a,b,s;
  a = read_W(REG68.a[reg0]-=2)<<WORD_SHIFT;
  b = read_W(REG68.a[reg9]-=2)<<WORD_SHIFT;
  SUBXW(s,a,b);
  write_W(REG68.a[reg9],(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line92A)
{
  /* SUB.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line92B)
{
  /* SUB.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line92C)
{
  /* SUB.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line92D)
{
  /* SUB.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line92E)
{
  /* SUB.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line92F)
{
  /* SUB.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line930)
{
  int68_t a,b,s;
  a = REG68.d[reg0]<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  SUBXL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line931)
{
  int68_t a,b,s;
  a = read_L(REG68.a[reg0]-=4)<<LONG_SHIFT;
  b = read_L(REG68.a[reg9]-=4)<<LONG_SHIFT;
  SUBXL(s,a,b);
  write_L(REG68.a[reg9],(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line932)
{
  /* SUB.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line933)
{
  /* SUB.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line934)
{
  /* SUB.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line935)
{
  /* SUB.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line936)
{
  /* SUB.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line937)
{
  /* SUB.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line938)
{
  /* SUB.L Dx,Ay */
  int68_t a,b,s;
  a = REG68.d[reg0]<<LONG_SHIFT;
  b = REG68.a[reg9];
  SUBAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line939)
{
  /* SUB.L Ax,Ay */
  int68_t a,b,s;
  a = REG68.a[reg0]<<LONG_SHIFT;
  b = REG68.a[reg9];
  SUBAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line93A)
{
  /* SUB.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[2](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  SUBAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line93B)
{
  /* SUB.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[3](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  SUBAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line93C)
{
  /* SUB.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[4](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  SUBAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line93D)
{
  /* SUB.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[5](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  SUBAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line93E)
{
  /* SUB.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[6](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  SUBAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line93F)
{
  /* SUB.L <Ae>,An */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[7](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.a[reg9];
  SUBAL(s,a,b);
  REG68.a[reg9] = s>>LONG_SHIFT;
}

