/**
 * @ingroup  file68_lib
 * @file     sc68/file68_vfs_curl.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    @ref cURL stream header.
 */
/* Time-stamp: <2013-07-22 01:15:35 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef _FILE68_VFS_CURL_H_
#define _FILE68_VFS_CURL_H_

#include "file68_vfs.h"

/**
 * @name     cURL stream
 * @ingroup  file68_vfs
 *
 * @anchor cURL
 *
 *   @b cURL is a client-side URL transfer library. For more informations
 *   see <a href="http://curl.planetmirror.com/libcurl/">cURL website</a>.
 *
 * @{
 */

FILE68_EXTERN
/**
 * Initialize curl engine.
 *
 *   The vfs68_curl_init() function initializes curl library.  It
 *   is called by the file68_init() function and everytime a new curl
 *   stream is created with the vfs68_curl_create() function.
 *
 * @return error code
 * @retval  0   success
 * @retval  -1  failure
 */
int vfs68_curl_init(void);

FILE68_EXTERN
/**
 * Shutdown curl engine.
 *
 *  The vfs68_curl_shutdoen() function shutdown curl library. It
 *  is called by the file68_shutdown() function.
 */
void vfs68_curl_shutdown(void);

FILE68_EXTERN
/**
 * Creates an URL based stream using @ref cURL.
 *
 * @param  url     URL
 * @param  mode    bit#0: read access, bit#1: write access.
 *
 * @return stream
 * @retval 0 on error
 *
 * @note     url is internally copied.
 * @todo     Implement write mode.
 */
vfs68_t * vfs68_curl_create(const char * url, int mode);

/**
 * @}
 */

#endif

