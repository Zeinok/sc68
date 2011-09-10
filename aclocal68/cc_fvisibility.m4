dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# Time-stamp: <2011-09-08 13:15:49 ben>
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009-2011 Benjamin Gerard
dnl#
dnl# Distribued under the term of the GPL3+

# serial 20110908 cc_fvisibility.m4

# AC_C_FVISIBILITY([VISIBILITY = hidden])
# ---------------------------------------
AC_DEFUN_ONCE([AC_C_FVISIBILITY],
[
m4_if($1,[],[AC_C_FVISIBILITY]([hidden]),
AC_MSG_CHECKING(whether $CC accepts -fvisibility=$1)
AC_CACHE_VAL(ac_cv_fvisibility,
    [ac_save_cc="$CC"
	CC="$CC -fvisibility=$1"
	AC_COMPILE_IFELSE(
	    [int main() { return 0; }],
	    [ac_cv_fvisibility=yes],
	    [ac_cv_fvisibility=no])
	CC="$ac_save_cc"])
AC_MSG_RESULT($ac_cv_fvisibility)
)
])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of cc_fvisibility.m4
dnl#
dnl# ----------------------------------------------------------------------
