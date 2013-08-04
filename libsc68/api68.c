/*
 * @file    api68.c
 * @brief   sc68 API
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-08-04 22:33:51 ben>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_IO68_CONFIG_OPTION68_H
# include "io68/config_option68.h"
#else
# include "io68/default_option68.h"
#endif

#include <sc68/file68_vfs.h> /* Need vfs68.h before sc68.h */

/* libsc68 includes */
#include "sc68.h"
#include "mixer68.h"
#include "conf68.h"

/* libsc68 emulators includes */
#include "emu68/emu68.h"
#include "emu68/excep68.h"
#include "emu68/ioplug68.h"
#include "io68/io68.h"

/* file68 includes */
#include <sc68/file68.h>
#include <sc68/file68_err.h>
#include <sc68/file68_str.h>
#include <sc68/file68_uri.h>
#include <sc68/file68_rsc.h>
#include <sc68/file68_msg.h>
#include <sc68/file68_opt.h>

/* stardard includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif

enum {
  /* Exception handler to catch badly initialize exception */
  INTR_ADDR = 0x800,
  /* TOS emulator */
  TRAP_ADDR = 0x500,
  /* # of instructions for music play code */
  PLAY_MAX_INST = 1000000u,
  /* # of instructions for music init code */
  INIT_MAX_INST = 10000000u,
  /* default music time in seconds  */
  TIME_DEF = 3 * 60,
  /* default music time in millisec */
  TIMEMS_DEF = (TIME_DEF * 1000),
  /* sc68_t magic identifier value. */
  SC68_MAGIC = (('s' << 24) | ('c' << 16) | ('6' << 8) | '8'),
  /* disk68_t magic identifier value. */
  DISK_MAGIC = SC68_DISK_ID,
  /* Error message maximum length */
  ERRMAX = 96,
};

static u8 trap_func[] = {
#include "sc68/trap68.h"
};

static const char * trap_type[16] =
{ 0, "gemdos", 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, "bios" , "xbios", 0 };

static u8 thex[16] = {
  '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'
};

/** sc68 instance. */
struct _sc68_s {
  int            magic;       /**< magic identifier.                     */
  char           name[16];    /**< short name.                           */
  int            version;     /**< sc68 version.                         */
  void         * cookie;      /**< User private data.                    */
  emu68_parms_t  emu68_parms; /**< 68k emulator parameters.              */

/* Keep the following in that order !!!!
 * {
 */
  emu68_t      * emu68;       /**< 68k emulator instance.                */
  io68_t *ymio,*mwio,*shifterio,*paulaio,*mfpio;
/* } */

  ym_t         * ym;          /**< YM emulator.                          */
  mw_t         * mw;          /**< MicroWire emulator.                   */
  paula_t      * paula;       /**< Amiga emulator.                       */

  int            tobe3;       /**< free disk memory be release on close. */
  const disk68_t  * disk;     /**< Current loaded disk.                  */
  const music68_t * mus;      /**< Current playing music.                */
  int            track;       /**< Current playing track.                */
  int            track_to;    /**< Track to set (0:n/a, -1:end req).     */
  int            loop_to;     /**< Loop to set (0:default -1:infinite).  */

  int            cfg_track;   /**< from config "force-track".            */
  int            cfg_loop;    /**< from config "force-loop".             */

  unsigned int   playaddr;    /**< Current play address in 68 memory.    */
  int            seek_to;     /**< Seek to this time (-1:n/a)            */
  int            remote;      /**< Allow remote access.                  */

/** Playing time info. */
  struct {
    unsigned int def_ms;      /**< default time in ms.                   */
    unsigned int length_ms;   /**< current track length in ms.           */
    unsigned int origin_ms;   /**< elapsed ms at track origin.           */
    unsigned int elapsed_ms;  /**< elapsed ms since track start.         */
  } time;

/** IRQ handler. */
  struct {
    int pc;                   /**< value of PC at last IRQ.              */
    int vector;               /**< what was the last IRQ type.           */
    int sysfct;               /**< Last gemdos/bios/xbios function       */
  } irq;

/** Mixer info struture. */
  struct
  {
    unsigned int   rate;         /**< Sampling rate in hz.               */
    u32          * buffer;       /**< Current PCM buffer.                */
    u32          * bufptr;       /**< Current PCM position.              */
    int            bufmax;       /**< buffer allocated size.             */
    int            bufreq;       /**< Required buffer size for track.    */
    int            buflen;       /**< PCM count in buffer.               */
    int            stdlen;       /**< Default number of PCM per pass.    */
    unsigned int   cycleperpass; /**< Number of 68K cycles per pass.     */
    int            aga_blend;    /**< Amiga LR blend factor [0..65536].  */

    unsigned int   pass_count;   /**< Pass counter.                      */
    unsigned int   loop_count;   /**< Loop counter.                      */
    unsigned int   pass_total;   /**< Total number of pass.              */
    unsigned int   loop_total;   /**< Total number of loop (0:infinite). */
    unsigned int   pass_2loop;   /**< Number of pass before next loop.   */
    unsigned int   pass_3loop;   /**< Reset pass_2loop after a loop.     */

  } mix;

  sc68_minfo_t     info;         /**< Disk and track info struct.        */

/* Error message */
  const char   * errstr;         /**< Last error message.                */
  char           errbuf[ERRMAX]; /**< For non-static error message.      */
};

#ifndef DEBUG_SC68_O
# ifndef DEBUG
#  define DEBUG_SC68_O 0
# else
#  define DEBUG_SC68_O 1
# endif
#endif

static int           sc68_cat = msg68_NEVER;
static int           sc68_id;        /* counter for auto generated name */
static volatile int  sc68_init_flag; /* Library init flag     */
static int           sc68_spr_def = SAMPLING_RATE_DEF;
extern option68_t  * config68_options;   /* conf68.c */
extern int           config68_opt_count; /* conf68.c */
static int           dbg68k;
static const char    not_available[] = SC68_NOFILENAME;
static char          appname[16];
static config68_t  * config;
static char          sc68_errstr[ERRMAX];

static void sc68_debug(sc68_t * sc68, const char * fmt, ...);
static int error_addx(sc68_t * sc68, const char * fmt, ...);
static int error_add(sc68_t * sc68, const char * fmt, const char *);
static int get_spr(const sc68_t * sc68);
static int set_spr(sc68_t * sc68, int hz);
static int get_pcm_fmt(sc68_t * sc68);
static int set_pcm_fmt(sc68_t * sc68, int pcmfmt);
static int get_pos(sc68_t * sc68, int origin);


/***********************************************************************
 * Check functions
 **********************************************************************/

static inline const char * ok_int(const int err) {
  return !err ? "success" : "failure";
}

static inline const char * never_null(const char * ptr) {
  return ptr ? ptr : "(nil)";
}

static inline int is_magic(const sc68_t * sc68) {
  return sc68->magic == SC68_MAGIC;
}

static inline int is_sc68(const sc68_t * sc68) {
  return sc68 && is_magic(sc68);
}

static inline int null_or_sc68(const sc68_t * sc68) {
  return !sc68 || is_magic(sc68);
}

static inline int is_disk(const disk68_t * disk) {
  return disk && disk->magic == DISK_MAGIC;
}

static inline int has_disk(const sc68_t * sc68) {
  return is_sc68(sc68) && is_disk(sc68->disk);
}

/***********************************************************************
 * Convenience functions
 **********************************************************************/

#ifndef EXTENSION_SEP
# define EXTENSION_SEP '.'
#endif
static void appname_from_path(char *path, char * appname, int max)
{
  char * e, *s;
  int len;

  appname[0] = 0;
  if (!path || !*path) return;
  s = basename(path);
  e = strrchr(s,EXTENSION_SEP);
  if (!e || e == s)
    len = strlen(s);
  else
    len = e - s;
  if (len >= max)
    len = max-1;
  strncpy(appname, s, len);
  appname[len] = 0;
}


/***********************************************************************
 * Emulators functions
 **********************************************************************/

static int stream_read_68k(sc68_t * sc68, unsigned int dest,
                           vfs68_t * is, unsigned int sz)
{
  u8 * mem68 = emu68_memptr(sc68->emu68, dest, sz);

  if (!mem68)
    return error_addx(sc68,
                      "libsc68: %s -- %s\n",
                      "vfs error", emu68_error_get(sc68->emu68));
  return (vfs68_read(is, mem68, sz) == sz) ? 0 : -1;
}

static int init_emu68(int * argc, char ** argv)
{
  int err;

  /* Initialize emu68 */
  err = emu68_init(argc, argv);
  if (err) {
    error_add(0, "libsc68: %s\n","emu68 library *FAILED*");
    goto error;
  }

  /* Initialize chipset */
  err = io68_init(argc, argv);
  if (err) {
    error_add(0, "libsc68: %s\n","chipset library *FAILED*");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "68k and chipset emulator initialized");

error:
  return err;
}

static void safe_io68_destroy(io68_t **pio)
{
  io68_destroy(*pio);
  *pio = 0;
}

static void safe_emu68_destroy(emu68_t **pemu)
{
  emu68_destroy(*pemu);
  *pemu = 0;
}

static void safe_destroy(sc68_t * sc68)
{
  assert(sc68);
  emu68_ioplug_unplug_all(sc68->emu68);
  safe_io68_destroy(&sc68->ymio);
  safe_io68_destroy(&sc68->mwio);
  safe_io68_destroy(&sc68->shifterio);
  safe_io68_destroy(&sc68->paulaio);
  safe_io68_destroy(&sc68->mfpio);
  safe_emu68_destroy(&sc68->emu68);
  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "68k and chipset emulators destroyed");
}

