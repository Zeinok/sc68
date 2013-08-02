/**
 * @ingroup  file68_lib
 * @file     sc68/file68_vfs_file.h
 * @author   Benjamin Gerard
 * @date     2007/08/08
 * @brief    FILE stream header.
 */
/* Time-stamp: <2013-08-02 21:25:12 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef _FILE68_VFS_FILE_H_
#define _FILE68_VFS_FILE_H_

#include "file68_vfs.h"

/**
 * @name     FILE stream
 * @ingroup  file68_vfs
 *
 *   Implements vfs68_t for stdio.h FILE.
 *
 * @{
 */

FILE68_EXTERN
/**
 * Init file VFS.
 *
 * @retval  0  always success
 */
int vfs68_file_init(void);

FILE68_EXTERN
/**
 * Creates a stream for "C" FILE.
 *
 * @param  fname    path of file.
 * @param  mode     bit#0 : read access, bit#1 : write access.
 *
 * @return stream
 * @retval 0 on error
 *
 * @note   filename is internally copied.
 */
vfs68_t * vfs68_file_create(const char * fname, int mode);

/**
 * @}
 */

#endif
