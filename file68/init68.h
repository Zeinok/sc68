/**
 * @ingroup  file68_lib
 * @file     file68/init68.h
 * @author   Benjamin Gerard
 * @date     2003-09-26
 * @brief    Initialization.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_INIT68_H_
#define _FILE68_INIT68_H_

#ifndef FILE68_API
#include "file68_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup   file68_lib
 *  @{
 */

/** Options help display function.
 *
 *  -# user data
 *  -# option
 *  -# envvar
 *  -# short description
 */
typedef void (*init68_help_t)(void *, const char*, const char*, const char*);


FILE68_API
/** Initialize file68 library.
 *
 *  @param  argv  argument array (as for main())
 *  @param  argc  argument count
 *  @return new argument count
 */
int file68_init(char **argv, int argc);

FILE68_API
/** Shutdown file68 library.
 *
 */
void file68_shutdown(void);

FILE68_API
/** Print defined options.
 *
 * @param  cookie  User data used as 1st argument for fct
 * @param  fct     Fonction call for each possible option
 */
void file68_option_help(void * cookie, init68_help_t fct);

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _FILE68_INIT68_H_ */
