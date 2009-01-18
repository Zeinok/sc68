dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for desa68
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id$
dnl#


# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT([desa68],[20090117],[benjihan -4t- users.sourceforge -d0t- net])
AC_CONFIG_SRCDIR([desa68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])
SC68_PACKAGE([Motoral MC68000 disassembler library.])
[LIBDESA68_LDFLAGS="-version-info 2:0:0"]
[LIBDESA68_CPPFLAGS="-DDESA68_EXPORT=1"]

# Check for -Wno-multichar C flags
# --------------------------------
AX_CHECK_COMPILER_FLAGS(
  [-Wno-multichar],
  [DESA68_CFLAGS="$DESA68_CFLAGS -Wno-multichar"])

# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_CONFIG_FILES([
    desa68.spec
    desa68-${VERSION}.ebuild:desa68.ebuild.in
    Makefile])
AC_OUTPUT

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,-----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|-----------------------])
AC_MSG_NOTICE([|   debug-mode          : $enable_debug])
AC_MSG_NOTICE([|   all-static-mode     : $enable_all_static])
AC_MSG_NOTICE([+-----------------------])
AC_MSG_NOTICE([])

dnl#
dnl# End of "$Id$".
dnl#
