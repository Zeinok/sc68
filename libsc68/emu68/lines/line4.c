/* line4.c : EMU68 generated code by
 * gen68 Wed May  2 21:50:31 CEST 2007
 * (C) 1998-2007 Benjamin Gerard
 *
 * $Id$
 */

static void funky4_m6_0(emu68_t * const emu68)
{
  RESET;
}

static void funky4_m6_1(emu68_t * const emu68)
{
  NOP;
}

static void funky4_m6_2(emu68_t * const emu68)
{
  STOP;
}

static void funky4_m6_3(emu68_t * const emu68)
{
  RTE;
}

static void funky4_m6_4(emu68_t * const emu68)
{
  ILLEGAL;
}

static void funky4_m6_5(emu68_t * const emu68)
{
  RTS;
}

static void funky4_m6_6(emu68_t * const emu68)
{
  TRAPV;
}

static void funky4_m6_7(emu68_t * const emu68)
{
  RTR;
}


static void (*const funky4_m6_func[8])(emu68_t * const) =
{ funky4_m6_0,funky4_m6_1,funky4_m6_2,funky4_m6_3,funky4_m6_4,funky4_m6_5,funky4_m6_6,funky4_m6_7, };

static void funky4_m0(emu68_t * const emu68, int reg0)
{
  int68_t a = (0<<3)+reg0;
  TRAP(a);
}

static void funky4_m1(emu68_t * const emu68, int reg0)
{
  int68_t a = (1<<3)+reg0;
  TRAP(a);
}

static void funky4_m2(emu68_t * const emu68, int reg0)
{
  LINK(reg0);
}

static void funky4_m3(emu68_t * const emu68, int reg0)
{
  UNLK(reg0);
}

static void funky4_m4(emu68_t * const emu68, int reg0)
{
  REG68.usp = REG68.a[reg0];
}

static void funky4_m5(emu68_t * const emu68, int reg0)
{
  REG68.a[reg0] = REG68.usp;
}

static void funky4_m6(emu68_t * const emu68, int reg0)
{
  funky4_m6_func[reg0](emu68);
}

static void funky4_m7(emu68_t * const emu68, int reg0)
{
  ILLEGAL;
}

static void (* const funky4_func[8])(emu68_t * const, int) =
{ funky4_m0,funky4_m1,funky4_m2,funky4_m3,funky4_m4,funky4_m5,funky4_m6,funky4_m7,
};

static void movemregw(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t m = (u16)get_nextw(), addr;
  int68_t *r = REG68.d;
  addr = get_eaw68[mode](emu68,reg0);
  for(; m; r++, m>>=1)
    if (m&1){ *r=(int68_t)read_W(addr); addr+=2; }
  if (mode==3) REG68.a[reg0] = addr;
}

static void movemregl(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t m = (u16)get_nextw(), addr;
  int68_t *r = REG68.d;
  addr = get_eal68[mode](emu68,reg0);
  for(; m; r++, m>>=1)
    if (m&1){ *r=(int68_t)read_L(addr); addr+=4; }
  if (mode==3) REG68.a[reg0] = addr;
}

static void movemmemw(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t m = (u16)get_nextw(), addr;
  if (mode==4) {
    int68_t *r = REG68.a+7;
    addr = get_eaw68[3](emu68,reg0);
    for(; m; r--, m>>=1)
      if (m&1) write_W(addr-=2,*r);
    REG68.a[reg0] = addr;
  } else {
    int68_t *r = REG68.d;
    addr = get_eaw68[mode](emu68,reg0);
    for(; m; r++, m>>=1)
      if (m&1) { write_W(addr,*r); addr+=2; }
  }
}

