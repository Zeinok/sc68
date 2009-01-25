/**
 * @ingroup  file68_lib
 * @file     file68/gzip68.h
 * @author   Benjamin Gerard
 * @date     2003-09-03
 * @brief    Gzip loader header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_GZIP68_H_
#define _FILE68_GZIP68_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  file68_gzip68  gzip file support.
 *  @ingroup   file68_lib
 *
 *    Provides functions for loading and saving gzip file.
 *
 *  @{
 */

/** Test gzip file header magic header.
 *
 *  @param  buffer  Buffer containing at least 3 bytes from gzip header.
 *
 *  @retval  1  buffer seems to be gzipped..
 *  @retval  0  buffer is not gzipped.
 */
int gzip68_is_magic(const void * buffer);

/** Load an optionnally gzipped file.
 *
 *    The gzip68_load() function allocates memory and loads the totality of the
 *    given file. If the file is a gzipped file, it will be inflate.
 *
 * @param  fname    Name of file to load.
 * @param  ulen     Pointer to uncompressed or total size of file.
 *                  May be set to 0.
 *
 * @return Pointer to the loaded file buffer.
 * @retval 0 Error
 */
void *gzip68_load(const char *fname, int *ulen);

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _FILE68_GZIP68_H_ */
