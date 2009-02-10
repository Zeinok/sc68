/**
 * @ingroup   io68_devel
 * @file      io68/io68.h
 * @author    Benjamin Gerard
 * @date      1999/03/20
 * @brief     all io68 headers.
 *
 * $Id$
 */

/* Copyright (C) 1998-2007 Benjamin Gerard */

#ifndef _IO68_H_
#define _IO68_H_

#include "io68_api.h"

#include "ym_io.h"
#include "mw_io.h"
#include "shifter_io.h"
#include "mfp_io.h"
#include "paula_io.h"

/** IO chip init parameters. */
typedef struct {
  paula_parms_t paula;		/**< paula init parms.      */
  mw_parms_t    mw;		/**< microwire init parms.  */
  ym_parms_t    ym;		/**< ym-2149 init parms.    */
  int          *argc;		/**< Argument count.        */
  char        **argv;		/**< Arguments.             */
} io68_init_t;

#ifdef __cplusplus
extern "C" {
#endif

IO68_API
int io68_init(io68_init_t * const parms);

IO68_API
void io68_shutdown(void);

IO68_API
void io68_destroy(io68_t * const io);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _IO68_H_ */