static void movemmeml(emu68_t * const emu68, int mode, int reg0)
{
  uint68_t m = (u16)get_nextw(), addr;
  if (mode==4) {
    int68_t *r = REG68.a+7;
    addr = get_eal68[3](emu68,reg0);
    for(; m; r--, m>>=1)
      if (m&1) write_L(addr-=4,*r);
    REG68.a[reg0] = addr;
  } else {
    int68_t *r = REG68.d;
    addr = get_eal68[mode](emu68,reg0);
    for(; m; r++, m>>=1)
      if (m&1) { write_L(addr,*r); addr+=4; }
  }
}

static void line4_r0_s0(emu68_t * const emu68, int mode, int reg0)
{
  /* NEGX.B <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<BYTE_SHIFT;
    NEGXB(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) | (uint68_t)s>>BYTE_SHIFT;
  }
  else
  {
    addr68_t addr=get_eab68[mode](emu68,reg0);
    int68_t s,a;
    a=read_B(addr)<<BYTE_SHIFT;
    NEGXB(s,a);
    write_B(addr,(uint68_t)s>>BYTE_SHIFT);
  }
}

static void line4_r0_s1(emu68_t * const emu68, int mode, int reg0)
{
  /* NEGX.W <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<WORD_SHIFT;
    NEGXW(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) | (uint68_t)s>>WORD_SHIFT;
  }
  else
  {
    addr68_t addr=get_eaw68[mode](emu68,reg0);
    int68_t s,a;
    a=read_W(addr)<<WORD_SHIFT;
    NEGXW(s,a);
    write_W(addr,(uint68_t)s>>WORD_SHIFT);
  }
}

static void line4_r0_s2(emu68_t * const emu68, int mode, int reg0)
{
  /* NEGX.L <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<LONG_SHIFT;
    NEGXL(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) | (uint68_t)s>>LONG_SHIFT;
  }
  else
  {
    addr68_t addr=get_eal68[mode](emu68,reg0);
    int68_t s,a;
    a=read_L(addr)<<LONG_SHIFT;
    NEGXL(s,a);
    write_L(addr,(uint68_t)s>>LONG_SHIFT);
  }
}

static void line4_r0_s3(emu68_t * const emu68, int mode, int reg0)
{
  /* MOVE FROM SR */
  if (mode)
    write_W(get_eaw68[mode](emu68,reg0),REG68.sr);
  else
    REG68.d[reg0]=(REG68.d[reg0]&0xFFFF0000)+(u16)REG68.sr;
}

