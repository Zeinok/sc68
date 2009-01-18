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


# AC_SYS_PLATFORM_WIN32([RESULT-VAR = ac_sys_platform_win32])
# -----------------------------------------------------------
# Check if host is a win32 platform
#
# TODO: -wince, -winnt -wince -uwin -windows ... probably others
#
AC_DEFUN_ONCE([AC_SYS_PLATFORM_WIN32],
[
  AC_MSG_CHECKING([for some Win32 platform])
  AC_REQUIRE([AC_CANONICAL_HOST])
  case "$host" in
      *-mingw*|*-cygwin*|*win32*)
          ac_test_res=yes ;;
      *)  ac_test_res=no ;;
  esac
  AC_MSG_RESULT([$ac_test_res])
  ifelse([$1], ,[ac_sys_platform_win32],[$1])="$ac_test_res"
])

dnl# ########################################################################
dnl#
dnl# End Of $Id$
dnl#
dnl# ########################################################################
