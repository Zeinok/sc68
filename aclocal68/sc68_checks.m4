dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# Time-stamp: <2013-06-05 03:02:50 ben>
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009-2013 Benjamin Gerard
dnl#
dnl# Distribued under the term of the GPL3+

# serial 20130605 sc68_checks.m4

# SC68_CHECKS()
# -------------
# Does common checks for all sc68 related packages.
AC_DEFUN_ONCE([SC68_CHECKS],[
    AC_REQUIRE([SC68_TOOLS])
    AC_REQUIRE([SC68_OPTIONS])
   
    # Check for __dllspec
    # -------------------
    AS_IF([test "x$ac_cc_declspec" = "xyes"],
      [AC_DEFINE([HAVE_DECLSPEC],[1],[Support __declspec()])])

    # visibility attribut
    # -------------------
    if test [$]has_visibility = yes; then
      AC_DEFINE([HAVE_VISIBILITY],[1],[Support visibility __attribute__])
      AX_CHECK_COMPILER_FLAGS(
        [-fvisibility=hidden],
        [fvisibility=yes],
        [fvisibility=no])
      if test [$]fvisibility = yes; then
        SC68_ADD_FLAG(LIB_CFLAGS,[-fvisibility=hidden])
      fi
    fi
    
    # Check some more (mostly GCC) switch
    # -----------------------------------
    AC_FOREACH([FLAG],
      [-std=${CSTD-c99} -pedantic -Wall],
      [AX_CHECK_COMPILER_FLAGS(FLAG,SC68_ADD_FLAG(ALL_CFLAGS,FLAG))])

    # --enable-debug
    if test X[$]enable_sc68_debug = Xyes; then
      if test X[$]GCC = Xyes; then
        :
      fi
    fi

    # --enable-release
    if test X[$]enable_sc68_release = Xyes; then
      if test X[$]GCC = Xyes; then
        :
      fi
    fi

    # --enable-all-static
    if test X[$]enable_sc68_static = Xyes; then
      AC_ENABLE_STATIC
      AC_DISABLE_SHARED
      SC68_ADD_FLAG(ALL_LFLAGS,[-all-static])
      SC68_ADD_FLAGS(ALL_CFLAG,[-static])
    fi

    # libtool shared library needs -no-undefined
    AC_MSG_CHECKING([whether shared library need -no-undefined])
    if test X[$]ac_sys_platform_win32 = Xyes; then
      AC_MSG_RESULT([yes])
      SC68_ADD_FLAG(LIB_LFLAGS,[-no-undefined])
    else
      AC_MSG_RESULT([no])
    fi

    # How to handle assert and add --enable-assert
    AC_CHECK_HEADERS([limits.h])
    AC_CHECK_HEADERS([assert.h])
    AC_HEADER_ASSERT

    # ALL_FLAGS : common for all (both libraries and programms)
    AC_SUBST(ALL_CFLAGS)
    AC_SUBST(ALL_LFLAGS)

    # LIB_FLAGS : for building libraries
    AC_SUBST(LIB_CFLAGS)
    AC_SUBST(LIB_LFLAGS)

    # BIN_FLAGS : for building programms
    AC_SUBST(BIN_CFLAGS)
    AC_SUBST(BIN_LFLAGS)

    # PAC_FLAGS : for others to compile/link against us
    AC_SUBST([PAC_CFLAGS])
    AC_SUBST([PAC_LFLAGS])
    AC_SUBST([PAC_REQUIRES])
    AC_SUBST([PAC_CONFLICTS])
    
    # help tracking libtool dependencies
    AC_SUBST([LIBTOOL_DEPS])

  ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of sc68_checks.m4
dnl#
dnl# ----------------------------------------------------------------------
