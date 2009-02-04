/**
 * @ingroup  file68_lib
 * @file     file68/option68.h
 * @author   Benjamin Gerard
 * @date     2009-02-04
 * @brief    Option header.
 *
 */

/* $Id: string68.h 18 2009-01-25 03:39:15Z benjihan $ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_OPTION68_H_
#define _FILE68_OPTION68_H_

#ifndef FILE68_API
#include "file68_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  file68_option68  Options
 *  @ingroup   file68_lib
 *
 *    Provides command line options manipulation functions.
 *
 *  @{
 */

/** option argument types. */
enum option68_e {
  option68_BOL = 0,		/**< Boolean (set or unset). */
  option68_STR = 1,		/**< String value.           */
  option68_INT = 2		/**< Intrger value.          */
};

typedef struct option68_s option68_t;

/** Options help display function.
 *
 *  -# user data
 *  -# option
 *  -# envvar
 *  -# short description
 */
typedef void (*option68_help_t)(void *, const char*, const char*, const char*);

struct option68_s {
  int            has_arg; /**< @see option68_e. 1st complement => setted */
  const char   * prefix;  /**< Key prefix.                               */
  const char   * name;	  /**< Key name (bare).                          */
  const char   * cat;	  /**< Category name.                            */
  const char   * desc;	  /**< Short description.                        */
  union {
    const char * str;	   /**< Value for string argument.               */
    int          num;	   /**< Value for integer argument.              */
  }              val;	   /**< Melted value.                            */

  /** @name internals
   *  @{
   */
  int prefix_len;      /**< length of option68_t::prefix */
  int name_len;	       /**< length of option68_t::name   */
  option68_t   * next; /**< Chain to next option.        */
  /**@}*/
};

FILE68_API
/** Print defined options.
 *
 * @param  cookie  User data used as 1st argument for fct
 * @param  fct     Fonction call for each possible option
 */
void option68_help(void * cookie, option68_help_t fct);


FILE68_API
/** Append option definitions.
 *
 * @param  options  Array of options
 * @param  n        Array size
 *
 * @notice Options are not internally copied.
 */
int option68_append(option68_t * options, int n);

FILE68_API
/** Parse command line options.
 *
 * @param  argc     argument count
 * @param  argv     arguments
 * @param  reset    reset all options before parsing
 * @retval remaining argc
 */
int option68_parse(int argc, char ** argv, int reset);

FILE68_API
/** Get option by name.
 *
 * @param   key      argument count
 * @param   setonly  only if option has been set
 * @return  option
 * @retval  0        not found
 *
 */
option68_t * option68_get(const char * key, const int setonly);

FILE68_API
int option68_isset(const option68_t * option);

FILE68_API
int option68_set(option68_t * opt, const char * str);

FILE68_API
int option68_iset(option68_t * opt, int val);

FILE68_API
int option68_unset(option68_t * opt);

FILE68_API
const char * option68_getenv(option68_t * opt, const int set); 


/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _FILE68_OPTION68_H_ */
