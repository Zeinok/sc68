/**
 * @ingroup  file68_lib
 * @file     sc68/alloc68.h
 * @author   Benjamin Gerard
 * @date     2003-04-11
 * @brief    Dynamic memory management header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */


#ifndef _FILE68_ALLOC68_H_
#define _FILE68_ALLOC68_H_

#ifndef FILE68_API
# include "file68_api.h"
#endif

/** @defgroup  file68_alloc  Dynamic memory management
 *  @ingroup   file68_lib
 *
 *    Provides dynamic memory management functions. All dynamically
 *    allocated buffer in the file68 library should use this set of
 *    function. Anyway it is possible that some third party library
 *    do not use them. The default allocator should be standard
 *    malloc() and free() function unless it has been specified
 *    at compile time.
 *
 *    @warning  the alloc/free handlers work together. It should be change
 *              only when no allocated buffers remains unless the new
 *              functions use the same real memory manager.
 *
 *    @warning  Use these functions with care in multi-thread context.
 *              Basically the function should be set once before the
 *              application goes multi-thread.
 *
 *  @{
 */

/** Alloc function (malloc). */
typedef void * (*alloc68_t)(unsigned int);

/** Free function (free). */
typedef void (*free68_t)(void *);

FILE68_API
/** Allocate dynamic memory.
 *
 *   The alloc68() function calls user defined dynamic memory
 *   allocation handler.
 *
 * @param  n        Size of buffer to allocate.
 *
 * @return pointer to allocated memory buffer.
 * @retval 0 error
 *
 * @see alloc68_set()
 * @see calloc68()
 * @see free68()
 */
void * alloc68(unsigned int n);

FILE68_API
/** Allocate and clean dynamic memory.
 *
 *   The calloc68() function calls user defined dynamic memory
 *   allocation handler and fills memory buffer with 0.
 *
 * @param  n        Size of buffer to allocate.
 *
 * @return pointer to allocated memory buffer.
 * @retval 0 error
 *
 * @see alloc68_set()
 * @see alloc68()
 * @see free68()
 */
void * calloc68(unsigned int n);

FILE68_API
/** Free dynamic memory.
 *
 *   The free68() function calls user defined dynamic memory
 *   free handler.
 *
 * @param  context  Context instance (0:default)
 * @param  data     Previously allocated memory buffer.
 *
 * @return pointer to allocated memory
 * @retval 0 Failure.
 *
 * @see free68_set()
 * @see alloc68()
 * @see calloc68()
 */
void free68(void * data);

FILE68_API
/** Set/get dynamic memory allocation handler.
 *
 * @param  alloc  Set new alloc handler (0:get old value).
 *
 * @return previous alloc handler.
 *
 * @see alloc68()
 * @see calloc68()
 *
 */
alloc68_t alloc68_set(alloc68_t alloc);

FILE68_API
/** Set/get dynamic memory free handler.
 *
 * @param  free  Set new free handler (0:get old value).
 * @return previous free handler.
 *
 * @see free68()
 * @see alloc68_set()
 */
free68_t free68_set(free68_t free);

/**
 *  @}
 */

#endif /* #define _FILE68_ALLOC68_H_ */
