/**
 * @ingroup  file68_istream68
 * @file     file68/istream68_z.h
 * @author   Benjamin Gerard
 * @date     2003-10-06
 * @brief    Z stream header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_ISTREAM68_Z_H_
#define _FILE68_ISTREAM68_Z_H_

#include "istream68.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @name     Z stream
 *  @ingroup  file68_istream68
 *
 *  @anchor zlib
 *
 *    Implements istream68_t for Z library.
 *    Z is a famous compression/decompression library. For more informations
 *    see <a href="http://www.gzip.org">gzip website</a>.
 *
 *  @todo      deflate (compression) mode.
 *  @{
 */

/** gzip options. */
typedef struct {
  unsigned int gzip:1;     /**< Read/Write gzip file format.                */
  unsigned int level:4;    /**< Compression level [0..9] or -1 for default. */
  unsigned int strategy:3; /**< Compression strategy (0 is default).        */
  unsigned int name:1;     /**< Include original name to gzip.              */
} istream68_z_option_t;

/** Default gzip option. */
extern const istream68_z_option_t istream68_z_default_option;

/** Create a @ref zlib "Z" stream.
 *
 *  @param  is     Stream to compress/decompress.
 *  @param  mode   bit-0: read access, bit-1: write access.
 *  @param  opt    gzip options.
 *
 *  @return stream
 *  @retval 0 on error
 */
istream68_t * istream68_z_create(istream68_t * is, int mode,
				 const istream68_z_option_t opt);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* #define _FILE68_ISTREAM68_Z_H_ */
