dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for unice68
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourcefourge -d0t- net>
dnl#
dnl# $Id$
dnl#


# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT([unice68],[20090117],[benjihan -4t- users.sourcefourge -d0t- net])
AC_CONFIG_SRCDIR([unice68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])
SC68_PACKAGE([ice depacker program and library.])
[LIBUNICE68_CPPFLAGS="-DUNICE68_EXPORT=1"]
[LIBUNICE68_LDFLAGS="-version-info 2:0:0"]


# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_CONFIG_FILES([
    Makefile
    unice68-${VERSION}.ebuild:unice68.ebuild.in
    unice68.1.tpl
    unice68.spec
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
AC_MSG_NOTICE([|   release-mode        : $enable_release])
AC_MSG_NOTICE([|   all-static-mode     : $enable_all_static])
AC_MSG_NOTICE([+-----------------------])
AC_MSG_NOTICE([])

dnl#
dnl# End of "$Id$".
dnl#
