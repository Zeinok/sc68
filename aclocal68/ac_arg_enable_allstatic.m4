dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2007 Benjamin Gerard <benjihan 4t users.sourceforge d0t net>
dnl#
dnl# Distributed under the term of the GPL
dnl#
dnl# $Id$
dnl#


# serial 20090116

# AC_ARG_ENABLE_ALLSTATIC([MORE-ENABLE-ACTION],[MORE-DISABLE-ACTION])
# -------------------------------------------------------------------
# Defines --enable-all-static option
AC_DEFUN_ONCE([AC_ARG_ENABLE_ALLSTATIC],
[
  AC_ARG_ENABLE(
    [all-static],
    [AS_HELP_STRING([--enable-all-static],
	[all static linkage @<:@default=no@:>@])],
    [],[enable_all_static=no]
    )
  AS_CASE([${enable_all_static}],
    [yes],[:],[no],[:],[
      set -- $(echo ${enable_all_static}|sed "s/,/ /g")
      enable_all_static=no
      for ac_eopt in : "[$]@"; do
	AS_IF([test "AC_PACKAGE_NAME" = "$ac_eopt"],[enable_all_static=yes])
      done
    ])
  AS_IF([test "x${enable_all_static-no}" = "xyes" ],[$1],
    [enable_all_static=no;$2])
])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
