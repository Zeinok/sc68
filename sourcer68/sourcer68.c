/*
 *                    sourcer68 - 68000 sourcer
 *             Copyright (C) 2001-2009 Benjamin Gerard
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 */

/* $Id$ */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#if defined(HAVE_BASENAME) && !defined(_GNU_SOURCE) && defined(HAVE_LIBGEN_H)
# include <libgen.h>
#endif

#include <desa68.h>

#ifdef USE_FILE68
# include <sc68/file68_api.h>
# include <sc68/init68.h>
# include <sc68/error68.h>
# include <sc68/alloc68.h>
# include <sc68/file68.h>
# include <sc68/url68.h>
# include <sc68/debugmsg68.h>
# include <sc68/option68.h>

static void debugmsg(const char * fmt, ...)
{
  static int sourcer68_feature = -1;
  va_list list;
  if (sourcer68_feature == -1) {
    sourcer68_feature =
      debugmsg68_feature("src","sourcer68 specific message",1);
  }
    va_start(list,fmt);
  if (sourcer68_feature != -1) {
    vdebugmsg68(sourcer68_feature,fmt,list);
  } else {
    vfprintf(stderr,fmt,list);
  }
  va_end(list);
}

#else

static void debugmsg(const char * fmt, ...)
{
  va_list list;
  vfprintf(stderr,fmt,list);
  va_end(list);
} 

#endif

#define TOS_RELOC_NEVER   0
#define TOS_RELOC_AUTO    1
#define TOS_RELOC_ALWAYS  2

#define SRC68_RELOC (1<<7) /**< Color relocated memory              */
#define SRC68_INST  (1<<6) /**< Color valid instruction cels        */
#define SRC68_ENTRY (1<<5) /**< Color entry points (need Label def) */
#define SRC68_RTS   (1<<4) /**< Color rts instructions              */
#define SRC68_DONE  (1<<3) /**< Color processed memory              */
/* bits 0-2 are reserved for opcode size (in word) */

static DESA68parm_t desa;
static char desastr[512];

static char * tabs = "\t";
static int opcode = 0;
static int ascii = 0;
static int no_symbol = 0;

static int start = 0;
static int end   = 0;

static unsigned char * mem_data = 0;
static unsigned char * chk_data = 0;
static int mem_datasz;

static char error_str[4][256];
static int  error_cnt = 0;

static const char * error_get(void * dummy)
{
  if (error_cnt > 0) {
    return error_str[--error_cnt];
  } else
    return 0;
}

static int error_add(void * dummy, const char * s, ...)
{
  if (error_cnt < 4) {
    va_list list;
    va_start(list,s);
    vsnprintf(error_str[error_cnt++],255,s,list);
    va_end(list);
  }
  return -1;
}

typedef struct {
  const char * name;
  unsigned int addr;
  unsigned int size;
  struct {
    int exe:1;
    int clr:1;
  } flags;
} section_t;



static int sections;
static section_t section[8];

static int cmp_section(const void *a, const void * b)
{
  const section_t * sa = (const section_t *)a;
  const section_t * sb = (const section_t *)b;
  return sa->addr - sb->addr;
}

static int add_section(const char * name, int addr, int size, int exe, int clr)
{
  if (sections == sizeof(section)/sizeof(*section )) {
    error_add(0,"Too many sections");
    return -1;
  }
  section[sections].name = name;
  section[sections].addr = addr;
  section[sections].size = size;
  section[sections].flags.exe = exe;
  section[sections].flags.clr = clr;

  ++sections;
  qsort(section, sections, sizeof(*section), cmp_section);
  return 1;
}

static void Warning(int addr, const char * fmt, ...)
{
  va_list list;

  va_start(list,fmt);
  fprintf(stderr, "warning:$%06X: ", addr);
  vfprintf(stderr, fmt, list);
  fputs("\n",stderr);
  va_end(list);

  va_start(list,fmt);
  fprintf(stdout, "; ** WARNING ** $%06X: ", addr);
  vfprintf(stdout, fmt, list);
  fputs("\n", stdout);

  va_end(list);
}


static int SpoolError(int code)
{
  const char * s = error_get(0);
  if (!s) {
    return 0;
  } else {
    do {
      fprintf(stderr, "%s\n",s);
    } while (s = error_get(0), s);
    return code;
  }
  return 0;
}

static void CleanErrorStack(void)
{
  while(error_get(0));
}

#if 0
static unsigned int FindNextColored(unsigned int from, unsigned to)
{
  for (; from<to; from += 2) {
    if ( (chk_data[from] | chk_data[from+1]) & SRC68_DONE) {
      return from;
    }
  }
  return 0x01000000;
}
#endif

static void WriteL(unsigned char *b, int v)
{
  b[0] = v>>24;
  b[1] = v>>16;
  b[2] = v>>8;
  b[3] = v;
}

static int ReadL(unsigned char *b)
{
  return
    (((int)*(signed char*)b)<<24) +
    (b[1]<<16) +
    (b[2]<<8) +
    b[3];
}

