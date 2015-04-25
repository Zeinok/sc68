/**
 * @ingroup  sourcer68_prg
 * @file     src68_def.h
 * @author   Benjamin Gerard
 * @date     2015-03-18
 * @brief    general defines header.
 *
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef SOURCER68_DEF_H
#define SOURCER68_DEF_H

#ifdef HAVE_ASSERT_H
# include <assert.h>
# else
# define assert(V)
#endif

#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif

typedef unsigned int uint_t;

#endif
