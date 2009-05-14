/*
 *          gen68 - "emu68" 68k instruction code generator
 *            Copyright (C) 1999-2009 Ben(jamin) Gerard
 *           <benjihan -4t- users.sourceforge -d0t- net>
 *
 * This  program is  free  software: you  can  redistribute it  and/or
 * modify  it under the  terms of  the GNU  General Public  License as
 * published by the Free Software  Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 *
 * You should have  received a copy of the  GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* becoz puts add a '\n' */
#define putsss printf

static int error(const char * format, ...)
{
  va_list list;
  va_start(list,format);
  vfprintf(stderr,format,list);
  va_end(list);
  return -1;
}

static int quiet = 0;
static void msg(const char * format, ...)
{
  if (!quiet) {
    va_list list;
    va_start(list,format);
    vfprintf(stderr,format,list);
    va_end(list);
  }
}

#define TAB   "  "
#define TAB2  "    "
#define TAB33 "      "

static const char modifdate[] = "Wed May  2 21:50:31 CEST 2007";

static int Usage(void)
{
  putsss("Usage: gen68 (T|[0-F])*|all [<output-dir>]\n"
         "\n"
         " 'C' code generator for sc68 project.\n"
         "\n"
         " T     Generate function table\n"
         " 0-F   Generate code for given lines\n"
         " all   Generate all (equiv. 0123456789ABCDEFT)\n"
         "\n"
         "Copyright (C) 1999-2009 Benjamin Gerard\n"
         "\n");
  return 1;
}

/* Convert ID (0 .. 1024) -> id-string
 * $$$ static string inside, use one at a time
 */
static char *to_line_id(int i)
{
  int line,num;
  static  char s[8];
  line = i>>6;
  switch(line)
  {
  case 0xa: case 0xf:
    num=0;
    break;
  default:
    num = i&63;
  }
  sprintf(s,"%X%02X",line,num);
  return s;
}

static void print_fileheader(const char * name)
{
  printf("/* %s : EMU68 generated code by\n"
         " * gen68 %s\n"
         " * (C) 1998-2007 Benjamin Gerard\n"
         " *\n"
         " * $Id$\n"
         " */\n"
         "\n", name, modifdate);
}

static void gene_table(char *s, int n)
{
  int i;

  putsss("#include \"struct68.h\"\n"
         "\n"
         "extern linefunc68_t");
  for (i=0; i<n; i++) {
    if (!(i&7)) putsss("\n"TAB);
    printf("%s%X%02X%c",s,i>>6,i&63,i==n-1?';':',');
  }

  printf("\n"
         "\n"
         "linefunc68_t *%s_func[%d] = \n",s,n);
  putsss("{");
  for(i=0; i<n; i++) {
    if ((i&7)==0) putsss("\n"TAB);
    printf("%s%s,",s,to_line_id(i));

  }
  putsss("\n};\n\n");
}


static char tsz[] = "bwl?";
static char *tm[] = { "&BYTE_MASK", "&WORD_MASK", "&LONG_MASK" };
static char *shift_name[] = { "BYTE_SHIFT", "WORD_SHIFT", "LONG_SHIFT" };

/* ,------------------------------------------------------------------.
 * | n   : opmode+mode                                                |
 * | s   : name                                                       |
 * | sz  : 0,1,2 ->b,w,l                                              |
 * '------------------------------------------------------------------'
 */
static void gene_any_move(int n, char *s, int sz)
{
  int srcmode=n&7, dstmode=(n>>3)&7;
  char c = tsz[sz],
    rs = srcmode==0 ? 'd' : 'a',
    rd = dstmode==0 ? 'd' : 'a';
  char *m = tm[sz];
  char *d = shift_name[sz];

  sz = 1<<sz; /* 1 2 4 */

  /* Generate header */
  printf(TAB"/* %s.%c ",s,c^32);
  if (srcmode<2)
    printf("%cn,",rs^32);
  else printf("<Ae>,");
  if (dstmode<2)
    printf("%cn */\n",rd^32);
  else printf("<Ae> */\n");

  /* printf(TAB"int68_t a;\n"); */
  /* if (srcmode>=2 || dstmode>=2) printf(TAB"addr68_t addr;\n"); */

  /* Source is register */
  if (srcmode<2) {
    printf(TAB"int68_t a = (u%d)REG68.%c[reg0]/*<<%s*/;\n",sz<<3,rs,d);
  }
  /* Source is memory */
  else {
    printf(TAB"addr68_t addr = get_ea%c68[%d](emu68,reg0);\n",c,srcmode);
    printf(TAB"int68_t a = /*(u%d)*/read_%c(addr)/*<<%s*/;\n",sz<<3,c^32,d);
  }

  /* Destination is data register */
  if (dstmode==0) {
    printf(TAB"REG68.d[reg9] = (REG68.d[reg9]%s)|a/*((uint68_t)a>>%s)*/;\n",
           m,d);
    printf(TAB"a <<= %s;\n",d);
    printf(TAB"%s(a);\n",s);
  }
  /* Destination is address register */
  else if (dstmode==1) {
    printf(TAB"REG68.a[reg9] = (s%d)a;\n",sz<<3);
  }
  /* Destination is memory */
  else {
    printf(TAB"%saddr = get_ea%c68[%d](emu68,reg9);\n",
           (srcmode<2)?"addr68_t ":"",c,dstmode);
    printf(TAB"write_%c(addr,a);\n",c^32);
    printf(TAB"a <<= %s;\n",d);
    printf(TAB"%s(a);\n",s);
  }
}

/* ,------------------------------------------------------------------.
 * | n   : opmode+mode                                                |
 * | s   : name                                                       |
 * | sz  : 0,1,2 ->b,w,l                                              |
 * | resflag : 0:no result                                            |
 * |           1:result is stored in same size                        |
 * |           2:result stored in long                                |
 * |           3:result stored in long , and reg9 is long (DIV like)  |
 * | regflag : 0:dn 1:an                                              |
 * '------------------------------------------------------------------'
 */
