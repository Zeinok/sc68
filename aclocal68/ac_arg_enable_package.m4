dnl# -*- mode:m4; tab-width:2 -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2007-2009 Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# Distribued under the term of the GPL
dnl#
dnl# $Id$
dnl#

# serial 20081222

# $1=Package $2=Varname $3=Description $4=Default-Value
m4_define([REAL_ARG_ENABLE_PACKAGE],[
	AC_ARG_ENABLE(
    [$1],
    [AS_HELP_STRING([--enable-$1],[$3 @<:@default=$4@:>@])],
		[],[enable_]$2[="$4"])
	AS_CASE(["[$][enable_]$2"],
	  [no|yes|check],[:],
		[AC_MSG_ERROR(
				[invalid value ([$]enable_$2) for --enable-$1 (use no/yes/check)])])
	$2_dir="[\$][srcdir/]$1"
	AS_IF([test [$][enable_]$2 = no],[has_$2=no],[
		AC_MSG_CHECKING([whether sub-package $1 exists])
		AS_IF([test -d "[$]srcdir/$1"],[has_$2="yes"],[has_$2="no"])
		AC_MSG_RESULT([[$]has_$2 ([$]$2_dir)])])
	AS_IF([test "[$]{has_$2}x[$]{enable_$2}" = "noxyes"],
		[AC_MSG_ERROR([unable to configure requested package ($1)])])
	enable_$2="[$]has_$2"
])

# AC_ARG_ENABLE_PACKAGE([PACKAGE],[DESC],[DEFVAL])
# ------------------------------------------------
# Defines --enable-<sub-package> configure option
AC_DEFUN([AC_ARG_ENABLE_PACKAGE],
[
	REAL_ARG_ENABLE_PACKAGE(
		[$1],m4_translit([$1],[-],[_]),
		m4_default([$2],[optionnal package $1]),
		m4_default([$3],[check]))
])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
