dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for sc68
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id$
dnl#


# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT([sc68],[3.0.0],[benjihan -4t- users.sourceforge -d0t- net])
AC_CONFIG_SRCDIR([sc68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])

SC68_PACKAGE([
    Atari ST and Amiga command line player.
    This is part of the sc68 project.
  ])

# ,----------------------------------------------------------------------.
# | Some headers, types and functions                                    |
# `----------------------------------------------------------------------'

AC_CHECK_HEADERS(
  [stdarg.h stdio.h stdlib.h string.h errno.h ctype.h unistd.h getopt.h])

AC_CHECK_FUNCS(
  [getopt getopt_long])

AC_CHECK_TYPES(
  [struct option],[],[],[
AC_INCLUDES_DEFAULT
[#]ifdef HAVE_GETOPT_H
[#] include <getopt.h>
[#]endif
])


# ,----------------------------------------------------------------------.
# | External libraries                                                   |
# `----------------------------------------------------------------------'

AC_WITH_PACKAGE([sc68],[yes],[sc68_version],[sc68/sc68.h],[libsc68])
AC_WITH_PACKAGE_HEADER([file68],[yes],[sc68/file68.h])

# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_CONFIG_FILES([
    Makefile
    package.texi
    sc68-${VERSION}.ebuild:sc68.ebuild.in
    sc68.1.tpl
    sc68.spec])

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
AC_MSG_NOTICE([|   file68 headers      : $has_file68 ($file68_ver)])
AC_MSG_NOTICE([|   libsc68             : $has_sc68 ($sc68_ver)])
AC_MSG_NOTICE([+-----------------------])

dnl#
dnl# End of "$Id$".
dnl#
