/**
 * @ingroup  file68_lib
 * @file     sc68/file68_vfs_fd.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    File descriptor stream header.
 */
/* Time-stamp: <2013-08-02 21:53:22 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef _FILE68_VFS_FD_H_
#define _FILE68_VFS_FD_H_

#include "file68_vfs.h"

/**
 * @name     File descriptor stream
 * @ingroup  file68_vfs
 *
 *   Implements vfs68_t for "un*x like" file descriptor.
 *
 * @{
 */

FILE68_EXTERN
/**
 * Init file descriptor VFS.
 *
 * @retval  0  always success
 */
int vfs68_fd_init(void);

FILE68_EXTERN
/**
 * Init file descriptor VFS.
 *
 * @retval  0  always success
 */
int vfs68_fd_init(void);

FILE68_EXTERN
/**
 * Creates a stream for "un*x" file descriptor.
 *
 *   If fd parameters is not -1, it is used to as file descriptor for
 *   the stream and fname is used for naming the stream. Else the file
 *   is open as a regular file with fname as path.
 *
 * @param  fname  path of file or 0.
 * @param  fd     file decriptor or -1.
 * @param  mode   bit-0: read access, bit-1: write access.
 *
 * @return stream
 * @retval 0 on error
 *
 * @note     filename is internally copied.
 * @note     Even if fd is given the vfs68_open() must be call.
 * @warning  When opening a stream with an already opened descriptor the
 *           mode should match the real open mode but since no tests are
 *           performed before calling r/w access, it should not failed in
 *           case of wrong access on given mode.
 */
vfs68_t * vfs68_fd_create(const char * fname, int fd, int mode);

/**
 * @}
 */

#endif