#define Bpeek(EMU68,ADDR) emu68_peek((EMU68), (ADDR))

static u16 Wpeek(emu68_t* const emu68, addr68_t  addr)
{
  return (u16) ( ( (u16) Bpeek(emu68, addr+0) << 8 ) |
                 ( (u16) Bpeek(emu68, addr+1)      ) );
}

static u32 Lpeek(emu68_t* const emu68, addr68_t  addr)
{
  return (u32) ( ( (u32) Bpeek(emu68, addr+0) << 24 ) |
                 ( (u32) Bpeek(emu68, addr+1) << 16 ) |
                 ( (u32) Bpeek(emu68, addr+2) <<  8 ) |
                 ( (u32) Bpeek(emu68, addr+3)       ) );
}

/* Timer awareness */
void except_name(const int vector, char * irqname)
{
  int c = 0;
  switch (vector * 4) {
  case 0x134: c = 'A'; break;
  case 0X120: c = 'B'; break;
  case 0X114: c = 'C'; break;
  case 0X110: c = 'D'; break;
  default:    c = 0;
  }
  if (c)
    sprintf(irqname,"timer-%c", c);
  else
    emu68_exception_name(vector,irqname);
}


/* Exception handler (runs when emu68 runs in debug mode (--sc68-dbg68k=1).
 *
 * It's default behaviour is to dump the exception and to break
 * exection. That will generate an error later in the simulation loop.
 *
 * HW-trace and MFP timer interrupts are ignored.
 *
 * Know trace vectors (gemdos/xbios/bios) are dumped but not breaked.
 *
 */
static void irqhandler(emu68_t* const emu68, int vector, void * cookie)
{
  sc68_t* sc68 = cookie;
  char    irqname[32];
  const char * stname;
  int     dumpex, savest, subvec;
  u32     sr, pc;

  /* Store last interruption */
  sc68->irq.pc     = emu68->reg.pc;
  sc68->irq.vector = vector;

  /* Exit those really fast as they happen quiet a lot, specially
   * HWTRACE_VECTOR that happem every single instruction. */
  if ( vector == HWTRACE_VECTOR ||
       vector == 0x134/4 /* Timer-A */ ||
       vector == 0X120/4 /* Timer-B */ ||
       vector == 0X114/4 /* Timer-C */ ||
       vector == 0X110/4 /* Timer-D */ )
    return;

  /* Get fancy names */
  except_name(vector,irqname);

  /* Init vars */
  stname = emu68_status_name(emu68->status);
  subvec = 0;
  dumpex = 1;
  savest = emu68->status;

  switch (savest) {
  case EMU68_NRM:
    /* Assume break, set back to NRM only hen the exception is known */
    emu68->status = EMU68_BRK;
    break;

  case EMU68_STP:
  case EMU68_BRK:
  case EMU68_HLT:
  case EMU68_ERR:
  case EMU68_XCT:
  default:
    break;
  }

  /***********************************************************************
   * Retrieve exception source (both pc and sr)
   */
  if ( vector < 0x100 ) {
    /* normal vectors, get sr and pc from the stack */
    sr = Wpeek(emu68, emu68->reg.a[7]);
    pc = Lpeek(emu68, emu68->reg.a[7]+2);
    sc68->irq.pc = pc;

    if (vector >= TRAP_VECTOR(0) && vector <= TRAP_VECTOR(15)) {
      subvec = vector - TRAP_VECTOR(0);
      vector = TRAP_VECTOR(0);
      if (trap_type[subvec]) {
        sc68->irq.sysfct = Wpeek(emu68, emu68->reg.a[7]+6) & 0xFFFF;
        emu68->status    = savest;         /* Don't break */
      }
    }
  } else {
    /* Special emu68 vectors, just grab cpu's reg as-is */
    sr = emu68->reg.sr;
    pc = emu68->reg.pc;

    if (vector >= HWBREAK_VECTOR && vector < HWTRACE_VECTOR) {
      subvec = vector - HWBREAK_VECTOR;
      vector = HWBREAK_VECTOR;
    }

    switch (vector) {

    case HWSTOP_VECTOR:
      if ( (sr & 0x3F00) == 0x2F00 ) {
        /* SR bit#11 (always 0) is used by our special code to catch
         * Unitialized exception.  The exception vector issued a stop
         * #$2FNN instruction with NN being the exception vector
         * number.
         */
        const int num = sr & 0xFF;
        addr68_t vaddr;
        strcpy(irqname,"NC-");
        vaddr = Lpeek(sc68->emu68, num * 4);
        if (vaddr != INTR_ADDR + num * 8)
          strcpy(irqname,"CH-");
        except_name(num,irqname+3);
        sr = Wpeek(emu68, emu68->reg.a[7]);
        pc = Lpeek(emu68, emu68->reg.a[7]+2);
        error_addx(sc68,
                   "libsc68: non-init exception #%d (%s) from %06x",
                   num, irqname, pc);
        emu68->status = EMU68_HLT;
      } else {
        emu68->status = EMU68_STP;
      }

    case HWBREAK_VECTOR:
      break;

    case HWHALT_VECTOR:
    case HWRESET_VECTOR:
      /* Recieve the reset vector when reset instruction has just been
       * executed, in our case it happen in the uninitialized
       * exception catcher code. */
      emu68->status = EMU68_HLT;
      break;

    case HWTRACE_VECTOR:
    default:
      assert(!"Invalid hardware vector");
      break;

    }
  }

  /***********************************************************************
   * Dump exception context
   */
  if (dumpex) {
    u8 * memptr = emu68_memptr(sc68->emu68,0,0);
    const addr68_t adr = emu68->reg.a[7] & emu68->memmsk;
    const addr68_t bot = emu68->memmsk+1-16;
    char line[16 * 3];
    int i, j, l = strlen(irqname);

    if (vector == ILLEGAL_VECTOR)
      sprintf(irqname+l, "#$%02x-%02x",
              memptr[pc&emu68->memmsk], memptr[(pc+1)&emu68->memmsk]);
    else if (vector == TRAP_VECTOR(0) && trap_type[subvec])
      sprintf(irqname, "%s(%d) ($%02x)",
              trap_type[subvec], sc68->irq.sysfct, sc68->irq.sysfct);

    /* dump cpu status */
    TRACE68(sc68_cat,
            "libsc68: 68k interruption -- emu68<%s> sc68<%s>\n"
            " status: in:%s out:%s\n"
            "   intr: #%02x+%02x %s\n"
            "   from: pc:%08x sr:%04x\n"
            "   regs: pc:%08x sr:%04x\n"
            "         d0:%08x d1:%08x d2:%08x d3:%08x\n"
            "         d4:%08x d5:%08x d6:%08x d7:%08x\n"
            "         a0:%08x a1:%08x a2:%08x a3:%08x\n"
            "         a4:%08x a5:%08x a6:%08x a7:%08x\n",
            emu68->name, sc68->name,
            stname, emu68_status_name(emu68->status),
            vector, subvec, irqname,
            pc, sr,
            emu68->reg.pc,   emu68->reg.sr,
            emu68->reg.d[0], emu68->reg.d[1],
            emu68->reg.d[2], emu68->reg.d[3],
            emu68->reg.d[4], emu68->reg.d[5],
            emu68->reg.d[6], emu68->reg.d[7],
            emu68->reg.a[0], emu68->reg.a[1],
            emu68->reg.a[2], emu68->reg.a[3],
            emu68->reg.a[4], emu68->reg.a[5],
            emu68->reg.a[6], emu68->reg.a[7]);

    /* dump stack */
    line[0] = 0;
    for (i = 0, j = 0; adr+i < bot && i<0x80; ++i) {
      int v = memptr[adr+i];

      line[j+0] = thex[v >> 4];
      line[j+1] = thex[v & 15];
      if (j != 3*15) {
        line[j+2] = '-';
        j += 3;
      } else {
        line[j+2] = 0;
        TRACE68(sc68_cat," %-6x: %s\n", adr + (i & ~15), line);
        j = 0;
      }
    }
    if (j) {
      line[j-1] = 0;
      TRACE68(sc68_cat," %-6x: %s\n", adr + (i & ~15), line);
    }
  }
}

