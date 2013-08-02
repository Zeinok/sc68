/**
 * @ingroup  file68_lib
 * @file     sc68/file68_vfs_mem.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    Memory stream header.
 */
/* Time-stamp: <2013-08-02 21:32:29 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef _FILE68_VFS_MEM_H_
#define _FILE68_VFS_MEM_H_

#include "file68_vfs.h"

/**
 * @name     Memory stream
 * @ingroup  file68_vfs
 *
 *   Implements vfs68_t for memory buffer.
 *
 * @{
 */

FILE68_EXTERN
/**
 * Init memory VFS (register mem: scheme).
 *
 * @retval  0  always success
 */
int vfs68_mem_init(void);

FILE68_EXTERN
/**
 * Creates a stream for memory buffer.
 *
 * If mode is enslaved the destroy callback call the free68() function
 * to release the memory buffer addr.
 *
 * @param  addr     Buffer base address.
 * @param  len      Buffer length.
 * @param  mode     Allowed open mode.
 *
 * @return stream
 * @retval 0 on error
 *
 * @note   filename is build with memory range.
 */
vfs68_t * vfs68_mem_create(const void * addr, int len, int mode);

/**
 * @}
 */

#endif