static void gene_any_rn(int n, char *s, int sz, int resflag, int regflag)
{
  char *m = tm[sz];
  char c = tsz[sz],
    r = regflag ? 'a' : 'd',
    q = (n&7)==0 ? 'd' : 'a';
  char *db, *d = shift_name[sz];
  sz = 1<<sz;
  db = (resflag==3) ? shift_name[2] : d; /* ? what if resflag == 2 */

  if (regflag==1 && sz==1) {
    printf(TAB"ILLEGAL;\n");
    return;
  }

  printf(TAB"/* %s.%c ",s,c^32);

  /* Source is memory */
  if ((n&7)>=2) {
    printf("<Ae>,%cn */\n",r^32);
    printf(TAB"int68_t a,b%s;\n",resflag ? ",s" : "");
    printf(TAB"addr68_t addr;\n");
    printf(TAB"addr = get_ea%c68[%d](emu68,reg0);\n",c,n&7);
    printf(TAB"a = read_%c(addr)<<%s;\n",c^32,d);
  }
  /* Source is register */
  else  {
    printf("%cx,%cy */\n",q^32,r^32);
    printf(TAB"int68_t a,b%s;\n",resflag ? ",s" : "");
    printf(TAB"a = REG68.%c[reg0]<<%s;\n",q,d);
  }

  switch(regflag) {
    /* Destination is data register */
  case 0:
    printf(TAB"b = REG68.d[reg9]<<%s;\n",db);
    printf(TAB"%s%c(%sa,b);\n",s,c^32,resflag ? "s,":"");
    break;
    /* Destination is address register */
  case 1:
    printf(TAB"b = REG68.a[reg9];\n");
    printf(TAB"%sA%c(%sa,b);\n",s,c^32,resflag ? "s,":"");
    break;
  }

  if (resflag==1) {
    printf(TAB"REG68.%c[reg9] = (REG68.%c[reg9]%s) + ((uint68_t)s>>%s);\n",
           r,r,m,d);
  } else if (resflag >= 2) {
    /* $$$ add shift must be verified */
    printf(TAB"REG68.%c[reg9] = s>>%s;\n",r,shift_name[2]);
  }
}

static void gene_any_dn(int n, char *s, int sz, int resflag)
{
  gene_any_rn(n,s,sz,resflag,0);
}

static void gene_any_an(int n, char *s, int sz, int resflag)
{
  gene_any_rn(n,s,sz,resflag,1);
}

/* ,------------------------------------------------------------------.
 * |  n       : opmode+mode                                           |
 * |  s       : name                                                  |
 * |  sz      : 0,1,2 -> b,w,l                                        |
 * |  resflag : result is stored                                      |
 * |  regflag : 0:dn 1:an 2:immediat 3 bit 3:imm                      |
 * '------------------------------------------------------------------'
 */
static void gene_any_rn_mem(int n, char *s, int sz, int resflag, int regflag)
{
  char *m = tm[sz];
  char c = tsz[sz],
    r = regflag ? 'a' : 'd',
    q = (n&7)==0 ? 'd' : 'a';
  char * d = shift_name[sz];
  sz = 1<<sz;

  if ((n&7)==1 && sz==1) {
    printf(TAB"ILLEGAL;\n");
    return;
  }

  printf(TAB"/* %s.%c ",s,c^32);

  /* Destination is memory */
  if ((n&7)>=2) {
    if (regflag>=2) printf("#imm,<Ae> */\n");
    else printf("%cn,<Ae> */\n",r^32);
    printf(TAB"int68_t a,b%s;\n",resflag ? ",s" : "");
    printf(TAB"addr68_t addr;\n");
    /* Source is quick imm */
    if (regflag==2)
      printf(TAB"a = (((reg9-1)&7)+1)<<%s;\n",d);
    /* Source is imm */
    else if (regflag==3)
      printf(TAB"a = get_next%c()<<%s;\n",sz==4 ? 'l' : 'w',d);
    /* Source is register */
    else
      printf(TAB"a = REG68.%c[reg9]<<%s;\n",r,d);

    printf(TAB"addr = get_ea%c68[%d](emu68,reg0);\n",c,n&7);
    printf(TAB"b = read_%c(addr)<<%s;\n",c^32,d);
    printf(TAB"%s%c(%sa,b);\n",s,c^32,resflag ? "s,":"");
    if (resflag) {
      printf(TAB"write_%c(addr,(uint68_t)s>>%s);\n",c^32,d);
    }
  }
  /* Destination is register */
  else {
    if (regflag>=2) printf("#imm,%cy */\n",q^32);
    else printf("%cx,%cy */\n",r^32,q^32);
    printf(TAB"int68_t a,b%s;\n",resflag ? ",s" : "");
    /* Source is quick imm */
    if (regflag==2)
      printf(TAB"a = (((reg9-1)&7)+1)<<%s;\n",d);
    else if (regflag==3)
      printf(TAB"a = get_next%c()<<%s;\n",sz==4 ? 'l' : 'w',d);
    else
      /* Source is register */
      printf(TAB"a = REG68.%c[reg9]<<%s;\n",r,d);

    printf(TAB"b = REG68.%c[reg0]",q);
    /* Destination is data register */
    if (q!='a')
    {
      printf("<<%s;\n",d);
      printf(TAB"%s%c(%sa,b);\n",s,c^32,resflag ? "s,":"");
    }
    /* Destination is address register */
    else {
      printf(";\n");
      printf(TAB"%sA%c(%sa,b);\n",s,c^32,resflag ? "s,":"");
    }

    if (resflag) {
      if (q=='a') {
        printf(TAB"REG68.a[reg0] = s;\n");
      } else {
        printf(TAB"REG68.d[reg0] = (REG68.d[reg0]%s) + ((uint68_t)s>>%s);\n",
               m,d);
      }
    }
  }
}

static void gene_any_dn_mem(int n, char *s, int sz, int resflag)
{
  gene_any_rn_mem(n,s,sz,resflag,0);
}

static void gene_any_an_mem(int n, char *s, int sz, int resflag)
{
  gene_any_rn_mem(n,s,sz,resflag,1);
}

/* ,-----------------------------------------------------------.
 * |                       LINE 9/D                            |
 * `-----------------------------------------------------------'
 */
static void gene_add_sub(int n, int t)
{
  char s[2][4] = { "ADD","SUB" };
  int sz = (n>>3)&3;

  if (n&040) {
    /* add dn,<ae> */
    gene_any_dn_mem(n,s[t],sz,1);
  } else {
    /* add <ae>,dn */
    gene_any_dn(n,s[t],sz,1);
  }
}

static void gene_addx_subx(int n, int t)
{
  char s[2][4] = { "ADD","SUB" };
  int sz = (n>>3)&3;
  char c = tsz[sz], *d = shift_name[sz], *m = tm[sz];
  sz = 1<<sz;

  printf(TAB"int68_t a,b,s;\n");

  /* addx -(ay),-(ax) */
  if (n&1) {
    printf(TAB"a = read_%c(REG68.a[reg0]-=%d)<<%s;\n",c^32,sz,d);
    printf(TAB"b = read_%c(REG68.a[reg9]-=%d)<<%s;\n",c^32,sz,d);
    printf(TAB"%sX%c(s,a,b);\n",s[t],c^32);
    printf(TAB"write_%c(REG68.a[reg9],(uint68_t)s>>%s);\n",c^32,d);
  }
  /* addx dy,dx */
  else {
    printf(TAB"a = REG68.d[reg0]<<%s;\n",d);
    printf(TAB"b = REG68.d[reg9]<<%s;\n",d);
    printf(TAB"%sX%c(s,a,b);\n",s[t],c^32);
    printf(TAB"REG68.d[reg9] = (REG68.d[reg9]%s) + ((uint68_t)s>>%s);\n",m,d);
  }

}

static void gene_adda_suba(int n, int t)
{
  char *s= t ? "SUB" : "ADD";
  int sz = (n&(1<<5)) ? 2 : 1;
  gene_any_an(n,s,sz,2);
}

