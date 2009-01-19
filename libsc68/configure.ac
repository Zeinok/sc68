dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for libsc68
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id: configure.ac,v 1.3 2007/01/12 09:43:07 benjihan Exp $
dnl#


# ,----------------------------------------------------------------------.
# | Autotools                                                            |
# `----------------------------------------------------------------------'

AC_INIT([libsc68],[3.0.0],[benjihan -4t- users.sourceforge -d0t- net])
AC_CONFIG_SRCDIR([api68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])
AM_MAINTAINER_MODE


# ,----------------------------------------------------------------------.
# | Package                                                              |
# `----------------------------------------------------------------------'

AC_PACKAGE_INFO(
  [Library for playing Atari ST and Amiga musics (part of the sc68 project).],
  [http://sc68.atari.org])
[
LIBSC68_CFLAGS=""                        # overall cflags
LIBSC68_CPPFLAGS="-DSC68_EXPORT"         # overall cppflags
LIBSC68_LA_LDFLAGS="-version-info 3:0:0" # library build cflags
LIBSC68_LDFLAGS=""                       # library build cppflags
SC68_CFLAGS=""                           # libtool library build options
SC68_CPPFLAGS=""                         # dependant libraries
]

# ,----------------------------------------------------------------------.
# | Build and host system                                                |
# `----------------------------------------------------------------------'

AC_CANONICAL_BUILD
AC_CANONICAL_HOST


# ,----------------------------------------------------------------------.
# | Guess win32 platform                                                 |
# `----------------------------------------------------------------------'

AC_SYS_PLATFORM_WIN32
AC_SYS_NATIVE_WIN32

AS_IF([test "x$ac_platform_win32" = "xyes"],
  [LIBSC68_LDFLAGS="$LIBSC68_LDFLAGS -no-undefined"])


# ,----------------------------------------------------------------------.
# | Compiler and companions                                              |
# `----------------------------------------------------------------------'

AC_PROG_CC
AM_PROG_CC_STDC
AC_PROG_INSTALL
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_LANG([C])
AC_C_INLINE

# ,----------------------------------------------------------------------.
# | "C" Compiler specific                                                |
# `----------------------------------------------------------------------'

# Check for GCC visibility patch
# ------------------------------
AC_C_VISIBILITY_ATTRIBUT
AS_IF(
  [test "x$ac_c_visibility_attribut" = "xyes"],
  [
    AC_DEFINE([HAVE_VISIBILITY],[1],[Support visibility __attribute__])
    AX_CHECK_COMPILER_FLAGS(
      [-fvisibility=hidden],
      [fvisibility=yes],
      [fvisibility=no])
    AS_IF(
      [test "x$fvisibility" = "xyes"],
      [CFLAGS="$CFLAGS -fvisibility=hidden"])
  ])

# Check for __dllspec
# -------------------
AC_PROG_CC_DECLSPEC
AS_IF(
  [test "x$ac_cc_declspec" = "xyes"],
  [AC_DEFINE([HAVE_DECLSPEC],[1],[Support __declspec()])])

# Check some more (mostly GCC) switch
# -----------------------------------
AC_FOREACH([FLAG],
  [-std=c99 -pedantic -Wall],
  [AX_CHECK_COMPILER_FLAGS(FLAG,[CFLAGS="$CFLAGS "]FLAG)])


# Debug support
# -------------
AS_IF([test "x${enable_debug}" = "xyes"],[
    AC_DEFINE([DEBUG],[1],[debug facilities])
    AX_CHECK_COMPILER_FLAGS([-g],[CFLAGS="$CFLAGS -g"])])

# ,----------------------------------------------------------------------.
# | Sized size integers                                                  |
# `----------------------------------------------------------------------'

AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INTMAX_T
AC_TYPE_LONG_LONG_INT
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINTMAX_T
AC_TYPE_UNSIGNED_LONG_LONG_INT


# ,----------------------------------------------------------------------.
# | Standard functions                                                   |
# `----------------------------------------------------------------------'

AC_CHECK_FUNCS([malloc free vsprintf vsnprintf getenv])


# ,----------------------------------------------------------------------.
# | Optional Features                                                    |
# `----------------------------------------------------------------------'

AC_ARG_ENABLE_DEBUG([
  AX_CHECK_COMPILER_FLAGS([-g],[SC68_CFLAGS=$(echo ${SC68_CFLAGS} -g)])
])

AC_ARG_ENABLE_ALLSTATIC([
  LIBSC68_LDFLAGS=$(echo -all-static ${LIBSC68_LDFLAGS})
])


# ,----------------------------------------------------------------------.
# | Required  libraries                                                  |
# `----------------------------------------------------------------------'

AC_WITH_PACKAGE([file68],[yes],[],[sc68/file68.h])


# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

PACKAGE_LIBS=$(echo $LDFLAGS $LIBS $file68_libs)

# Used by other programs to compile and to link against this package
AC_SUBST([PACKAGE_CFLAGS])      # cflags only (no cpp)
AC_SUBST([PACKAGE_DEFS])        # defines only 
AC_SUBST([PACKAGE_LDFLAGS])     # ldflags (-L)
AC_SUBST([PACKAGE_LIBS])        # libraries (-l)

# Used to compile this package
AC_SUBST([SC68_CFLAGS])         # overall cflags
AC_SUBST([SC68_CPPFLAGS])       # overall cppflags
AC_SUBST([LIBSC68_CFLAGS])      # library build cflags
AC_SUBST([LIBSC68_CPPFLAGS])    # library build cppflags
AC_SUBST([LIBSC68_LA_LDFLAGS])  # libtool library build options
AC_SUBST([LIBSC68_LDFLAGS])     # dependant libraries


AC_CONFIG_FILES([
    libsc68.spec
    libsc68-${VERSION}.ebuild:libsc68.ebuild.in
    sc68-config
    Makefile
    io68/Makefile
    emu68/Makefile
  ])
AC_OUTPUT

# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,-----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|-----------------------])
AC_MSG_NOTICE([|   debug-mode          : $enable_debug])
AC_MSG_NOTICE([|   all-static          : $enable_all_static])
AC_MSG_NOTICE([|   file68              : $has_file68 ($file68_ver)])
AC_MSG_NOTICE([+-----------------------])

dnl#
dnl# End of "$Id: configure.ac,v 1.1.1.1 2007/01/02 22:15:28 benjihan Exp $".
dnl#
