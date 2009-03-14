/**
 * @ingroup  file68_lib
 * @file     sc68/msg68.h
 * @author   Benjamin Gerard
 * @date     2003-08-22
 * @brief    Message header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_MSG68_H_
#define _FILE68_MSG68_H_

#ifndef FILE68_API
# include "file68_api.h"
#endif
#include <stdarg.h>


/** @defgroup  file68_debug  Debug messages
 *  @ingroup   file68_lib
 *
 *    Provides functions for debugging message.
 *
 *  @{
 */

/** Debug handler type.
 *
 *  - bit
 *  - user data
 *  - format
 *  - va_list
 */
typedef void (*msg68_t)(const int, void*, const char*, va_list);

/** Debug feature help display function.
 *
 *  - user data
 *  - bit
 *  - name
 *  - description
 */
typedef void (*msg68_help_t)(void*, const int, const char*, const char*);

/** Predefined features. */
enum enum_msg68_bit
{
  msg68_NEVER    = -3, /**< Never printed message.   */
  msg68_ALWAYS   = -2, /**< Always printed message.  */
  msg68_CURRENT  = -1, /**< Current feature message. */

  msg68_CRITICAL = 0,  /**< Critical error message. */
  msg68_ERROR    = 1,  /**< Error message.          */
  msg68_WARNING  = 2,  /**< Warning message.        */
  msg68_INFO     = 3,  /**< Informationnal message. */
  msg68_DEBUG    = 4,  /**< Debug message.          */
  msg68_TRACE    = 5,  /**< Trace message.          */
};

FILE68_API
/** Get/Set current message mask.
 */
unsigned int msg68_mask(unsigned int clr, unsigned int set);

FILE68_API
/** Get named features. */
int msg68_feature_bit(const char * name);

FILE68_API
/** Create/Modify a feature. */
int msg68_feature(const char * name, const char * desc, const int masked);

FILE68_API
/** Get/Set current feature. */
int msg68_feature_current(const int feature);

FILE68_API
/** Free/Destroy a debug feature. */
void msg68_feature_free(const int feature);

FILE68_API
/** Set all predefined features mask according to given level. */
int msg68_feature_level(const int feature);

FILE68_API
/** Get info on a feature. */
int msg68_feature_info(const int feature, const char **name,
                       const char **desc, int * next);

FILE68_API
/** Print defined features. */
void msg68_feature_help(void * cookie, msg68_help_t fct);

FILE68_API
/** Set debug function.
 *
 *  @param  handler  pointer to a function to display debug messages
 *                   (0 desactive debug message).
 *  @return previous handler
 */
msg68_t msg68_set_handler(msg68_t handler);

FILE68_API
/** Set debug function user parameter.
 *
 *  @param  cookie  user parameter that will be sent as first parameter to
 *                  the debug handler.
 *
 *  @see msg68_set_handler();
 */
void * msg68_set_cookie(void * cookie);

FILE68_API
/** Print debug message (variable argument).
 *
 *  @param  feature  message type (feature).
 *  @param  fmt      printf() like format string.
 *  @param  list     variable argument list (stdarg.h)
 *
 *  @see enum_msg68_bit
 *  @see msg68
 */
void msg68_va(const int feature, const char * fmt, va_list list);

FILE68_API
/** Print debug message.
 *
 *  @param  feature  message type (feature).
 *  @param  fmt  printf() like format string.
 *
 *  @see enum_msg68_bit
 *  @see vmsg68
 */
void msg68(const int feature, const char * fmt, ...);

FILE68_API
/** Print trace message (trace level).
 *  @param  fmt   printf() like format string.
 *  @see msg68
 */
void msg68_trace(const char * fmt, ...);

FILE68_API
/** Print debug message (debug level).
 *  @param  fmt   printf() like format string.
 *  @see msg68
 */
void msg68_debug(const char * fmt, ...);

FILE68_API
/** Print debug message (info level).
 *  @param  fmt   printf() like format string.
 *  @see msg68
 */
void msg68_info(const char * fmt, ...);

FILE68_API
/** Print debug message (warning level).
 *  @param  fmt   printf() like format string.
 *  @see msg68
 */
void msg68_warning(const char * fmt, ...);

FILE68_API
/** Print debug message (error level).
 *  @param  fmt   printf() like format string.
 *  @see msg68
 */
void msg68_error(const char * fmt, ...);

FILE68_API
/** Print debug message (critical level).
 *  @param  fmt   printf() like format string.
 *  @see msg68
 */
void msg68_critical(const char * fmt, ...);

/** Define TRACE68 and VTRACE68 macros. */
#ifdef NDEBUG
# define TRACE68  while (0) msg68_dummy
# define VTRACE68 while (0) vmsg68_dummy
static inline
void msg68_dummy(const int feature, const char * fmt, ...) {}
static inline
void vmsg68_dummy(const int feature, const char * fmt, va_list list) {}
#else
# define TRACE68  msg68         /**< Trace only in non-release mode  */
# define VTRACE68 vmsg68        /**< Trace only in non-release mode  */
#endif

/** Define msg68_DEFAULT macros */
#ifndef msg68_DEFAULT
# ifndef DEBUG
#  define msg68_DEFAULT msg68_NEVER
# else
#  define msg68_DEFAULT msg68_CURRENT
# endif
#endif

/**
 *  @}
 */

#endif /* #ifndef _FILE68_MSG68_H_ */
