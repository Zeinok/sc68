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

AC_INIT(
  [unice68],
  [20110825],
  [benjihan -4t- users.sourceforge -d0t- net],,
  [http://sc68.atari.org])
AC_CONFIG_SRCDIR([unice68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 silent-rules check-news color-tests dist-bzip2])

AC_USE_SYSTEM_EXTENSIONS
LT_INIT([win32-dll])

# ,----------------------------------------------------------------------.
# | sc68                                                                 |
# `----------------------------------------------------------------------'

SC68_PACKAGE([ice depacker program and library.])
SC68_SYSTEM
SC68_OPTIONS
SC68_TOOLS
SC68_CHECKS

# ,----------------------------------------------------------------------.
# | Optionnal features                                                   |
# `----------------------------------------------------------------------'

# Command line depacker.
AC_ARG_ENABLE(
  [unice68-cli],
  [AS_HELP_STRING([--enable-unice68-cli],
      [enable command line depacker @<:@default=yes@:>@])],
  [],[enable_unice68_cli="yes"])
AM_CONDITIONAL([UNICE68_CLI],[test "x$enable_unice68_cli" = "xyes"])

# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_CONFIG_FILES([
    Makefile
    package.texi
    unice68-${VERSION}.ebuild:unice68.ebuild.in
    unice68.1.tpl
    unice68.pc
    unice68.spec])
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
AC_MSG_NOTICE([|   build cli depacker  : $enable_unice68_cli])
AC_MSG_NOTICE([+-----------------------])
AC_MSG_NOTICE([])

dnl#
dnl# End of "$Id$".
dnl#
