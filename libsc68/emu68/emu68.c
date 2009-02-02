/*
 *                           emu68 - api
 *             Copyright (C) 2001-2009 Benjamin Gerard
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "emu68.h"
#include "ioplug68.h"

#include <string.h>

extern linefunc68_t *line_func[1024];

/* ,-----------------------------------------------------------------.
 * |                           Historic                              |
 * `-----------------------------------------------------------------'
 */
#ifdef DEBUG_HISTOTIC
static u32 historic68[1024], *h68=historic68;
static unsigned int count_pass_68 = 0;
#endif

/* ,-----------------------------------------------------------------.
 * |                      Dynamic memory handler                     |
 * `-----------------------------------------------------------------'
 */
static emu68_alloc_t emu68_alloc_fct;
static void * emu68_alloc(uint68_t size)
{
  return emu68_alloc_fct
    ? emu68_alloc_fct(size)
    : 0;
} 

static emu68_free_t  emu68_free_fct;
static void emu68_free(void * ptr)
{
  if (emu68_free_fct) {
    emu68_free_fct(ptr);
  }
} 


/* ,-----------------------------------------------------------------.
 * |                     Internal struct access                      |
 * `-----------------------------------------------------------------'
 */

/*  Set new interrupt IO
 *  return previous pointer
*/
io68_t * emu68_set_interrupt_io(emu68_t * emu68, io68_t * io)
{
  if (emu68) {
    io68_t * oldio = emu68->interrupt_io;
    emu68->interrupt_io = io;
    return oldio;
  }
  return 0;
}

void emu68_set_registers(emu68_t * emu68, const reg68_t *r, int mask)
{
  if (emu68 && r) {
    int i;
    if (mask&(1<<REG68_US_IDX)) REG68.usp = r->usp;
    if (mask&(1<<REG68_PC_IDX)) REG68.pc  = r->pc;
    if (mask&(1<<REG68_SR_IDX)) REG68.sr  = r->sr;
    for(i=0; i<8; i++)
      if (mask&(1<<(REG68_D0_IDX+i))) REG68.d[i] = r->d[i];
    for(i=0; i<8; i++)
      if (mask&(1<<(REG68_A0_IDX+i))) REG68.a[i] = r->a[i];
  }
}

void emu68_get_registers(const emu68_t * const emu68, reg68_t *r, int mask)
{
  int i;

  if (!r) {
    return;
  }
  if (mask&(1<<REG68_US_IDX)) r->usp = REG68.usp;
  if (mask&(1<<REG68_PC_IDX)) r->pc  = REG68.pc;
  if (mask&(1<<REG68_SR_IDX)) r->sr  = REG68.sr;
  for(i=0; i<8; i++)
    if (mask&(1<<(REG68_D0_IDX+i))) r->d[i] = REG68.d[i];
  for(i=0; i<8; i++)
    if (mask&(1<<(REG68_A0_IDX+i))) r->a[i] = REG68.a[i];
}

void emu68_set_cycle(emu68_t * const emu68, cycle68_t cycle)
{
  emu68->cycle = cycle;
}

cycle68_t emu68_get_cycle(emu68_t * const emu68)
{
  return emu68->cycle;
}


/* ,-----------------------------------------------------------------.
 * |                      Onboard memory access                      |
 * `-----------------------------------------------------------------'
 */

u8 * emu68_memptr(emu68_t * const emu68,
		  addr68_t dst, uint68_t sz)
{
  u8 * ptr = 0;
  if (emu68) {
    addr68_t end = (dst+sz)&MEMMSK68;
    if (sz > MEMMSK68+1) {
      emu68_error_add(emu68,
		      "Not enough 68K memory ($%X>=$%X)",sz,emu68->memmsk);
    } else if (end < dst) {
      emu68_error_add(emu68,
		      "68K memory overflow :($%X-%X,$%X)",dst,dst+sz,sz);
    } else {
      ptr = emu68->mem + dst;
    }
  }
  return ptr;
}

/* Peek & Poke
*/
int emu68_peek(emu68_t * const emu68, addr68_t addr)
{
  return !emu68 ? -1 : emu68->mem[addr&MEMMSK68];
}

int emu68_poke(emu68_t * const emu68, addr68_t addr, int68_t v)
{
  return !emu68 ? -1 : (emu68->mem[addr&MEMMSK68] = v);
}

/*  Write memory block to 68K on board memory
 */
int emu68_memput(emu68_t * const emu68, addr68_t dst, u8 *src, uint68_t sz)
{
  u8 * ptr = emu68_memptr(emu68,dst,sz);
  if (ptr) {
    memcpy(ptr,src,sz);
    return 0;
  }
  return -1;
}

