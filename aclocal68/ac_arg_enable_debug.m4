dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2007-2009 Benjamin Gerard
dnl#
dnl# Distributed under the term of the GPL
dnl#
dnl# $Id$
dnl#

# serial 20090116

# AC_ARG_ENABLE_DEBUG([MORE-ENABLE-ACTION],[MORE-DISABLE-ACTION])
# ---------------------------------------------------------------
# Defines --enable-debug option
AC_DEFUN_ONCE([AC_ARG_ENABLE_DEBUG],
  [
    AC_ARG_ENABLE(
      [debug],
      [AS_HELP_STRING([--enable-debug],[debug facilities @<:@default=no@:>@])],
      [],[enable_debug=no])
    AS_CASE([${enable_debug}],
      [yes],[:],[no],[:],[
        set -- $(echo ${enable_debug}|sed "s/,/ /g")
        enable_debug=no
        for ac_eopt in : "[$]@"; do
	  AS_IF([test "AC_PACKAGE_NAME" = "$ac_eopt"],[enable_debug=yes])
        done
      ])
    AS_TR_SH(AC_PACKAGE_NAME)_enable_debug=${enable_debug-no}
    AS_IF([test "x${enable_debug}" = "xyes"],[
        AC_DEFINE([DEBUG],[1],[debug facilities])
        AC_DEFINE([DEBUG_]AS_TR_CPP(AC_PACKAGE_NAME),[1],
 	  [debug facilities for ]AC_PACKAGE_NAME)
        $1],[
        enable_debug=no
        $2])
  ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
