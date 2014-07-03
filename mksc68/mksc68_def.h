/**
 * @ingroup  mksc68_prg
 * @file     mksc68/mksc68_def.h
 * @author   Benjamin Gerard
 * @date     2009-01-01
 * @brief    general defines header.
 *
 */

/* Copyright (c) 1998-2014 Benjamin Gerard */

#ifndef MKSC68_DEF_H
# define MKSC68_DEF_H

# ifndef EXTERN68
#  ifdef __cplusplus
#   define EXTERN68 extern "C"
#  else
#   define EXTERN68
#  endif
# endif

#ifdef HAVE_ASSERT_H
# include <assert.h>
# else
# define assert(V)
#endif

#endif
