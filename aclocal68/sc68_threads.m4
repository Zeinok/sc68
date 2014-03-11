dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009-2014 Benjamin Gerard
dnl#
dnl# Distribued under the term of the GPL3+

# serial 20140309 sc68_threads.m4

# SC68_THREADS()
# ------------
# How to compile with threads support (only POSIX threads ATM).
AC_DEFUN([SC68_THREADS],[

    dnl # Only support pthread right now.
    AC_CHECK_HEADERS([pthread.h],[],
      [AC_MSG_ERROR([could not locate pthread.h header])])

    sc68_threads=no
    if test X"[$]enable_sc68_static" = Xlib &&
      test X"[$]GCC" = Xyes; then
      dnl # Not accurate: should try to link only static only
      AC_SEARCH_LIBS([pthread_create],[pthread],[sc68_threads=pthread])
    else
      sc68_threads_cflags="[$]CFLAGS"
      SC68_ADD_FLAG(CFLAGS,-pthread)
      AC_SEARCH_LIBS([pthread_create],[pthread],
        [
          sc68_threads=pthread
          SC68_ADD_FLAG([ALL_CFLAGS],-pthread)
        ],
        [
          CFLAGS="[$]sc68_threads_cflags"
          AC_SEARCH_LIBS([pthread_create],[pthread],[sc68_threads=pthread])
        ]
      )
      CFLAGS="[$]sc68_threads_cflags"
      unset sc68_threads_cflags
    fi

    if test x"[$]sc68_threads" = xno; then
      AC_MSG_ERROR([unable to configure thread support])
    fi
  ]
)

dnl# ----------------------------------------------------------------------
dnl#
dnl# End of sc68_threads.m4
dnl#
dnl# ----------------------------------------------------------------------