static int ReadW(unsigned char *b)
{
  return
    (((int)*(signed char*)b)<<8) +
    b[1];
}

static void PrintLabel(unsigned int addr)
{
  printf("L%06X: \n", addr);
}

static void PrintRelLabel(unsigned addr, unsigned int base)
{
/*   if (base == pc) { */
    printf("L%06X: = *+%d \n", addr, addr-base);
/*   } else { */
/*     printf("L%06X: = L%06X+%d \n", addr, base, addr-base); */
/*   } */
}

#if 0
static int CheckSection(unsigned int pc, int newline)
{
  int i, found = 0;

  for (i=0; i<sections; ++i) {
    if (pc == section[i].addr) {
      if (!found) {
	found = 1;
	printf("\n\n"+!(newline));
	printf("%s; ------------------------------------\n",tabs);
      }
      printf("%s; SECTION %4s ;%06x-%06x (%6d)\n",
	     tabs,
	     section[i].name, section[i].addr,
	     section[i].addr+section[i].size, section[i].size);
    }
  }
  if (found) {
    printf("%s; ------------------------------------\n",tabs);
    printf("\n");
  }
  return found;
}
#endif

static void PrintDCW(unsigned int from, unsigned int to)
{
  const int collect = 8;
  int n = to - from;
  int i = 0;
  int current_size = 0;
  int block_size = 0;

/*   printf("; Do DATA [%06X-%06X]\n",to,from); */

  if (n<0 || n&1) {
/*     BREAKPOINT68; */
  }
  n >>= 1;

  do {
    int wanted_size;
    int label, reloc;
    int j;

/*     if (CheckSection(from, current_size)) { */
/*       current_size = 0; */
/*     } */

    reloc = !!(chk_data[from] & SRC68_RELOC);
    wanted_size = current_size;

    if (reloc) {
      wanted_size = 2;
    } else if (n<2 || (chk_data[from+2] & (SRC68_RELOC|SRC68_ENTRY))) {
      wanted_size = 1;
    } else if (!wanted_size) {
      wanted_size = block_size ? block_size : 2;
    }

    if (wanted_size > n) {
      wanted_size = n;
      if (reloc) {
	if (current_size) {
	  current_size = 0;
	  printf("\n");
	}
	Warning(from,"Missing data for relocation");
      }
    }

    if (!block_size) {
      block_size = wanted_size;
    }

    /* Check for LABEL needs in that disassembly. */
    for (label=j=0; j < (wanted_size<<1); ++j) {
      label |= (!!(chk_data[from+j]&SRC68_ENTRY)) << j;
    }

    if (current_size && (label || (wanted_size != current_size))) {
      /* Need a newline here */
      printf("\n");
      current_size = 0;
    }

    /* Print LABEL */
    if (label) {
      block_size = 0;
      if (label&1) {
	PrintLabel(from);
      }
      for (j = 1, label >>= 1; label; ++j, label >>= 1) {
	if (label & 1) {
	  PrintRelLabel(from+j, from);
	}
      }
    }

    if (!current_size) {
      printf("%sDC.%c ", tabs, (wanted_size==1) ? 'W' : 'L');
      block_size = current_size = wanted_size;
      i = 0;
    } else {
      printf(",");
    }

    if (current_size == 1) {
      printf("$%04x", ReadW(mem_data+from)&0xFFFF);
    } else {
      unsigned int v = ReadL(mem_data+from);
      if (no_symbol || !reloc) {
	printf("$%08x", v);
      } else {
	printf("L%06X", v);
	if (!(chk_data[v]&SRC68_ENTRY)) {
	  if (v < from) {
	    printf("\n");
	    i = collect; /* Force newline */
	    Warning(from, "Adding backward label L%06X", v);
	    PrintRelLabel(v, from);
	  } else {
	    printf("\n");
	    i = collect; /* Force newline */
/* 	    Warning(from, "Adding forward label L%06X", v); */
	    chk_data[v] |= SRC68_ENTRY;
	  }
	}
      }
    }
    i += current_size;
    from += current_size << 1;
    n -= current_size;

    if (i >= collect) {
      current_size = 0;
      printf("\n");
    }

  } while(n>0);

  if (n < 0) {
    printf("\n; ** ERROR, too many data process by DCW !!\n");
    exit(127);
/*     BREAKPOINT68; */
  }

  if (current_size) {
    printf("\n");
  }
}

