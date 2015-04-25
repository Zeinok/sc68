/**
 * @ingroup  sourcer68_prg
 * @file     src68_mbk.h
 * @author   Benjamin Gerard
 * @date     2015-03-18
 * @brief    memory block.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef SOURCER68_MBK_H
#define SOURCER68_MBK_H

#include "src68_def.h"

enum {
  /* Both Even and odd address. */
  MIB_READ  = 1,                    /**< Read by disassembler. */
  MIB_ADDR  = 2,                    /**< Symbol/address.       */
  MIB_BYTE  = 4,                    /**< Byte access.          */

  /* Even address only. */
  MIB_WORD  = MIB_BYTE*2,           /**< word access.               */
  MIB_LONG  = MIB_WORD*2,           /**< Long access.               */
  MIB_ENTRY = 32,                   /**< Disassembly entry point.   */
  MIB_EXEC  = 64,                   /**< Instruction start address. */
  MIB_RELOC = 128                   /**< Long has been relocated.   */
};

typedef struct {
  uint_t   org;                      /**< origin of the memory block. */
  uint_t   len;                      /**< length of the memory block. */
  uint8_t *mem;                      /**< memory data bits. */
  uint8_t *mib;                      /**< memory info bits. */
  uint8_t  buf[1];
} mbk_t;

/**
 * Create a new memory block.
 *
 * @param  org  origin address of the memory block.
 * @param  len  length in byte of the memory block.
 * @return pointer to a memory block descriptor
 */
mbk_t * mbk_new(uint_t org, uint_t len);

/**
 * Destroy a memory block.
 *
 * @param  mbk  memory block descriptor
 */
void mbk_del(mbk_t * mbk);

/**
 * Clear the memory flags.
 *
 * @param  mbk  memory block descriptor.
 */
void mbk_clr(mbk_t * mbk);

/**
 * Clear a part of the memory flags.
 *
 * @param  mbk  memory block descriptor.
 * @param  addr start address.
 * @param  size length in byte
 */
void mbk_clr_part(mbk_t * mbk, uint_t addr, uint_t size);

/**
 * Test if an address is in block range.
 *
 * @retval  0  address is not in block range
 * @retval  1  address is in block range
 */
int mbk_ismine(const mbk_t * mbk, uint_t addr);

#endif