static int init68k(sc68_t * sc68, int log2mem, int emu68_debug)
{
  int err = -1;
  emu68_parms_t * const parms = &sc68->emu68_parms;

  if (sc68->emu68)
    safe_destroy(sc68);

  /* setup parameters. */
  parms->name    = "sc68/emu68";
  parms->log2mem = log2mem;
  parms->clock   = EMU68_ATARIST_CLOCK;
  parms->debug   = emu68_debug & 1;

  TRACE68(sc68_cat,
          "libsc68: init 68k -- '%s' mem:%d-bit(%dkB) clock:%uhz debug:%s\n",
          parms->name,
          parms->log2mem,parms->log2mem>10?1<<(parms->log2mem-10):0,
          parms->clock,parms->debug?"On":"Off");

  /* Do initialization. */
  sc68->emu68 = emu68_create(parms);
  if (!sc68->emu68) {
    error_add(sc68, "libsc68: %s\n", "68k emulator creation failed");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: init 68k -- %s\n",
          "CPU emulator created");

  /* Install cookie and interruption handler (debug mode only). */
  emu68_set_handler(sc68->emu68, (emu68_debug& 1 ) ? irqhandler : 0);
  emu68_set_cookie(sc68->emu68, sc68);
  sc68->irq.pc     = 0xDEAD;
  sc68->irq.vector = -1;

  /* Setup critical 68K registers (SR and SP) */
  sc68->emu68->reg.sr   = 0x2000;
  sc68->emu68->reg.a[7] = sc68->emu68->memmsk+1-4;

  /* Initialize chipset */
  sc68->ymio = ymio_create(sc68->emu68,0);
  sc68->ym   = ymio_emulator(sc68->ymio);
  if (!sc68->ymio) {
    error_add(sc68,"libsc68: %s","YM-2149 creation failed");
    goto error;
  }
  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "init 68k -- chipset -- YM-2149");

  sc68->mwio = mwio_create(sc68->emu68,0);
  sc68->mw   = mwio_emulator(sc68->mwio);
  if (!sc68->mwio) {
    error_add(sc68,"libsc68: %s\n", "STE-MW creation failed");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "init 68k -- chipset -- STE-MW");

  sc68->shifterio = shifterio_create(sc68->emu68,0);
  if (!sc68->shifterio) {
    error_add(sc68,"libsc68: %s\n","Atari Shifter creation failed");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "init 68k -- chipset -- ST shifter");

  sc68->paulaio = paulaio_create(sc68->emu68,0);
  sc68->paula   = paulaio_emulator(sc68->paulaio);
  if (!sc68->paulaio) {
    error_add(sc68,"libsc68: %s\n","create Paula emulator failed");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "init 68k -- chipset -- Amiga Paula");

  sc68->mfpio = mfpio_create(sc68->emu68);
  if (!sc68->mfpio) {
    error_add(sc68,"libsc68: %s\n","MK-68901 creation failed");
    goto error;
  }

  TRACE68(sc68_cat,
          "libsc68: %s\n",
          "init 68k -- chipset -- MFP");

  err = 0;
error:
  if (err)
    safe_destroy(sc68);

  TRACE68(sc68_cat,
          "libsc68: init 68k -- %s\n",
          strok68(err));

  return err;
}

/***********************************************************************
 * Config functions
 **********************************************************************/


/* Get integer config vaue.
 * - just command line if available
 * - else get the value from the config file
 * - fallback to default otherwise.
 */
static int myconfig_get_int(config68_t * c,
                            const char * name,
                            int def)
{
  option68_t    * opt;
  int             v = def, idx = -1;

  switch (option68_type(opt = option68_get(name, 1))) {
  case option68_BOL: case option68_INT:
    v = opt->val.num;
    TRACE68(sc68_cat,
            "libsc68: get config from cli -- name='%s' val=%d\n", name, v);
  default:
    if ( config68_get(c, &idx, &name) == CONFIG68_INT ) {
      v = idx;
      TRACE68(sc68_cat,
              "libsc68: get config from cfg -- name='%s' val=%d\n", name, v);
    }
  }
  return v;
}

/* Get string config value
 * @myconfig_get_int for details,
 */
static const char * myconfig_get_str(config68_t * c,
                                     const char * name,
                                     const char * def)
{
  option68_t    * opt;
  const char    * v = def, *key = name;
  int             idx = -1;

  switch (option68_type(opt = option68_get(name, 1))) {
  case option68_STR:
    v = opt->val.str;
    TRACE68(sc68_cat,
            "libsc68: get config from cli -- name='%s' val='%s'\n", name, v);
  default:
    if ( config68_get(c, &idx, &key) == CONFIG68_STR ) {
      v = key;
      TRACE68(sc68_cat,
              "libsc68: get config from cfg -- name='%s' val=%d\n", name, v);
    }
  }
  return v;
}

static void set_config(sc68_t * sc68)
{
  config68_t * const c = config;
  const char * lmusic, * rmusic;

  sc68->version       = myconfig_get_int(c, "version",       PACKAGE_VERNUM);
  sc68->remote        = myconfig_get_int(c, "allow-remote",  1);
  sc68->mix.aga_blend = myconfig_get_int(c, "amiga-blend",   0x4000);
  sc68->cfg_track     = myconfig_get_int(c, "force-track",   0);
  sc68->cfg_loop      = myconfig_get_int(c, "force-loop",    0);
  sc68->time.def_ms   = myconfig_get_int(c, "default-time",  TIME_DEF) * 1000;
  sc68->mix.rate      = myconfig_get_int(c, "sampling-rate", SAMPLING_RATE_DEF);

  rsc68_get_path(0,0,&lmusic, &rmusic);
  if (!lmusic) {
    lmusic = myconfig_get_str(c, "music_path", 0);
    rsc68_set_music(lmusic);
  }
  if (!rmusic) {
    rmusic = myconfig_get_str(c, "remote_music_path", 0);
    rsc68_set_remote_music(rmusic);
  }

}

static void myconfig_set_int(config68_t * c,
                             const char * name,
                             int v)
{
  if (c) {
    config68_set(c, -1, name, v, 0);
  }
}

int sc68_config_load(void)
{
  int err;

  if (!config)
    config = config68_create(appname, 0);
  err = config68_load(config);
  myconfig_set_int(config, "version", PACKAGE_VERNUM);

  sc68_debug(0,"libsc68: load config -- %s\n", strok68(err));
  return err;
}

int sc68_config_save(void)
{
  int err = -1;
  if (config)
    err = config68_save(config);
  sc68_debug(0,"libsc68: save config -- %s\n", strok68(err));
  return err;
}

void sc68_config_apply(sc68_t * sc68)
{
  if (!config)
    sc68_config_load();

  if (sc68) {
    config68_valid(config);
    set_config(sc68);
  }
}

int sc68_config_idx(const char * name)
{
  return config68_get_idx(config, name);
}

config68_type_t sc68_config_range(int idx,
                                  int * min, int * max, int * def)
{
  return config68_range(config, idx, min, max, def);
}

config68_type_t sc68_config_get(int * idx, const char ** name)
{
  return config68_get(config, idx, name);
}

config68_type_t sc68_config_set(int idx,
                                const char * name,
                                int v,
                                const char * s)
{
  return config68_set(config,idx,name,v,s);
}


/***********************************************************************
 * API functions
 **********************************************************************/

int sc68_init(sc68_init_t * init)
{
  int err = -1;
  option68_t * opt;
  sc68_init_t dummy_init;

  static option68_t debug_options[] = {
    {
      option68_BOL, "sc68-","dbg68k","debug",
      "run 68K in debug mode"
    }
  };

  if (sc68_init_flag) {
    err = error_add(0, "libsc68: %s\n","already initialized");
    goto error_no_shutdown;
  }
  sc68_errstr[0] = 0;

  if (!init) {
    memset(&dummy_init, 0, sizeof(dummy_init));
    init = &dummy_init;
  }

  sc68_cat = msg68_cat("sc68","sc68 library",DEBUG_SC68_O);

  /* 1st thing to do : set debug handler. */
  msg68_set_handler((msg68_t)init->msg_handler);
  msg68_set_cookie(0);

  msg68_cat_filter(init->debug_clr_mask,init->debug_set_mask);

  appname[0] = 0;
  if (init->argc > 0 && init->argv)
    appname_from_path(init->argv[0], appname, sizeof(appname));
  if (!appname[0])
    strcpy(appname,"sc68");

  /* Init config module */
  config68_init(0);

  /* Intialize file68. */
  init->argc = file68_init(init->argc, init->argv);

  option68_append(debug_options,sizeof(debug_options)/sizeof(*debug_options));
  /* option68_append(&emuopt,1); */
  option68_append(config68_options, config68_opt_count);
  init->argc = option68_parse(init->argc, init->argv, 0);

  /* Load config */
  sc68_config_load();

  /* Initialize emulators. */
  err = init_emu68(&init->argc, init->argv);

  /* Set default sampling rate. */
  set_spr(0, SC68_SPR_DEFAULT);

  /* $$$: No good */
  opt = option68_get("debug", 1);
  if (opt) {
    int val;
    if (!strcmp(opt->val.str, "yes"))
      val = 1 << sc68_cat;
    else
      val = strtoul(opt->val.str,0,0);
    msg68_cat_filter(~0,val);
  }

  opt    = option68_get("dbg68k", 1);
  dbg68k = opt ? opt->val.num : 0;

  sc68_init_flag = !err;

  if (err) {
    sc68_shutdown();
  }
error_no_shutdown:
  sc68_debug(0,"libsc68: initialized as '%s' -- %s\n", appname, ok_int(err));
  return -!!err;
}

void sc68_shutdown(void)
{
  if (config) {
    sc68_config_save();
    config68_destroy(config);
    config = 0;
  }

  if (sc68_init_flag) {
    sc68_init_flag = 0;
    file68_shutdown();
    config68_shutdown();          /* always after file68_shutdown() */
  }
  sc68_debug(0,"libsc68: shutdowned -- %s\n",ok_int(0));
}