/*  Read memory block from 68K on board memory
*/
int emu68_memget(emu68_t * const emu68, u8 *dst, addr68_t src, uint68_t sz)
{
  u8 * ptr = emu68_memptr(emu68,src,sz);
  if (ptr) {
    memcpy(dst,ptr,sz);
  }
  return -!ptr;
}

/* Fill a 68k on board memory block with a value.
 */
int emu68_memset(emu68_t * const emu68, addr68_t dst, u8 val, uint68_t sz)
{
  u8 * ptr;
  if (!sz) {
    sz = emu68->memmsk+1-dst;
  }
  ptr = emu68_memptr(emu68,dst,sz);
  if (ptr) {
    while (sz--) *ptr++ = val;
  }
  return -!ptr;
}

/* 68000 OP-WORD format :
 * 1111 0000 0000 0000 ( LINE  )
 * 0000 0001 1111 1000 ( OP    )
 * 0000 0000 0000 0111 ( REG 0 )
 * 0000 1110 0000 0000 ( REG 9 )
*/


static void step68(emu68_t * const emu68)
{
  int line,opw,reg9;

#ifdef DEBUG_HISTORIC
  /* Fill historic buffer */
  if (h68 == historic68) {
    h68 = historic68 + (sizeof(historic68)/sizeof(*historic68));
  }
  --h68;
  *h68 = REG68.pc;
  opw = (u16)get_nextw();
#else
  u8 * mem = emu68->mem + (REG68.pc & MEMMSK68);
  REG68.pc += 2;
  opw  = (mem[0]<<8) | mem[1];
#endif
  /*                       LINE RG9 OPCODE  RG0 */
  /*                       -------------------- */ 
  line = opw & 0170000; /* 1111 000 000-000 000 */
  opw -= line;          /* 0000 111 111-111 111 */
  reg9 = opw &   07000; /* 0000 111 000-000 000 */
  opw -= reg9;          /* 0000 000 111-111 111 */
  reg9 >>= 9;           /* reg9 */
  line |= opw<<3;
  line >>= 6;
  opw  &=  7;           /* 0000 000 000-000 111 */
  (line_func[line])(emu68, reg9, opw);
}

/* Emulator 68000 : Step 1 instruction
*/
void emu68_step(emu68_t * const emu68)
{
  if (emu68) {
    step68(emu68);
  }
}

static void poll_rte(emu68_t * const emu68, const addr68_t stack)
{
  do {
    emu68_step(emu68);
  } while( (addr68_t) REG68.a[7] < stack);
}

/* Run until RTS level0 occurs
 * and play Interruption for 1 pass.
 * ( Specific SC68 !!! )
*/
void emu68_level_and_interrupt(emu68_t * const emu68,
			       cycle68_t cycleperpass)
{
  addr68_t stack, pc;
  cycle68_t cycle;
  io68_t *io;

  if (!emu68) {
    return;
  }
  stack = REG68.a[7];
  pc = REG68.pc;
  REG68.a[7] = stack - 4;

#if _DEBUG
  emu68_poke(emu68, REG68.a[7]+0, 0x12);
  emu68_poke(emu68, REG68.a[7]+1, 0x34);
  emu68_poke(emu68, REG68.a[7]+2, 0x56);
  emu68_poke(emu68, REG68.a[7]+3, 0x78);
#endif

  /* Clear ORed memory access flags ... */
  emu68->framechk = 0;

#if 0/* _DEBUG */
  if (!count_pass_68) {
    if (cycleperpass) {
      BREAKPOINT68;
    }
    if (emu68->cycle) {
      BREAKPOINT68;
    }
  } else {
    if (!cycleperpass) {
      BREAKPOINT68;
    }
    if (count_pass_68 == 1) {
      if (emu68->cycle) {
        BREAKPOINT68;
      }
    } else {
      if (!emu68->cycle) {
        BREAKPOINT68;
      }
    }

  }
  count_pass_68++;
#endif

  /* Adjust internal IO cycle counter */
  for (io=emu68->iohead; io; io=io->next) {
    (io->adjust_cycle)(io,emu68->cycle);
  }
  emu68->cycle = 0;

  /* Do until RTS */
  do {
    step68(emu68);
  } while (stack>(addr68_t)REG68.a[7]);

  cycle=0;

#if defined (_DEBUG) && 0
  if (stack != (addr68_t)REG68.a[7]) {
    BREAKPOINT68;
  }
  if (REG68.pc != 0x12345678) {
    BREAKPOINT68;
  }
#endif

  /* Get interrupt IO (MFP) if any */
  if (emu68->interrupt_io) {
    interrupt68_t * t;
    int ipl = (REG68.sr & (7<<SR_I_BIT)) >> SR_I_BIT;
#ifdef _DEBUG
    int ipl_lost = 0;
    int irq_pass = 0;
#endif
    /* $$$ HACK: mfp interupt at level 5 */
    /* $$$ TEMP: FOR DEBUGIN A MUSIC !! */
    if ( /*1 ||*/ 6 > ipl ) {
      do {
	
	/* Spool interrupt */
	/* Execute mfp interrupt emulation */
	t = emu68->interrupt_io->interrupt(emu68->interrupt_io, cycleperpass);
	if (t) {
#ifdef DEBUG
	  if (t->cycle >= cycleperpass) {
	    *(int *)0 = 0xDEADBEEF;
	  }
#endif


	  emu68->cycle=t->cycle;
	  REG68.a[7] = stack; /* $$$ Safety net */
	  REG68.pc = 0x12345678;
	  EXCEPTION(t->vector,t->level);
	  poll_rte(emu68, stack);
#ifdef _DEBUG
	  if (REG68.pc != 0x12345678) {
	    BREAKPOINT68;
	  }
	  if (REG68.a[7] != stack) {
            BREAKPOINT68;
	  }
	  if (!(REG68.sr & 0x2000)) {
	    BREAKPOINT68;
	  }
#endif
	} else {
          //          BREAKPOINT68;
	  break; //$$$ ???
        }
      } while (t);
    } else {
      // ipl masks irq
#ifdef _DEBUG
      // This happens sometime
      //BREAKPOINT68;
      ipl_lost++;
#endif
    }
  }

  /* Restore A7, PC, and new cycle counter */
  REG68.a[7]  = stack;
  REG68.pc    = pc;
  emu68->cycle = cycleperpass;
}

