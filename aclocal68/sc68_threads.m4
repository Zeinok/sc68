dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# Time-stamp: <2013-05-13 01:58:34 ben>
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009-2013 Benjamin Gerard
dnl#
dnl# Distribued under the term of the GPL3+

# serial 20130513 sc68_threads.m4

# SC68_THREADS()
# ------------
# How to compile with POSIX threads
AC_DEFUN([SC68_THREADS],[
    has_threads=no

    dnl # DNW: gcc print an error message but nothing critical.
    AX_CHECK_COMPILER_FLAGS([-pthread],[CFLAGS="$CFLAGS -pthread"])
      
    AC_CHECK_HEADERS(
      [pthread.h],
      [
        sc68_threads_CFLAGS="$CFLAGS"

        AC_SEARCH_LIBS(
          [pthread_create],
          [pthread],
          [has_threads=pthread],
          [],
          [])
        
        AC_CHECK_FUNCS([pthread_timedjoin_np])
      ]
    )
    if test x"$has_threads" = xno; then
      AC_MSG_ERROR([unable to configure thread support])
    fi
  ]
)

dnl# ----------------------------------------------------------------------
dnl#
dnl# End of sc68_threads.m4
dnl#
dnl# ----------------------------------------------------------------------
