/**
 * @ingroup  file68_lib
 * @file     sc68/file68_vfs_ao.h
 * @author   Benjamin Gerard
 * @date     2007-03-08
 * @brief    AO stream header.
 */
/* Time-stamp: <2013-08-02 16:38:23 ben> */

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
 * Initialize ao virtual file system.
 */
int vfs68_ao_init(void);

FILE68_EXTERN
/**
 * Shutdown ao virtual file system.
 */
void vfs68_ao_shutdown(void);

/**
 * @}
 */

#endif
