/**
 * @ingroup  file68_istream68
 * @file     file68/istream68_def.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    Generic stream interface.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_ISTREAM68_DEF_H_
#define _FILE68_ISTREAM68_DEF_H_

#include "istream68.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @name     Open modes
 *  @ingroup  file68_istream68
 *  @{
 */

/** Read open mode bit. */
#define ISTREAM68_OPEN_READ_BIT 0

/** Read open mode value. */
#define ISTREAM68_OPEN_READ  (1<<ISTREAM68_OPEN_READ_BIT)

/** Write open mode bit. */
#define ISTREAM68_OPEN_WRITE_BIT 1

/** Write open mode value. */
#define ISTREAM68_OPEN_WRITE (1<<ISTREAM68_OPEN_WRITE_BIT)

/** Open modes mask. */
#define ISTREAM68_OPEN_MASK (ISTREAM68_OPEN_READ|ISTREAM68_OPEN_WRITE)

/** Test if any open flags is set (returns 0 or 1). */
#define ISTREAM68_IS_OPEN(V) (!!((V)&(ISTREAM68_OPEN_READ|ISTREAM68_OPEN_WRITE)))

/** Test if READ open flags is set (returns 0 or 1). */
#define ISTREAM68_IS_OPEN_READ(V) (((V)>>ISTREAM68_OPEN_READ_BIT)&1)

/** Test if WRITE open flags is set (returns 0 or 1). */
#define ISTREAM68_IS_OPEN_WRITE(V) (((V)>>ISTREAM68_OPEN_WRITE_BIT)&1)

/**@}*/

/** @name     Stream function prototypes.
 *  @ingroup  file68_istream68
 *  @{
 */
typedef const char * (* istream68_name_t) (istream68_t *);
typedef int (* istream68_open_t) (istream68_t *);
typedef int (* istream68_close_t) (istream68_t *);
typedef int (* istream68_length_t) (istream68_t *);
typedef int (* istream68_tell_t) (istream68_t *);
typedef int (* istream68_seek_t) (istream68_t *, int);
typedef int (* istream68_read_t) (istream68_t *, void *, int);
typedef int (* istream68_write_t) (istream68_t *, const void *, int);
typedef void (* istream68_destroy_t) (istream68_t *);
/**@}*/

/** Generic stream structure.
 *
 *     Set of functions needed to handle a stream.
 *
 *  @ingroup  file68_istream68
 */
struct _istream68_t {
  istream68_name_t name;       /**< Get stream name.        */
  istream68_open_t open;       /**< Open stream.            */
  istream68_close_t close;     /**< Close stream.           */
  istream68_read_t read;       /**< Read data from stream.  */
  istream68_write_t write;     /**< Write data to stream/   */
  istream68_length_t length;   /**< Get stream data length. */
  istream68_tell_t tell;       /**< Get stream position.    */
  istream68_seek_t seekf;      /**< Seek forward.           */
  istream68_seek_t seekb;      /**< Seek backward.          */
  istream68_destroy_t destroy; /**< Destructor.             */
};

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _FILE68_ISTREAM68_DEF_H_ */
