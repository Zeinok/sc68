/**
 * @ingroup  file68_lib
 * @file     sc68/file68_vfs_ao.h
 * @author   Benjamin Gerard
 * @date     2007-03-08
 * @brief    AO stream header.
 */
/* Time-stamp: <2013-07-22 01:28:10 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef _FILE68_VFS_AO_H_
#define _FILE68_VFS_AO_H_

#include "file68_vfs.h"

/**
 * @name     Audio VFS
 * @ingroup  file68_vfs
 *
 *   Implements vfs68_t for XIPH libao (audio output).
 *
 * @{
 */

FILE68_EXTERN
/**
 * Creates a stream for libao.
 *
 * @param  fname    path of file.
 * @param  mode     bit#0 : read access, bit#1 : write access.
 *
 * @return stream
 * @retval 0 on error
 *
 * @note    filename is internally copied.
 * @warning write mode only.
 */
vfs68_t * vfs68_ao_create(const char * fname, int mode);

/**
 * @}
 */

#endif