/* Run like a JSR at pc, exec interrupt if cycleperpass > 0.
 */
void emu68_run_rts(emu68_t * const emu68,
		   addr68_t pc, cycle68_t cycleperpass)
{
  addr68_t stack;

  if (!emu68) {
    return;
  }

  /* Push current pc .*/
  stack = REG68.a[7];
  pushl(REG68.pc);
  REG68.pc = pc;

  /* Clear ORed memory access flags ... */
  emu68->framechk = 0;

  /* Adjust internal IO cycle counter & find MFP */
  if (emu68->cycle) {
    io68_t *io;
    for (io=emu68->iohead; io; io=io->next) {
      (io->adjust_cycle)(io, emu68->cycle);
    }
    emu68->cycle = 0;
  }

  /* Do until RTS */
  do {
    emu68_step(emu68);
  } while (stack > (addr68_t)REG68.a[7]);

  /* Get interrupt IO (MFP) if any */
  if (cycleperpass && emu68->interrupt_io) {
    interrupt68_t *t;
    int ipl = (REG68.sr & (7<<SR_I_BIT)) >> SR_I_BIT;
    cycle68_t cycle = 0;

    if (6 > ipl ) {
      cycle68_t fd;
    
      /* Spool interrupt */
      while (fd =
	     emu68->interrupt_io->next_interrupt(emu68->interrupt_io,cycle),
	     fd != IO68_NO_INT) {
        /* ... This must not happen ...  */
        if( (int)fd < 0 ) {
          fd = 0;
        }

        /* ... */

        /* TIME travel to next interruption */
        cycle += fd;

        if(cycle >= cycleperpass) {
          /* interrupt after this pass */
          break;
        }

        /* Execute mfp interrupt emulation */
        t = emu68->interrupt_io->interrupt(emu68->interrupt_io,cycle);
        if (t) {
          emu68->cycle=cycle;
          REG68.a[7] = stack; /* $$$ Safety net */
          EXCEPTION(t->vector,t->level);
          poll_rte(emu68,stack);
        }
      }
    }
  }
  emu68->cycle = cycleperpass;
}


/* Run emulation for given number of cycle
 */
