/* line3.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

DECL_LINE68(line300)
{
  /* MOVE.W Dn,Dn */
  int68_t a = (u16)REG68.d[reg0]/*<<WORD_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK)|a/*((uint68_t)a>>WORD_SHIFT)*/;
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line301)
{
  /* MOVE.W An,Dn */
  int68_t a = (u16)REG68.a[reg0]/*<<WORD_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK)|a/*((uint68_t)a>>WORD_SHIFT)*/;
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line302)
{
  /* MOVE.W <Ae>,Dn */
  addr68_t addr = get_eaw68[2](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK)|a/*((uint68_t)a>>WORD_SHIFT)*/;
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line303)
{
  /* MOVE.W <Ae>,Dn */
  addr68_t addr = get_eaw68[3](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK)|a/*((uint68_t)a>>WORD_SHIFT)*/;
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line304)
{
  /* MOVE.W <Ae>,Dn */
  addr68_t addr = get_eaw68[4](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK)|a/*((uint68_t)a>>WORD_SHIFT)*/;
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line305)
{
  /* MOVE.W <Ae>,Dn */
  addr68_t addr = get_eaw68[5](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK)|a/*((uint68_t)a>>WORD_SHIFT)*/;
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line306)
{
  /* MOVE.W <Ae>,Dn */
  addr68_t addr = get_eaw68[6](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK)|a/*((uint68_t)a>>WORD_SHIFT)*/;
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line307)
{
  /* MOVE.W <Ae>,Dn */
  addr68_t addr = get_eaw68[7](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&WORD_MASK)|a/*((uint68_t)a>>WORD_SHIFT)*/;
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line308)
{
  /* MOVE.W Dn,An */
  int68_t a = (u16)REG68.d[reg0]/*<<WORD_SHIFT*/;
  REG68.a[reg9] = (s16)a;
}

DECL_LINE68(line309)
{
  /* MOVE.W An,An */
  int68_t a = (u16)REG68.a[reg0]/*<<WORD_SHIFT*/;
  REG68.a[reg9] = (s16)a;
}

DECL_LINE68(line30A)
{
  /* MOVE.W <Ae>,An */
  addr68_t addr = get_eaw68[2](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.a[reg9] = (s16)a;
}

DECL_LINE68(line30B)
{
  /* MOVE.W <Ae>,An */
  addr68_t addr = get_eaw68[3](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.a[reg9] = (s16)a;
}

DECL_LINE68(line30C)
{
  /* MOVE.W <Ae>,An */
  addr68_t addr = get_eaw68[4](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.a[reg9] = (s16)a;
}

DECL_LINE68(line30D)
{
  /* MOVE.W <Ae>,An */
  addr68_t addr = get_eaw68[5](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.a[reg9] = (s16)a;
}

DECL_LINE68(line30E)
{
  /* MOVE.W <Ae>,An */
  addr68_t addr = get_eaw68[6](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.a[reg9] = (s16)a;
}

DECL_LINE68(line30F)
{
  /* MOVE.W <Ae>,An */
  addr68_t addr = get_eaw68[7](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  REG68.a[reg9] = (s16)a;
}

DECL_LINE68(line310)
{
  /* MOVE.W Dn,<Ae> */
  int68_t a = (u16)REG68.d[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[2](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line311)
{
  /* MOVE.W An,<Ae> */
  int68_t a = (u16)REG68.a[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[2](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line312)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[2](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[2](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line313)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[3](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[2](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line314)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[4](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[2](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line315)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[5](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[2](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line316)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[6](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[2](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line317)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[7](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[2](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line318)
{
  /* MOVE.W Dn,<Ae> */
  int68_t a = (u16)REG68.d[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[3](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line319)
{
  /* MOVE.W An,<Ae> */
  int68_t a = (u16)REG68.a[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[3](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line31A)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[2](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[3](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line31B)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[3](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[3](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line31C)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[4](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[3](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line31D)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[5](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[3](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line31E)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[6](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[3](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line31F)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[7](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[3](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line320)
{
  /* MOVE.W Dn,<Ae> */
  int68_t a = (u16)REG68.d[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[4](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line321)
{
  /* MOVE.W An,<Ae> */
  int68_t a = (u16)REG68.a[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[4](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line322)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[2](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[4](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line323)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[3](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[4](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line324)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[4](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[4](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line325)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[5](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[4](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line326)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[6](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[4](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line327)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[7](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[4](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line328)
{
  /* MOVE.W Dn,<Ae> */
  int68_t a = (u16)REG68.d[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[5](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line329)
{
  /* MOVE.W An,<Ae> */
  int68_t a = (u16)REG68.a[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[5](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line32A)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[2](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[5](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line32B)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[3](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[5](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line32C)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[4](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[5](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line32D)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[5](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[5](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line32E)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[6](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[5](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line32F)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[7](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[5](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line330)
{
  /* MOVE.W Dn,<Ae> */
  int68_t a = (u16)REG68.d[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[6](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line331)
{
  /* MOVE.W An,<Ae> */
  int68_t a = (u16)REG68.a[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[6](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line332)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[2](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[6](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line333)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[3](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[6](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line334)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[4](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[6](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line335)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[5](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[6](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line336)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[6](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[6](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line337)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[7](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[6](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line338)
{
  /* MOVE.W Dn,<Ae> */
  int68_t a = (u16)REG68.d[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[7](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line339)
{
  /* MOVE.W An,<Ae> */
  int68_t a = (u16)REG68.a[reg0]/*<<WORD_SHIFT*/;
  addr68_t addr = get_eaw68[7](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line33A)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[2](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[7](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line33B)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[3](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[7](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line33C)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[4](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[7](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line33D)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[5](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[7](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line33E)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[6](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[7](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

DECL_LINE68(line33F)
{
  /* MOVE.W <Ae>,<Ae> */
  addr68_t addr = get_eaw68[7](emu68,reg0);
  int68_t a = /*(u16)*/read_W(addr)/*<<WORD_SHIFT*/;
  addr = get_eaw68[7](emu68,reg9);
  write_W(addr,a);
  a <<= WORD_SHIFT;
  MOVE(a);
}

