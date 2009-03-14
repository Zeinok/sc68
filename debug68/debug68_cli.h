/**
 * @ingroup   debug68_prg
 * @file      debug68_cli.h
 * @author    Benjamin Gerard
 * @date      1999/06/06
 * @brief     debug68 command line header.
 */

/* $Id: SC68debug_cli.h,v 2.0 2003/08/21 04:58:35 benjihan Exp $ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _DEBUG68_CLI_H_
#define _DEBUG68_CLI_H_

#ifndef DEBUG68_EXTERN
# ifdef __cplusplus
#  define DEBUG68_EXTERN extern "C"
#else
#  define DEBUG68_EXTERN extern
# endif
#endif

/** Command line . */
typedef struct {
  int    argc;                /**< Number of args.              */
  char * coms[128];           /**< Arguments (pointers to cli). */
  char * cli;                 /**< Command line buffer.         */
} debug68_cli_t;

DEBUG68_EXTERN
/** Read and parse command line.
 *
 *  The debug68_cli_read() function display prompt and read command line.
 *  The previous command line is freed by debug68_cli_free().
 *  Optionnaly the function can use readline library. In that case it handles
 *  an history and any readline basic functionnalities..
 *
 * @param  prompt  Displayed prompt if not 0.
 * @param  cli     Command line struct filled by the function.
 *
 * @return number of arguments in command line.
 * @retval -1  error
 * @retval 0   no arguments (empty commad line)
 *
 * @warning Be sure to clear the comlime struct before the first call to
 *          prevents debug68_cli_free() to try to free invalid buffer.
 */
int debug68_cli_read(const char * prompt, debug68_cli_t * cli);

DEBUG68_EXTERN
/** Free command line.
 *
 *  @param  cli  Free internal buffer and clean structure.
 *
 */
void debug68_cli_free(debug68_cli_t * cli);

#endif /* #ifndef _DEBUG68_CLI_H_ */