sc68_t * sc68_create(sc68_create_t * create)
{
  sc68_t      * sc68 = 0;
  sc68_create_t dummy_create;

  if (!create) {
    memset(&dummy_create, 0, sizeof(dummy_create));
    create = &dummy_create;
  }

  /* Alloc SC68 struct. */
  sc68 = calloc(sizeof(sc68_t),1);
  if (!sc68)
    goto error;

  /* Set magic */
  sc68->magic = SC68_MAGIC;

  /* User private data. */
  sc68->cookie = create->cookie;

  /* Pick a short name */
  if (create->name) {
    strncpy(sc68->name, create->name, sizeof(sc68->name));
  } else {
    snprintf(sc68->name,sizeof(sc68->name),"sc68#%02d",++sc68_id);
  }
  sc68->name[sizeof(sc68->name)-1] = 0;

  /* Apply config */
  sc68_config_apply(sc68);

  /* Override config. */
  if (create->sampling_rate) {
    sc68->mix.rate = create->sampling_rate;
  }
  if (!sc68->mix.rate) {
    sc68->mix.rate = sc68_spr_def;
  }
  if (!sc68->time.def_ms) {
    sc68->time.def_ms = TIMEMS_DEF;
  }

  /* Create 68k emulator and pals. */
  if (init68k(sc68, create->log2mem, create->emu68_debug | dbg68k)) {
    goto error;
  }

  /* Set IO chipsets sampling rates */
  sc68->mix.rate = set_spr(sc68, sc68->mix.rate);
  if (sc68->mix.rate <= 0) {
    error_addx(sc68, "libsc68: %s\n", "invalid sampling rate");
    goto error;
  }
  create->sampling_rate = sc68->mix.rate;
  sc68_debug(sc68,"libsc68: sampling rate -- *%dhz*\n", create->sampling_rate);

  /* Finally gets all pathes. */
  /*   rsc68_get_path(&init->shared_path, */
  /*               &init->user_path, */
  /*               &init->lmusic_path, */
  /*               &init->rmusic_path); */

  /*   debugmsg68(-1,"sc68_create: shared-path=[%s]\n",init->shared_path); */
  /*   debugmsg68(-1,"sc68_create: user-path=[%s]\n",init->user_path); */
  /*   debugmsg68(-1,"sc68_create: music-path=[%s]\n",init->lmusic_path); */
  /*   debugmsg68(-1,"sc68_create: music-rpath=[%s]\n",init->rmusic_path); */

  /*   sc68_unic = sc68; */

  sc68_debug(0,"libsc68: sc68<%s> create -- %s\n", sc68->name, ok_int(0));
  return sc68;

error:
  sc68_destroy(sc68);
  sc68 = 0;
  sc68_debug(sc68,"libsc68: create -- %s\n", ok_int(-1));
  return 0;
}

void sc68_destroy(sc68_t * sc68)
{
  if (is_sc68(sc68)) {
    free(sc68->mix.buffer);
    sc68_close(sc68);
    safe_destroy(sc68);
    sc68_debug(sc68,"libsc68: sc68<%s> destroyed\n", sc68->name);
    free(sc68);
  } else {
    assert(!sc68);                      /* catch bad magic */
  }
}

static int get_spr(const sc68_t * sc68)
{
  return sc68 ? sc68->mix.rate : sc68_spr_def;
}

static int set_spr(sc68_t * sc68, int hz)
{
  if (hz == SC68_SPR_QUERY)
    hz = get_spr(sc68);
  else {
    const int min = SAMPLING_RATE_MIN;
    const int max = SAMPLING_RATE_MAX;
    const int def = SAMPLING_RATE_DEF;
    if (hz == SC68_SPR_DEFAULT)
      hz = def;
    if (hz <  min) hz = min;
    if (hz >  max) hz = max;
    if (sc68) {
      hz = ymio_sampling_rate(sc68->ymio, hz);
      hz = mwio_sampling_rate(sc68->mwio, hz);
      hz = paulaio_sampling_rate(sc68->paulaio, hz);
      sc68->mix.rate = hz;
    } else {
      sc68_spr_def = hz;
    }
  }
  return hz;
}

static int get_pcm_fmt(sc68_t * sc68)
{
  return SC68_PCM_S16;
}

static int set_pcm_fmt(sc68_t * sc68, int pcmfmt)
{
  return -(pcmfmt != SC68_PCM_S16);
}

/* void sc68_set_share(sc68_t * sc68, const char * path) */
/* { */
/*   rsc68_set_share(path); */
/* } */

/* void sc68_set_user(sc68_t * sc68, const char * path) */
/* { */
/*   rsc68_set_user(path); */
/* } */

/* static unsigned int calc_disk_time(sc68_t * sc68, const disk68_t * d) */
/* { */
/*   return (sc68 && sc68->disk == d && d->nb_mus == 1 && d->mus == sc68->mus) */
/*     ? sc68->time.length_ms */
/*     : d->time_ms; */
/* } */

static inline unsigned int fr_to_ms(unsigned int frames, unsigned int hz) {
  return (unsigned int) ( (u64)frames * 1000u / hz );
}

static inline unsigned int ms_to_fr(unsigned int ms, unsigned int hz) {
  return (unsigned int) ( (u64)ms * hz / 1000u );
}

static void stop_track(sc68_t * sc68, const int real_stop)
{
  assert(sc68);
  assert(sc68->disk);

  if (sc68->mix.buflen) {
    msg68_warning("libsc68: discard data -- *%d pcm*\n", sc68->mix.buflen);
  }

  sc68->mus             = 0;
  sc68->track           = 0;
  sc68->seek_to         = -1;
  sc68->track_to        = 0;
  sc68->loop_to         = 0;

  if (real_stop)
    sc68->time.origin_ms  = 0;
  else
    sc68->time.origin_ms += sc68->time.elapsed_ms;
  TRACE68(sc68_cat,
          "libsc68: %s -- *%d ms*\n",
          "new track origin is", sc68->time.origin_ms);

  sc68->time.elapsed_ms = 0;
  sc68->time.length_ms  = 0;

  sc68->mix.pass_count  = 0;
  sc68->mix.loop_count  = 0;
  sc68->mix.bufptr      = 0;
  sc68->mix.buflen      = 0;
}

static int finish(sc68_t * sc68, addr68_t pc, int sr,uint68_t maxinst)
{
  int status;

  sc68->emu68->reg.pc  = pc;
  sc68->emu68->reg.sr  = sr;
  sc68->emu68->reg.a[7] = sc68->emu68->memmsk+1-16;
  emu68_pushl(sc68->emu68, 0);

  status = emu68_finish(sc68->emu68, maxinst);
  while (status == EMU68_STP) {
    sc68_debug(sc68,
               "libsc68: stop #$%04X ignored @ $%6X\n",
               sc68->emu68->reg.sr, sc68->emu68->reg.pc);
    status = emu68_finish(sc68->emu68, EMU68_CONT);
  }

  if (status != EMU68_NRM) {
    char irqname[32];
    if (status == EMU68_HLT && (sc68->emu68->reg.sr & 0X3F00) == 0X2F00) {
      addr68_t vaddr;
      sc68->irq.vector = sc68->emu68->reg.sr & 0xFF;
      strcpy(irqname,"NC-");
      except_name(sc68->irq.vector,irqname+3);
      vaddr = Lpeek(sc68->emu68, sc68->irq.vector * 4);
      if (vaddr != INTR_ADDR + sc68->irq.vector * 8)
        strcpy(irqname,"CH-");
      if (sc68->irq.vector >= TRAP_VECTOR(0) &&
          sc68->irq.vector <= TRAP_VECTOR(15)) {
        int n = sc68->irq.vector-TRAP_VECTOR(0);
        if (trap_type[n])
          sprintf(irqname+3, "%s-$%X", trap_type[n], sc68->irq.sysfct);
      }
    } else if (status == EMU68_BRK && !sc68->emu68->instructions) {
      strcpy(irqname,"inst-overflow");
    } else {
      except_name(sc68->irq.vector,irqname);
    }
    error_addx(sc68,
               "libsc68: pass#%d @$%X"
               " %s (%02X)"
               " $%X/$%04X irq#%d (%s) @$%X\n",
               sc68->mix.pass_count, pc,
               emu68_status_name(status), status,
               sc68->emu68->reg.pc, sc68->emu68->reg.sr,
               sc68->irq.vector, irqname, sc68->irq.pc);
  }
  return status;
}