static void ExeSectionDesa(section_t * section)
{
  unsigned int pc, opc, from, to;

  from = section->addr;
  to   = from + section->size;

  for (opc=pc=from; pc < to; ) {

    if (chk_data[pc] & SRC68_INST) {
      /* Got a valid instruction: Need to flush data. */
      if (opc != pc) {
        chk_data[opc] |= SRC68_ENTRY;
        PrintDCW(opc,pc);
        printf("\n");
      }

      /* Check memory access entry point (added by symbolic disassembly) */
      if (desa.ea_src <= desa.memmsk) {
	if (!(chk_data[desa.ea_src]&SRC68_ENTRY)) {
	  if (desa.ea_src < pc) {
	    Warning(pc, "Adding backward label L%06X", desa.ea_src);
	    PrintRelLabel(desa.ea_src, pc);
	  } else {
	    chk_data[desa.ea_src] |= SRC68_ENTRY;
	  }
	}
      }

      if (desa.ea_dst <= desa.memmsk) {
	if (!(chk_data[desa.ea_dst]&SRC68_ENTRY)) {
	  if (desa.ea_dst<pc) {
	    Warning(pc, "Adding backward label L%06X", desa.ea_dst);
	    PrintRelLabel(desa.ea_dst, pc);
	  } else {
	    chk_data[desa.ea_dst] |= SRC68_ENTRY;
	  }
	}
      }

      if (chk_data[pc] & SRC68_RELOC) {
	Warning(pc, "Disassembly a relocated position");
      }

      if (chk_data[pc] & SRC68_ENTRY) {
        PrintLabel(pc);
      }
      desa.pc = pc;

      /* Set force symbol for disassembly */
      opc = 0 ;
      if (!no_symbol) {
	unsigned int sz = chk_data[pc] & 7;
	for (; opc < sz; ++opc) {
	  if (chk_data[pc + (opc<<1)] & SRC68_RELOC) {
	    desa.flags |= (DESA68_FORCESYMB_FLAG<<opc);
	  } else {
	    desa.flags &= ~(DESA68_FORCESYMB_FLAG<<opc);
	  }
	}
      }
      for (;opc < 5; ++opc) {
	desa.flags &= ~(DESA68_FORCESYMB_FLAG<<opc);
      }

      desa68(&desa);
      if (((desa.pc-pc)>>1) != (chk_data[pc]&7)) {
	Warning(pc, "opcode size has changed between pass (%d != %d)",
		(desa.pc-pc)>>1,chk_data[pc]&7);
      } 

      /* Check ENTRY inside instruction */
      for (opc = pc+1; opc<desa.pc; opc++) {
        if (chk_data[opc] & SRC68_ENTRY) {
          PrintRelLabel(opc, pc);
        }
      }

      /* Print disassembly */
      printf("%s%s", tabs, desa.str);
      if (opcode) {
	/* Print opcodes */
	int l=strlen(tabs) + strlen(desa.str);
	while (++l < 32) fputc(' ',stdout);
	printf(";");
	for (opc=pc; opc<desa.pc; ++opc) {
	  printf("%c%02X",
		 chk_data[opc]&SRC68_RELOC?'.':' ',
		 mem_data[opc]&255);
	}
      }
      printf("\n");
      /* Supplemental newline for RTS makes things prettier */
      if (chk_data[pc] & SRC68_RTS) {
        printf("\n");
      }
      pc = opc = desa.pc;
    } else {
      pc += 2;
    }
  }

  if (opc != pc) {
    chk_data[opc] |= SRC68_ENTRY;
    PrintDCW(opc,pc);
  }
}

static void SectionDesa(section_t * section, section_t * next_section)
{
  unsigned int from, to;

  printf("\n");
  printf("%s; ------------------------------------\n",tabs);
  printf("%s; SECTION %4s ;%06x-%06x (%6d)\n", tabs,
	 section->name, section->addr,
	 section->addr+section->size, section->size);
  printf("%s; ------------------------------------\n\n",tabs);

  from = section->addr;
  to   = from+section->size;

  if (section->flags.clr) {
/*     PrintLabel(from); */
    for (; ++from < to;) {
      if (chk_data[from]&SRC68_ENTRY) {
	PrintRelLabel(from,section->addr);
      }
    }
    if (section->size) {
      printf("%sDS.B %d\n", tabs, section->size);
    }
  } else if (!section->flags.exe) {
    chk_data[from] |= SRC68_ENTRY;
    PrintDCW(from,to);
  } else {
    printf("%sORG $%06X\n\n",tabs, section->addr);
    ExeSectionDesa(section);
  }

  if (next_section) {
    unsigned int next_addr = next_section->addr;
    if (next_addr > to) {
      printf("%sDS.B %d ; Pad to section %s\n", tabs,
	     next_addr-to, next_section->name);
      next_addr = to;
    }
    if (next_addr != to) {
      PrintRelLabel(next_addr, next_addr);
    }
  } else {
    PrintLabel(to);
  }
}

static void FileHeader(char *fname, int *entries)
{
  int *e;
  int i;

  printf("; file     : \"%s\"\n", fname);
  printf("; sections : %d\n", sections);
  for (i=0; i <sections; ++i) {
    printf("; - %6s : $%06X-$%06X (%6d)\n", section[i].name,
	   section[i].addr, section[i].addr+section[i].size, section[i].size);
  }
  if (entries && *entries!=-1) {
    printf("; entries  :");
    for (i=0, e=entries; *e != -1; ++e,++i) {
      int v = *e, range=(v>>24)&1;
      if (i >= 7 && !range) {
	i=0;
	printf("\n;           ");
      }
      printf("%c$%06X"," -"[range], v&0xFFFFFF);
    }
  }
  printf(
    "\n"
    ";\n"
    "; sourcer68 - Copyright (C) 2001-2003 Benjamin Gerard\n"
    ";\n");
}
  
  

