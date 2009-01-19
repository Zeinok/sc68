/* line8.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

DECL_LINE68(line800)
{
  /* OR.B Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ORB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line801)
{
  ILLEGAL;
}

DECL_LINE68(line802)
{
  /* OR.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ORB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line803)
{
  /* OR.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ORB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line804)
{
  /* OR.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ORB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line805)
{
  /* OR.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ORB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line806)
{
  /* OR.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ORB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line807)
{
  /* OR.B <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  a = read_B(addr)<<BYTE_SHIFT;
  b = REG68.d[reg9]<<BYTE_SHIFT;
  ORB(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line808)
{
  /* OR.W Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ORW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line809)
{
  ILLEGAL;
}

DECL_LINE68(line80A)
{
  /* OR.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ORW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line80B)
{
  /* OR.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ORW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line80C)
{
  /* OR.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ORW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line80D)
{
  /* OR.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ORW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line80E)
{
  /* OR.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ORW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line80F)
{
  /* OR.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  ORW(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line810)
{
  /* OR.L Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ORL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line811)
{
  ILLEGAL;
}

DECL_LINE68(line812)
{
  /* OR.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[2](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ORL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line813)
{
  /* OR.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[3](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ORL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line814)
{
  /* OR.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[4](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ORL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line815)
{
  /* OR.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[5](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ORL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line816)
{
  /* OR.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[6](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ORL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line817)
{
  /* OR.L <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eal68[7](emu68,reg0);
  a = read_L(addr)<<LONG_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  ORL(s,a,b);
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line818)
{
  /* DIVU.W Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line819)
{
  /* DIVU.W Ax,Dy */
  int68_t a,b,s;
  a = REG68.a[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line81A)
{
  /* DIVU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line81B)
{
  /* DIVU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line81C)
{
  /* DIVU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line81D)
{
  /* DIVU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line81E)
{
  /* DIVU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line81F)
{
  /* DIVU.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVUW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line820)
{
  /* SBCD Dy,Dx */
  reg0=reg0; reg9=reg9;
}

DECL_LINE68(line821)
{
  /* SBCD -(Ay),-(Ax) */
  REG68.a[reg0] -= 2;
  REG68.a[reg9] -= 2;
}

DECL_LINE68(line822)
{
  /* OR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line823)
{
  /* OR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line824)
{
  /* OR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line825)
{
  /* OR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line826)
{
  /* OR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line827)
{
  /* OR.B Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ORB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line828)
{
  /* OR.W Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg9]<<WORD_SHIFT;
  b = REG68.d[reg0]<<WORD_SHIFT;
  ORW(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line829)
{
  ILLEGAL;
}

DECL_LINE68(line82A)
{
  /* OR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line82B)
{
  /* OR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line82C)
{
  /* OR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line82D)
{
  /* OR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line82E)
{
  /* OR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line82F)
{
  /* OR.W Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ORW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line830)
{
  /* OR.L Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg9]<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  ORL(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line831)
{
  ILLEGAL;
}

DECL_LINE68(line832)
{
  /* OR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line833)
{
  /* OR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line834)
{
  /* OR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line835)
{
  /* OR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line836)
{
  /* OR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line837)
{
  /* OR.L Dn,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = REG68.d[reg9]<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ORL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line838)
{
  /* DIVS.W Dx,Dy */
  int68_t a,b,s;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line839)
{
  /* DIVS.W Ax,Dy */
  int68_t a,b,s;
  a = REG68.a[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line83A)
{
  /* DIVS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line83B)
{
  /* DIVS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line83C)
{
  /* DIVS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line83D)
{
  /* DIVS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line83E)
{
  /* DIVS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

DECL_LINE68(line83F)
{
  /* DIVS.W <Ae>,Dn */
  int68_t a,b,s;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<LONG_SHIFT;
  DIVSW(s,a,b);
  REG68.d[reg9] = s>>LONG_SHIFT;
}

