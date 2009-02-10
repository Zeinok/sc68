/*
 *                        debug68 - debugger
 *
 *            Copyright (C) 1998-2009 Ben(jamin) Gerard
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
 * $Id: sc68.c 57 2009-01-31 18:24:54Z benjihan $
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sc68/error68.h"
#include "sc68/string68.h"
#include "sc68/sc68.h"
#include "sc68/istream68_file.h"
#include "sc68/alloc68.h"

#include "debug68_debugcom.h"
#include "debug68_breakpoint.h"
#include "debug68_eval.h"
#include "debug68_error.h"

#include "emu68/emu68.h"
#include "emu68/mem68.h"

#include "desa68.h"

/* static reg68_t * reg68;		/\*  *\/ */
/* static emu68_t * emu68;		/\*  *\/ */
static emu68_t * dbg68;		/*  */

/* Entering status backup */
static struct {
  reg68_t    reg68;
  u8       * mem, * chk;
  addr68_t   memmsk;
} save;

/* Command line backup */
#define DEBUGGER_MAX_ARG 32
static int argc = 0;
static char *argv[DEBUGGER_MAX_ARG];
static int last_com;

/* Disassembler */
static desa68_parm_t desaparm;
static int default_ndesa=10;
static char desastr[256];

#define DEFAULT_MAX_EXEC 5000000


/* disassambler run and format output */
static void line()
{
  printf("------ ----  ------------\n");
}

static int mydesa(desa68_parm_t *d, u8 *chk, u32 curpc, char *tail)
{
  u32 pc = d->pc & d->memmsk;
  d->flags = DESA68_ASCII_FLAG;
  desa68(d);
  printf("$%06X [%c%c%c%c]%c%-30s%s",
	 pc,
	 chk[pc]&READ_68     ? 'R' : ' ',
	 chk[pc]&WRITTEN_68  ? 'W' : ' ',
	 chk[pc]&EXECUTED_68 ? 'X' : ' ',
	 chk[pc]&BREAKED_68  ? 'B' : ' ',
	 pc==curpc           ? '>' : ' ',
	 d->str,
	 tail==NULL ? "\n" : tail
	 );
  return 0;
}

static char * mydesa_str(desa68_parm_t *d, u8 *chk, u32 curpc, char *tail)
{
  static char tmp[128];
  u32 pc = d->pc & d->memmsk;

  desa68(d);
  sprintf(tmp,"$%06X [%c%c%c%c]%c%-30s%s",
	  pc,
	  chk[pc]&READ_68     ? 'R' : ' ',
	  chk[pc]&WRITTEN_68  ? 'W' : ' ',
	  chk[pc]&EXECUTED_68 ? 'X' : ' ',
	  chk[pc]&BREAKED_68  ? 'B' : ' ',
	  pc==curpc           ? '>' : ' ',
	  d->str,
	  tail==NULL ? "\n" : tail
	  );
  return tmp;
}


static unsigned last_desa = (unsigned)~0;


static int desa(u32 pc, int ndesa)
{
  reg68_t * const reg68 = &dbg68->reg;
  int i;

  desaparm.pc     = (pc==(unsigned)~0)? reg68->pc : pc;
  desaparm.mem    = dbg68->mem;
  desaparm.memmsk = dbg68->memmsk;
  desaparm.str    = desastr;
  desaparm.flags  = 0;
  if (!ndesa) ndesa = default_ndesa;
  for(i=0; i<ndesa; i++)
    {
      last_desa = desaparm.pc;
      mydesa(&desaparm,dbg68->chk,reg68->pc,NULL);
    }
  return 0;
}

static int desa1(u32 pc)
{
  reg68_t * const reg68 = &dbg68->reg;

  desaparm.pc     = (pc==(unsigned)~0)? reg68->pc : pc;
  desaparm.mem    = dbg68->mem;
  desaparm.memmsk = dbg68->memmsk;
  desaparm.str    = desastr;
  desaparm.flags  = 0;
  mydesa(&desaparm,dbg68->chk,reg68->pc,"!");
  return 0;
}

