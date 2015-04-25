/**
 * @ingroup  sourcer68_prg
 * @file     src68_dis.h
 * @author   Benjamin Gerard
 * @date     2015-03-18
 * @brief    disassembler and code walker.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef SOURCER68_DIS_H
#define SOURCER68_DIS_H

#include "src68_exe.h"

/**
 * Walk options.
 */
typedef struct {
  uint_t brk_on_ndef_jmp:1;           /**< stop the walk. */
} walkopt_t;

void walkopt_set_default(walkopt_t * wopt);


typedef struct {
  /**
   * @name input fields.
   * @{
   */
  uint_t    adr;                        /**< Entry point.  */
  exe_t    *exe;                        /**< Executable.   */
  walkopt_t opt;                        /**< Walk options. */

  /**
   * @}
   */

  /**
   * @name output fields.
   * @{
   */

  uint_t score;     /**< consistency score. */
  uint_t depth;     /**< maximum depth. */
  uint_t inscnt;    /**< total number of disassembled instructions. */

  /**
   * @}
   */

} walk_t;

/**
 * Start a walk.
 *
 * @param  walk
 */
int dis_walk(walk_t * walk);

/**
 * Disasemble an instruction.
 *
 * @paran  exe
 * @paran  adr
 * @paran  buf
 * @paran  max
 * @return instruction type (@see desa68_inst_types)
 */
int dis_disa(exe_t * exe, uint_t * adr, char * buf, uint_t max);

#endif