static void gene_line9_D(int n, int t)
{
  printf("DECL_LINE68(line%c%02X)\n{\n",t ? '9' : 'D',n);
  if ((n&030)==030) gene_adda_suba(n,t);
  else if ((n&046)==040 ) gene_addx_subx(n,t);
  else gene_add_sub(n,t);

  printf("}\n\n");
}

/* ,-----------------------------------------------------------.
 * |                        LINE E                             |
 * `-----------------------------------------------------------'
 */

static void gene_tbl_lsl_mem(void)
{
  static char shf_str[][4] = { "AS", "LS", "ROX", "RO" };
  int d;
  int n;
  char c;

  for (d=0; d<2; d++) {
    c = d ? 'L' : 'R';
    for (n=0; n<4; n++) {
      printf("static void %s%c_mem"
             "(emu68_t * const emu68, int reg, int mode)\n{\n",
             shf_str[n],c);

      printf(TAB"%sint68_t a;\n",(n&3)?"u":"s");
      putsss(TAB"const addr68_t addr = get_eaw68[mode](emu68,reg);\n");
      putsss(TAB"int dec=1;\n");
      putsss(TAB"a = read_W(addr)<<WORD_SHIFT;\n");
      printf(TAB"%s%cW(a,dec);\n",shf_str[n],c);
      putsss(TAB"write_W(addr,a>>WORD_SHIFT);\n");
      putsss("}\n\n");
    }
  }

  for (d=0; d<2; d++) {
    c = d ? 'L' : 'R';
    printf("static void (*const lslmem%c_fc[4])"
           "(emu68_t *const,int,int)=\n{\n", c);
    for(n=0; n<4; n++) {
      printf(TAB"%s%c_mem,",shf_str[n], c);
    }
    printf("\n};\n\n");
  }
}

static void gene_any_lsl_mem(int n)
{
  printf(TAB"lslmem%c_fc[reg9&3](emu68,reg0,%d);\n", n&(1<<5)?'L':'R', n&7);
}

static void gene_any_lsl_reg(int n)
{
  static char shf_str[][4] = { "AS", "LS", "ROX", "RO" };
  int sz = (n>>3)&3;
  char c = tsz[sz], *m = tm[sz], *d = shift_name[sz];
  sz = 1<<sz;
  /* d = 32-sz*8; */
  printf(TAB"const int dec = reg9, reg = reg0;\n");
  printf(TAB"%cint68_t a;\n",(n&3) ? 'u' : 's' );
  printf(TAB"uint68_t d;\n");
  printf(TAB"a=REG68.d[reg]<<%s;\n",d);
  if (n&4) {
    printf(TAB"d = REG68.d[dec]&63;\n");
  } else {
    printf(TAB"d = ((dec-1)&7)+1;\n");
  }
  printf(TAB"%s%c%c(a,d);\n",shf_str[n&3], (n&(1<<5)) ? 'L' : 'R', c^32);
  printf(TAB"REG68.d[reg] = (REG68.d[reg]%s) + ((uint68_t)a>>%s);\n",m,d);
}

static void gene_lineE(int n)
{
  int sz;

  if (!n) gene_tbl_lsl_mem();

  sz=(n>>3)&3;
  printf("DECL_LINE68(lineE%02X)\n{\n",n);
  if (sz==3) gene_any_lsl_mem(n);
  else gene_any_lsl_reg(n);
  printf("}\n\n");
}

/* ,-----------------------------------------------------------.
 * |                        LINE B                             |
 * `-----------------------------------------------------------'
 */

static void gene_cmp_eor(int n)
{
  int t = (n>>5)&1;
  char s[2][4] = { "CMP","EOR" };
  int sz = (n>>3)&3;
  if (t)
    gene_any_dn_mem(n,s[t],sz,t);
  else
    gene_any_dn(n,s[t],sz,t);
}

static void gene_cmp_mem(int n)
{
  int sz = (n>>3)&3;
  char c = tsz[sz];
  sz = 1<<sz;
  printf(TAB"/* CMPM.%c (Ay)+,(Ax)+ */\n",c^32);
  printf(TAB"int68_t x,y;\n");
  printf(TAB"y = read_%c(REG68.a[reg0]-=%d);\n",c^32,sz);
  printf(TAB"x = read_%c(REG68.a[reg9]-=%d);\n",c^32,sz);
  printf(TAB"CMP%c(y,x);\n",c^32);
}

static void gene_cmpa(int n)
{
  int sz = n&(1<<5) ? 2 : 1;
  gene_any_an(n,"CMP",sz,0);
}

static void gene_lineB(int n)
{
  printf("DECL_LINE68(lineB%02X)\n{\n",n);
  if (((n>>3)&3)==3) gene_cmpa(n);
  else if ((n&047)==041) gene_cmp_mem(n);
  else gene_cmp_eor(n);
  printf("}\n\n");
}


/* ,-----------------------------------------------------------.
 * |                       LINE 8&C                            |
 * `-----------------------------------------------------------'
 */

static void gene_abcd_sbcd(char *s, int n)
{
  printf(TAB"/* %s ",s);
  if (n&1) {
    printf("-(Ay),-(Ax) */\n");
    printf(TAB"REG68.a[reg0] -= 2;\n");
    printf(TAB"REG68.a[reg9] -= 2;\n");
  }
  else {
    printf("Dy,Dx */\n");
    printf(TAB"reg0=reg0; reg9=reg9;\n");
  }
}

static void gene_mul_div(char *s, int n, int typ)
{
  char na[8];
  sprintf(na,"%s%c",s,(n&(1<<5)) ? 'S' : 'U');
  gene_any_dn(n,na,1,2+typ);
}

static void gene_exg(int n)
{
  switch(n&0x1f)
  {
  case 8:
    printf(TAB"/* EXG Dx,Dy */\n");
    printf(TAB"EXG(REG68.d[reg9],REG68.d[reg0]);\n");
    break;
  case 9:
    printf(TAB"/* EXG Ax,Ay */\n");
    printf(TAB"EXG(REG68.a[reg9],REG68.a[reg0]);\n");
    break;
  case 17:
    printf(TAB"/* EXG Dx,Ay */\n");
    printf(TAB"EXG(REG68.d[reg9],REG68.a[reg0]);\n");
    break;
  default:
    printf(TAB"/*ILLEGAL in EXG*/\n");
    printf(TAB"ILLEGAL;\n");
    break;
  }
}

static void gene_and_or(char *s,int n)
{
  if ((n&7)==1) {
    printf(TAB"ILLEGAL;\n");
  }
  else if (n&(1<<5)) {
    gene_any_dn_mem(n,s,(n>>3)&3,1);
  }
  else {
    gene_any_dn(n,s,(n>>3)&3,1);
  }
}