static char * desa1_str(u32 pc)
{
  reg68_t * const reg68 = &dbg68->reg;

  desaparm.pc     = (pc==(unsigned)~0)? reg68->pc : pc;
  desaparm.mem    = dbg68->mem;
  desaparm.memmsk = dbg68->memmsk;
  desaparm.str    = desastr;
  desaparm.flags  = 0;
  return mydesa_str(&desaparm,dbg68->chk,reg68->pc,"!");
}


static u32 next_inst(u32 pc)
{
  reg68_t * const reg68 = &dbg68->reg;

  u32 spc = desaparm.pc, ni;
  desaparm.pc     = (pc==(unsigned)~0) ? reg68->pc : pc;;
  desaparm.mem    = save.mem;
  desaparm.memmsk = save.memmsk;
  desaparm.str    = desastr;
  desa68(&desaparm);
  ni = desaparm.pc;
  desaparm.pc = spc;
  return ni;
}

static int trace(addr68_t addr, int display, int do_bp)
{
  reg68_t oldreg68;
  reg68_t * const reg68 = &dbg68->reg;
  int bp,i;

  if (addr!=(unsigned)~0)
    reg68->pc = addr;

  last_desa = reg68->pc & dbg68->memmsk;

  /* Evaluate breakpoint */
  if (do_bp && (bp=debug68_breakp_test(reg68->pc), bp>=0))
    return bp;

  oldreg68 = *reg68;
  emu68_step(dbg68);
  if (display)
    for(i=0; i<16; i++)
      if (reg68->d[i]!=oldreg68.d[i])
	printf("%c%d : %08X => %08X\n",
	       i<8 ? 'D' : 'A', i&7, oldreg68.d[i], reg68->d[i]);

  last_desa = reg68->pc&dbg68->memmsk;
  return -1;

}

void debug68_debugger_help(void)
{
  printf
    (
     "debug68 debugger commands:\n"
     "Syntax :\n"
     " [0123456789]       : decimal digit (DIG)\n"
     " [:DIG:]|[A-Fa-f]   : hexadecimal digit (XDIG)\n"
     " $[:XDIG:]+         : unsigned hexadecimal (UHEX)\n"
     " [:UDEC:]+          : unsigned integer (UINT)\n"
     " -[:UINT:]|[:UHEX:] : signed integer (INT)\n\n"
     "t [@]               : Trace instruction\n"
     "s [@]               : Skip instruction\n"
     "m [@]               : Memory dump\n"
     "D#|A#|PC|SR=#       : register value change\n"
     "reg                 : show registers\n"
     "e expression        : Evaluate expression\n"
     "b [@] [#] [r#]      : Set Breakpoint\n"
     "d [@] [lines]       : disassemble N instruction\n"
     "m @ [#]             : Memory dump from @ to @+#\n"
     "r @ [#]             : run at pc=@ # instruction OR rts\n"
     "u @ [#]             : run at pc UNTIL pc=@ OR # inst executed\n"
     "sbin fname @ #      : Save memory from @ to @+# into file\n"
     );
}


/* Display debugger shell prompt as diskname track disasm >
 */
char * debug68_prompt(char *diskname, int track_num)
{
  reg68_t * const reg68 = &dbg68->reg;
  return desa1_str(reg68->pc);
}

static int outofbound(addr68_t addr)
{
  return addr > (addr68_t)save.memmsk;
}

/* Clean/Set bits in chk mem
 * use with anded=0 and ored=0 to clear
 */
static void and_or_chk(addr68_t start, addr68_t end, u8 anded, u8 ored)
{
  u8 *chk = save.chk;
  if (chk==NULL || outofbound(start) || outofbound(end) || start>end)
    return;
  for(;start<=end;start++)
    chk[start] = (chk[start]&anded)|ored;
  return;
}

/********************************
 * command line arguments things *
 ********************************/
static void free_arg(void)
{
  int i;
  for (i=0; i<argc; i++) {
    free68(argv[i]);
    argv[i] = NULL;
  }
  argc = 0;
}

static int copy_arg(int na, char **a)
{
  int i;
  free_arg();
  if (na>DEBUGGER_MAX_ARG) na=DEBUGGER_MAX_ARG;
  for (i=0; i<na; i++) {
    if (!a[i]) continue;
    argv[i] = strdup68(a[i]);
    if (!argv[i]) {
      return debug68_error_add("Failed to copy command line arguments");
    }
  }
  argc = na;
  return 0;
}

