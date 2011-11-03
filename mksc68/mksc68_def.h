/**
 * @ingroup  mksc68_prg
 * @file     mksc68/mksc68_def.h
 * @author   Benjamin Gerard
 * @date     2009-01-01
 * @brief    general defines header.
 *
 */

/* Copyright (C) 1998-2011 Benjamin Gerard */

#ifndef _MKSC68_DEF_H_
# define _MKSC68_DEF_H_

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

#endif /* ifndef _MKSC68_DEF_H_ */
