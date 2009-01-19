/* line5.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

DECL_LINE68(line500)
{
  /* ADD.B #imm,Dy */
  int68_t a,b,s;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  b = REG68.d[reg0]<<BYTE_SHIFT;
  ADDB(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line501)
{
  ILLEGAL;
}

DECL_LINE68(line502)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line503)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line504)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line505)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line506)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line507)
{
  /* ADD.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  ADDB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line508)
{
  /* ADD.W #imm,Dy */
  int68_t a,b,s;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  b = REG68.d[reg0]<<WORD_SHIFT;
  ADDW(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line509)
{
  /* ADD.W #imm,Ay */
  int68_t a,b,s;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  b = REG68.a[reg0];
  ADDAW(s,a,b);
  REG68.a[reg0] = s;
}

DECL_LINE68(line50A)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line50B)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line50C)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line50D)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line50E)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line50F)
{
  /* ADD.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  ADDW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line510)
{
  /* ADD.L #imm,Dy */
  int68_t a,b,s;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  ADDL(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line511)
{
  /* ADD.L #imm,Ay */
  int68_t a,b,s;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  b = REG68.a[reg0];
  ADDAL(s,a,b);
  REG68.a[reg0] = s;
}

DECL_LINE68(line512)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line513)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line514)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line515)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line516)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line517)
{
  /* ADD.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  ADDL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line518)
{
  /* Scc Dn */
  REG68.d[reg0] = 
(REG68.d[reg0]&0xFFFFFF00)+(u8)-(is_cc68[reg9](REG68.sr)^1);
}

DECL_LINE68(line519)
{
  /* DBcc Dn,<adr> */
  if (is_cc68[reg9](REG68.sr)) {
    int68_t a;
    a=REG68.d[reg0]-1;
    a&=0xFFFF;
    REG68.d[reg0] = (REG68.d[reg0]&0xFFFF0000) + a;
    if (a!=0xFFFF) {
      uint68_t pc=REG68.pc;
      REG68.pc = pc+get_nextw();
    } else REG68.pc += 2;
  } else REG68.pc += 2;
}

DECL_LINE68(line51A)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  write_B(addr,-(is_cc68[reg9](REG68.sr)^1));
}

DECL_LINE68(line51B)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  write_B(addr,-(is_cc68[reg9](REG68.sr)^1));
}

DECL_LINE68(line51C)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  write_B(addr,-(is_cc68[reg9](REG68.sr)^1));
}

DECL_LINE68(line51D)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  write_B(addr,-(is_cc68[reg9](REG68.sr)^1));
}

DECL_LINE68(line51E)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  write_B(addr,-(is_cc68[reg9](REG68.sr)^1));
}

DECL_LINE68(line51F)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  write_B(addr,-(is_cc68[reg9](REG68.sr)^1));
}

DECL_LINE68(line520)
{
  /* SUB.B #imm,Dy */
  int68_t a,b,s;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  b = REG68.d[reg0]<<BYTE_SHIFT;
  SUBB(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) + ((uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line521)
{
  ILLEGAL;
}

DECL_LINE68(line522)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[2](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line523)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[3](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line524)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[4](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line525)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[5](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line526)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[6](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line527)
{
  /* SUB.B #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<BYTE_SHIFT;
  addr = get_eab68[7](emu68,reg0);
  b = read_B(addr)<<BYTE_SHIFT;
  SUBB(s,a,b);
  write_B(addr,(uint68_t)s>>BYTE_SHIFT);
}

DECL_LINE68(line528)
{
  /* SUB.W #imm,Dy */
  int68_t a,b,s;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  b = REG68.d[reg0]<<WORD_SHIFT;
  SUBW(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) + ((uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line529)
{
  /* SUB.W #imm,Ay */
  int68_t a,b,s;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  b = REG68.a[reg0];
  SUBAW(s,a,b);
  REG68.a[reg0] = s;
}

DECL_LINE68(line52A)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[2](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line52B)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[3](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line52C)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[4](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line52D)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[5](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line52E)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[6](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line52F)
{
  /* SUB.W #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<WORD_SHIFT;
  addr = get_eaw68[7](emu68,reg0);
  b = read_W(addr)<<WORD_SHIFT;
  SUBW(s,a,b);
  write_W(addr,(uint68_t)s>>WORD_SHIFT);
}

DECL_LINE68(line530)
{
  /* SUB.L #imm,Dy */
  int68_t a,b,s;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  b = REG68.d[reg0]<<LONG_SHIFT;
  SUBL(s,a,b);
  REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) + ((uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line531)
{
  /* SUB.L #imm,Ay */
  int68_t a,b,s;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  b = REG68.a[reg0];
  SUBAL(s,a,b);
  REG68.a[reg0] = s;
}

DECL_LINE68(line532)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[2](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line533)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[3](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line534)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[4](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line535)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[5](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line536)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[6](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line537)
{
  /* SUB.L #imm,<Ae> */
  int68_t a,b,s;
  addr68_t addr;
  a = (((reg9-1)&7)+1)<<LONG_SHIFT;
  addr = get_eal68[7](emu68,reg0);
  b = read_L(addr)<<LONG_SHIFT;
  SUBL(s,a,b);
  write_L(addr,(uint68_t)s>>LONG_SHIFT);
}

DECL_LINE68(line538)
{
  /* Scc Dn */
  REG68.d[reg0] = 
(REG68.d[reg0]&0xFFFFFF00)+(u8)-is_cc68[reg9](REG68.sr);
}

DECL_LINE68(line539)
{
  /* DBcc Dn,<adr> */
  if (!is_cc68[reg9](REG68.sr)) {
    int68_t a;
    a=REG68.d[reg0]-1;
    a&=0xFFFF;
    REG68.d[reg0] = (REG68.d[reg0]&0xFFFF0000) + a;
    if (a!=0xFFFF) {
      uint68_t pc=REG68.pc;
      REG68.pc = pc+get_nextw();
    } else REG68.pc += 2;
  } else REG68.pc += 2;
}

DECL_LINE68(line53A)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[2](emu68,reg0);
  write_B(addr,-is_cc68[reg9](REG68.sr));
}

DECL_LINE68(line53B)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[3](emu68,reg0);
  write_B(addr,-is_cc68[reg9](REG68.sr));
}

DECL_LINE68(line53C)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[4](emu68,reg0);
  write_B(addr,-is_cc68[reg9](REG68.sr));
}

DECL_LINE68(line53D)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[5](emu68,reg0);
  write_B(addr,-is_cc68[reg9](REG68.sr));
}

DECL_LINE68(line53E)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[6](emu68,reg0);
  write_B(addr,-is_cc68[reg9](REG68.sr));
}

DECL_LINE68(line53F)
{
  /* Scc <AE> */
  addr68_t addr;
  addr = get_eab68[7](emu68,reg0);
  write_B(addr,-is_cc68[reg9](REG68.sr));
}