/*********************************
 * 68K environment backup things *
 ********************************/

/*  Restore entering status
 */
static int restore_entering_status( void )
{
/*   reg68_t * const reg68 = &dbg68->reg; */

/*   memcpy(saveemu68->mem, savemem68,                  saveemu68->memsz); */
/*   memcpy(saveemu68->chk, savemem68+saveemu68->memsz, saveemu68->memsz); */
/*   reg68 = savereg68; */
  return 0;
}

static void debugger_destroy_emu(void)
{
  emu68_destroy(dbg68);
  dbg68 = 0;
}

static int debugger_create_emu(emu68_t * slave_emu68)
{
  int err = -1;
  debugger_destroy_emu();
  dbg68 = emu68_duplicate(slave_emu68,"debug68/debugger");
  if (!dbg68)
    goto error;

 error:
  return err;
}
  

/* Debugger mode clean enter
 */
int debug68_entering(emu68_t * emu68)
{
  int err;

  err = debugger_create_emu(emu68);

  /* Save 68K registers and info */

/*   savereg68 = reg68; */

  /* No previous command */
  last_com = last_desa = (unsigned)~0;

  restore_entering_status();

  return 0;
}

/*  Debugger mode clean exit
 */
static int debug68_debugger_exit(int return_code)
{
/*   free_restore(); */
/*   free_arg(); */
  return return_code;
}

static void show_reg(void)
{
  reg68_t * const reg68 = &dbg68->reg;

  int i;
  int ccr = GET_CCR(reg68->sr);
  for(i=0; i<8; i++)
    printf("  D%d=%08X  A%d=%08X\n",i,reg68->d[i],i,reg68->a[i]);
  printf("  PC=%08X USP=%08X\n"
         "  SR=%04X %c%c%c%c%c\n",
         reg68->pc,reg68->usp,
         reg68->sr&0xFFFF,
         (ccr&SR_X) ? 'X' : '-',
         (ccr&SR_N) ? 'N' : '-',
         (ccr&SR_Z) ? 'Z' : '-',
         (ccr&SR_V) ? 'V' : '-',
         (ccr&SR_C) ? 'C' : '-'
         );
}

static int *is_reg_change(char *s);

/* Run a new debugger command. If no arg are given, previous command
 * is used.
 */