/* t=0 -> and, t=1 -> or */
static void gene_line8_C(int n, int t)
{
  printf("DECL_LINE68(line%c%02X)\n{\n",t ? '8' : 'C', n);
  if ((n&0x3e)==0x20)
    gene_abcd_sbcd(t ? "SBCD" : "ABCD", n);
  else if ( (n&0x18) == 0x18 )
    gene_mul_div(t ? "DIV" : "MUL", n, t);
  else if ( t==0 && ((n&07)==1 || (n&077) == 050) )
    gene_exg(n);
  else
    gene_and_or(t ? "OR" : "AND",n);
  printf("}\n\n");
}

/* ,-----------------------------------------------------------.
 * |                        LINE 5                             |
 * `-----------------------------------------------------------'
 */

static void gene_dbcc(int n)
{
  printf(TAB"/* DBcc Dn,<adr> */\n");
  if (n&(1<<5))
    printf(TAB"if (!is_cc68[reg9](REG68.sr)) {\n");
  else
    printf(TAB"if (is_cc68[reg9](REG68.sr)) {\n");
  printf(TAB2"int68_t a;\n");
  printf(TAB2"a=REG68.d[reg0]-1;\n");
  printf(TAB2"a&=0xFFFF;\n");
  printf(TAB2"REG68.d[reg0] = (REG68.d[reg0]&0xFFFF0000) + a;\n");
  printf(TAB2"if (a!=0xFFFF) {\n");
  printf(TAB33"uint68_t pc=REG68.pc;\n");
  printf(TAB33"REG68.pc = pc+get_nextw();\n");
  printf(TAB2"} else REG68.pc += 2;\n");
  printf(TAB"} else REG68.pc += 2;\n");
}

static void gene_scc(int n)
{
  if ((n&7)>=2) {
    printf(TAB"/* Scc <AE> */\n");
    printf(TAB"addr68_t addr;\n");
    printf(TAB"addr = get_eab68[%d](emu68,reg0);\n",n&7);
    if (n&(1<<5))
      printf(TAB"write_B(addr,-is_cc68[reg9](REG68.sr));\n");
    else
      printf(TAB"write_B(addr,-(is_cc68[reg9](REG68.sr)^1));\n");
  }
  else {
    printf(TAB"/* Scc Dn */\n");
    if (n&(1<<5))
      printf(TAB
             "REG68.d[reg0] = \n"
             "(REG68.d[reg0]&0xFFFFFF00)+(u8)-is_cc68[reg9](REG68.sr);\n");
    else
      printf(TAB
             "REG68.d[reg0] = \n"
             "(REG68.d[reg0]&0xFFFFFF00)+"
             "(u8)-(is_cc68[reg9](REG68.sr)^1);\n");
  }
}

static void gene_line5(int n)
{
  int sz=(n>>3)&3;
  printf("DECL_LINE68(line5%02X)\n{\n",n);
  if (sz!=3)
    gene_any_rn_mem(n, (n&(1<<5)) ? "SUB" : "ADD", sz, 1, 2);
  else if ((n&7)==1)
    gene_dbcc(n);
  else if ((n&030)==030)
    gene_scc(n);
  else printf(TAB"ILLEGAL\n");
  printf("}\n\n");
}

/* ,-----------------------------------------------------------.
 * |                        LINE 0                             |
 * `-----------------------------------------------------------'
 */
static void gene_movep(int n)
{
  int sz = (n&(1<<3)) ? 4 : 2;
  int cycle;
  int sens = n&(1<<4);
  int i;

  cycle = 4*sz; /* 8 or 16 */
  printf(TAB"/* MOVEP.%c %s */\n","WL"[sz>>2],
         !sens ? "d(An),Dn" : "Dn,d(An)");

  printf(TAB"const addr68_t addr = REG68.a[reg0] + get_nextw();\n");

  /* Dn -> Mem */
  if (sens) {
    putsss(TAB"const uint68_t a = REG68.d[reg9];\n");
    for(i=0; i<sz; i++) {
      printf(TAB"write_B(addr+%d,a>>%d);\n", i*2, (sz-i-1)*8);
    }
  }
  /* Mem -> Dn */
  else {
    putsss(TAB"uint68_t a;\n");
    for(i=0; i<sz; i++) {
      printf(TAB"a %c=(u8)read_B(addr+%d)<<%d;\n",
             i ? '+' : ' ', i*2, (sz-i-1)*8);
    }
    if (sz==2) {
      putsss(TAB"REG68.d[reg9] = (REG68.d[reg9]&~0xFFFF) | a;\n");
    } else {
      putsss(TAB"REG68.d[reg9] = a;\n");
    }
  }
  printf(TAB"ADDCYCLE(%d);\n",cycle);
}

static void gene_bitop(int n,int mode)
{
  static char s[4][5] = { "BTST", "BCHG", "BCLR", "BSET" };
  static int cycler[4] = { 2, 4, 6, 4 };
  static int cyclem[4] = { 0, 4, 4, 4 };
  int t = (n>>3)&3;

  if (!mode) {
    printf(TAB"/* %s Dx,Dy */\n",s[t]);
    putsss(TAB"int68_t a,b;\n");
    putsss(TAB"a = REG68.d[reg0];\n");
    putsss(TAB"b = REG68.d[reg9]&31;\n");
    printf(TAB"%s(a,b);\n",s[t]);
    if (t) {
      putsss(TAB"REG68.d[reg0] = a;\n");
    }
    printf(TAB"ADDCYCLE(%d);\n",cycler[t]);
  } else {
    printf(TAB"/* %s Dn,<Ae> */\n",s[t]);
    putsss(TAB"int68_t a,b;\n");
    putsss(TAB"addr68_t addr;\n");
    printf(TAB"addr = get_eab68[%d](emu68,reg0);\n",mode);
    putsss(TAB"a = read_B(addr);\n");
    putsss(TAB"b = REG68.d[reg9]&7;\n");
    printf(TAB"%s(a,b);\n",s[t]);
    if (t) putsss(TAB"write_B(addr,a);\n");
    printf(TAB"ADDCYCLE(%d);\n",cyclem[t]);
  }
}

static void gene_bxxx_mem(int t)
{
  static char s[4][5] = { "BTST", "BCHG", "BCLR", "BSET" };
  static int cyclem[4] = { 0, 4, 4, 4 };
  printf("static void %s_mem"
         "(emu68_t * const emu68, int b, int mode, int reg0)\n{\n",
         s[t]);
  putsss(TAB"addr68_t addr = get_eab68[mode](emu68,reg0);\n");
  putsss(TAB"int68_t a = read_B(addr);\n");
  putsss(TAB"b &= 7;\n");
  printf(TAB"%s(a,b);\n",s[t]);
  if (t) putsss(TAB"write_B(addr,a);\n");
  printf(TAB"ADDCYCLE(%d);\n",cyclem[t]);
  putsss("}\n\n");
}

static void gene_bxxx_reg(int t)
{
  static char s[4][5] = { "BTST", "BCHG", "BCLR", "BSET" };
  static int cycler[4] = { 2, 4, 6, 4 };
  printf("static void %s_reg"
         "(emu68_t * const emu68, int b, int reg0)\n{\n",s[t]);
  putsss(TAB"int68_t a = REG68.d[reg0];\n");
  putsss(TAB"b &= 31;\n");
  printf(TAB"%s(a,b);\n",s[t]);
  if (t) putsss(TAB"REG68.d[reg0] = a;\n");
  printf(TAB"ADDCYCLE(%d);\n",cycler[t]);
  putsss("}\n\n");
}