static void line4_r1_s0(emu68_t * const emu68, int mode, int reg0)
{
  /* CLR.B <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=0;
    CLRB(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) | (uint68_t)s>>BYTE_SHIFT;
  }
  else
  {
    addr68_t addr=get_eab68[mode](emu68,reg0);
    int68_t s,a;
    a=0;
    CLRB(s,a);
    write_B(addr,(uint68_t)s>>BYTE_SHIFT);
  }
}

static void line4_r1_s1(emu68_t * const emu68, int mode, int reg0)
{
  /* CLR.W <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=0;
    CLRW(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) | (uint68_t)s>>WORD_SHIFT;
  }
  else
  {
    addr68_t addr=get_eaw68[mode](emu68,reg0);
    int68_t s,a;
    a=0;
    CLRW(s,a);
    write_W(addr,(uint68_t)s>>WORD_SHIFT);
  }
}

static void line4_r1_s2(emu68_t * const emu68, int mode, int reg0)
{
  /* CLR.L <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=0;
    CLRL(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) | (uint68_t)s>>LONG_SHIFT;
  }
  else
  {
    addr68_t addr=get_eal68[mode](emu68,reg0);
    int68_t s,a;
    a=0;
    CLRL(s,a);
    write_L(addr,(uint68_t)s>>LONG_SHIFT);
  }
}

static void line4_r1_s3(emu68_t * const emu68, int mode, int reg0)
{
  ILLEGAL;
}

static void line4_r2_s0(emu68_t * const emu68, int mode, int reg0)
{
  /* NEG.B <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<BYTE_SHIFT;
    NEGB(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) | (uint68_t)s>>BYTE_SHIFT;
  }
  else
  {
    addr68_t addr=get_eab68[mode](emu68,reg0);
    int68_t s,a;
    a=read_B(addr)<<BYTE_SHIFT;
    NEGB(s,a);
    write_B(addr,(uint68_t)s>>BYTE_SHIFT);
  }
}

static void line4_r2_s1(emu68_t * const emu68, int mode, int reg0)
{
  /* NEG.W <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<WORD_SHIFT;
    NEGW(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) | (uint68_t)s>>WORD_SHIFT;
  }
  else
  {
    addr68_t addr=get_eaw68[mode](emu68,reg0);
    int68_t s,a;
    a=read_W(addr)<<WORD_SHIFT;
    NEGW(s,a);
    write_W(addr,(uint68_t)s>>WORD_SHIFT);
  }
}

static void line4_r2_s2(emu68_t * const emu68, int mode, int reg0)
{
  /* NEG.L <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<LONG_SHIFT;
    NEGL(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) | (uint68_t)s>>LONG_SHIFT;
  }
  else
  {
    addr68_t addr=get_eal68[mode](emu68,reg0);
    int68_t s,a;
    a=read_L(addr)<<LONG_SHIFT;
    NEGL(s,a);
    write_L(addr,(uint68_t)s>>LONG_SHIFT);
  }
}

static void line4_r2_s3(emu68_t * const emu68, int mode, int reg0)
{
  /* MOVE TO CCR */
  if (mode)
    SET_CCR(REG68.sr,read_W(get_eaw68[mode](emu68,reg0)));
  else
    SET_CCR(REG68.sr,REG68.d[reg0]);
}

static void line4_r3_s0(emu68_t * const emu68, int mode, int reg0)
{
  /* NOT.B <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<BYTE_SHIFT;
    NOTB(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) | (uint68_t)s>>BYTE_SHIFT;
  }
  else
  {
    addr68_t addr=get_eab68[mode](emu68,reg0);
    int68_t s,a;
    a=read_B(addr)<<BYTE_SHIFT;
    NOTB(s,a);
    write_B(addr,(uint68_t)s>>BYTE_SHIFT);
  }
}

static void line4_r3_s1(emu68_t * const emu68, int mode, int reg0)
{
  /* NOT.W <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<WORD_SHIFT;
    NOTW(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&WORD_MASK) | (uint68_t)s>>WORD_SHIFT;
  }
  else
  {
    addr68_t addr=get_eaw68[mode](emu68,reg0);
    int68_t s,a;
    a=read_W(addr)<<WORD_SHIFT;
    NOTW(s,a);
    write_W(addr,(uint68_t)s>>WORD_SHIFT);
  }
}

static void line4_r3_s2(emu68_t * const emu68, int mode, int reg0)
{
  /* NOT.L <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<LONG_SHIFT;
    NOTL(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&LONG_MASK) | (uint68_t)s>>LONG_SHIFT;
  }
  else
  {
    addr68_t addr=get_eal68[mode](emu68,reg0);
    int68_t s,a;
    a=read_L(addr)<<LONG_SHIFT;
    NOTL(s,a);
    write_L(addr,(uint68_t)s>>LONG_SHIFT);
  }
}

static void line4_r3_s3(emu68_t * const emu68, int mode, int reg0)
{
  /* MOVE TO SR */
  if (mode)
    REG68.sr = read_W(get_eaw68[mode](emu68,reg0));
  else
    REG68.sr = (u16)REG68.d[reg0];
}