static int FinalDesa(char *fname, int *entries)
{
  int i;

  FileHeader(fname, entries);
  desa.flags = 0;
  if (!no_symbol) {
    desa.flags |= DESA68_SYMBOL_FLAG;
  }
  if (ascii) {
    desa.flags |= DESA68_ASCII_FLAG;
  }
  desa.immsym_min = section[0].addr;
  desa.immsym_max = section[sections-1].addr + section[sections-1].size + 1;
  for (i=0; i<sections; ++i) {
    SectionDesa(section+i, i+1<sections ? section+i+1 : 0);
  }

  return 0;
}

static void Color(unsigned int from, unsigned int to, int color)
{
  for (; from<to; ++from) {
    chk_data[from] |= color;
  }
}

static int rPreSource(void)
{
  unsigned int pc;
  int test_jmp_table=0;
  int status;

  pc = desa.pc;
  if ( (pc & 1) || pc > desa.memmsk) {
    fprintf(stderr,"; ERROR: address %06X\n", pc);
    return -1;
  }
  chk_data[pc] |= SRC68_ENTRY;

  do {
    pc = desa.pc;

    /* Can't disassemble odd memory or outside memory. */
    if (pc & 1 || pc > desa.memmsk) {
      fprintf(stderr,"; ERROR: address %06X\n", pc);
      return -1;
    }

    if (chk_data[pc] & SRC68_DONE) {
      return 0;
    }

    /* Disassemble */
    desa68(&desa);
    status = desa.status;

    /* Set instruction length. */
    chk_data[pc] = (chk_data[pc]&0xF8) | ((desa.pc-pc)>>1);

    /* If just testing jump table, exit if not BRA or RTS or nop */
    if (test_jmp_table &&
	(!( desa.status & (DESA68_BRA|DESA68_BSR|DESA68_RTS|DESA68_NOP)))) {
      return 0;
    }

    /* Color this cels */
    Color(pc, desa.pc, SRC68_DONE | ((status&DESA68_INST) ? SRC68_INST : 0));

    if (status & DESA68_INST) {

      /* Check memory access entry point */
      if (desa.ea_src != -1) {
        chk_data[desa.ea_src] |= SRC68_ENTRY;
      }
      if (desa.ea_dst != -1) {
        chk_data[desa.ea_dst] |= SRC68_ENTRY;
      }

      if (status & (DESA68_BRA|DESA68_BSR)) {
        pc      = desa.pc;
        desa.pc = desa.branch;
        /* Do not follow JMP (An) ... */
        if (desa.branch != -1) {
          rPreSource();
        }
        desa.pc = pc;
	/* Set jump table if BRA, remove other */
        test_jmp_table = ! (status & DESA68_BSR);
      } else if (desa.status & DESA68_RTS) {
        if (!test_jmp_table) {
          chk_data[pc] |= SRC68_RTS;
          return 0;
        }
      }
    } else {
      return 0;
    }

  } while(1);

}


static int PreSource(unsigned int from, unsigned int to, int *entries)
{
  int pipo[2] = {from,-1};
  if (!entries || *entries==-1) {
    entries = pipo;
  }

  desa.flags = 0;
  desa.immsym_min = from;
  desa.immsym_max = to;

  while (desa.pc = *entries++, desa.pc != -1) {
    int pc = desa.pc;
    int range = *entries;

    /* Start disassembly. */
    rPreSource();
    if (range & 0x1000000 && range != -1) {
      range &= 0xFFFFFF;
      ++entries;
      do {
	/* Skip instruction already disassembled */
	while (pc < range && (chk_data[pc] & SRC68_DONE)) {
	  int bytes = (chk_data[pc]&7) << 1;
	  if (!bytes) {
	    bytes = 2;
	  }
	  pc += bytes;
	}
	if (pc < range) {
	  desa.pc = pc;
	  rPreSource();
	}
      } while (pc < range);
    }
  }
  return 0;
}


static int Init68k(unsigned int size)
{
  void * data;
  unsigned int alloc_size;

  alloc_size = size<<1;
  data = malloc(alloc_size);
  if (!data) {
    error_add(0,"Init68k(%d) : %s", alloc_size, strerror(errno));
    return -1;
  }
  mem_datasz = size;
  mem_data   = data;
  chk_data   = mem_data+size;
  memset (data,0,alloc_size);

  desa.mem    = data;
  desa.memmsk = size-1;
  desa.pc     = 0;

  desa.str    = desastr;
  desa.strmax = sizeof(desastr)-1;
  desa.str[0] = desa.str[sizeof(desastr)-1] = 0;

  desa.flags = 0;
  return 0;
}