static int reset_emulators(sc68_t * sc68, const hwflags68_t * const hw)
{
  u8 * memptr;
  const addr68_t base = INTR_ADDR;
  int i;

  assert(sc68);
  assert(sc68->emu68);
  assert(hw);

  emu68_ioplug_unplug_all(sc68->emu68);
  emu68_mem_reset(sc68->emu68);

  if (hw->bit.amiga) {
    TRACE68(sc68_cat," -> Add Paula hardware\n");
    emu68_ioplug(sc68->emu68, sc68->paulaio);
    TRACE68(sc68_cat," -> Set PAULA as interruptible\n");
    emu68_set_interrupt_io(sc68->emu68, sc68->paulaio);
  }
  if (hw->bit.ym) {
    TRACE68(sc68_cat," -> Add SHIFTER hardware\n");
    emu68_ioplug(sc68->emu68, sc68->shifterio);
    TRACE68(sc68_cat," -> Add YM hardware\n");
    emu68_ioplug(sc68->emu68, sc68->ymio);
    TRACE68(sc68_cat," -> Add MFP hardware\n");
    emu68_ioplug(sc68->emu68, sc68->mfpio);
    TRACE68(sc68_cat," -> Set MFP as interruption\n");
    emu68_set_interrupt_io(sc68->emu68, sc68->mfpio);
  }
  if (hw->bit.ste) {
    TRACE68(sc68_cat," -> Add MW (STE) hardware\n");
    emu68_ioplug(sc68->emu68, sc68->mwio);
  }
  emu68_reset(sc68->emu68);

  /* disable that we should not need it */
  if (emu68_debugmode(sc68->emu68)) {
    TRACE68(sc68_cat," -> Clear 68k memory\n");
    emu68_memset(sc68->emu68,0,0,0);
  }
  memptr = emu68_memptr(sc68->emu68,0,0x1000);

  TRACE68(sc68_cat," -> Exception detection code in $%06x\n", INTR_ADDR);

  /* Install non initialized exception detection code. */
  for (i=0; i<256; ++i) {
    addr68_t vector = base + i*8;

    /* setup vector */
    memptr[ (i<<2) + 0 ] = vector >> 24;
    memptr[ (i<<2) + 1 ] = vector >> 16;
    memptr[ (i<<2) + 2 ] = vector >>  8;
    memptr[ (i<<2) + 3 ] = vector;

    /* Timer-C vector (69) might be programmed without setting the
     * vector. It is legitimate as the ST system uses this timer at
     * 200hz. We could check this frequence instead of acknowledge
     * this interruption.
     */

    /* install instruction stop #$2FNN */
    memptr[ vector + 0 ] = 0x4e;
    memptr[ vector + 1 ] = 0x72 /* + (i == 69) */;
    memptr[ vector + 2 ] = 0x2F; /* magic SR value (see handler) */
    memptr[ vector + 3 ] = i;    /* vector number                */
    memptr[ vector + 4 ] = 0x4e; /* RESET                        */
    memptr[ vector + 5 ] = 0x70;
    memptr[ vector + 6 ] = 0x4e; /* RTE                          */
    memptr[ vector + 7 ] = 0x73;
  }

  /* $$$ Legacy fix for Zound Dragger. Would be good to get rid of
   * this. */
  memptr[0x41a] = 0x00;
  memptr[0x41b] = 0x10;

  /* Install TOS trap emulator unless AMIGA */
  if (!hw->bit.amiga) {
    int status;

    TRACE68(sc68_cat," -> Load TOS trap emulator @$%06x-$%06x\n",
            TRAP_ADDR,TRAP_ADDR+sizeof(trap_func)-1);

    /* Ensure trap emulator do not override exception handler */
    assert(sizeof(trap_func) <= INTR_ADDR-TRAP_ADDR);

    /* Install trap emulator */
    emu68_memput(sc68->emu68, TRAP_ADDR, trap_func, sizeof(trap_func));
    sc68->emu68->cycle = 0;
    TRACE68(sc68_cat," -> Running trap init code -- $%06x ...\n", TRAP_ADDR);
    status = finish(sc68, TRAP_ADDR, 0x2300, INIT_MAX_INST);
    if ( status != EMU68_NRM ) {
      error_addx(sc68,
                 "libsc68: abnormal 68K status %d (%s) in trap code\n",
                 status, emu68_status_name(status));
      return SC68_ERROR;
    }
  }
  return SC68_OK;
}

static int load_replay(sc68_t * sc68, const char * replay, int a0)
{
  int err, size;
  vfs68_t * is;
  char rname[256];
  assert(sc68);
  assert(replay);

  TRACE68(sc68_cat, " -> external replay -- %s\n", replay);
  strcpy(rname,"sc68://replay/");    /* $$$ to be change a sc68://replay/... */
  strcat68(rname, replay, sizeof(rname)-1);
  rname[sizeof(rname)-1] = 0;
  is = uri68_vfs(rname, 1, 0);
  err = vfs68_open(is);
  err = err || (size = vfs68_length(is), size < 0);
  err = err || stream_read_68k(sc68, a0, is, size);
  vfs68_destroy(is);
  if (err) {
    error_add(sc68,
              "libsc68: failed to load external replay -- %s",
              rname);
    return SC68_ERROR;
  }
  TRACE68(sc68_cat," -> external replay -- [%06x-%06x]\n", a0, a0+size-1);
  return a0 + ((size + 1) & ~1);
}

static int run_music_init(sc68_t * sc68, const music68_t * m, int a0, int a6)
{
  int status;

  assert(sc68);
  assert(m);

  /* Set 68K registers. */
  sc68->emu68->reg.d[0] = m->d0;
  sc68->emu68->reg.d[1] = !m->hwflags.bit.ste;
  sc68->emu68->reg.d[2] = m->datasz;
  sc68->emu68->reg.a[0] = a0;
  sc68->emu68->reg.a[6] = a6;           /* original replay address  */
  if (m->has.asid_trk)
    sc68->emu68->reg.d[7] = 0
      | ( ('A'+m->has.asid_tA) << 24 )
      | ( ('A'+m->has.asid_tB) << 16 )
      | ( ('A'+m->has.asid_tC) <<  8 )
      | ( ('A'+m->has.asid_tX) <<  0 );

  /* Run music init code. */
  sc68->emu68->cycle = 0;
  TRACE68(sc68_cat," -> Running music init code ...\n");
  status = finish(sc68, sc68->playaddr, 0x2300, INIT_MAX_INST);
  if ( status != EMU68_NRM ) {
    error_addx(sc68, "libsc68: abnormal 68K status %d (%s) in init code",
               status, emu68_status_name(status));
    return SC68_ERROR;
  }
  return SC68_OK;
}

