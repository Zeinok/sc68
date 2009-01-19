/* line2.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

DECL_LINE68(line200)
{
  /* MOVE.L Dn,Dn */
  int68_t a = (u32)REG68.d[reg0]/*<<LONG_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK)|a/*((uint68_t)a>>LONG_SHIFT)*/;
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line201)
{
  /* MOVE.L An,Dn */
  int68_t a = (u32)REG68.a[reg0]/*<<LONG_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK)|a/*((uint68_t)a>>LONG_SHIFT)*/;
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line202)
{
  /* MOVE.L <Ae>,Dn */
  addr68_t addr = get_eal68[2](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK)|a/*((uint68_t)a>>LONG_SHIFT)*/;
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line203)
{
  /* MOVE.L <Ae>,Dn */
  addr68_t addr = get_eal68[3](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK)|a/*((uint68_t)a>>LONG_SHIFT)*/;
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line204)
{
  /* MOVE.L <Ae>,Dn */
  addr68_t addr = get_eal68[4](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK)|a/*((uint68_t)a>>LONG_SHIFT)*/;
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line205)
{
  /* MOVE.L <Ae>,Dn */
  addr68_t addr = get_eal68[5](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK)|a/*((uint68_t)a>>LONG_SHIFT)*/;
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line206)
{
  /* MOVE.L <Ae>,Dn */
  addr68_t addr = get_eal68[6](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK)|a/*((uint68_t)a>>LONG_SHIFT)*/;
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line207)
{
  /* MOVE.L <Ae>,Dn */
  addr68_t addr = get_eal68[7](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&LONG_MASK)|a/*((uint68_t)a>>LONG_SHIFT)*/;
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line208)
{
  /* MOVE.L Dn,An */
  int68_t a = (u32)REG68.d[reg0]/*<<LONG_SHIFT*/;
  REG68.a[reg9] = (s32)a;
}

DECL_LINE68(line209)
{
  /* MOVE.L An,An */
  int68_t a = (u32)REG68.a[reg0]/*<<LONG_SHIFT*/;
  REG68.a[reg9] = (s32)a;
}

DECL_LINE68(line20A)
{
  /* MOVE.L <Ae>,An */
  addr68_t addr = get_eal68[2](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.a[reg9] = (s32)a;
}

DECL_LINE68(line20B)
{
  /* MOVE.L <Ae>,An */
  addr68_t addr = get_eal68[3](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.a[reg9] = (s32)a;
}

DECL_LINE68(line20C)
{
  /* MOVE.L <Ae>,An */
  addr68_t addr = get_eal68[4](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.a[reg9] = (s32)a;
}

DECL_LINE68(line20D)
{
  /* MOVE.L <Ae>,An */
  addr68_t addr = get_eal68[5](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.a[reg9] = (s32)a;
}

DECL_LINE68(line20E)
{
  /* MOVE.L <Ae>,An */
  addr68_t addr = get_eal68[6](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.a[reg9] = (s32)a;
}

DECL_LINE68(line20F)
{
  /* MOVE.L <Ae>,An */
  addr68_t addr = get_eal68[7](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  REG68.a[reg9] = (s32)a;
}

DECL_LINE68(line210)
{
  /* MOVE.L Dn,<Ae> */
  int68_t a = (u32)REG68.d[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[2](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line211)
{
  /* MOVE.L An,<Ae> */
  int68_t a = (u32)REG68.a[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[2](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line212)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[2](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[2](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line213)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[3](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[2](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line214)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[4](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[2](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line215)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[5](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[2](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line216)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[6](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[2](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line217)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[7](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[2](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line218)
{
  /* MOVE.L Dn,<Ae> */
  int68_t a = (u32)REG68.d[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[3](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line219)
{
  /* MOVE.L An,<Ae> */
  int68_t a = (u32)REG68.a[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[3](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line21A)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[2](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[3](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line21B)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[3](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[3](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line21C)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[4](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[3](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line21D)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[5](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[3](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line21E)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[6](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[3](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line21F)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[7](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[3](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line220)
{
  /* MOVE.L Dn,<Ae> */
  int68_t a = (u32)REG68.d[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[4](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line221)
{
  /* MOVE.L An,<Ae> */
  int68_t a = (u32)REG68.a[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[4](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line222)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[2](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[4](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line223)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[3](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[4](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line224)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[4](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[4](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line225)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[5](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[4](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line226)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[6](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[4](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line227)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[7](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[4](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line228)
{
  /* MOVE.L Dn,<Ae> */
  int68_t a = (u32)REG68.d[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[5](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line229)
{
  /* MOVE.L An,<Ae> */
  int68_t a = (u32)REG68.a[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[5](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line22A)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[2](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[5](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line22B)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[3](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[5](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line22C)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[4](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[5](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line22D)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[5](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[5](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line22E)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[6](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[5](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line22F)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[7](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[5](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line230)
{
  /* MOVE.L Dn,<Ae> */
  int68_t a = (u32)REG68.d[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[6](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line231)
{
  /* MOVE.L An,<Ae> */
  int68_t a = (u32)REG68.a[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[6](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line232)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[2](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[6](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line233)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[3](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[6](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line234)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[4](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[6](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line235)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[5](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[6](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line236)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[6](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[6](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line237)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[7](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[6](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line238)
{
  /* MOVE.L Dn,<Ae> */
  int68_t a = (u32)REG68.d[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[7](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line239)
{
  /* MOVE.L An,<Ae> */
  int68_t a = (u32)REG68.a[reg0]/*<<LONG_SHIFT*/;
  addr68_t addr = get_eal68[7](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line23A)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[2](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[7](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line23B)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[3](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[7](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line23C)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[4](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[7](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line23D)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[5](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[7](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line23E)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[6](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[7](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

DECL_LINE68(line23F)
{
  /* MOVE.L <Ae>,<Ae> */
  addr68_t addr = get_eal68[7](emu68,reg0);
  int68_t a = /*(u32)*/read_L(addr)/*<<LONG_SHIFT*/;
  addr = get_eal68[7](emu68,reg9);
  write_L(addr,a);
  a <<= LONG_SHIFT;
  MOVE(a);
}

