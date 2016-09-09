dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for unice68
dnl#
dnl# by Benjamin Gerard <http://sourceforge.net/users/benjihan>
dnl#

# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

define([sc_version], esyscmd([sh vcversion.sh 2.0.0]))dnl

AC_INIT(
  [unice68],
  sc_version,
  [http://sourceforge.net/projects/sc68/],
  [unice68],
  [http://sc68.atari.org])
AC_CONFIG_SRCDIR([unice68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign 1.11 silent-rules dist-xz
                  no-installman no-installinfo])
AC_CONFIG_HEADERS([config.h])
AC_USE_SYSTEM_EXTENSIONS
LT_INIT([win32-dll])

# ,----------------------------------------------------------------------.
# | sc68                                                                 |
# `-------------------------------------------------------------------

AC_SUBST([PACKAGE_SHORTDESC],["ice packer/depacker program and library"])
SC68_CC
AC_PROG_INSTALL

# ,----------------------------------------------------------------------.
# | Optionnal features                                                   |
# `----------------------------------------------------------------------'

# Command line depacker.
AC_ARG_ENABLE(
  [unice68-cli],
  [AS_HELP_STRING([--enable-unice68-cli],
      [enable command line program @<:@default=yes@:>@])],
  [],[enable_unice68_cli="yes"])
AM_CONDITIONAL([UNICE68_CLI],[test "x$enable_unice68_cli" = "xyes"])

# ,----------------------------------------------------------------------.
# | Functions and headers                                                |
# `----------------------------------------------------------------------'
AC_HEADER_ASSERT
AC_CHECK_HEADERS([assert.h stdio.h stdlib.h string.h stdarg.h])
AC_CHECK_HEADERS([unistd.h fcntl.h errno.h libgen.h io.h])
AC_CHECK_FUNCS([basename _fileno fileno _setmode])

# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_ARG_VAR([lib_LDFLAGS],
           [Supplemental LDFLAGS for building the library.
            Typically libtool flags such as -no-undefined.])

AC_ARG_VAR([bin_LDFLAGS],
           [Supplemental LDFLAGS for building the binary.
            Typically libtool flags such as -all-static.])

AC_CONFIG_FILES([
    unice68.pc
    Makefile])
AC_OUTPUT

# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,-----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|-----------------------])
AC_MSG_NOTICE([|   build cli           : $enable_unice68_cli])
AC_MSG_NOTICE([+-----------------------])
AC_MSG_NOTICE([])
