/* $Id$ */

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
