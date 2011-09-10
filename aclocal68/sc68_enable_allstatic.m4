dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# Time-stamp: <2011-09-08 13:20:01 ben>
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009-2011 Benjamin Gerard
dnl#
dnl# Distribued under the term of the GPL3+

# serial 20110908 sc68_enable_allstatic.m4

# SC68_ENABLE_ALLSTATIC([MORE-ENABLE-ACTION],[MORE-DISABLE-ACTION])
# -----------------------------------------------------------------
# Defines --enable-sc68-static option
AC_DEFUN_ONCE([SC68_ENABLE_ALLSTATIC],[
    AC_ARG_ENABLE(
      [sc68-static],
      [AS_HELP_STRING([--enable-sc68-static],
	  [all static linkage @<:@default=no@:>@])],
      [],[enable_sc68_static=no])
    SC68_ENABLE_THIS([enable_sc68_static])
  ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of sc68_enable_allstatic.m4
dnl#
dnl# ----------------------------------------------------------------------