void emu68_cycle(emu68_t * const emu68, cycle68_t cycleperpass)
{
  cycle68_t cycle;
  io68_t *io;

  /* Clear ORed memory access flags ... */
#ifdef EMU68DEBUG
  REG68.framechk = 0;
#endif

  /* Adjust internal IO cycle counter */
  for (io=emu68->iohead; io; io=io->next) {
    (io->adjust_cycle)(io, emu68->cycle);
  }

  /* Do until RTS */
  cycle = emu68->cycle = 0;
  do
  {
    /* If not stopped, execute an instruction */
    if (1/* !REG68.status */) {
      emu68_step(emu68);
      cycle += 4;
    }
    emu68->cycle = cycle;

    if (emu68->interrupt_io) {
      interrupt68_t *t;
      int ipl;

      ipl = (REG68.sr & (7<<SR_I_BIT)) >> SR_I_BIT;

      /* $$$ HACK : mfp interupt at level 5 */
      if (6 > ipl) {
	t = emu68->interrupt_io->interrupt(emu68->interrupt_io,cycle);
	if (t) {
	  EXCEPTION(t->vector,t->level);
	  /* $$$ extra cycle should be added here for exception handle */
	}
      }
    }
  } while(cycle < cycleperpass);

  // $$$ This is hack : should be "cycle" and handle later
  cycle = cycleperpass;
  emu68->cycle = cycle;
}


/* Run until PC = breakpc
*/
void emu68_break (emu68_t * const emu68, addr68_t  breakpc)
{
  if (emu68) {
    while((addr68_t)REG68.pc != breakpc) {
      emu68_step(emu68);
    }
  }
}


/* Get debug mode.  */
int emu68_debugmode(emu68_t * const emu68)
{
  return emu68 ? !!emu68->chk : -1;
}


/* ,-----------------------------------------------------------------.
 * |                     Emulator init functions                     |
 * `-----------------------------------------------------------------'
 */

static emu68_parms_t def_parms = {
  0,       /* name                        */
  19,      /* log2mem: 2^19 => 512Kb      */
  8000000, /* master clock for Atari520ST */
  0        /* debug mode off              */ 
};


emu68_t * emu68_create(emu68_parms_t * const parms)
{
  emu68_t * emu68 = 0;
  int memsize, membyte;
  emu68_parms_t * const p = parms ? parms : &def_parms; 

  if (!p->log2mem) {
    p->log2mem = def_parms.log2mem;
  }
  if (p->log2mem<16||p->log2mem>24) {
    goto error;
  }

  if (!p->clock) {
    p->clock = def_parms.clock;
  }
  if (p->clock<500000u || p->clock>60000000u) {
    goto error;
  }

  memsize = 1<<p->log2mem;
  membyte = sizeof(emu68_t) + (memsize<<!!p->debug);
  emu68   = emu68_alloc(membyte);
  if (!emu68) {
    goto error;
  }
  memset(emu68,0,sizeof(membyte));
  strncpy(emu68->name,p->name?p->name:"emu68",sizeof(emu68->name)-1);
  emu68->alloc = emu68_alloc_fct;
  emu68->free  = emu68_free_fct;
  emu68->clock = p->clock;

  emu68->log2mem = p->log2mem;
  emu68->memmsk  = memsize-1;
  if (p->debug) {
    emu68->chk = emu68->mem + memsize+4;
  }

  emu68_mem_init(emu68);
  emu68_reset(emu68);

 error:
  return emu68;
}

/* Destroy emulator instance. */
void emu68_destroy(emu68_t * const emu68)
{
  if (emu68) {
    emu68_ioplug_destroy_all(emu68);
    emu68_free(emu68);
  }
}

/* 68000 Hardware Reset
 * - PC = 0
 * - SR = 2700
 * - A7 = end of mem - 4
 * - All registers cleared
 * - All IO reseted
*/
void emu68_reset(emu68_t * const emu68)
{
  if (emu68) {
    io68_t *io;
    for (io=emu68->iohead; io; io=io->next) {
      (io->reset)(io);
    }
    memset(REG68.d,0,sizeof(REG68.d)*2);
    emu68->cycle  = 0;
    REG68.pc     = 0;
    REG68.sr     = 0x2700;
    REG68.a[7]   = MEMMSK68+1-4;
    /* REG68.status = 0; */
  }

#ifdef DEBUG_HISTOTIC
  for (h68=historic68;
       h68<historic68+(sizeof(historic68)/sizeof(*historic68)); h68++) {
    *h68 = 0x12345678;
  }
  count_pass_68=0;
#endif
}


/* ,-----------------------------------------------------------------.
 * |                    Library init functions                       |
 * `-----------------------------------------------------------------'
 */

/* Library initialize. */
int emu68_init(emu68_init_t * init)
{
  int err = -1;
  if (init && init->alloc && init->free && !emu68_alloc_fct) {
    err = 0;
    emu68_alloc_fct = init->alloc;
    emu68_free_fct  = init->free;
  }
  return err;
}

/* Library shutdown. */
void emu68_shutdown(void)
{
  emu68_alloc_fct = 0;
  emu68_free_fct  = 0;
}