static int change_track(sc68_t * sc68, int track, int loop)
{
  const disk68_t  * d;
  const music68_t * m;
  int         a0,a6;
  int         force_ms;

  assert(sc68);
  assert(sc68->disk);
  assert(track > 0 && track <= sc68->disk->nb_mus);
  assert(loop >= -1);

  stop_track(sc68, 0);

  d = sc68->disk;
  m = d->mus + track - 1;

  /* ReInit 68K & IO */
  reset_emulators(sc68, &m->hwflags);

  /* Set music replay address in 68K memory. */
  sc68->playaddr = a0 = m->a0;
  TRACE68(sc68_cat," -> play address -- $%06x\n", sc68->playaddr);

  if (m->has.asid_trk) {
    if (a0 = load_replay(sc68, "asidifier", a0), a0 == SC68_ERROR)
      return SC68_ERROR;
  }
  a6 = a0;

  /* Load external replay into 68K memory. */
  if (m->replay && (a0 = load_replay(sc68, m->replay, a0), a0 == SC68_ERROR))
    return SC68_ERROR;

  /* Copy music data into 68K memory */
  if (emu68_memput(sc68->emu68, a0, (u8 *)m->data, m->datasz)) {
    error_add(sc68,"libsc68: %s\n", emu68_error_get(sc68->emu68));
    return SC68_ERROR;
  }
  TRACE68(sc68_cat," -> music data -- [%06x-%06x]\n", a0, a0+m->datasz-1);

  /* Run music init code */
  if ( run_music_init(sc68, m, a0, a6) == SC68_ERROR )
    return SC68_ERROR;

  /* Ensure sampling rate */
  if (sc68->mix.rate <= 0)
    sc68->mix.rate = sc68_spr_def;
  if (sc68->mix.rate < SAMPLING_RATE_MIN ||
      sc68->mix.rate > SAMPLING_RATE_MAX) {
    error_addx(sc68,
               "libsc68: %s -- %dhz\n",
               "invalid sampling rate",
               sc68->mix.rate);
    return SC68_ERROR;
  }

  /* force length if asked or if we don't have a length */
  force_ms = d->force_ms;
  if (!force_ms && !m->first_fr)
    force_ms = sc68->time.def_ms;

  assert(m->loops >= -1 && m->loops != 0);
  if (loop == SC68_DEF_LOOP)
    loop = m->loops;
  sc68->mix.loop_count = 0;
  sc68->mix.loop_total = (loop == SC68_INF_LOOP) ? 0 : loop;
  sc68->mix.pass_count = 0;


  if (force_ms > 0) {
    TRACE68(sc68_cat,"libsc68: disk force length -- *%d ms*\n", force_ms);
    sc68->time.length_ms = force_ms;
    sc68->mix.pass_2loop = ms_to_fr(sc68->time.length_ms, m->frq);
    sc68->mix.pass_3loop = sc68->mix.pass_2loop;
    sc68->mix.pass_total = sc68->mix.loop_total * sc68->mix.pass_2loop;
  } else {
    sc68->mix.pass_2loop = m->first_fr;
    sc68->mix.pass_3loop = m->loops_fr;
    sc68->mix.pass_total = (sc68->mix.loop_total > 0)
      ? sc68->mix.pass_2loop + (sc68->mix.loop_total-1) * sc68->mix.pass_3loop
      : 0
      ;
    sc68->time.length_ms = fr_to_ms(sc68->mix.pass_total, m->frq);
  }

  TRACE68(sc68_cat," -> loops           : %u\n", sc68->mix.loop_total);
  TRACE68(sc68_cat," -> first pass ms   : %u\n",
          fr_to_ms(sc68->mix.pass_2loop, m->frq));
  TRACE68(sc68_cat," -> loops pass ms   : %u\n",
          fr_to_ms(sc68->mix.pass_3loop, m->frq));
  TRACE68(sc68_cat," -> total ms        : %u\n", sc68->time.length_ms);

  /* $$$$ fix start time on the fly because it is broken ! */
  // m->start_ms = sc68->time.save_ms;
  // TRACE68(sc68_cat," -> start ms        : %u\n", m->start_ms);

  TRACE68(sc68_cat," -> first frames    : %u\n", sc68->mix.pass_2loop);
  TRACE68(sc68_cat," -> loops frames    : %u\n", sc68->mix.pass_3loop);
  TRACE68(sc68_cat," -> total frames    : %u\n", sc68->mix.pass_total);


  TRACE68(sc68_cat," -> replay rate     : %u\n", m->frq);
  sc68->mix.cycleperpass =
    ( m->frq % 50u == 0 && sc68->emu68->clock == EMU68_ATARIST_CLOCK)
    ? 160256u * 50u / m->frq    /* exact value for genuine Atari ST */
    : (sc68->emu68->clock / m->frq)
    ;
  TRACE68(sc68_cat," -> cycle (exact)   : %u\n", sc68->mix.cycleperpass);

  if (m->hwflags.bit.ym) {
    cycle68_t cycles;
    /* Make sure ym-cycle are mutiple of 32 as required by current
     * emulator.
     *
     * $$$ I am not sure this works in all cases of frequency but it
     * is should be ok as far as cpu and ym frequency are multiple.
     *
     * $$$ In fact I am now pretty sure it does not work if
     * frequencies are not multiple.
     */
    cycles = ymio_cycle_cpu2ym(sc68->ymio,sc68->mix.cycleperpass);
    cycles = (cycles+31) & ~31;
    sc68->mix.cycleperpass = ymio_cycle_ym2cpu(sc68->ymio,cycles);

    /* verify */
    cycles = ymio_cycle_cpu2ym(sc68->ymio,sc68->mix.cycleperpass);
    TRACE68(sc68_cat," -> ym cycles       : %u [%s]\n",
            cycles, strok68(cycles&31));
  }
  sc68->mix.cycleperpass = (sc68->mix.cycleperpass+31) & ~31;
  TRACE68(sc68_cat," -> cycle (round)   : %u\n", sc68->mix.cycleperpass);

  if (m->frq == 60 && sc68->shifterio) {
    TRACE68(sc68_cat," -> Force shifter to 60Hz\n");
    shifterio_reset(sc68->shifterio,60);
  }

  /* Compute size of buffer needed for cycleperpass length at current rate. */
  if (1) {
    u64 len;
    len  = sc68->mix.rate;
    len *= sc68->mix.cycleperpass;
    len /= sc68->emu68->clock;
    sc68->mix.stdlen = (int) len;
    TRACE68(sc68_cat," -> std buffer len  : %u\n", sc68->mix.stdlen);
  }

  /* Compute *REAL* required size (in PCM) for buffer and realloc */
  if (1) {
    sc68->mix.bufreq = m->hwflags.bit.ym
      ? ymio_buffersize(sc68->ymio, sc68->mix.cycleperpass)
      : sc68->mix.stdlen
      ;
    TRACE68(sc68_cat," -> mix buffer len  : %u\n", sc68->mix.bufreq);

    /* Should not happen. Anyway it does not hurt. */
    if (m->hwflags.bit.amiga && sc68->mix.stdlen > sc68->mix.bufreq)
      sc68->mix.bufreq = sc68->mix.stdlen;

    TRACE68(sc68_cat," -> required PCM buffer size -- *%u pcm*\n",
            sc68->mix.bufreq);
    TRACE68(sc68_cat," ->  current PCM buffer size -- *%u pcm*\n",
            sc68->mix.bufmax);

    if (sc68->mix.bufreq > sc68->mix.bufmax) {
      free(sc68->mix.buffer);
      sc68->mix.bufmax = 0;
      TRACE68(sc68_cat," -> Alloc new PCM buffer -- *%u pcm*\n",
              sc68->mix.bufreq);
      sc68->mix.buffer = malloc(sc68->mix.bufreq << 2);
      if (!sc68->mix.buffer) {
        error_add(sc68,"libsc68: %s\n", strerror(errno));
        return SC68_ERROR;
      }
      sc68->mix.bufmax = sc68->mix.bufreq;
    }
  }
  TRACE68(sc68_cat," -> buffer length -- %u pcm\n", sc68->mix.bufreq);

  /* Setup internal info struct */
  if (sc68_music_info(sc68, &sc68->info, track, 0))
    return SC68_ERROR;
  sc68->mus           = m;
  sc68->track         = track;

  TRACE68(sc68_cat, "libsc68: track ready -- %s %s - %s - %s\n",
          sc68->info.trk.time,
          sc68->info.artist,
          sc68->info.album,
          sc68->info.title);

  return SC68_OK;
}

static int check_track_range(sc68_t * const sc68,
                             const disk68_t * const disk,
                             const int track)
{
  if (track < 1 || track > disk->nb_mus) {
    error_addx(sc68, "libsc68: %s\n","track out of range");
    return -1;
  }
  return 0;
}

/** Start current music of current disk.
 */
static int apply_change_track(sc68_t * const sc68)
{
  int track, loop;

  assert(has_disk(sc68));

  if (track = sc68->track_to, !track)
    return SC68_OK;
  loop = sc68->loop_to;
  sc68->track_to = sc68->loop_to = 0;

  /* -1 : stop */
  if (track == -1) {
    TRACE68(sc68_cat,"libsc68: %s\n","stop requested");
    sc68->mix.buflen = 0; /* warning removal in stop_track() */
    stop_track(sc68, 1);
    sc68->time.origin_ms = 0;
    return SC68_END;
  }
  TRACE68(sc68_cat,
          "libsc68: %s -- *%02d*\n",
          "change track requested", track);

  if (check_track_range(sc68, sc68->disk, track))
    return SC68_ERROR;

  if (change_track(sc68, track, loop) != SC68_OK)
    return SC68_ERROR;

  return SC68_CHANGE;
}

static unsigned int calc_current_ms(sc68_t * const sc68)
{
  u64 ms;
  ms  = sc68->mix.pass_count;
  ms *= sc68->mix.cycleperpass * 1000u;
  ms /= sc68->emu68->clock;

  assert(ms == (u64)(unsigned int)ms);  /* check overflow */

  return sc68->time.elapsed_ms = (unsigned int) ms;
}

int sc68_process(sc68_t * sc68, void * buf16st, int * _n)
{
  int ret;

  if (!is_sc68(sc68)) {
    ret = SC68_ERROR;
  } else if (!_n) {
    /* Flush internal PCM buffer and apply change track request. */
    ret = SC68_IDLE | apply_change_track(sc68);
  } else if (!buf16st) {
    ret = SC68_ERROR;
  } else {
    int n = *_n;
    ret = (n < 0) ? SC68_ERROR : SC68_IDLE;

    while (n > 0) {
      int len;

      /* No more pcm in internal buffer ... */
      if (!sc68->mix.buflen) {
        int status;

        /* Checking for loop */
        if (sc68->mix.pass_2loop && !--sc68->mix.pass_2loop) {
          sc68->mix.pass_2loop = sc68->mix.pass_3loop;
          sc68->mix.loop_count++;
          ret |= SC68_LOOP;
        }

        /* Checking for end */
        if (sc68->mix.pass_total &&
            sc68->mix.pass_count >= sc68->mix.pass_total) {
          int next_track = sc68->track+1;
          sc68->track_to =
            (sc68->disk->force_track || next_track > sc68->disk->nb_mus)
            ? -1                        /* stop */
            : next_track                /* next track */
            ;
          sc68->seek_to  = -1;
        }

        ret |= apply_change_track(sc68);
        if (ret & (SC68_END|SC68_CHANGE)) /* exit on error|end|change */
          break;
        ret &= ~SC68_IDLE;              /* No more idle */

        /* Run 68K emulator */
        status = finish(sc68, sc68->playaddr+8, 0x2300, PLAY_MAX_INST);
        if (status == EMU68_NRM)
          status = emu68_interrupt(sc68->emu68, sc68->mix.cycleperpass);
        if (status != EMU68_NRM) {
          error_addx(sc68,
                     "libsc68: abnormal 68K status %d (%s) in play pass %u\n",
                     status, emu68_status_name(status),
                     sc68->mix.pass_count);
          ret = SC68_ERROR;
          break;
        }

        /* Reset pcm pointer. */
        sc68->mix.bufptr = sc68->mix.buffer;
        sc68->mix.buflen = sc68->mix.bufreq;

        /* Fill pcm buufer depending on architecture */
        if (sc68->mus->hwflags.bit.amiga) {
          /* Amiga - Paula */
          paula_mix(sc68->paula,(s32*)sc68->mix.bufptr,sc68->mix.buflen);
          mixer68_blend_LR(sc68->mix.bufptr, sc68->mix.bufptr, sc68->mix.buflen,
                           sc68->mix.aga_blend, 0, 0);
        } else {
          if (sc68->mus->hwflags.bit.ym) {
            int err =
              ymio_run(sc68->ymio, (s32*)sc68->mix.bufptr,
                       sc68->mix.cycleperpass);
            if (err < 0) {
              ret = SC68_ERROR;
              sc68->mix.buflen = 0;
              break;
            }
            sc68->mix.buflen = err;
          } else {
            mixer68_fill(sc68->mix.bufptr, sc68->mix.buflen=sc68->mix.bufreq, 0);
          }

          if (sc68->mus->hwflags.bit.ste)
            /* STE / MicroWire */
            mw_mix(sc68->mw, (s32 *)sc68->mix.bufptr, sc68->mix.buflen);
          else
            /* Else simply process with left channel duplication. */
            mixer68_dup_L_to_R(sc68->mix.bufptr, sc68->mix.bufptr,
                               sc68->mix.buflen, 0);
        }

        /* Advance time */
        calc_current_ms(sc68);
        sc68->mix.pass_count++;
      }

      assert(sc68->mix.buflen > 0);

      /* Copy to destination buffer. */
      len = sc68->mix.buflen <= n ? sc68->mix.buflen : n;
      mixer68_copy((u32 *)buf16st,sc68->mix.bufptr,len);
      buf16st = (u32 *)buf16st + len;
      sc68->mix.bufptr += len;
      sc68->mix.buflen -= len;
      n                -= len;
    }

    *_n -= n;
  }
  return ret;
}