/**
 * @param b         Base of 68000 memory
 * @param b_sz      Size of 68000 memory
 * @param addr      Offset to TOS program to reloc
 * @param n         Size of TOS program file.
 * @param clear_bss Clear BSS flag (0:not cleared)
 */

static int TOSreloc(unsigned char *b, int b_sz, int addr, int n,
		    int clear_bss, int check_only)
{
  int const header_size = 0x1c;
  int symbol, symbol_sz;
  int reloc, reloc_sz;
  int end, size;
  int magic;

  int text, text_sz;
  int data, data_sz;
  int bss, bss_sz;

  if (n < header_size) {
    return error_add(0,"; Too few bytes (%d) to be a TOS executable.", n);
  }

  magic = ReadW(b+addr);
  if (magic != 0x601a) {
    return error_add(0,"Invalid TOS executable header. "
		     "Expected 0x601a, found %02X.", magic);
  }

  text_sz   = ReadL(b+addr+2);
  data_sz   = ReadL(b+addr+2+4);
  bss_sz    = ReadL(b+addr+2+8);
  symbol_sz = ReadL(b+addr+2+12);
  size      = text_sz + data_sz + symbol_sz;
  reloc_sz  = (n-header_size)-size;

  if (reloc_sz < 0) {
    return error_add(0,"Sections size greater than file size (%d>%d).",
		     size, n-header_size);
  }

  text    = addr+header_size; /* TEXT section begins just after the header. */
  data    = text+text_sz;     /* DATA section stand after TEXT.             */
  bss     = data+data_sz;     /* BSS stands after DATA.                     */
  symbol  = bss;              /* SYMBOL stands at the BSS.                  */
  reloc   = symbol+symbol_sz; /* RELOCATION table stands after SYMBOL.      */
  end       = text+text_sz+data_sz+bss_sz;
  if (end > b_sz) {
    return error_add(0,"Not emougth space in 68K memory reloc this program. "
			 "Missing %d bytes.", end-b_sz);
  }

  if (!check_only) {
    add_section("TOS",addr,header_size,1,0);
    add_section("TEXT",text,text_sz,1,0);
    add_section("DATA",data,data_sz,0,0);
/*     add_section("SYMBOL",symbol,symbol_sz,0,0); */
/*     add_section("RELOC",reloc,reloc_sz,0,0); */
    add_section("BSS",bss,bss_sz,0,1);
  }

  if (reloc_sz) {
    int reloc_ptr, d5;
    if (reloc_sz < 4) {
      return error_add(0,"RELOCATION section size (%d) must > than 4. (%d).",
		       reloc_sz);
    }

    reloc_ptr = text;
    d5 = ReadL(b+reloc); reloc+=4;
    while (d5) {
      reloc_ptr += d5;
      if (d5 == 1) {
        reloc_ptr += 253;
      } else if (d5&1) {
        return error_add(0,"RELOCATION table corrupt: "
			 "unexpected odd value (%d).", d5);
      } else {
        int v;
        if (reloc_ptr >= end) {
          return error_add(0,"RELOCATION goes outside 68000 memeory.");
        }
        v = ReadL(b+reloc_ptr);
        v += text;
        if (!check_only) {
          WriteL(b+reloc_ptr, v);
	  chk_data[reloc_ptr] |= SRC68_RELOC;
        }
        d5 = 0;
      }
      if (reloc_ptr >= addr+n) {
        return error_add(0,"RELOCATION table must be corrupt.");
      }
      d5 = b[reloc++];
    }
  }

  if (!check_only && clear_bss && bss_sz>0) {
/*     memset(b+bss,0,bss_sz); */
  }

  return 0;
}

static char * LoadBinary(char * fname, int * fsize)
{
  int size;
  char *b = 0;


#ifdef USE_FILE68

  istream68_t *is = url68_stream_create(fname,1);

  if (istream68_open(is) == -1) {
    goto error;
  }
  size = istream68_length(is);
  if (size < 0) {
    goto error;
  }

  b = alloc68(size);
  if (!b) {
    goto error;
  }

  if (istream68_read(is, b,size) != size) {
    goto error;
  }
  if (fsize) {
    *fsize = size;
  }
  istream68_destroy(is);
  return b;

error:
  istream68_destroy(is);
  free68(b);
  return 0;

#else

  FILE * in = fopen(fname,"rb");
  if (!in) {
    goto error;
  }
  if (fseek(in,0L,SEEK_END) == -1) {
    goto error;
  }
  size = ftell(in);
  if (size == -1) {
    goto error;
  }
  if (fseek(in,0L,SEEK_SET) == -1) {
    goto error;
  }

  b = malloc(size);
  if (!b) {
    goto error;
  }

  if (fread(b,1,size,in) != size) {
    goto error;
  }
  if (fsize) {
    *fsize = size;
  }
  fclose(in);
  return b;

error:
  if (in) fclose(in);
  free(b);
  return 0;
#endif
}

static char * Basename(char *name);
static char *IsOption(char *a,char *o);
static int * parse_entry(char *entries, unsigned char *mem, int from, int to);

