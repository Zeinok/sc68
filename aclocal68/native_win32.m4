dnl# -*- m4 -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2007 Benjamin Gerard <benjihan [at] users.sourceforge [dot] net>
dnl#
dnl# Distribued under the term of the GPL
dnl#
dnl# $Id$
dnl#


# AC_SYS_NATIVE_WIN32()
# ---------------------
AC_DEFUN_ONCE([AC_SYS_NATIVE_WIN32],
    [
	AC_MSG_CHECKING([for native Win32])
	AC_REQUIRE([AC_CANONICAL_HOST])
	case "$host" in
	    *-mingw*)
		ac_sys_native_win32=yes ;;
	    *)  ac_sys_native_win32=no ;;
	esac
	AC_MSG_RESULT([$ac_sys_native_win32])
    ])


dnl# ########################################################################
dnl#
dnl# End Of $Id: configure.ac,v 1.3 2007/01/12 09:43:07 benjihan Exp $
dnl#
dnl# ########################################################################