int sc68_is_our_uri(const char * uri, const char *exts, int * is_remote)
{
  assert(!"TO DO");
  return 0;
  /* return file68_is_our_uri(uri,exts,is_remote); */
}

static int load_disk(sc68_t * sc68, disk68_t * d, int free_on_close)
{
  // int track, loop;

  if (!sc68 || !d) {
    goto error;
  }

  if (sc68->disk) {
    error_add(sc68,"libsc68: %s","disk already loaded");
    goto error;
  }

  sc68->tobe3 = !!free_on_close;
  sc68->disk  = d;
  sc68->track = 0;
  sc68->mus   = 0;

  return sc68_play(sc68, SC68_DEF_TRACK, SC68_DEF_LOOP);

error:
  free(d);
  return -1;
}

int sc68_load(sc68_t * sc68, vfs68_t * is)
{
  return load_disk(sc68, file68_load(is), 1);
}

int sc68_load_uri(sc68_t * sc68, const char * uri)
{
  return load_disk(sc68, file68_load_uri(uri), 1);
}

int sc68_load_mem(sc68_t * sc68, const void * buffer, int len)
{
  return load_disk(sc68, file68_load_mem(buffer, len), 1);
}


sc68_disk_t sc68_load_disk(vfs68_t * is)
{
  return (sc68_disk_t) file68_load(is);
}

sc68_disk_t sc68_load_disk_uri(const char * uri)
{
  return (sc68_disk_t) file68_load_uri(uri);
}

sc68_disk_t sc68_disk_load_mem(const void * buffer, int len)
{
  return (sc68_disk_t) file68_load_mem(buffer, len);
}

void sc68_disk_free(sc68_disk_t disk)
{
  file68_free(disk);                    /* $$$ check this */
}

int sc68_open(sc68_t * sc68, sc68_disk_t disk)
{
  if (!disk) {
    sc68_close(sc68);
    return -1; /* Not an error but notifiy no disk has been loaded */
  }
  if (!sc68) {
    return -1;
  }
  return load_disk(sc68, disk, 0);
}

void sc68_close(sc68_t * sc68)
{
  if (sc68 && sc68->disk) {
    sc68->mix.buflen = 0; /* warning removal in stop_track() */
    stop_track(sc68, 1);
    if (sc68->tobe3)
      file68_free((disk68_t *)sc68->disk);
    sc68->tobe3     = 0;
    sc68->disk      = 0;
  }
}

int sc68_play(sc68_t * sc68, int track, int loop)
{
  const disk68_t * d;

  if (!sc68) {
    return -1;
  }
  d = sc68->disk;
  if (!d) {
    return -1;
  }

  /* track == -1 : read current track or current loop. */
  if (track == SC68_GET_TRACK) {
    return loop == SC68_GET_LOOP
      ? sc68->mix.loop_count
      : sc68->track;
  }

  /* Choose track */
  if (d->force_track > 0) {
    /* Mean track has been forced while the disk was loaded (via
     * URI) */
    track = d->force_track;
    TRACE68(sc68_cat,"libsc68: disk has a forced track -- *%02d*\n",track);
  } else if (track == SC68_DEF_TRACK) {
    track = d->def_mus + 1;
    if (sc68->cfg_track > 0 && sc68->cfg_track <= d->nb_mus) {
      /* track forced in config only applied if default track was
       * asked and it is not out of range. */
      track = sc68->cfg_track;
      TRACE68(sc68_cat,"libsc68: config has a forced track -- *%02d*\n", track);
    }
  }

  /* Unless disk has an overide. */
  if (d->force_loops) {
    loop = d->force_loops;
    TRACE68(sc68_cat,"libsc68: disk has a forced loop -- %02d\n",loop);
  }
  else if (loop == SC68_DEF_LOOP && sc68->cfg_loop != SC68_DEF_LOOP) {
    loop = sc68->cfg_loop;
    TRACE68(sc68_cat,"libsc68: config has a forced loop -- %02d\n", loop);
  }

  /* Check track range. */
  if (check_track_range(sc68, sc68->disk, track)) {
    return -1;
  }

  /* Set change track. Real track loading occurs during process thread to
     avoid multi-threading problems. */
  sc68->track_to = track;
  sc68->seek_to  = -1;
  sc68->loop_to  = loop;

  return 0;
}

int sc68_stop(sc68_t * sc68)
{
  if (!sc68 || !sc68->disk) {
    return -1;
  }
  sc68->track_to = -1;
  sc68->seek_to  = -1;
  return 0;
}

enum {
  SC68_POS_PLAY,
  SC68_POS_TRACK
};

static int get_pos(sc68_t * sc68, int origin)
{
  int pos = -1;
  if (sc68->track > 0) {
    pos = 0;
    switch (origin) {
    case SC68_POS_PLAY:
      pos += sc68->time.origin_ms;
    case SC68_POS_TRACK:
      pos += sc68->time.elapsed_ms;
    }
  }
  return pos;
}

int sc68_music_info(sc68_t * sc68, sc68_music_info_t * info, int track,
                    sc68_disk_t disk)
{
  const disk68_t  * d;
  const music68_t * m;

  static const char * hwtable[8] = {
    "none",
    "Yamaha-2149",
    "MicroWire (STE)",
    "Yamaha-2149 & MicroWire (STE)",
    "Amiga/Paula",
    "Yamaha-2149 & Amiga/Paula",
    "MicroWire (STE) & Amiga/Paula",
    "Yamaha-2149 & MicroWire (STE) & Amiga/Paula",
  };

  /* If disk is given use it, else use sc68 disk. */
  d = disk ? disk : (sc68 ? sc68->disk : 0);
  if (!d || !info)
    return -1;

  /* Asking for current or default track */
  if (track == 0 || track == -1)
    track = (disk || !sc68->track) ? d->def_mus+1 : sc68->track;

  if (check_track_range(sc68, d, track))
    return -1;

  /* Asking for the track being played by sc68, simply copy internal
   * info struct, unless off course it is the one being filled.
   */
  if (sc68 && track == sc68->track && info != &sc68->info) {
    *info = sc68->info;
    return 0;
  }
  m = d->mus + track - 1;

  info->tracks      = d->nb_mus;

  /* $$$ TODO: this
     info->start_ms    = m->start_ms;
  */
  info->start_ms = 0;


  info->loop_ms     = m->loops_ms;
  info->replay      = m->replay ? m->replay : "built-in";
  info->rate        = m->frq;
  info->addr        = m->a0;

  info->dsk.track   = d->def_mus+1;

  /* $$$ TODO: this */
  info->dsk.time_ms = d->time_ms; /* calc_disk_time(sc68,d); */

  strtime68(info->dsk.time, info->tracks, (info->dsk.time_ms+999u)/1000u);
  info->dsk.ym      = d->hwflags.bit.ym;
  info->dsk.ste     = d->hwflags.bit.ste;
  info->dsk.amiga   = d->hwflags.bit.amiga;
  info->dsk.hw      = hwtable[info->dsk.ym+(info->dsk.ste<<1)+(info->dsk.amiga<<2)];
  info->dsk.tags    = file68_tag_count(d, 0);
  info->dsk.tag     = (sc68_tag_t *) d->tags.array;

  info->trk.track   = track;

  /* $$$ TODO: this
     info->trk.time_ms = (sc68 && m == sc68->mus) ? sc68->time.length_ms : m->total_ms; */
  info->trk.time_ms = m->first_ms;


  strtime68(info->trk.time, track,(info->trk.time_ms+999u)/1000u);

  info->trk.ym    = m->hwflags.bit.ym;
  info->trk.ste   = m->hwflags.bit.ste;
  info->trk.amiga = m->hwflags.bit.amiga;
  info->trk.hw    = hwtable[info->trk.ym+(info->trk.ste<<1)+(info->trk.amiga<<2)];

  info->trk.tags  = file68_tag_count(d, track);
  info->trk.tag   = (sc68_tag_t *) m->tags.array;

  info->album     = d->tags.tag.title.val;
  info->title     = m->tags.tag.title.val;
  info->artist    = m->tags.tag.artist.val;

  return 0;
}


int sc68_tag_get(sc68_t * sc68, sc68_tag_t * tag, int track,
                 sc68_disk_t disk)
{
  const disk68_t  * d;

  /* If disk is given use it, else use sc68 disk. */
  d = disk ? disk : (sc68 ? sc68->disk : 0);
  if (!d || !tag)
    return -1;

  /* Asking for current or default track */
  if (track == -1)
    track = (disk || !sc68->track) ? d->def_mus+1 : sc68->track;

  tag->val = (char *) file68_tag_get(d, track, tag->key);
  return -!tag->val;
}