/* Display version number. */
static int print_version(void)
{
  puts
    (PACKAGE_STRING "\n"
     "\n"
     "Copyright (C) 2001-2009 Benjamin Gerard.\n"
     "License GPLv3+ or later <http://gnu.org/licenses/gpl.html>\n"
     "This is free software: you are free to change and redistribute it.\n"
     "There is NO WARRANTY, to the extent permitted by law.\n"
     "\n"
     "Written by Benjamin Gerard <" PACKAGE_BUGREPORT ">"
     );
  return 0;
}

static void print_option(void * data, 
			 const char * option,
			 const char * envvar,
			 const char * desc)
{
  fprintf(data, 
	  "  %s or `$%s'\n"
	  "     %s \n",
	  option, envvar, desc);
}	 

static int print_usage(void)
{
  puts
    (
     "Usage: source68 [OPTIONS] <input>\n"
     ""
     "  A 68000 disassembler/sourcer\n"
     "\n"
#ifndef USE_FILE68
     "Input: binary or TOS file\n"
#else
     "Input: sc68 or binary or TOS file\n"
#endif
     "\n"
     "Options:\n"
     "  --help                 This message and exit\n"
     "  --version              Display version and exit\n"
     "  --reloc=[yes|no|auto]  TOS relocation (default:auto).\n"
     "  --entry=[ENTRY-LIST]   Set disassembly entry points (default:+0)\n"
     "  --tab=[STRING]         Set tabulation string\n"
     "  --org=[ADDR]           Load address (default:0x8000)\n"
     "  --opcode               Print opcodes\n"
     "  --ascii                Convert immediat to ASCII\n"
     "  --no-symbol            Disable symbol in disassembly output\n");

#ifdef USE_FILE68
  option68_help(stdout,print_option);
#endif
  puts
    (
     "  ENTRY-LIST := ENTRY|RANGE(,ENTRY|RANGE ...)\n"
     "\n"
     "  It consist on a coma ',' separated list of ENTRY or RANGE.\n"
     "\n"
     "  Basically an ENTRY is a number corresponding to an address\n"
     "  where to start a disassemble pass.\n"
     "\n"
     "  RANGE := [ENTRY,ENTRY]\n"
     "\n"
     "  A range starts as many passes needed to disassemble the range.\n"
     "  Using one or more indirection 'l' before a range will use the range\n"
     "  as an indirect table of entry. And using a '+' before a range adds\n"
     "  origin to each value of the range (for all indirections too).\n"
     "  Expression can be used mixing \"C\" like numbers, operators,\n"
     "  and parenthesis '()'. Operators are '+-/*&|^<>' with '<' and '>' for\n"
     "  shifting. Warning: NO PRECEDENCE !!!.\n"
     "\n"
     "Copyright (C) 2001-2009 Benjamin Gerard\n"
     "\n"
     "Report bugs to <benjihan -4t- users.sourceforge -d0t- net>"
     );
  return 1;
}