static void gene_line0_mix(int n)
{
  static char s[4][5] = { "BTST", "BCHG", "BCLR", "BSET" };
  int mode = n&7;
  int t=(n>>3)&3;

  putsss(TAB"if (reg9==4) {\n");
  if (!mode) printf(TAB2"%s_reg(emu68,get_nextw(),reg0);\n",s[t]);
  else       printf(TAB2"%s_mem(emu68,get_nextw(),%d,reg0);\n",s[t], mode);
  putsss(TAB"} else {\n");
  if (n&(1<<5)) {
    putsss(TAB2"ILLEGAL;\n");
    return;
  } else {
    printf(TAB2"line0_imm[reg9][%d](emu68,reg0);\n",n&31);
  }
  putsss(TAB"}\n");
}

static void gene_imm_sr(int n, int sz)
{
/*  printf("static void l0_%s%s(int reg0)\n{\n",s,t ? "CCR" : "SR" );*/

  putsss(TAB"if (reg0==4) {\n");
  putsss(TAB2"uint68_t a;\n");

  switch(n)
  {
  case 0: /* OR */
    if (!sz) putsss(TAB2"a = get_nextw()&255;\n");
    else     putsss(TAB2"a = get_nextw();\n");
    putsss(TAB2"REG68.sr |= a;\n");
    break;

  case 1: /* AND */
    if (!sz) putsss(TAB2"a = get_nextw()|0xFF00;\n");
    else     putsss(TAB2"a = get_nextw();\n");
    putsss(TAB2"REG68.sr &= a;\n");
    break;

  case 2: /*EOR */
    if (!sz) putsss(TAB2"a = get_nextw()&255;\n");
    else     putsss(TAB2"a = get_nextw();\n");
    putsss(TAB2"REG68.sr ^= a;\n");
    break;
  }
  putsss(TAB"} else {\n");

  switch(n)
  {
  case 0:
    gene_any_rn_mem(7,"OR",sz,1,3);
    break;
  case 1:
    gene_any_rn_mem(7,"AND",sz,1,3);
    break;
  case 2:
    gene_any_rn_mem(7,"EOR",sz,1,3);
    break;
  }
  printf(TAB"}\n");
}

static void gene_imm_op(int n, char *s, int sz, int op)
{
  if ((n&7)==1) return;

  printf("static void l0_%s%c%d(emu68_t * const emu68, int reg0)\n{\n",
         s,tsz[sz],n);
  if ((n&7)!=7 || op>2 || sz==2)
    gene_any_rn_mem(n,s,sz,op!=5,3);
  else
    gene_imm_sr(op,sz);

  putsss("}\n\n");
}


static void gene_imm_illegal(void)
{
  printf("static void l0_ill(emu68_t * const emu68, int reg0)\n{\n");
  printf(TAB"reg0=reg0; ILLEGAL;\n}\n\n");

}


static void gene_line0(int n)
{
  int i;
  static char ts[8][5] = {"OR","AND","SUB","ADD","?","EOR","CMP","?"};

  if (!n) {
    for(i=0; i<4; i++) {
      gene_bxxx_reg(i);
      gene_bxxx_mem(i);
    }

    gene_imm_illegal();

    for(i=0; i<8; i++) {
      int j;
      for(j=0; j<3; j++) {
        gene_imm_op(i, "OR" ,j,0);
        gene_imm_op(i, "AND",j,1);
        gene_imm_op(i, "EOR",j,2);
        gene_imm_op(i, "ADD",j,3);
        gene_imm_op(i, "SUB",j,4);
        gene_imm_op(i, "CMP",j,5);
      }
    }

    printf("static void (*const line0_imm[8][32])"
           "(emu68_t * const emu68, int) =\n{\n");
    for(i=0; i<8; i++) {
      int j;
      char *s;
      s = ts[i];
      printf("/* %s */\n  {\n",s);
      for(j=0;j<32; j++)
      {
        char c;

        if ((j&7)==0) printf(TAB);
        c = tsz[j>>3];
        printf("l0_");
        switch(i)
        {

        case 0: /* OR */
        case 1: /* AND */
        case 5: /* EOR */
          /*if (j==007)
            printf("%sCCR,",s);
            else if (j==017)
            printf("%sSR,",s);
            else */if ((j&030)==030 || (j&7)==1)
            printf("ill,");
          else
            printf("%s%c%d,",s,c,j&7);
          break;

        case 2: /* SUB */
        case 3: /* ADD */
        case 6: /* CMP */
          if ((j&030)==030 || (j&7)==1)
            printf("ill,");
          else
            printf("%s%c%d,",s,c,j&7);
          break;

        default:
          printf("ill,");
          break;
        }
        if ((j&7)==7) printf("\n");
      }
      printf("\n  },\n");
    }
    printf("};\n\n");
  }
  printf("DECL_LINE68(line0%02X)\n{\n",n);
  if ( (n & 047) == 041 ) gene_movep(n);
  else if ( (n & 040) == 040 ) gene_bitop(n,n&7);
  else gene_line0_mix(n);
/*  else if ( (n & (0xFF00) == 0x0800 ) desa_bitop(d);
    else if ( (n & 0677) == 0074 ) desa_IMM_SR(d);
    else if ( (n & 0x100 )==0) desa_imm_op(d);
    else printf(TAB"ILLEGAL;\n");            */
  putsss("}\n\n");
}

/* ,-----------------------------------------------------------.
 * |                      LINE 1/2/3                           |
 * `-----------------------------------------------------------'
 */

static void gene_line1_2_3(int n,int sz, int l)
{
  printf("DECL_LINE68(line%d%02X)\n{\n",l,n);
  gene_any_move(n,"MOVE",sz);
  putsss("}\n\n");
}

/* ,-----------------------------------------------------------.
 * |                        LINE 6                             |
 * `-----------------------------------------------------------'
 */
static void gene_bcc(int n)
{
  int hdep = (signed char)((n&31)<<3), ccnot = (n>>5)&1;

  putsss(TAB"uint68_t pc = REG68.pc;\n");

  /* Possible jmp word */
  if (hdep==0) {
    putsss(TAB"/* Bcc.w */\n");
    putsss(TAB"if (reg0==0) pc += get_nextw();\n");
    putsss(TAB"/* Bcc.s */\n");
    putsss(TAB"else pc += reg0;\n");
  }
  /* only jmp short */
  else {
    putsss(TAB"/* Bcc.s */\n");
    printf(TAB"pc += reg0+%d;\n",hdep);
  }

  /* Possible BSR */
  if (ccnot) {
    putsss(TAB"/* BSR */ \n");
    putsss(TAB"if (reg9==0)\n");
    putsss(TAB"{\n");
    putsss(TAB2"pushl(REG68.pc);\n");
    putsss(TAB2"REG68.pc = pc;\n");
    putsss(TAB2"ADDCYCLE(12);\n");
    putsss(TAB"}\n");
    putsss(TAB"else");
  }
  putsss(TAB"/* Bcc */ \n");
  if (ccnot) {
    putsss(TAB"if (is_cc68[reg9](REG68.sr)) REG68.pc = pc;\n");
  } else {
    putsss(TAB"if (!is_cc68[reg9](REG68.sr)) REG68.pc = pc;\n");
  }
}