int debug68_newcom(int na, char **a)
{
  reg68_t * const reg68 = &dbg68->reg;

  int err;
  if (na>0 && a) {
    if (err=copy_arg(na,a), err<0)
      debug68_debugger_exit(err);
  } else if (argc < 1) {
    return 0;
  }

  if (!strcmp68(argv[0],"x") || !strcmp68(argv[0],"exit")) {
/*     free_restore(); */
    return debug68_debugger_exit(1);
  }

  /* (H)elp */
  else if (!strcmp68(argv[0],"?")    ||
          !strcmp68(argv[0],"help") ||
          !strcmp68(argv[0],"h")) {
    debug68_debugger_help();
    return 0;
  }

  /* (T)race [address] */
  else if (!strcmp68(argv[0],"t")) {
    addr68_t addr = (unsigned)~0;
    int bp;
    char *error;

    if (na>1) {
      addr = debug68_eval(dbg68, argv[1],&error);
      if (error!=NULL) {
	printf( "Can't trace at this address : syntax error\n");
	return 0;
      }
    }
    bp = trace(addr,1,0);
    if (bp >= 0) {
      printf("Reach: ");
      debug68_breakp_display(addr);
    }
  }

  /* show (REG)isters */
  else if (!strcmp68(argv[0],"reg")) {
    show_reg();
  }

  /* set breakpoint ( b [address] [count] [reset] ) */
  else if (!strcmp68(argv[0],"b")) {
    addr68_t addr = (unsigned)-1;
    int count=1, reset=-1, n;
    char *err=NULL;

    if (na>1) {
      addr = debug68_eval(dbg68, argv[1], &err);
      if (err) {
	printf("Invalid breakpoint address : Syntax error\n");
	return 0;
      }
    }
    if (na>2) {
      count = debug68_eval(dbg68, argv[2], &err);
      if (err) {
	printf("Invalid breakpoint count : Syntax error\n");
	return 0;
      }
    }
    if (na>3) {
      reset = debug68_eval(dbg68, argv[3], &err);
      if (err) {
	printf("Invalid breakpoint count-reset : Syntax error\n");
	return 0;
      }
    }
    if (reset==-1) reset=count;

    /* count==0 : KILL */
    if (!count) {
      n = debug68_breakp_kill(addr);
      if (addr==(unsigned)~0) {
	printf("All breakpoint exterminated\n");
      } else {
	if (n>=0) {
	  printf("B%02d killed\n",n);
	} else {
	  printf("Breakpoint not found\n");
	}
      }
    }

    /* SET or DISPLAY */
    else {
      /* addr==-1 : DISPLAY */
      if (addr==(unsigned)~0) {
	n=debug68_breakp_display(addr);
	if (n<=0) printf("No breakpoint\n");
      }
      /* SET */
      else {
	n=debug68_breakp_set(addr,count,reset);
	if (n>=0) {
	  printf("New breakpoint : ");
	  debug68_breakp_display(addr);
	} else {
	  printf("Can't set this breakpoint !!! sorry !!!");
	}
      }
    }
    return 0;
  }

  /* (E)val expression */
  else if (!strcmp68(argv[0],"e")) {
    char *error_loc;
    int v;
    if (na>1) {
      v = debug68_eval(dbg68, argv[1], &error_loc);
      if (error_loc) {
	printf("syntax error at %s\n",error_loc);
      } else {
	printf("=%d ($%X)\n",v,v);
      }
    }
  }

  /* (D)isassemble [@{=PC}] [lines{=default_desa}] */
  else if (!strcmp68(argv[0],"d")) {
    addr68_t addr = last_desa/*(unsigned)~0*/;
    int ndesa = 0;
    char *err;
    if (na>1) {
      addr = debug68_eval(dbg68, argv[1], &err);
      if (err) {
	printf("Invalid disassemble address : Syntax error\n");
	return 0;
      }
    }
    if (na>2) {
      ndesa = debug68_eval(dbg68, argv[2], &err);
      if (err) {
	ndesa = 0;
      }
    }
    line();
    desa(addr, ndesa);
    line();
  }

  /* (M)emory dump [@] [# bytes] */
  else if (!strcmp68(argv[0],"m")) {
    char *err=0;
    u32  addr;
    int  n=16,i;
    
    if (na<2) {
      printf("use : m @  [# bytes]\n");
      return 0;
    }

    if (na>1) {
      addr = debug68_eval(dbg68, argv[1],&err);
    }
    if (na>2 && !err) {
      n = debug68_eval(dbg68, argv[2],&err);
    }
    if (n<=0) n = 1;

    if (err) {
      printf("Invalid memory address syntax");
      return 0;
    }

    line();
    for(i=0; i<n ; i+=16 ) {
      int j,l;
      printf("%06X ",(addr+i)&0xFFFFFF);
      l = n-i;
      if (l>16) l=16;
      for(j=0; j<l; j++)
	printf("%02X",emu68_peek(dbg68, addr+i+j));
      for(  ; j<16; j++) printf("  ");
      printf("|");
      
      for(j=0; j<l; j++) {
	u8 c;
	c = emu68_peek(dbg68, addr+i+j);
	printf("%c", isgraph(c) ? c : '.');
      }
      for(  ; j<16; j++) printf(" ");
      printf("\n");
    }
    line();
  }

  /* (R)un [pc] [max-instruction] */
  else if (!strcmp68(argv[0],"r")) {
    addr68_t addr = (unsigned)~0;
    s32 savesp;
    unsigned max = DEFAULT_MAX_EXEC, ct;
    char *err;
    
    if (na>1) {
      addr = debug68_eval(dbg68, argv[1],&err);
      if (err!=NULL) {
	printf("Invalid run address");
	return 0;
      }
      reg68->pc = addr;
    }
    
    if (na>2) {
      max = debug68_eval(dbg68, argv[2],&err);
      if (err!=NULL) {
	printf("Invalid instruction limit");
	return 0;
      }
    }

    for(savesp=reg68->a[7], ct=0;
	(max==0 || ct!=max) && reg68->a[7]<=savesp; ct++) {
      int bp;
      bp = trace((unsigned)~0,0,1);
      if (bp>=0) {
	printf("Reach: ");
	debug68_breakp_display(reg68->pc);
	return 0;
      }
    }
    /*if (reg68->a[7]>savesp)
	reg68->a[7]=savesp;*/
    if (ct==max) {
      printf("%u instruction%s executed\n",max,max>1 ? "s" : "");
      return 0;
    }
  }

  /* Run until pc=value <value> [max-instruction]*/
  else if (!strcmp68(argv[0],"u")) {
    addr68_t addr = (unsigned)~0;
    unsigned max  = DEFAULT_MAX_EXEC, ct, breakat;
    char *err;
    
    if (na<2) {
      printf("Missing break point address\n");
      return 0;
    }

    addr = debug68_eval(dbg68, argv[1],&err);
    if (err) {
      printf("Invalid break point address");
      return 0;
    }
    breakat = addr&dbg68->memmsk;

    if (na>2) {
      max = debug68_eval(dbg68, argv[2],&err);
      if (err) {
	printf("Invalid instructions limit");
	return 0;
      }
    }
    
    for(ct=0;
	(max==0 || ct!=max) && (reg68->pc&dbg68->memmsk)!=(dbg68->memmsk&breakat);
	ct++) {
      int bp;
      bp = trace((unsigned)~0,0,1);
      if (bp>=0) {
	printf("Reach: ");
	debug68_breakp_display(reg68->pc);
	return 0;
      }
    }
    if ((reg68->pc&dbg68->memmsk)!=(dbg68->memmsk&breakat)) {
      printf("%u instrction%s executed\n",max,max>1 ? "s" : "");
      return 0;
    }
  }

  /* Save binary */
  else if (!strcmp68(argv[0],"sbin")) {
    if (argc<4) {
      printf("sbin filename start-@ size\n");
      return 0;
    } else {
      istream68_t *os=0;
      unsigned int start,size;
      int err=0;
      char *errstr=NULL;
      
      u8 * onboard;

      if (start=debug68_eval(dbg68, argv[2],&errstr),errstr!=NULL)
	return debug68_error_add("Invalid start address");
      if (size=debug68_eval(dbg68, argv[3],&errstr),errstr!=NULL)
	return debug68_error_add("Invalid size");
      onboard = emu68_memptr(dbg68, start,size);
      if (!onboard) {
	debug68_error_add(emu68_error_get(dbg68));
	return -1;
      }
      if (os=istream68_file_create(argv[1],2), !os)
	return -1;
      if (istream68_open(os)) {
	istream68_destroy(os);
	return -1;
      }
      err =
	istream68_write(os, onboard, size) != size;
      istream68_destroy(os);
      if (!err)
	printf("[%x-%x] (%d bytes) => <%s>\n",
	       start,start+size-1,size,argv[1]);
      return -!!err;
    }
  }
  
  /* Set register */
  else {
      int *p;
      if (p=is_reg_change(a[0]), p) {
	char *n=NULL,*err=NULL;
	int v=0;
	for(n=a[0]+3; *n && isspace(*n); n++);
	if (*n==0 && argc>1)
	  for(n=a[1]; *n && isspace(*n); n++);
	if (*n==0 || (v=debug68_eval(dbg68, n,&err),err!=NULL))
	  printf("Bad value for register\n");
	else *p = v;
      } else {
	printf("Unknown command [%s] (? for help)\n", argv[0]);
      }
  }

  return 0;
}

/* @return pointer to register
 * @retval 0 not a REG= command
 */
static int68_t * is_reg_change(char *s)
{
  reg68_t * const reg68 = &dbg68->reg;

  if (!s || strlen(s)<3) {
    return 0;
  }
  if (s[2] != '=') {
    return 0;
  }

  switch (toupper(*s)) {
  case 'D':
    return (s[1]<'0' || s[1]>'7') ? 0 : reg68->d+(s[1]-'0');
  case 'A':
    return (s[1]<'0' || s[1]>'7') ? 0 : reg68->a+(s[1]-'0');
  case 'P':
    return (toupper(s[1])!='C') ? 0 : &reg68->pc;
  case 'S':
    switch (toupper(s[1])) {
    case 'R': return (int*)&reg68->sr;
    case 'P': return &reg68->usp;
    }
    break;
  }
  return 0;
}
