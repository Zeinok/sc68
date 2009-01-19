/* lineE.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

static void ASR_mem(emu68_t * const emu68, int reg, int mode)
{
  sint68_t a;
  const addr68_t addr = get_eaw68[mode](emu68,reg);
  int dec=1;
  a = read_W(addr)<<WORD_SHIFT;
  ASRW(a,dec);
  write_W(addr,a>>WORD_SHIFT);
}

static void LSR_mem(emu68_t * const emu68, int reg, int mode)
{
  uint68_t a;
  const addr68_t addr = get_eaw68[mode](emu68,reg);
  int dec=1;
  a = read_W(addr)<<WORD_SHIFT;
  LSRW(a,dec);
  write_W(addr,a>>WORD_SHIFT);
}

static void ROXR_mem(emu68_t * const emu68, int reg, int mode)
{
  uint68_t a;
  const addr68_t addr = get_eaw68[mode](emu68,reg);
  int dec=1;
  a = read_W(addr)<<WORD_SHIFT;
  ROXRW(a,dec);
  write_W(addr,a>>WORD_SHIFT);
}

static void ROR_mem(emu68_t * const emu68, int reg, int mode)
{
  uint68_t a;
  const addr68_t addr = get_eaw68[mode](emu68,reg);
  int dec=1;
  a = read_W(addr)<<WORD_SHIFT;
  RORW(a,dec);
  write_W(addr,a>>WORD_SHIFT);
}

static void ASL_mem(emu68_t * const emu68, int reg, int mode)
{
  sint68_t a;
  const addr68_t addr = get_eaw68[mode](emu68,reg);
  int dec=1;
  a = read_W(addr)<<WORD_SHIFT;
  ASLW(a,dec);
  write_W(addr,a>>WORD_SHIFT);
}

static void LSL_mem(emu68_t * const emu68, int reg, int mode)
{
  uint68_t a;
  const addr68_t addr = get_eaw68[mode](emu68,reg);
  int dec=1;
  a = read_W(addr)<<WORD_SHIFT;
  LSLW(a,dec);
  write_W(addr,a>>WORD_SHIFT);
}

static void ROXL_mem(emu68_t * const emu68, int reg, int mode)
{
  uint68_t a;
  const addr68_t addr = get_eaw68[mode](emu68,reg);
  int dec=1;
  a = read_W(addr)<<WORD_SHIFT;
  ROXLW(a,dec);
  write_W(addr,a>>WORD_SHIFT);
}

static void ROL_mem(emu68_t * const emu68, int reg, int mode)
{
  uint68_t a;
  const addr68_t addr = get_eaw68[mode](emu68,reg);
  int dec=1;
  a = read_W(addr)<<WORD_SHIFT;
  ROLW(a,dec);
  write_W(addr,a>>WORD_SHIFT);
}

static void (*const lslmemR_fc[4])(emu68_t *const,int,int)=
{
  ASR_mem,  LSR_mem,  ROXR_mem,  ROR_mem,
};

static void (*const lslmemL_fc[4])(emu68_t *const,int,int)=
{
  ASL_mem,  LSL_mem,  ROXL_mem,  ROL_mem,
};

DECL_LINE68(lineE00)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = ((dec-1)&7)+1;
  ASRB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE01)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = ((dec-1)&7)+1;
  LSRB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE02)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = ((dec-1)&7)+1;
  ROXRB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE03)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = ((dec-1)&7)+1;
  RORB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE04)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = REG68.d[dec]&63;
  ASRB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE05)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = REG68.d[dec]&63;
  LSRB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE06)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = REG68.d[dec]&63;
  ROXRB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE07)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = REG68.d[dec]&63;
  RORB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE08)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = ((dec-1)&7)+1;
  ASRW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE09)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = ((dec-1)&7)+1;
  LSRW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE0A)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = ((dec-1)&7)+1;
  ROXRW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE0B)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = ((dec-1)&7)+1;
  RORW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE0C)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = REG68.d[dec]&63;
  ASRW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE0D)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = REG68.d[dec]&63;
  LSRW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE0E)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = REG68.d[dec]&63;
  ROXRW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE0F)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = REG68.d[dec]&63;
  RORW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE10)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = ((dec-1)&7)+1;
  ASRL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE11)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = ((dec-1)&7)+1;
  LSRL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE12)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = ((dec-1)&7)+1;
  ROXRL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE13)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = ((dec-1)&7)+1;
  RORL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE14)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = REG68.d[dec]&63;
  ASRL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE15)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = REG68.d[dec]&63;
  LSRL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE16)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = REG68.d[dec]&63;
  ROXRL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE17)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = REG68.d[dec]&63;
  RORL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE18)
{
  lslmemR_fc[reg9&3](emu68,reg0,0);
}

DECL_LINE68(lineE19)
{
  lslmemR_fc[reg9&3](emu68,reg0,1);
}

DECL_LINE68(lineE1A)
{
  lslmemR_fc[reg9&3](emu68,reg0,2);
}

DECL_LINE68(lineE1B)
{
  lslmemR_fc[reg9&3](emu68,reg0,3);
}

DECL_LINE68(lineE1C)
{
  lslmemR_fc[reg9&3](emu68,reg0,4);
}

DECL_LINE68(lineE1D)
{
  lslmemR_fc[reg9&3](emu68,reg0,5);
}

DECL_LINE68(lineE1E)
{
  lslmemR_fc[reg9&3](emu68,reg0,6);
}

DECL_LINE68(lineE1F)
{
  lslmemR_fc[reg9&3](emu68,reg0,7);
}

DECL_LINE68(lineE20)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = ((dec-1)&7)+1;
  ASLB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE21)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = ((dec-1)&7)+1;
  LSLB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE22)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = ((dec-1)&7)+1;
  ROXLB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE23)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = ((dec-1)&7)+1;
  ROLB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE24)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = REG68.d[dec]&63;
  ASLB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE25)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = REG68.d[dec]&63;
  LSLB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE26)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = REG68.d[dec]&63;
  ROXLB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE27)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<BYTE_SHIFT;
  d = REG68.d[dec]&63;
  ROLB(a,d);
  REG68.d[reg] = (REG68.d[reg]&BYTE_MASK) + ((uint68_t)a>>BYTE_SHIFT);
}

DECL_LINE68(lineE28)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = ((dec-1)&7)+1;
  ASLW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE29)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = ((dec-1)&7)+1;
  LSLW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE2A)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = ((dec-1)&7)+1;
  ROXLW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE2B)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = ((dec-1)&7)+1;
  ROLW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE2C)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = REG68.d[dec]&63;
  ASLW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE2D)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = REG68.d[dec]&63;
  LSLW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE2E)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = REG68.d[dec]&63;
  ROXLW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE2F)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<WORD_SHIFT;
  d = REG68.d[dec]&63;
  ROLW(a,d);
  REG68.d[reg] = (REG68.d[reg]&WORD_MASK) + ((uint68_t)a>>WORD_SHIFT);
}

DECL_LINE68(lineE30)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = ((dec-1)&7)+1;
  ASLL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE31)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = ((dec-1)&7)+1;
  LSLL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE32)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = ((dec-1)&7)+1;
  ROXLL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE33)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = ((dec-1)&7)+1;
  ROLL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE34)
{
  const int dec = reg9, reg = reg0;
  sint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = REG68.d[dec]&63;
  ASLL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE35)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = REG68.d[dec]&63;
  LSLL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE36)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = REG68.d[dec]&63;
  ROXLL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE37)
{
  const int dec = reg9, reg = reg0;
  uint68_t a;
  uint68_t d;
  a=REG68.d[reg]<<LONG_SHIFT;
  d = REG68.d[dec]&63;
  ROLL(a,d);
  REG68.d[reg] = (REG68.d[reg]&LONG_MASK) + ((uint68_t)a>>LONG_SHIFT);
}

DECL_LINE68(lineE38)
{
  lslmemL_fc[reg9&3](emu68,reg0,0);
}

DECL_LINE68(lineE39)
{
  lslmemL_fc[reg9&3](emu68,reg0,1);
}

DECL_LINE68(lineE3A)
{
  lslmemL_fc[reg9&3](emu68,reg0,2);
}

DECL_LINE68(lineE3B)
{
  lslmemL_fc[reg9&3](emu68,reg0,3);
}

DECL_LINE68(lineE3C)
{
  lslmemL_fc[reg9&3](emu68,reg0,4);
}

DECL_LINE68(lineE3D)
{
  lslmemL_fc[reg9&3](emu68,reg0,5);
}

DECL_LINE68(lineE3E)
{
  lslmemL_fc[reg9&3](emu68,reg0,6);
}

DECL_LINE68(lineE3F)
{
  lslmemL_fc[reg9&3](emu68,reg0,7);
}

