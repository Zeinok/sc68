/**
 * @ingroup  file68_istream68
 * @file     file68/istream68_mem.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    Memory stream header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_ISTREAM68_MEM_H_
#define _FILE68_ISTREAM68_MEM_H_

#include "istream68.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @name      Memory stream
 *  @ingroup   file68_istream68
 *
 *    Implements istream68_t for memory buffer.
 *
 *  @{
 */

/** Creates a stream for memory buffer.
 *
 *  @param  addr     Buffer base address.
 *  @param  len      Buffer length.
 *  @param  mode     Allowed open mode.
 *
 *  @return stream
 *  @retval 0 on error
 *
 *  @note   filename is build with memory range.
 */
istream68_t * istream68_mem_create(const void * addr, int len, int mode);

/**@}*/

#ifdef __cplusplus
}
#endif


#endif /* #define _FILE68_ISTREAM68_MEM_H_ */
