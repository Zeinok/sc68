dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for info68
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id$
dnl#


# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT([info68],[20090117],[benjihan -4t- users.sourceforge -d0t- net])
AC_CONFIG_SRCDIR([info68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])

SC68_PACKAGE([
    Get tag info from sc68 music files.
    This is part of the sc68 project.
  ])

# ,----------------------------------------------------------------------.
# | External libraries                                                   |
# `----------------------------------------------------------------------'

AC_WITH_PACKAGE([file68],[yes],[],[file68/file68.h])


# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_CONFIG_FILES([
    info68-${VERSION}.ebuild:info68.ebuild.in
    info68.1.tpl
    info68.spec
    Makefile])

AC_OUTPUT


# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

AC_MSG_NOTICE([])

AC_MSG_NOTICE([,----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|----------------------])
AC_MSG_NOTICE([|   debug-mode          : $enable_debug])
AC_MSG_NOTICE([|   all-static-mode     : $enable_all_static])
AC_MSG_NOTICE([|   file68              : $has_file68 ($file68_ver)])
AC_MSG_NOTICE([+----------------------])
AC_MSG_NOTICE([])

dnl#
dnl# End of "$Id$".
dnl#