static void gene_line6( int n )
{
  printf("DECL_LINE68(line6%02X)\n{\n",n);
  gene_bcc(n);
  putsss("}\n\n");
}

/* ,-----------------------------------------------------------.
 * |                        LINE 7                             |
 * `-----------------------------------------------------------'
 */
static void gene_any_moveq(int n)
{
  putsss(TAB"/* MOVEQ #xx,Dn */\n");
  printf(TAB"int68_t a=(int68_t)(s8) (reg0+((%d&31)<<3));\n",n);
  putsss(TAB"REG68.d[reg9] = a;\n");
  putsss(TAB"MOVE(a);\n");
}

static void gene_line7( int n )
{
  printf("DECL_LINE68(line7%02X)\n{\n",n);
  gene_any_moveq(n);
  putsss("}\n\n");
}

/* ,-----------------------------------------------------------.
 * |                       LINE A/F                            |
 * `-----------------------------------------------------------'
 */

/* t=1 -> lineA  t=0 -> lineF  */
static void gene_lineA_F(int n,int t)
{
  if (!n) {
    printf("DECL_LINE68(line%c%02X)\n{\n"
           ,t ? 'A' : 'F',n);
    printf(TAB"LINE%c;\n",t ? 'A' : 'F' );
    putsss("}\n\n");
  }
}

/* ,-----------------------------------------------------------.
 * |                        LINE 4                             |
 * `-----------------------------------------------------------'
 */
static void gene_chk(int n)
{
  if ((n&7)==1)
  {
    putsss(TAB"/* CHK <AE>,Dn */;\n");
    putsss(TAB"ILLEGAL;\n");
  }
  else gene_any_dn(n,"CHK",1,0);
}

static void gene_lea(int n)
{
  int mode=n&7;
  putsss(TAB"/* LEA <AE>,an */\n");
  if (mode<5 && mode!=2)
  {
    putsss(TAB"ILLEGAL;\n");
    return;
  }
  if (mode==2)
    putsss(TAB"REG68.a[reg9] = REG68.a[reg0];\n");
  else if (mode==5)
    putsss(TAB"REG68.a[reg9] = REG68.a[reg0] + get_nextw();\n");
  else
    printf(TAB"REG68.a[reg9] = (get_eal68[%d])(emu68,reg0);\n",mode);
}

static void gene_movefromsr(void)
{
  putsss(TAB"/* MOVE FROM SR */\n");
  putsss(TAB"if (mode)\n");
  putsss(TAB2"write_W(get_eaw68[mode](emu68,reg0),REG68.sr);\n");
  putsss(TAB"else\n");
  putsss(TAB2"REG68.d[reg0]=(REG68.d[reg0]&0xFFFF0000)+(u16)REG68.sr;\n");
}

static void gene_movetosr(void)
{
  putsss(TAB"/* MOVE TO SR */\n");
  putsss(TAB"if (mode)\n");
  putsss(TAB2"REG68.sr = read_W(get_eaw68[mode](emu68,reg0));\n");
  putsss(TAB"else\n");
  putsss(TAB2"REG68.sr = (u16)REG68.d[reg0];\n");
}

static void gene_moveccr(void)
{
  putsss(TAB"/* MOVE TO CCR */\n");
  putsss(TAB"if (mode)\n");
  putsss(TAB2"SET_CCR(REG68.sr,read_W(get_eaw68[mode](emu68,reg0)));\n");
  putsss(TAB"else\n");
  putsss(TAB2"SET_CCR(REG68.sr,REG68.d[reg0]);\n");
}

static void gene_pea_swap(void)
{
  putsss(TAB"/* SWP or PEA */\n");
  putsss(TAB"if (!mode){ SWAP(REG68.d[reg0]); }\n");
  putsss(TAB"else pushl(get_eal68[mode](emu68,reg0));\n");
}

static void gene_movemmem_ext(int sz)
{
  printf(TAB"if (!mode){ EXT%c(REG68.d[reg0]); }\n", sz ? 'L' : 'W');
  printf(TAB"else movemmem%c(emu68,mode,reg0);\n", sz ? 'l' : 'w');
}

static void gene_movemreg( int sz )
{
  printf(TAB"movemreg%c(emu68,mode,reg0);\n", sz ? 'l' : 'w');
}

static void gene_movemregfunc( int sz )
{
  char c=sz ? 'l' : 'w';
  sz = (sz+1)*2;
  printf("static void movemreg%c"
         "(emu68_t * const emu68, int mode, int reg0)\n",c);
  printf("{\n");
  printf(TAB"uint68_t m = (u16)get_nextw(), addr;\n");
  printf(TAB"int68_t *r = REG68.d;\n");
  printf(TAB"addr = get_ea%c68[mode](emu68,reg0);\n",c);
  printf(TAB"for(; m; r++, m>>=1)\n");
  printf(TAB2"if (m&1){ *r=(int68_t)read_%c(addr); addr+=%d; }\n",c^32,sz);
  printf(TAB"if (mode==3) REG68.a[reg0] = addr;\n");
  printf("}\n\n");
}

static void gene_movemmemfunc( int sz )
{
  char c=sz ? 'l' : 'w';
  sz = (sz+1)*2;
  printf("static void movemmem%c"
         "(emu68_t * const emu68, int mode, int reg0)\n",c);
  putsss("{\n");
  putsss(TAB"uint68_t m = (u16)get_nextw(), addr;\n");
  putsss(TAB"if (mode==4) {\n");
  putsss(TAB2"int68_t *r = REG68.a+7;\n");
  printf(TAB2"addr = get_ea%c68[3](emu68,reg0);\n",c);
  putsss(TAB2"for(; m; r--, m>>=1)\n");
  printf(TAB33"if (m&1) write_%c(addr-=%d,*r);\n",c^32,sz);
  putsss(TAB2"REG68.a[reg0] = addr;\n");
  putsss(TAB"} else {\n");
  putsss(TAB2"int68_t *r = REG68.d;\n");
  printf(TAB2"addr = get_ea%c68[mode](emu68,reg0);\n",c);
  putsss(TAB2"for(; m; r++, m>>=1)\n");
  printf(TAB33"if (m&1) { write_%c(addr,*r); addr+=%d; }\n",c^32,sz);
  putsss(TAB"}\n");
  putsss("}\n\n");
}

