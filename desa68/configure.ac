dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for desa68
dnl#
dnl# by Benjamin Gerard <http://sourceforge.net/users/benjihan>
dnl#

# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

define([sc_version], esyscmd([sh vcversion.sh 2.0.0]))dnl

AC_INIT(
  [desa68],
  sc_version,
  [http://sourceforge.net/projects/sc68/],
  [desa68],
  [http://sc68.atari.org])
AC_CONFIG_SRCDIR([desa68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign 1.11 silent-rules dist-xz
                  no-installman no-installinfo])
AC_CONFIG_HEADERS([config.h])
AC_USE_SYSTEM_EXTENSIONS
LT_INIT([win32-dll])

# ,----------------------------------------------------------------------.
# | sc68                                                                 |
# `----------------------------------------------------------------------'

AC_SUBST([PACKAGE_SHORTDESC],["Motoral MC68000 disassembler library"])
SC68_CC
AC_PROG_INSTALL

# ,----------------------------------------------------------------------.
# | Functions and Headers
# `----------------------------------------------------------------------'

AC_HEADER_ASSERT
AC_CHECK_HEADERS([assert.h stdint.h])

# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_ARG_VAR([lib_LDFLAGS],
           [Supplemental LDFLAGS for building the library.
            Typically libtool flags such as -no-undefined.])

AC_CONFIG_FILES([
    desa68.pc
    Makefile])
AC_OUTPUT

# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,-----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([+-----------------------])
AC_MSG_NOTICE([])
