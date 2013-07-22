/**
 * @ingroup  file68_lib
 * @file     sc68/file68_vfs_null.h
 * @author   Benjamin Gerard
 * @date     2003-10-10
 * @brief    Null stream header.
 */
/* Time-stamp: <2013-07-22 01:18:35 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef _FILE68_VFS_NULL_H_
#define _FILE68_VFS_NULL_H_

#include "file68_vfs.h"

/**
 * @name     Null VFS
 * @ingroup  file68_vfs
 *
 *   Implements a null vfs68_t.
 *
 *   Null stream does nothing but checking some trivial errors (like
 *   access without opening) and dealing with a virtual stream
 *   position.  The null stream length is the position the highest
 *   byte that has been either read or write. The length is resetted
 *   at open.  It implies that stream length can be retrieved by the
 *   vfs68_length() function after vfs68_close() call.
 *
 * @{
 */

FILE68_EXTERN
/**
 * Creates a null stream.
 *
 * @param  name     Optionnal name *  @return stream
 * @retval 0 on error
 *
 * @note   filename is prefixed by "null://".
 */
vfs68_t * vfs68_null_create(const char * name);

/**
 * @}
 */

#endif