static void line4_r4_s0(emu68_t * const emu68, int mode, int reg0)
{
  /* NBCD.B <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<BYTE_SHIFT;
    NBCDB(s,a);
    REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) | (uint68_t)s>>BYTE_SHIFT;
  }
  else
  {
    addr68_t addr=get_eab68[mode](emu68,reg0);
    int68_t s,a;
    a=read_B(addr)<<BYTE_SHIFT;
    NBCDB(s,a);
    write_B(addr,(uint68_t)s>>BYTE_SHIFT);
  }
}

static void line4_r4_s1(emu68_t * const emu68, int mode, int reg0)
{
  /* SWP or PEA */
  if (!mode){ SWAP(REG68.d[reg0]); }
  else pushl(get_eal68[mode](emu68,reg0));
}

static void line4_r4_s2(emu68_t * const emu68, int mode, int reg0)
{
  if (!mode){ EXTW(REG68.d[reg0]); }
  else movemmemw(emu68,mode,reg0);
}

static void line4_r4_s3(emu68_t * const emu68, int mode, int reg0)
{
  if (!mode){ EXTL(REG68.d[reg0]); }
  else movemmeml(emu68,mode,reg0);
}

static void line4_r5_s0(emu68_t * const emu68, int mode, int reg0)
{
  /* TST.B <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<BYTE_SHIFT;
    TSTB(s,a);
  }
  else
  {
    addr68_t addr=get_eab68[mode](emu68,reg0);
    int68_t s,a;
    a=read_B(addr)<<BYTE_SHIFT;
    TSTB(s,a);
  }
}

static void line4_r5_s1(emu68_t * const emu68, int mode, int reg0)
{
  /* TST.W <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<WORD_SHIFT;
    TSTW(s,a);
  }
  else
  {
    addr68_t addr=get_eaw68[mode](emu68,reg0);
    int68_t s,a;
    a=read_W(addr)<<WORD_SHIFT;
    TSTW(s,a);
  }
}

static void line4_r5_s2(emu68_t * const emu68, int mode, int reg0)
{
  /* TST.L <Ae> */
  if (mode<2)
  {
    int68_t s;
    int68_t a=REG68.d[reg0]<<LONG_SHIFT;
    TSTL(s,a);
  }
  else
  {
    addr68_t addr=get_eal68[mode](emu68,reg0);
    int68_t s,a;
    a=read_L(addr)<<LONG_SHIFT;
    TSTL(s,a);
  }
}

static void line4_r5_s3(emu68_t * const emu68, int mode, int reg0)
{
  if (mode<2) {
    int68_t a = REG68.d[reg0]<<BYTE_SHIFT;
    TAS(a);
    REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) | ((uint68_t)a>>BYTE_SHIFT);
  } else {
    int68_t a;
    if (mode==7 && reg0>1) {
      ILLEGAL;
    } else {
      const addr68_t addr = get_eab68[mode](emu68,reg0);
      a = read_B(addr)<<BYTE_SHIFT;
      TAS(a);
      write_B(addr,(uint68_t)a>>BYTE_SHIFT);
    }
  }
}

static void line4_r6_s0(emu68_t * const emu68, int mode, int reg0)
{
  ILLEGAL;
}

static void line4_r6_s1(emu68_t * const emu68, int mode, int reg0)
{
  ILLEGAL;
}

static void line4_r6_s2(emu68_t * const emu68, int mode, int reg0)
{
  movemregw(emu68,mode,reg0);
}

static void line4_r6_s3(emu68_t * const emu68, int mode, int reg0)
{
  movemregl(emu68,mode,reg0);
}

static void line4_r7_s0(emu68_t * const emu68, int mode, int reg0)
{
  ILLEGAL;
}

static void line4_r7_s1(emu68_t * const emu68, int mode, int reg0)
{
  funky4_func[mode](emu68,reg0);
}

static void line4_r7_s2(emu68_t * const emu68, int mode, int reg0)
{
  /* JSR <Ae> */
  const addr68_t addr = get_eal68[mode](emu68,reg0);
  pushl(REG68.pc);
  REG68.pc = addr;
  ADDCYCLE(4);
}

