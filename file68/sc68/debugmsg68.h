/**
 * @ingroup  file68_lib
 * @file     file68/debugmsg68.h
 * @author   Benjamin Gerard
 * @date     2003-08-22
 * @brief    Debug message header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_DEBUGMSG68_H_
#define _FILE68_DEBUGMSG68_H_

#ifndef FILE68_API
#include "file68_api.h"
#endif

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  file68_debug68  Debug messages
 *  @ingroup   file68_lib
 *
 *    Provides functions for debugging message.
 *
 *  @{
 */

/** Debug handler type.
 *
 *  - user data
 *  - format
 *  - va_list
 */
typedef void (*debugmsg68_t)(void*, const char*, va_list);

/** Debug feature help display function.
 *
 *  - user data
 *  - bit
 *  - name
 *  - description
 */
typedef void (*debugmsg68_help_t)(void*, const int, const char*, const char*);


FILE68_API
/** Filter debug messages.
 *
 *  Bit have to be clear to mask/filter a debug feature. The default
 *  value depend on compilation. If debug feature was enabled (DEBUG
 *  defined) debugmsg68_mask value is set to ~0, allowing all
 *  messages. On the contrary its debugmsg68_mask is set to 0,
 *  filtering out all messages (except
 *  enum_debugmsg68_bit::debugmsg68_unmaskable).
 */
unsigned int debugmsg68_mask;

/** Predefined features. */
enum enum_debugmsg68_bit
{
  debugmsg68_NEVER    = -3, /**< Never printed message.   */
  debugmsg68_ALWAYS   = -2, /**< Always printed message.  */
  debugmsg68_CURRENT  = -1, /**< Current feature message. */

  debugmsg68_CRITICAL = 0,  /**< Critical error message. */
  debugmsg68_ERROR    = 1,  /**< Error message.          */
  debugmsg68_WARNING  = 2,  /**< Warning message.        */
  debugmsg68_INFO     = 3,  /**< Informationnal message. a*/
  debugmsg68_DEBUG    = 4,  /**< Debug message.          */
};

FILE68_API
/** Get named features. */
int debugmsg68_feature_bit(const char * name);

FILE68_API
/** Create/Modify a feature. */
int debugmsg68_feature(const char * name, const char * desc, const int masked);

FILE68_API
/** Get/Set current feature. */
int debugmsg68_feature_current(const int feature);

FILE68_API
/** Free/Destroy a debug feature. */
void debugmsg68_feature_free(const int feature);

FILE68_API
/** Set all predefined features mask according to given level. */
int debugmsg68_feature_level(const int feature);

FILE68_API
/** Get info on a feature. */
int debugmsg68_feature_info(const int feature, const char **name,
			    const char **desc, int * next);

FILE68_API
/** Print defined features. */
void debugmsg68_feature_help(void * cookie, debugmsg68_help_t fct);

FILE68_API
/** Set debug function.
 *
 *  @param  handler  pointer to a function to display debug messages
 *                   (0 desactive debug message).
 *  @return previous handler
 */
debugmsg68_t debugmsg68_set_handler(debugmsg68_t handler);

FILE68_API
/** Set debug function user parameter.
 *
 *  @param  cookie  user parameter that will be sent as first parameter to
 *                  the debug handler.
 *
 *  @see debugmsg68_set_handler();
 */
void * debugmsg68_set_cookie(void * cookie);

FILE68_API
/** Print debug message (variable argument).
 *
 *  @param  feature  message type (feature). 
 *  @param  fmt      printf() like format string.
 *  @param  list     variable argument list (stdarg.h)
 *
 *  @see enum_debugmsg68_bit
 *  @see debugmsg68
 */
void vdebugmsg68(const int feature, const char * fmt, va_list list);

FILE68_API
/** Print debug message.
 *
 *  @param  feature  message type (feature).
 *  @param  fmt  printf() like format string.
 *
 *  @see enum_debugmsg68_bit
 *  @see vdebugmsg68
 */
void debugmsg68(const int feature, const char * fmt, ...);

FILE68_API
/** Print debug message (debug level).
 *  @param  fmt   printf() like format string.
 *  @see debugmsg68
 */
void debugmsg68_debug(const char * fmt, ...);

FILE68_API
/** Print debug message (info level).
 *  @param  fmt   printf() like format string.
 *  @see debugmsg68
 */
void debugmsg68_info(const char * fmt, ...);

FILE68_API
/** Print debug message (warning level).
 *  @param  fmt   printf() like format string.
 *  @see debugmsg68
 */
void debugmsg68_warning(const char * fmt, ...);

FILE68_API
/** Print debug message (error level).
 *  @param  fmt   printf() like format string.
 *  @see debugmsg68
 */
void debugmsg68_error(const char * fmt, ...);

FILE68_API
/** Print debug message (critical level).
 *  @param  fmt   printf() like format string.
 *  @see debugmsg68
 */
void debugmsg68_critical(const char * fmt, ...);

/** Define TRACE68 and VTRACE68 macros. */
#ifdef NDEBUG
# define TRACE68  while (0) debugmsg68_dummy
# define VTRACE68 while (0) vdebugmsg68_dummy
static inline
void debugmsg68_dummy(const int feature, const char * fmt, ...) {}
static inline
void vdebugmsg68_dummy(const int feature, const char * fmt, va_list list) {}
#else
# define TRACE68  debugmsg68
# define VTRACE68 vdebugmsg68
#endif

/** Define debugmsg68_DEFAULT macros */
#ifndef debugmsg68_DEFAULT
# ifndef DEBUG
#  define debugmsg68_DEFAULT debugmsg68_NEVER
# else
#  define debugmsg68_DEFAULT debugmsg68_CURRENT
# endif
#endif

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _FILE68_DEBUGMSG68_H_ */
