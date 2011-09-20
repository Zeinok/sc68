dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# Time-stamp: <2011-09-18 17:32:43 ben>
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009-2011 Benjamin Gerard
dnl#
dnl# Distribued under the term of the GPL3+

# serial 20110918 sc68_system.m4

# SC68_SYSTEM()
# -------------
# Setup host and build for all sc68 related packages.
AC_DEFUN_ONCE([SC68_SYSTEM],[
    AC_REQUIRE([AC_CANONICAL_BUILD])
    AC_REQUIRE([AC_CANONICAL_HOST])
    
    # build for win32 platform ?
    AC_MSG_CHECKING([for some win32 platform])
    ac_sys_platform_win32=no
    case "[$]host" in
      *-mingw*|*-cygwin*|*win32*)
        ac_sys_platform_win32=yes ;;
    esac
    AC_MSG_RESULT([$ac_sys_platform_win32])

    # build for native win32 platform (w/o un*x style layout)
    AC_MSG_CHECKING([for native win32 platform])
    ac_sys_native_win32=no
    case x-"[$]host_os" in
      x-mingw32 | x-win32)
        ac_sys_native_win32=yes ;;
    esac
    AC_MSG_RESULT([$ac_sys_native_win32])
  ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of sc68_system.m4
dnl#
dnl# ----------------------------------------------------------------------
