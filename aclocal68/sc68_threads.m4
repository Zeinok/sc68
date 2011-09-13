dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# Time-stamp: <2011-09-13 18:07:50 ben>
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009-2011 Benjamin Gerard
dnl#
dnl# Distribued under the term of the GPL3+

# serial 20110910 sc68_threads.m4

# SC68_THREADS()
# ------------
# How to compile with POSIX threads
AC_DEFUN([SC68_THREADS],[
    AX_CHECK_COMPILER_FLAGS(
      [-pthread],
      [SC68_ADD_FLAG([ALL_CFLAGS],[-pthread])])
  ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End of sc68_threads.m4
dnl#
dnl# ----------------------------------------------------------------------
