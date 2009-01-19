/**
 * @ingroup  file68_istream68
 * @file     file68/istream68_file.h
 * @author   Benjamin Gerard
 * @date     2007/08/08
 * @brief    FILE stream header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_ISTREAM68_FILE_H_
#define _FILE68_ISTREAM68_FILE_H_

#include "istream68.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @name      FILE stream
 *  @ingroup   file68_istream68
 *
 *    Implements istream68_t for stdio.h FILE.
 *
 *  @{
 */

/** Creates a stream for "C" FILE.
 *
 *  @param  fname    path of file.
 *  @param  mode     bit#0 : read access, bit#1 : write access.
 *
 *  @return stream
 *  @retval 0 on error
 *
 *  @note   filename is internally copied.
 */
istream68_t * istream68_file_create(const char * fname, int mode);

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #define _FILE68_ISTREAM68_FILE_H_ */
