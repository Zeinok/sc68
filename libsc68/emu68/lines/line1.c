/* line1.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

DECL_LINE68(line100)
{
  /* MOVE.B Dn,Dn */
  int68_t a = (u8)REG68.d[reg0]/*<<BYTE_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK)|a/*((uint68_t)a>>BYTE_SHIFT)*/;
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line101)
{
  /* MOVE.B An,Dn */
  int68_t a = (u8)REG68.a[reg0]/*<<BYTE_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK)|a/*((uint68_t)a>>BYTE_SHIFT)*/;
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line102)
{
  /* MOVE.B <Ae>,Dn */
  addr68_t addr = get_eab68[2](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK)|a/*((uint68_t)a>>BYTE_SHIFT)*/;
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line103)
{
  /* MOVE.B <Ae>,Dn */
  addr68_t addr = get_eab68[3](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK)|a/*((uint68_t)a>>BYTE_SHIFT)*/;
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line104)
{
  /* MOVE.B <Ae>,Dn */
  addr68_t addr = get_eab68[4](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK)|a/*((uint68_t)a>>BYTE_SHIFT)*/;
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line105)
{
  /* MOVE.B <Ae>,Dn */
  addr68_t addr = get_eab68[5](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK)|a/*((uint68_t)a>>BYTE_SHIFT)*/;
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line106)
{
  /* MOVE.B <Ae>,Dn */
  addr68_t addr = get_eab68[6](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK)|a/*((uint68_t)a>>BYTE_SHIFT)*/;
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line107)
{
  /* MOVE.B <Ae>,Dn */
  addr68_t addr = get_eab68[7](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.d[reg9] = (REG68.d[reg9]&BYTE_MASK)|a/*((uint68_t)a>>BYTE_SHIFT)*/;
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line108)
{
  /* MOVE.B Dn,An */
  int68_t a = (u8)REG68.d[reg0]/*<<BYTE_SHIFT*/;
  REG68.a[reg9] = (s8)a;
}

DECL_LINE68(line109)
{
  /* MOVE.B An,An */
  int68_t a = (u8)REG68.a[reg0]/*<<BYTE_SHIFT*/;
  REG68.a[reg9] = (s8)a;
}

DECL_LINE68(line10A)
{
  /* MOVE.B <Ae>,An */
  addr68_t addr = get_eab68[2](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.a[reg9] = (s8)a;
}

DECL_LINE68(line10B)
{
  /* MOVE.B <Ae>,An */
  addr68_t addr = get_eab68[3](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.a[reg9] = (s8)a;
}

DECL_LINE68(line10C)
{
  /* MOVE.B <Ae>,An */
  addr68_t addr = get_eab68[4](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.a[reg9] = (s8)a;
}

DECL_LINE68(line10D)
{
  /* MOVE.B <Ae>,An */
  addr68_t addr = get_eab68[5](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.a[reg9] = (s8)a;
}

DECL_LINE68(line10E)
{
  /* MOVE.B <Ae>,An */
  addr68_t addr = get_eab68[6](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.a[reg9] = (s8)a;
}

DECL_LINE68(line10F)
{
  /* MOVE.B <Ae>,An */
  addr68_t addr = get_eab68[7](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  REG68.a[reg9] = (s8)a;
}

DECL_LINE68(line110)
{
  /* MOVE.B Dn,<Ae> */
  int68_t a = (u8)REG68.d[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[2](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line111)
{
  /* MOVE.B An,<Ae> */
  int68_t a = (u8)REG68.a[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[2](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line112)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[2](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[2](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line113)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[3](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[2](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line114)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[4](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[2](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line115)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[5](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[2](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line116)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[6](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[2](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line117)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[7](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[2](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line118)
{
  /* MOVE.B Dn,<Ae> */
  int68_t a = (u8)REG68.d[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[3](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line119)
{
  /* MOVE.B An,<Ae> */
  int68_t a = (u8)REG68.a[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[3](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line11A)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[2](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[3](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line11B)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[3](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[3](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line11C)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[4](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[3](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line11D)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[5](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[3](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line11E)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[6](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[3](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line11F)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[7](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[3](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line120)
{
  /* MOVE.B Dn,<Ae> */
  int68_t a = (u8)REG68.d[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[4](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line121)
{
  /* MOVE.B An,<Ae> */
  int68_t a = (u8)REG68.a[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[4](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line122)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[2](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[4](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line123)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[3](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[4](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line124)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[4](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[4](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line125)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[5](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[4](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line126)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[6](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[4](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line127)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[7](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[4](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line128)
{
  /* MOVE.B Dn,<Ae> */
  int68_t a = (u8)REG68.d[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[5](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line129)
{
  /* MOVE.B An,<Ae> */
  int68_t a = (u8)REG68.a[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[5](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line12A)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[2](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[5](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line12B)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[3](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[5](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line12C)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[4](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[5](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line12D)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[5](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[5](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line12E)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[6](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[5](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line12F)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[7](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[5](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line130)
{
  /* MOVE.B Dn,<Ae> */
  int68_t a = (u8)REG68.d[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[6](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line131)
{
  /* MOVE.B An,<Ae> */
  int68_t a = (u8)REG68.a[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[6](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line132)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[2](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[6](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line133)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[3](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[6](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line134)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[4](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[6](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line135)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[5](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[6](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line136)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[6](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[6](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line137)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[7](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[6](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line138)
{
  /* MOVE.B Dn,<Ae> */
  int68_t a = (u8)REG68.d[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[7](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line139)
{
  /* MOVE.B An,<Ae> */
  int68_t a = (u8)REG68.a[reg0]/*<<BYTE_SHIFT*/;
  addr68_t addr = get_eab68[7](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line13A)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[2](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[7](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line13B)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[3](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[7](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line13C)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[4](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[7](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line13D)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[5](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[7](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line13E)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[6](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[7](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

DECL_LINE68(line13F)
{
  /* MOVE.B <Ae>,<Ae> */
  addr68_t addr = get_eab68[7](emu68,reg0);
  int68_t a = /*(u8)*/read_B(addr)/*<<BYTE_SHIFT*/;
  addr = get_eab68[7](emu68,reg9);
  write_B(addr,a);
  a <<= BYTE_SHIFT;
  MOVE(a);
}