int sc68_tag_enum(sc68_t * sc68, sc68_tag_t * tag, int track, int idx,
                  sc68_disk_t disk)
{
  const disk68_t  * d;

  /* If disk is given use it, else use sc68 disk. */
  d = disk ? disk : (sc68 ? sc68->disk : 0);
  if (!d || !tag)
    return -1;

  /* Asking for current or default track */
  if (track == -1)
    track = (disk || !sc68->track) ? d->def_mus+1 : sc68->track;

  return file68_tag_enum(d, track, idx,
                         (const char **)&tag->key,
                         (const char **)&tag->val);
}


#if 0
sc68_music_info_t * sc68_music_info(sc68_t * sc68, int track, sc68_disk_t disk)
{
  sc68_music_info_t * info = 0;
  int infosz;

  disk68_t * d;
  music68_t * m = 0;
  int hw;
  hwflags68_t hwf;
  int loop, force_loop, loop_to;

  char * tagdata;
  const char * key, * val;
  int tag_cnt, tag_len;

  static const char * hwtable[8] = {
    "none",
    "Yamaha-2149",
    "MicroWire (STE)",
    "Yamaha-2149 & MicroWire (STE)",
    "Amiga/Paula",
    "Yamaha-2149 & Amiga/Paula",
    "MicroWire (STE) & Amiga/Paula",
    "Yamaha-2149 & MicroWire (STE) & Amiga/Paula",
  };

  if (!sc68 && !disk)
    return 0;

  d = disk ? disk : sc68->disk;
  if (!d)
    return 0;

  /* -1 : use current track or default track (depending if disk was given) */
  if (track == -1)
    track = disk ? d->def_mus+1 : sc68->track;

  if (track <= 0 || track > d->nb_mus)
    return 0;

  /* count and measure tags */
  tag_cnt = tag_len = 0;
  for (tag_cnt = 0;
       !file68_tag_enum(d, track, tag_cnt, &key, &val);
       ++tag_cnt) {
    tag_len += strlen(key) + strlen(val) + 2;
  }

  /* alloc the info struct */
  infosz
    = sizeof(*info)
    - sizeof(info->tag)
    + sizeof(*info->tag) * tag_cnt
    + tag_len
    ;
  if (info = sc68_calloc(infosz), !info) {
    return 0;
  }
  info->tags = tag_cnt;

  /* copy tag data */
  tagdata = (char *)(info->tag+tag_cnt);
  for (tag_cnt = 0;
       !file68_tag_enum(d, track, tag_cnt, &key, &val);
       ++tag_cnt) {
    info->tag[tag_cnt].key = tagdata;
    tagdata = stpcpy(tagdata, key) + 1;
    info->tag[tag_cnt].val = tagdata;
    tagdata = stpcpy(tagdata, val) + 1;
  }

  info->title = (tag_cnt > TAG68_ID_TITLE)
    ? info->tag[TAG68_ID_TITLE].val
    : not_available
    ;

  info->author = (tag_cnt > TAG68_ID_ARTIST)
    ? info->tag[TAG68_ID_ARTIST].val
    : not_available
    ;

  m = d->mus + ((!track) ? d->def_mus : (track - 1));
  info->track  = track;
  info->tracks = d->nb_mus;

  force_loop   = sc68 ? sc68->force_loop : -1;
  loop_to      = sc68 ? sc68->loop_to    : -1;
  loop         = calc_loop_total(force_loop, loop_to, m->loop);

  if (!track) {
    /* disk info */
    info->replay   = 0;
    info->time_ms  = calc_disk_time(sc68,d);
    info->start_ms = 0;
    hwf.all        = d->hwflags.all;
    track          = info->tracks;
  } else {
    /* track info */
    info->replay   = m->replay ? m->replay : "built-in";
    info->time_ms  = (sc68 && m == sc68->mus)
      ? sc68->time.length_ms : (m->time_ms ? m->time_ms : TIMEMS_DEF);
    info->start_ms = m->start_ms;
    hwf.all        = m->hwflags.all;
  }

  loop = calc_loop_total(force_loop,
                         (sc68 && m==sc68->mus) ? loop_to : 1,
                         m->loop);
  info->time_ms  *= loop;
  info->start_ms *= loop;

  /* If there is a track remapping always use it. */
  if (m->track) {
    track = info->track = m->track;
  }

  /* info->author    = m->tags.tag.artist.val ? m->tags.tag.artist.val  : "n/a"; */
  /* info->composer  = /\* m->composer  ? m->composer  : *\/ "n/a"; */
  /* info->ripper    = /\* m->ripper    ? m->ripper    : *\/ "n/a"; */
  /* info->converter = /\* m->converter ? m->converter : *\/ "n/a"; */

  hw = 7 &
    (  (hwf.bit.ym      ? SC68_YM    : 0)
       | (hwf.bit.ste   ? SC68_STE   : 0)
       | (hwf.bit.amiga ? SC68_AMIGA : 0)
      )
    ;

  info->hw.ym    = hwf.bit.ym;
  info->hw.ste   = hwf.bit.ste;
  info->hw.amiga = hwf.bit.amiga;
  info->rate     = m->frq;
  info->addr     = m->a0;
  info->hwname   = hwtable[hw];
  strtime68(info->time, track, (info->time_ms+999u)/1000u);

  return info;
}
#endif

vfs68_t * sc68_vfs(const char * uri, int mode, int argc, ...)
{
  vfs68_t * vfs;
  va_list list;

  va_start(list, argc);
  vfs = uri68_vfs_va(uri, mode, argc, list);
  va_end(list);

  return vfs;
}

const char * sc68_error(sc68_t * sc68)
{
  return is_sc68(sc68)
    ? sc68->errstr
    : sc68_errstr
    ;
}

static int error_addx(sc68_t * sc68, const char * fmt, ...)
{
  va_list list;
  char * errstr;
  int n;

  n = strncmp(fmt,"libsc68: ",9) ? 0 : 9; /* skip "libsc68: " */
  errstr = is_sc68(sc68) ? sc68->errbuf : sc68_errstr;
  va_start(list, fmt);
  n = vsnprintf(errstr, ERRMAX, fmt+n, list);
  va_end(list);
  if (n > 0 && errstr[n-1] == '\n')
    errstr[n-1] = 0;
  if (sc68)
    sc68->errstr = sc68->errbuf;
  va_start(list,  fmt);
  error68_va(fmt,list);
  va_end(list);
  return -1;
}

static int error_add(sc68_t * sc68, const char * fmt, const char * errstr)
{
  if (is_sc68(sc68)) {
    sc68->errstr = errstr;
    error68x(sc68,fmt,errstr);
  } else
    error68(fmt,errstr);
  return -1;
}

static void sc68_debug(sc68_t * sc68, const char * fmt, ...)
{
  va_list list;

  assert(null_or_sc68(sc68));
  va_start(list,fmt);
  if (!sc68)
    msg68_va(sc68_cat, fmt, list);
  else if (is_magic(sc68))
    msg68x_va(sc68_cat, sc68, fmt, list);
  va_end(list);
}

int sc68_cntl(sc68_t * sc68, int fct, ...)
{
  int res;
  va_list list;

  assert(null_or_sc68(sc68));

  /* Default to error if no sc68 instance is given. Function that
   * don't need a valid sc68 object have to clear the error flag
   */
  if (!sc68)
    res = -1;
  else if (is_magic(sc68))
    res = 0;
  else
    return -1;

  va_start(list, fct);

  switch (fct) {
    /* Functions accepting null */

  case SC68_NOP:
    break;

  case SC68_GET_NAME:
    *va_arg(list, char **) = sc68 ? sc68->name : appname;
    res = 0;
    break;

  case SC68_GET_PCM:
    res = get_pcm_fmt(sc68);
    break;

  case SC68_SET_PCM:
    res = set_pcm_fmt(sc68, va_arg(list, int));
    break;

  case SC68_GET_SPR:
    res = get_spr(sc68);
    break;

  case sc68_SET_SPR:
    res = set_spr(sc68, va_arg(list, int));
    break;

  default:
    if (!sc68)
      break;

    switch (fct) {
      /* Functions not accepting null */

    case SC68_GET_DISK:
      *va_arg(list, const disk68_t **)
        = is_disk(sc68->disk) ? sc68->disk : 0;
      break;

    case SC68_GET_TRACKS:
      res = is_disk(sc68->disk) ? sc68->disk->nb_mus : -1;
      break;

    case SC68_GET_TRACK:
      res = sc68->track;
      break;

    case SC68_DEF_TRACK:
      if (is_disk(sc68->disk))
        res = sc68->disk->def_mus+1;
      break;

    case SC68_GET_LOOPS:
      if (sc68->track > 0)
        res = sc68->mix.loop_total;
      break;

    case SC68_GET_LOOP:
      if (sc68->track > 0)
        res = sc68->mix.loop_count;
      break;

    case SC68_GET_POS:
      res = get_pos(sc68, SC68_POS_TRACK);
      break;

    case SC68_GET_PLAYPOS:
      res = get_pos(sc68, SC68_POS_PLAY);
      break;

    case SC68_GET_COOKIE:
      *va_arg(list, void **) = sc68->cookie;
      break;

    case SC68_SET_COOKIE:
      sc68->cookie = va_arg(list, void *);
      break;

    case SC68_EMULATORS:
      *va_arg(list, void **) = &sc68->emu68;
      break;

    case SC68_SET_POS:
    default:
      res = error_addx(sc68,
                       "libsc68: %s (%d)\n",
                       "invalid control function", fct);
      break;
    }
  }
  va_end(list);
  return res;
}

const char * sc68_mimetype(void)
{
  return SC68_MIMETYPE;
}