static void gene_tas_illegal(void)
{
  putsss(TAB"if (mode<2) {\n");
  putsss(TAB2"int68_t a = REG68.d[reg0]<<BYTE_SHIFT;\n");
  putsss(TAB2"TAS(a);\n");
  putsss(TAB2"REG68.d[reg0] = (REG68.d[reg0]&BYTE_MASK) | "
         "((uint68_t)a>>BYTE_SHIFT);\n");
  putsss(TAB"} else {\n");
  putsss(TAB2"int68_t a;\n");
  putsss(TAB2"if (mode==7 && reg0>1) {\n");
  putsss(TAB33"ILLEGAL;\n");
  putsss(TAB2"} else {\n");
  putsss(TAB33"const addr68_t addr = get_eab68[mode](emu68,reg0);\n");
  putsss(TAB33"a = read_B(addr)<<BYTE_SHIFT;\n");
  putsss(TAB33"TAS(a);\n");
  putsss(TAB33"write_B(addr,(uint68_t)a>>BYTE_SHIFT);\n");
  putsss(TAB2"}\n");
  putsss(TAB"}\n");
}

static void gene_jmp(int s)
{
  printf(TAB"/* J%s <Ae> */\n",s ? "MP" : "SR");
  putsss(TAB"const addr68_t addr = get_eal68[mode](emu68,reg0);\n");
  if (!s) putsss(TAB"pushl(REG68.pc);\n");
  putsss(TAB"REG68.pc = addr;\n");
  putsss(TAB"ADDCYCLE(4);\n");
}

/* flags : bit 0:no read bit 1:no write */
static void gene_any_impl(char *s, int sz, int flags)
{
  char c=tsz[sz]^32;
  char * d = shift_name[sz];
  char *m=tm[sz];
  sz = 1<<sz;
/*   d = 32-8*sz; */

  printf(TAB"/* %s.%c <Ae> */\n",s,c);

  putsss(TAB"if (mode<2)\n");
  putsss(TAB"{\n");

  putsss(TAB2"int68_t s;\n");
  if (flags & 1) {
    putsss(TAB2"int68_t a=0;\n");
  } else {
    printf(TAB2"int68_t a=REG68.d[reg0]<<%s;\n",d);
  }
  printf(TAB2"%s%c(s,a);\n",s,c);
  if (! (flags & 2)) {
    printf(TAB2"REG68.d[reg0] = (REG68.d[reg0]%s) | (uint68_t)s>>%s;\n",m,d);
  }
  putsss(TAB"}\n");

  putsss(TAB"else\n");
  putsss(TAB"{\n");
  printf(TAB2"addr68_t addr=get_ea%c68[mode](emu68,reg0);\n",c^32);
  printf(TAB2"int68_t s,a;\n");
  if (!(flags & 1)) {
    printf(TAB2"a=read_%c(addr)<<%s;\n",c,d);
  } else {
    printf(TAB2"a=0;\n");
  }
  printf(TAB2"%s%c(s,a);\n",s,c);
  if (!(flags & 2)) {
    printf(TAB2"write_%c(addr,(uint68_t)s>>%s);\n",c,d);
  }
  putsss(TAB"}\n");
}

static void gene_funky4_mode6( void)
{
  int reg0;
  for(reg0=0; reg0<8;reg0++)
  {
    printf("static void funky4_m6_%d(emu68_t * const emu68)\n{\n",reg0);
    switch(reg0)
    {
    case 0: putsss(TAB"RESET;\n"); break;
    case 1: putsss(TAB"NOP;\n"); break;
    case 2: putsss(TAB"STOP;\n"); break;
    case 3: putsss(TAB"RTE;\n"); break;
    case 4: putsss(TAB"ILLEGAL;\n"); break;
    case 5: putsss(TAB"RTS;\n"); break;
    case 6: putsss(TAB"TRAPV;\n"); break;
    case 7: putsss(TAB"RTR;\n"); break;
    }
    putsss("}\n\n");
  }
  putsss("\nstatic void (*const funky4_m6_func[8])(emu68_t * const) =\n{ ");
  for(reg0=0; reg0<8;reg0++) {
    printf("funky4_m6_%d,",reg0);
  }
  putsss(" };\n\n");
}

static void gene_funky4_mode(void)
{
  int mode;
  for(mode=0; mode<8; mode++)
  {
    printf("static void funky4_m%d(emu68_t * const emu68, int reg0)\n{\n",
           mode);
    switch(mode)
    {
    case 0: case 1:
      printf(TAB"int68_t a = (%d<<3)+reg0;\n",mode);
      putsss(TAB"TRAP(a);\n");
      break;
    case 2:
      putsss(TAB"LINK(reg0);\n");
      break;
    case 3:
      putsss(TAB"UNLK(reg0);\n");
      break;
    case 4:
      putsss(TAB"REG68.usp = REG68.a[reg0];\n");
      break;
    case 5:
      putsss(TAB"REG68.a[reg0] = REG68.usp;\n");
      break;
    case 6:
      putsss(TAB"funky4_m6_func[reg0](emu68);\n");
      break;
    case 7:
      putsss(TAB"ILLEGAL;\n");
    }
    printf("}\n\n");
  }

  printf("static void (* const funky4_func[8])(emu68_t * const, int) =\n{ ");
  for(mode=0; mode<8; mode++)
    printf("funky4_m%d,",mode);
  printf("\n};\n\n");
}

static void gene_funky4(void)
{
  printf(TAB"funky4_func[mode](emu68,reg0);\n");
}

static void gene_line4(int n)
{
  if (!n) {
    int i,j,r,s;

    gene_funky4_mode6();
    gene_funky4_mode();
    gene_movemregfunc(0);
    gene_movemregfunc(1);
    gene_movemmemfunc(0);
    gene_movemmemfunc(1);

    for (r=0; r<8; r++) {
      for (s=0; s<4; s++) {
        printf("static void line4_r%d_s%d"
               "(emu68_t * const emu68, int mode, int reg0)\n{\n",r,s);
        switch (r) {
        case 0:
          if (s==3) gene_movefromsr();
          else gene_any_impl("NEGX",s,0);
          break;

        case 1:
          if (s==3) printf(TAB"ILLEGAL;\n");
          else gene_any_impl("CLR",s,1);
          break;

        case 2:
          if (s==3) gene_moveccr();
          else gene_any_impl("NEG",s,0);
          break;

        case 3:
          if (s==3) gene_movetosr();
          else gene_any_impl("NOT",s,0);
          break;

        case 4:
          if (s==0) gene_any_impl("NBCD",s,0);
          else if (s==1) gene_pea_swap();
          else gene_movemmem_ext(s-2);
          break;

        case 5:
          if (s==3) gene_tas_illegal();
          else gene_any_impl("TST",s,2);
          break;

        case 6:
          if (s>=2) gene_movemreg(s-2);
          else printf(TAB"ILLEGAL;\n");
          break;

        case 7:
          if (s==0) printf(TAB"ILLEGAL;\n");
          else if (s==1) gene_funky4();
          else gene_jmp(s-2);
          break;

        }
        printf("}\n\n");
      }
    }

    for(i=0; i<4; i++) {
      printf("static DECL_LINE68((* const line4_%d_func[8])) =\n{\n"TAB,i);
      for(j=0; j<8;j++) {
        printf("line4_r%d_s%d,",j,i);
      }
      printf("\n};\n\n");
    }
  }


  printf("DECL_LINE68(line4%02X)\n{\n",n);
  if (n&040) {
    if ((n&070)==060) gene_chk(n);
    else if ((n&070)==070) gene_lea(n);
    else printf(TAB"ILLEGAL;\n");
  } else {
    printf(TAB"line4_%d_func[reg9](emu68,%d,reg0);\n",(n&070)>>3,n&7);
  }
  printf("}\n\n");
}