int main(int na, char **a)
{
  int i;
  char *fname = 0, *s;
#ifdef USE_FILE68
  disk68_t *d=0;
#endif
  char * buf = 0;
  int fsize=0;

#ifdef WITH_EMU68
  int size68=512<<10;
#else
  int size68=2048<<10;
#endif

  int tosreloc = TOS_RELOC_AUTO;
  char *entry="+0";
  int *entries = 0;
  int org = 0x8000;

#ifdef USE_FILE68
  alloc68_set(malloc);
  free68_set(free);
  na = file68_init(a, na);
#endif

  for (i=1; i<na; ++i) {
    if (!strcmp(a[i],"--help")) {
      return print_usage();
    } else if (!strcmp(a[i],"--version")) {
      return print_version();
    } else if (!strcmp(a[i], "--opcode")) {
      opcode = 1;
    } else if (!strcmp(a[i], "--ascii")) {
      ascii = 1;
    } else if (!strcmp(a[i], "--no-symbol")) {
      no_symbol = 1;
    } else if (s = IsOption(a[i],"--org="), s) {
      char * err = 0;
      int v = strtol(s, &err, 0);
      if (err == s || *err || v<0 || v>0xFFFFFF) {
        fprintf(stderr, "--org bad value. Try --help.\n");
	return 2;
      }
      org = v;
    } else if (s = IsOption(a[i],"--reloc="), s) {
      if (!strcmp(s, "no")) {
        tosreloc = TOS_RELOC_NEVER;
      } else if (!strcmp(s, "yes")) {
        tosreloc = TOS_RELOC_ALWAYS;
      } else if (!strcmp(s, "auto")) {
        tosreloc = TOS_RELOC_AUTO;
      } else {
        fprintf(stderr, "--reloc bad value. Try --help.\n");
        return 2;
      }
    } else if (s = IsOption(a[i],"--entry="), s) {
        entry = s;
    } else if (s = IsOption(a[i],"--tab="), s) {
        tabs = s;
    } else if (!fname) {
      fname = a[i];
    } else {
      fprintf(stderr, "Too many parameters. Try --help.\n");
      return 2;
    }
  }

  /* Check if input filename was given */
  if (!fname) {
    fprintf(stderr, "Missing input file. Try --help.\n");
    return 3;
  }
  debugmsg("sourcer68: input is '%s'\n",fname);

  /* Init 68000 memory buffer */
  if (Init68k(size68)) {
    return SpoolError(5);
  }

  /* Verify if input is an sc68 file */
#ifdef USE_FILE68
  if (!file68_verify_url(fname)) {
    music68_t * m;
    debugmsg("sourcer68: sc68 file detected\n");

    d = file68_load_url(fname);
    if (!d) {
      return SpoolError(4);
    }
    m = d->mus+(d->default_six%(unsigned)d->nb_six);
    debugmsg("sourcer68: sc68 using default music %d/%d\n",
	     d->default_six,d->nb_six);
    

    if (m->replay) {
      debugmsg("sourcer68: sc68 use external replay '%s'\n",
	       m->replay);

      error_add(0, "Music with external replay (%s) not yet supported",
		m->replay);
      return SpoolError(4);
    }

    buf = m->data;
    org = m->a0;
    fsize = m->datasz;

  }
#else
  if (0) {}
#endif
  else {
    CleanErrorStack();
    buf = LoadBinary(fname, &fsize);
    if (!buf) {
      return SpoolError(5);
    }
  }

  if (buf) {
    int addr = org;

    if (addr+fsize > mem_datasz) {
      error_add(0,"Data out range [%x-%x] > %x\n",
		addr,addr+fsize-1,mem_datasz);
      return SpoolError(6);
    } else {
      memcpy(mem_data+addr,buf,fsize);
    }

    if (tosreloc == TOS_RELOC_AUTO) {
      int err = TOSreloc(mem_data, mem_datasz, addr, fsize, 0, 1);
      if (err < 0) {
        const char *s = error_get(0);
        fprintf(stderr, "; Not a TOS file: %s\n", s ? s : "no error !");
        tosreloc = TOS_RELOC_NEVER;
      }
    }

    if (tosreloc != TOS_RELOC_NEVER) {
      if (TOSreloc(mem_data, mem_datasz, addr, fsize, 0, 0) < 0) {
        return SpoolError(6);
      }
    } else {
      add_section("TEXT",addr, fsize, 1, 0);
    }

    start = end = 0;
    if (sections < 1) {
      error_add(0,"No section found");
    } else {
      start = section[0].addr;
      end   = section[sections-1].addr + section[sections-1].size;
      entries = parse_entry(entry, mem_data, start, end);
      SpoolError(0);
      for (i=0; i<sections; ++i) {
	if (section[i].flags.exe) {
	  PreSource(section[i].addr, section[i].addr+section[i].size, entries);
	}
      }
    }
    FinalDesa(Basename(fname), entries);
  }

#ifdef USE_FILE68
  if (d) {
    free68(d);
    buf = 0; /* buf was not mallocated. */
  }
#endif

  free(buf);
  free(entries);

  return 0;
}


static char *IsOption(char *a,char *o)
{
  return (strstr(a,o)==a) ? a + strlen(o) : 0;
}


static int * AllocEntries(int * e, int n, int * max)
{
  if (n >= *max) {
    int * e2 = realloc(e, (n+256) * sizeof(int));
    if (!e2) {
      error_add(0,"Entries allocation error (%s)", strerror(errno));
      free(e);
      *max = 0;
    } else {
      *max = n+256;
    }
    e = e2;
  }
  return e;
}


typedef struct {
  unsigned char *mem; /* Memory buffer */
  int from, to;       /* Memory range allowed */
  
  char *s;            /* String to parse. */
  int brk;            /* Break char */
 
  int * e;            /* entries */
  int i;              /* current entry */
  int max;            /* max entry */

} parser_t;

static void set_parser(parser_t * p2, const parser_t * p, char *s, int brk)
{
  *p2 = *p;
  p2->s = s;
  p2->brk = brk;
}

static int res_parser(parser_t * p, const parser_t * p2)
{
  int n;

  n = p2->i - p->i;
  p->s   = p2->s;
  p->e   = p2->e;
  p->i   = p2->i;
  p->max = p2->max;

  return n;
}

