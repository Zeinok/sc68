/**
 *
 */

/* $Id$ */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifndef EMU68_MONOLITIC
# error "EMU68_MONOLITIC should be defined when compiling this file."
#else
# include "emu68_api.h"

/* Monolitic build: remove all forced inlines */
# undef  inline
# define inline

# include "lines68.h"
# include "lines/line0.c"
# include "lines/line1.c"
# include "lines/line2.c"
# include "lines/line3.c"
# include "lines/line4.c"
# include "lines/line5.c"
# include "lines/line6.c"
# include "lines/line7.c"
# include "lines/line8.c"
# include "lines/line9.c"
# include "lines/lineA.c"
# include "lines/lineB.c"
# include "lines/lineC.c"
# include "lines/lineD.c"
# include "lines/lineE.c"
# include "lines/lineF.c"
# include "lines/table.c"
#endif