static void line4_r7_s3(emu68_t * const emu68, int mode, int reg0)
{
  /* JMP <Ae> */
  const addr68_t addr = get_eal68[mode](emu68,reg0);
  REG68.pc = addr;
  ADDCYCLE(4);
}

static DECL_LINE68((* const line4_0_func[8])) =
{
  line4_r0_s0,line4_r1_s0,line4_r2_s0,line4_r3_s0,line4_r4_s0,line4_r5_s0,line4_r6_s0,line4_r7_s0,
};

static DECL_LINE68((* const line4_1_func[8])) =
{
  line4_r0_s1,line4_r1_s1,line4_r2_s1,line4_r3_s1,line4_r4_s1,line4_r5_s1,line4_r6_s1,line4_r7_s1,
};

static DECL_LINE68((* const line4_2_func[8])) =
{
  line4_r0_s2,line4_r1_s2,line4_r2_s2,line4_r3_s2,line4_r4_s2,line4_r5_s2,line4_r6_s2,line4_r7_s2,
};

static DECL_LINE68((* const line4_3_func[8])) =
{
  line4_r0_s3,line4_r1_s3,line4_r2_s3,line4_r3_s3,line4_r4_s3,line4_r5_s3,line4_r6_s3,line4_r7_s3,
};

DECL_LINE68(line400)
{
  line4_0_func[reg9](emu68,0,reg0);
}

DECL_LINE68(line401)
{
  line4_0_func[reg9](emu68,1,reg0);
}

DECL_LINE68(line402)
{
  line4_0_func[reg9](emu68,2,reg0);
}

DECL_LINE68(line403)
{
  line4_0_func[reg9](emu68,3,reg0);
}

DECL_LINE68(line404)
{
  line4_0_func[reg9](emu68,4,reg0);
}

DECL_LINE68(line405)
{
  line4_0_func[reg9](emu68,5,reg0);
}

DECL_LINE68(line406)
{
  line4_0_func[reg9](emu68,6,reg0);
}

DECL_LINE68(line407)
{
  line4_0_func[reg9](emu68,7,reg0);
}

DECL_LINE68(line408)
{
  line4_1_func[reg9](emu68,0,reg0);
}

DECL_LINE68(line409)
{
  line4_1_func[reg9](emu68,1,reg0);
}

DECL_LINE68(line40A)
{
  line4_1_func[reg9](emu68,2,reg0);
}

DECL_LINE68(line40B)
{
  line4_1_func[reg9](emu68,3,reg0);
}

DECL_LINE68(line40C)
{
  line4_1_func[reg9](emu68,4,reg0);
}

DECL_LINE68(line40D)
{
  line4_1_func[reg9](emu68,5,reg0);
}

DECL_LINE68(line40E)
{
  line4_1_func[reg9](emu68,6,reg0);
}

DECL_LINE68(line40F)
{
  line4_1_func[reg9](emu68,7,reg0);
}

DECL_LINE68(line410)
{
  line4_2_func[reg9](emu68,0,reg0);
}

DECL_LINE68(line411)
{
  line4_2_func[reg9](emu68,1,reg0);
}

DECL_LINE68(line412)
{
  line4_2_func[reg9](emu68,2,reg0);
}

DECL_LINE68(line413)
{
  line4_2_func[reg9](emu68,3,reg0);
}

DECL_LINE68(line414)
{
  line4_2_func[reg9](emu68,4,reg0);
}

DECL_LINE68(line415)
{
  line4_2_func[reg9](emu68,5,reg0);
}

DECL_LINE68(line416)
{
  line4_2_func[reg9](emu68,6,reg0);
}

DECL_LINE68(line417)
{
  line4_2_func[reg9](emu68,7,reg0);
}

DECL_LINE68(line418)
{
  line4_3_func[reg9](emu68,0,reg0);
}

DECL_LINE68(line419)
{
  line4_3_func[reg9](emu68,1,reg0);
}

