dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for as68
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id$
dnl#

# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT(
  [as68],
  [20090117],
  [SC68_EMAIL],,
  [SC68_URL])
AC_CONFIG_SRCDIR([as68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])

AC_USE_SYSTEM_EXTENSIONS
LT_INIT([win32-dll])

# ,----------------------------------------------------------------------.
# | sc68                                                                 |
# `----------------------------------------------------------------------'

SC68_PACKAGE([Motoral MC68000 macro assembler.])
SC68_SYSTEM
SC68_OPTIONS
SC68_TOOLS
SC68_CHECKS

# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_CONFIG_FILES([
  Makefile
  as68-${VERSION}.ebuild:as68.ebuild.in
  as68.1.tpl
  as68.spec
  package.texi])
AC_OUTPUT

# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,-----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|-----------------------])
AC_MSG_NOTICE([|   debug-mode          : $enable_sc68_debug])
AC_MSG_NOTICE([|   release-mode        : $enable_sc68_release])
AC_MSG_NOTICE([|   all-static-mode     : $enable_sc68_static])
AC_MSG_NOTICE([+-----------------------])
AC_MSG_NOTICE([])

dnl#
dnl# End of "$Id$".
dnl#
