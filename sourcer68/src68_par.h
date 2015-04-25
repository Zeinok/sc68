/**
 * @ingroup  sourcer68_prg
 * @file     src68_par.h
 * @author   Benjamin Gerard
 * @date     2015-03-18
 * @brief    memory partition
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef SOURCER68_PAR_H
#define SOURCER68_PAR_H

#include "src68_vec.h"

enum {
  PART_N = 0,
  PART_DCB = 1,
  PART_DCW = 2,
  PART_DCL = 3,

  PART_CODE = 4,

};

typedef struct {
  obj_t  obj;                           /* must be first */
  uint_t addr;
  uint_t size;
  uint_t flag;
} par_t;

/**
 * Create a partition container.
 *
 * @param  max  Initial maximum number of partitions
 * @retval 0 on error
 * @return partition container.
 */
vec_t * parts_new(uint_t max);

/**
 * Add a new section. Adding a section might invalidated other section
 * index and/or values.
 *
 * @param  addr  address of partition start
 * @param  size  size of the partition (in byte)
 * @param  flag  set of flags for the section.
 *
 * @return section
 * @retval 0 on error
 */
par_t * part_new(uint_t addr, uint_t size, uint_t flag);


/**
 * Get section at index.
 */
par_t * part_get(vec_t * parts, int idx);

/**
 * Create and add a section.
 *
 * @param  parts  the section container
 * @param  addr   address of section start
 * @param  size   size of the section (in byte)
 * @param  flag   set of flags for the section.
 *
 * @return added section index
 * @retval -1 on error
 */
int part_add(vec_t * parts, uint_t addr, uint_t size, uint_t flag);

#endif