DECL_LINE68(line41A)
{
  line4_3_func[reg9](emu68,2,reg0);
}

DECL_LINE68(line41B)
{
  line4_3_func[reg9](emu68,3,reg0);
}

DECL_LINE68(line41C)
{
  line4_3_func[reg9](emu68,4,reg0);
}

DECL_LINE68(line41D)
{
  line4_3_func[reg9](emu68,5,reg0);
}

DECL_LINE68(line41E)
{
  line4_3_func[reg9](emu68,6,reg0);
}

DECL_LINE68(line41F)
{
  line4_3_func[reg9](emu68,7,reg0);
}

DECL_LINE68(line420)
{
  ILLEGAL;
}

DECL_LINE68(line421)
{
  ILLEGAL;
}

DECL_LINE68(line422)
{
  ILLEGAL;
}

DECL_LINE68(line423)
{
  ILLEGAL;
}

DECL_LINE68(line424)
{
  ILLEGAL;
}

DECL_LINE68(line425)
{
  ILLEGAL;
}

DECL_LINE68(line426)
{
  ILLEGAL;
}

DECL_LINE68(line427)
{
  ILLEGAL;
}

DECL_LINE68(line428)
{
  ILLEGAL;
}

DECL_LINE68(line429)
{
  ILLEGAL;
}

DECL_LINE68(line42A)
{
  ILLEGAL;
}

DECL_LINE68(line42B)
{
  ILLEGAL;
}

DECL_LINE68(line42C)
{
  ILLEGAL;
}

DECL_LINE68(line42D)
{
  ILLEGAL;
}

DECL_LINE68(line42E)
{
  ILLEGAL;
}

DECL_LINE68(line42F)
{
  ILLEGAL;
}

DECL_LINE68(line430)
{
  /* CHK.W Dx,Dy */
  int68_t a,b;
  a = REG68.d[reg0]<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CHKW(a,b);
}

DECL_LINE68(line431)
{
  /* CHK <AE>,Dn */;
  ILLEGAL;
}

DECL_LINE68(line432)
{
  /* CHK.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[2](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CHKW(a,b);
}

DECL_LINE68(line433)
{
  /* CHK.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[3](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CHKW(a,b);
}

DECL_LINE68(line434)
{
  /* CHK.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[4](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CHKW(a,b);
}

DECL_LINE68(line435)
{
  /* CHK.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[5](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CHKW(a,b);
}

DECL_LINE68(line436)
{
  /* CHK.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[6](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CHKW(a,b);
}

DECL_LINE68(line437)
{
  /* CHK.W <Ae>,Dn */
  int68_t a,b;
  addr68_t addr;
  addr = get_eaw68[7](emu68,reg0);
  a = read_W(addr)<<WORD_SHIFT;
  b = REG68.d[reg9]<<WORD_SHIFT;
  CHKW(a,b);
}

DECL_LINE68(line438)
{
  /* LEA <AE>,an */
  ILLEGAL;
}

DECL_LINE68(line439)
{
  /* LEA <AE>,an */
  ILLEGAL;
}

DECL_LINE68(line43A)
{
  /* LEA <AE>,an */
  REG68.a[reg9] = REG68.a[reg0];
}

DECL_LINE68(line43B)
{
  /* LEA <AE>,an */
  ILLEGAL;
}

DECL_LINE68(line43C)
{
  /* LEA <AE>,an */
  ILLEGAL;
}

DECL_LINE68(line43D)
{
  /* LEA <AE>,an */
  REG68.a[reg9] = REG68.a[reg0] + get_nextw();
}

DECL_LINE68(line43E)
{
  /* LEA <AE>,an */
  REG68.a[reg9] = (get_eal68[6])(emu68,reg0);
}

DECL_LINE68(line43F)
{
  /* LEA <AE>,an */
  REG68.a[reg9] = (get_eal68[7])(emu68,reg0);
}