/*  Create a filename from path + name,
 *  Check trailing '/' at end of path
 *  returns NULL if any error
 *  or pointer to internal static string : !! use once a time !!
 */
static char *output_fname(char *path, char *name)
{
  static char fname[1024];
  snprintf(fname,sizeof(fname),"%s/%s",path,name);
  fname[sizeof(fname)-1] = 0;
  return fname;
}

static FILE * outfile(char *path, char *name)
{
  FILE *output;
  char *outname;
  if (!path) {
    print_fileheader(name);
    return stdout;
  } else if (!name) {
    return NULL;
  } else {
    outname = output_fname(path,name);
    if (output = freopen(outname,"wt",stdout), !output) {
      perror(outname);
      return NULL;
    }
    print_fileheader(name);
    return output;
  }
}

int main(int na, char **a)
{
  int i;
  int linetogen = 0;
  char *destpath=NULL;
  FILE *output,*savestdout=stdout;

  if (na < 2) {
    return Usage();
  }
  for(i=1; i<na; i++) {
    if (!strcmp("--help",a[i])  ||
        !strcmp("--usage",a[i]) ||
        !strcmp("-h",a[i])) {
      return Usage();
    }
  }

  if (!strcmp(a[1],"all")) {
    linetogen = 0x1FFFF;
  } else {
    char *s;
    for(s=a[1]; *s; s++) {
      if ((*s>='0' && *s<='9')) linetogen |= 1<<(*s-'0');
      else if ((*s>='a' && *s<='f')) linetogen |= 1<<(*s-'a'+10);
      else if ((*s>='A' && *s<='F')) linetogen |= 1<<(*s-'A'+10);
      else if ((*s=='T' || *s=='t')) linetogen |= 1<<16;
      else  {
        error("Invalid parameter in %s: hexa digit or 't' expected\n",a[1]);
        return 3;
      }
    }
  }

  if (na>2) {
    destpath = a[2];
  }

  if (!linetogen) {
    msg("Nothing to generate.\n");
    return 0;
  }
  if (destpath) {
    msg("Files generate into '%s' directory\n", destpath);
  } else {
    msg("Files generate to stdout\n");
  }

  if (linetogen&(1<<0x0)) {
    msg("generating line 0\n");
    if (output=outfile(destpath,"line0.c"), output==NULL)
      return 10+0;
    for(i=0; i<64; i++) gene_line0(i);
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0x1)) {
    msg("generating line 1\n");
    if (output=outfile(destpath,"line1.c"), output==NULL)
      return 10+0x1;
    for(i=0; i<64; i++) gene_line1_2_3(i,0,1);
    /*gene_table("line1",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0x2)) {
    msg("generating line 2\n");
    if (output=outfile(destpath,"line2.c"), output==NULL)
      return 10+0x2;
    for(i=0; i<64; i++) gene_line1_2_3(i,2,2);
    /*gene_table("line2",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0x3)) {
    msg("generating line 3\n");
    if (output=outfile(destpath,"line3.c"), output==NULL)
      return 10+0x3;
    for(i=0; i<64; i++) gene_line1_2_3(i,1,3);
    /*gene_table("line3",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0x4)) {
    msg("generating line 4\n");
    if (output=outfile(destpath,"line4.c"), output==NULL)
      return 10+0x4;
    for(i=0; i<64; i++) gene_line4(i);
    /*gene_table("line4",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0x5)) {
    msg("generating line 5\n");
    if (output=outfile(destpath,"line5.c"), output==NULL)
      return 10+0x5;
    for(i=0; i<64; i++) gene_line5(i);
    /*gene_table("line5",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0x6)) {
    msg("generating line 6\n");
    if (output=outfile(destpath,"line6.c"), output==NULL)
      return 10+0x6;
    for(i=0; i<64; i++) gene_line6(i);
    /*gene_table("line6",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0x7))
  {
    msg("generating line 7\n");
    if (output=outfile(destpath,"line7.c"), output==NULL)
      return 10+0x7;
    for(i=0; i<64; i++) gene_line7(i);
    /*gene_table("line7",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0x8))
  {
    msg("generating line 8\n");
    if (output=outfile(destpath,"line8.c"), output==NULL)
      return 10+0x8;
    for(i=0; i<64; i++) gene_line8_C(i,1);
    /*gene_table("line8",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0x9))
  {
    msg("generating line 9\n");
    if (output=outfile(destpath,"line9.c"), output==NULL)
      return 10+0x9;
    for(i=0; i<64; i++) gene_line9_D(i,1);
    /*gene_table("line9",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0xA))
  {
    msg("generating line A\n");
    if (output=outfile(destpath,"lineA.c"), output==NULL)
      return 10+0xA;
    for(i=0; i<64; i++) gene_lineA_F(i,1);
    /*gene_table("lineA",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0xB))
  {
    msg("generating line B\n");
    if (output=outfile(destpath,"lineB.c"), output==NULL)
      return 10+0xB;
    for(i=0; i<64; i++) gene_lineB(i);
    /*gene_table("lineB",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0xC))
  {
    msg("generating line C\n");
    if (output=outfile(destpath,"lineC.c"), output==NULL)
      return 10+0xC;
    for(i=0; i<64; i++) gene_line8_C(i,0);
    /*gene_table("lineC",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0xD))
  {
    msg("generating line D\n");
    if (output=outfile(destpath,"lineD.c"), output==NULL)
      return 10+0xD;
    for(i=0; i<64; i++) gene_line9_D(i,0);
    /*gene_table("lineD",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0xE))
  {
    msg("generating line E\n");
    if (output=outfile(destpath,"lineE.c"), output==NULL)
      return 10+0xE;
    for(i=0; i<64; i++) gene_lineE(i);
    /*gene_table("lineE",64);*/
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0xF))
  {
    msg("generating line F\n");
    if (output=outfile(destpath,"lineF.c"), output==NULL)
      return 10+0xF;
    for(i=0; i<64; i++) gene_lineA_F(i,0);
    if (output!=savestdout) fclose(output);
  }

  if (linetogen&(1<<0x10))
  {
    msg("generating function table\n");
    if (output=outfile(destpath,"table.c"), output==NULL)
      return 10+0x10;
    gene_table("line",64*16); fflush(output);
    if (output!=savestdout) fclose(output);
  }

  return 0;
}
