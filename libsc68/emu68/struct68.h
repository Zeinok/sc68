/**
 * @ingroup   emu68_type68_devel
 * @file      emu68/struct68.h
 * @author    Benjamin Gerard
 * @date      13/03/1999
 * @brief     Struture definitions.
 * 
 * $Id$
 *
 */

/* Copyright (C) 1998-2007 Benjamin Gerard */

#ifndef _EMU68_STRUCT68_H_
#define _EMU68_STRUCT68_H_

#include "type68.h"

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup emu68_type68_devel
 *  @{
 */

/** IO no pending interrupetion return value.
 *
 *    The next_int function of IO plugin must return IO68_NO_INT when no
 *    interruption are expected.
 */
#define IO68_NO_INT (0x80000000)

/** Convenience macro to access reg68 in the current emu68 instance. */
#define REG68 (emu68->reg)

/** Mask for memory overflow. */
#define MEMMSK68 (emu68->memmsk)


/** @name  Memory access caller type
 *  @{
 */

/** Write memory function. */
typedef void (*memwfunc68_t)(emu68_t * const);
typedef void (*iomemfunc68_t)(io68_t * const);

/*
typedef void (*iomemRfunc68_t)(io68_t * const, addr68_t, cycle68_t);
typedef void (*iomemWfunc68_t)(io68_t * const, addr68_t, int68_t, cycle68_t);
*/

/**@}*/

/** First level (16 lines) decoder function. */
typedef void (linefunc68_t)(emu68_t * const/*emu68*/,int/*reg9*/,int/*reg0*/);
#define DECL_LINE68(N) void N(emu68_t * const emu68,int reg9 ,int reg0)

/** Alloc function type. */
typedef void * (*emu68_alloc_t)(unsigned int);

/** Free function type. */
typedef void (*emu68_free_t)(void *);

/**  68K interruption exception structure.
 */
typedef struct
{
  addr68_t vector;  /**< Interrupt vector.             */
  int level;        /**< Interrupt level [0..7].       */
  cycle68_t cycle;  /**< Cycle the interruption falls. */
} interrupt68_t;

/** IO emulator plugin structure.
 *
 *      All 68K IO must have a filled io68_t structure to be warm plug
 *      or unplug with ioplug interface.
 *
 */
struct io68_s
{
  struct io68_s * next;             /**< IO list; pointer to next.       */
  char name[32];                    /**< IO identifier name.             */
  addr68_t addr_lo;                 /**< IO mapping area start address.  */
  addr68_t addr_hi;                 /**< IO mapping area end address.    */

  iomemfunc68_t r_byte;             /**< IO read byte function.          */
  iomemfunc68_t r_word;             /**< IO read word function.          */
  iomemfunc68_t r_long;             /**< IO read long function.          */

  iomemfunc68_t w_byte;             /**< IO write byte function.         */
  iomemfunc68_t w_word;             /**< IO write word function.         */
  iomemfunc68_t w_long;             /**< IO write long function.         */

  /** IO interruption function claim. */
  interrupt68_t *(*interrupt)(io68_t * const, const cycle68_t);
  /** IO get next interruption cycle. */
  cycle68_t (*next_interrupt)(io68_t * const, const cycle68_t);
  /** IO adjust cycle function.       */
  void (*adjust_cycle)(io68_t * const, const cycle68_t);
  /** IO reset function.              */
  int (*reset)(io68_t * const);
  /** Destructor. */
  void (*destroy)(io68_t * const);

  /** Emulator this IO is attached to. */
  emu68_t *emu68;
};


/** 68000 internal registers.
 */
typedef struct
{
  int68_t d[8];   /**< 68000 data registers.      */
  int68_t a[8];   /**< 68000 address registers.   */
  int68_t usp;    /**< 68000 User Stack Pointers. */
  int68_t pc;     /**< 68000 Program Counter.     */
  int68_t sr;     /**< 68000 Status Register.     */
} reg68_t;

#define REG68_D0_IDX 000
#define REG68_D1_IDX 001
#define REG68_D2_IDX 002
#define REG68_D3_IDX 003
#define REG68_D4_IDX 004
#define REG68_D5_IDX 005
#define REG68_D6_IDX 006
#define REG68_D7_IDX 007
#define REG68_DN_IDX(N) (N&7)

#define REG68_A0_IDX 010
#define REG68_A1_IDX 011
#define REG68_A2_IDX 012
#define REG68_A3_IDX 013
#define REG68_A4_IDX 014
#define REG68_A5_IDX 015
#define REG68_A6_IDX 016
#define REG68_A7_IDX 017
#define REG68_AN_IDX(N) (8+(N&7))

#define REG68_US_IDX 020
#define REG68_PC_IDX 021
#define REG68_SR_IDX 022

/** 68K Emulator struct. */
struct emu68_s {
  char name[32];               /**< Identifier.               */
  
  /* Error. */
  char err[128][4];            /**< Error message stack.      */
  int  nerr;                   /**< Error counter.            */

  /* Dynamic memory handlers. */
  emu68_alloc_t alloc;         /**< Alloc fonction.           */
  emu68_free_t  free;          /**< Free function.            */

  reg68_t reg;                 /**< 68000 internal registers. */
  cycle68_t cycle;             /**< Internal cycle counter.   */
  uint68_t clock;              /**< Master clock frequency.   */

  /* IO chips. */
  int nio;                     /**< # IO plug in IO-list.  */
  io68_t *iohead;              /**< Head of IO-list.       */
  io68_t *interrupt_io;        /**< Current interptible io */
  io68_t *mapped_io[256];

  /* Memory access. */
  addr68_t bus_addr;           /**< bus address for memory access.     */
  int68_t  bus_data;           /**< bus data for memory access.        */
  int framechk;                /**< ORed chk change for current frame. */
  u8  *chk;                    /**< Access-Control-Memory buffer.      */
  
  /* Onboard memory. */
  addr68_t memmsk; /**< Onboard memory mask (2^log2mem-1).              */
  int log2mem;     /**< Onboard memory buffer size (2^log2mem).         */
  u8 mem[16];      /**< Onboard memory buffer.
		        @notice Must be last in struct.                 */
};

/**
 *@}
 */


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _EMU68_STRUCT68_H_ */
