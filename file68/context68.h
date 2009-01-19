/**
 * @ingroup  file68_lib
 * @file     file68/context68.h
 * @author   Benjamin Gerard
 * @date     2007-02-25
 * @brief    Thread safe context.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_CONTEXT68_H_
#define _FILE68_CONTEXT68_H_

#error "context"

#ifndef FILE68_API
#include "file68_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup   file68_context68  Instance context (multi-thread support)
 *  @ingroup    file68_lib
 *
 *    Provides functions for handling file68 instance context. Basically
 *    each thread must have its own context instance unless the application
 *    use its own mechasmism to ensure that only one file68 function is
 *    called at a time.
 *
 *  @{
 */

/**  context structure holds per-context data. */
struct context68_s {

  /** Identification string. */
  char id[16];
  
  /** Set if the context was successfully created. */
  int created:1;

  /** Set if the context was dynamically allocated. */ 
  int allocated:1;
  
  /** Error stack.
   *  @{
   */
  /** Max number of error string. */
# define CONTEXT68_MAXERROR    4
  /** Max char per error string.  */
# define CONTEXT68_MAXERRORSTR 512
  /** Error string lifo. */
  char err[CONTEXT68_MAXERROR][CONTEXT68_MAXERRORSTR];
  /** Error counter. */
  int nerr;
  /**@}*/
  
};

/** Context type. */
typedef struct context68_s context68_t;

/** Default context.
 *
 *      This context is used each time a NUL context is given to any
 *      function that require one in the file68 library. This is
 *      convenient for single-threaded application but should not be use
 *      in multi-thread context. This context is also used by the
 *      file68 initialization and context functions (no other
 *      context is available).
 */
context68_t context68_default;

/** Check context and set to context68_default if NULL. */
#define CONTEXT68_CHECK(context)\
 do {context=context?context:&context68_default;} while(0)

FILE68_API
/** Create a new context.
 *
 *     The context68_create() function creates a new context.
 *     If context is NUL a new context is allocated with the
 *     alloc68() function, context68_t::allocated is set.
 *     If id is NUL a default is automatically created.
 *     The context68_t::created flag is set in case of success.
 *     
 *   @param  context  0:create new context, other:init context
 *   @param  id       id string (0:automatic default)
 *
 *   @return context
 *   @retval 0 error
 */
context68_t * context68_create(context68_t * context, const char * id);

FILE68_API
/** Destroy a context.
 *
 *   @param  context  context instance to destroy.
 */
void context68_destroy(context68_t * context);

FILE68_API
/** Context initialization.
 *
 *  @return error-code
 *  @retval  0  sucess
 *  @retval -1  failure
 */
int context68_init(void);

FILE68_API
/** Context shutdown.
 */
void context68_shutdown(void);

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _FILE68_CONTEXT68_H_ */
