dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009 Benjamin Gerard <benjihan 4t users.sourceforge d0t net>
dnl#
dnl# Distributed under the term of the GPL
dnl#
dnl# $Id$
dnl#


# serial 20090727

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
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