static int r_parse_number(parser_t * p)
{
  parser_t p2;
  int err, v;

  err = 1;     /* Invalid first operand */
  v = p->from; /* Get base value. */

  while (1) {
    int ind, c , op, w;

    c = *p->s;

    /* Test end */
    if (!c || c == p->brk) {
      break;
    }

    /* Get operator */
    if (strchr("+-*/<>|&^", c)) {
      ++p->s;
      op = c;
    } else if (err == 1) {
      /* first operand, fake '+' operator */
      op = '+';
      v = 0;
    } else {
      break;
    }

    /* Get indirection level */
    for (ind=0; c = p->s[ind], (c=='l'); ++ind)
      ;
    p->s += ind;

    if (c == '(') {
      int n;
      set_parser(&p2, p, p->s+1, ')');
      err = r_parse_number(&p2);
      n = res_parser(p,&p2);
      if (err) {
	break;
      }
      w = p->e[--p->i];
    } else {
      w = strtol(p->s, &p->s, 0);
      if (errno) {
	err = error_add(0,"entry syntax error: can't get number (%s)",
			strerror(errno));
	break;
      }
      err = 0; /* Valid first operand */
    }

    /* Get indirected values */
    while (ind--) {
      int a = w & 0xFFFFFF;
      if (a<p->from || a>=p->to) {
	err = error_add(0,"entry out of range $%X [$%06X-$%06X]",
			    a, p->from, p->to-1);
	break;
      }
      w = ReadL(p->mem+a);
    }
    if (err) {
      break;
    }

    /* Apply operator function */
    switch (op) {
    case 0:
    case '+':
      v += w; break;
    case '-':
      v -= w; break;
    case '|':
      v |= w; break;
    case '&':
      v &= w; break;
    case '^':
      v ^= w; break;
    case '*':
      v *= w; break;
    case '/':
      if (w == 0) {
	err = error_add(0,"entry zero divide error");
      } else {
	v /= w;
      }
      break;
    case '<':
      v <<= (w&31); break;
    case '>':
      v >>= (w&31); break;
    default:
      err = error_add(0,"entry unexpected operator: #%d,'%c'",op, op);
    }

    if (err) {
      break;
    }
  }

  if (err == 1) {
    err = error_add(0,"entry syntax error: empty expression");
  } else if (!err && p->brk) {
    if (*p->s != p->brk) {
      err = error_add(0,"entry syntax error: missing '%c'",p->brk);
    } else {
      ++p->s;
    }
  }

  if (!err) {
    p->e = AllocEntries(p->e, p->i+1, &p->max);
    if (!p->e) {
      err = -1;
    } else {
      p->e[p->i++] = v;
    }
  }

  return err;
}

/* Set a -1 terminated array of entry point from --entry= option string */
static int * parse_entry(char *entries, unsigned char *mem, int from, int to)
{
  parser_t p;
  int err = 0;

  if (!entries) {
    entries = "+0";
  }

  p.mem  = mem;
  p.from = from;
  p.to   = to;
  p.s    = entries;
  p.brk  = 0;
  p.e    = 0;
  p.i    = 0;
  p.max  = 0;

  while (!err) {
    int ind, rel;

    /* Get number of indiretion */
    for (ind=0; p.s[ind] == 'l'; ++ind)
      ;

    rel = 0;
    if (p.s[ind] == '+') {
      rel = 1;
    }
    if (p.s[ind+rel] != '[') {
      ind = 0;
      err = r_parse_number(&p);
    } else {
      p.s += ind+1+rel;
      err = r_parse_number(&p);
      if (*p.s != ',') {
	err = error_add(0,"entry syntax error: missing ',' in range");
      } else {
	++p.s;
	p.brk = ']';
	err = r_parse_number(&p);
	p.brk = 0;
      }
      if (!err) {
	rel = rel ? p.from : 0;
	if (!ind) {
	  p.e[p.i-2] += rel;
	  p.e[p.i-1] = (p.e[p.i-1]+rel)|(1<<24);
	} else {
	  int rs, re;
	  rs = p.e[p.i-2];
	  re = p.e[p.i-1];
	  p.i -= 2;
	  while (!err && rs < re) {
	    int i, v;
	    for (v=rs, i=ind; i; --i) {
	      int a = v & 0xFFFFFF;
	      if (a&1) {
		err = error_add(0,"entry odd address $%X",a);
		break;
	      } else if (a < p.from || a >= p.to) {
		err = error_add(0,"entry out of range $%0X [$%06X-$%06X]",
				    a, p.from, p.to-1);
		break;
	      }
	      v = ReadL(p.mem+a)+rel;
	    }
	    rs += 4;
	    if (!err) {
	      p.e = AllocEntries(p.e, p.i+1, &p.max);
	      if (!p.e) {
		err = -1;
	      } else {
		p.e[p.i++] = v;
	      }
	    }
	  }
	}
      }
    }

    if (err || !*p.s) {
      break;
    }

    if (*p.s != ',') {
      err = error_add(0,"entry syntax error: missing ',' in list");
      break;
    }
    ++p.s;
  }

  if (!err) {
    p.e = AllocEntries(p.e, p.i+1, &p.max);
    if (p.e) {
      p.e[p.i++] = -1;
      return p.e;
    }
  }

  if (p.e) {
    free(p.e);
  }
  return 0;
}

static char * Basename(char *name)
{
#ifdef HAVE_BASENAME
  return basename(name);
#else
  char *p1 = strrchr(name, '/');
  char *p2 = strrchr(name, '\\');
  if (p2 > p1) {
    p1 = p2;
  }
  return p1 ? p1+1 : name;
#endif
}
