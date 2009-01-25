dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for libsc68
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id$
dnl#


# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT([libsc68],[3.0.0],[benjihan -4t- users.sourcefourge -d0t- net])
AC_CONFIG_SRCDIR([api68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])
AC_CONFIG_HEADERS([config.h])
SC68_PACKAGE([sc68 engine library.])
[LIBSC68_CPPFLAGS="-DSC68_EXPORT=1"]
[LIBSC68_LDFLAGS="-version-info 3:0:0"]


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
# | Required  libraries                                                  |
# `----------------------------------------------------------------------'

AC_WITH_PACKAGE([file68],[yes],[],[file68/file68.h])


# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'
AC_CONFIG_FILES([
    Makefile
    emu68/Makefile
    io68/Makefile
    libsc68-${VERSION}.ebuild:libsc68.ebuild.in
    libsc68.spec
    sc68-config
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
dnl# End of "$Id$".
dnl#
