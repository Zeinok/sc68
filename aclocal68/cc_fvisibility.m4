dnl# -*- m4 -*-
dnl#
dnl# sc68 - autoconf macros
dnl#
dnl# by Benjamin Gerard <ben [at] sashipa [dot] com>
dnl#
dnl# $Id$
dnl#


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


dnl# ########################################################################
dnl#
dnl# End Of $Id$
dnl#
dnl# ########################################################################
