dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for sc68-ng
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id: configure.ac,v 1.3 2007/01/12 09:43:07 benjihan Exp $
dnl#


# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT([sc68-ng],[20090117],[benjihan -4t- users.sourceforge -d0t- net])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])

SC68_PACKAGE([
    Atari ST and Amiga music player.
  ])

# ,----------------------------------------------------------------------.
# | Optional Packages                                                    |
# `----------------------------------------------------------------------'

AC_ARG_ENABLE(
  [devtools],
  [AS_HELP_STRING(
    [--enable-devtools],
    [optionnal developer tools @<:@default=no@:>@])],
  [],[enable_devtools=no])

AC_ARG_ENABLE(
  [all],
  [AS_HELP_STRING(
    [--enable-all],
    [enable all sub-packages @<:@default=no@:>@])],
  [],[enable_all="check"])

AS_CASE(["x$enable_all"],
  [xno|xyes],[enable_devtools="$enable_all"],
  [enable_all="check"])

# Dev Tools
AC_ARG_ENABLE_PACKAGE([as68],[68k macro assembler],[[$]enable_devtools])
AC_ARG_ENABLE_PACKAGE([sourcer68],[68k sourcer],[[$]enable_devtools])
AC_ARG_ENABLE_PACKAGE([debug68],[68k debugger],[[$]enable_devtools])
AC_ARG_ENABLE_PACKAGE([desa68],[68k disassembler library],[[$]enable_devtools])

# API Libraries
AC_ARG_ENABLE_PACKAGE([unice68],[ice depacker support],[[$]enable_all])
AC_ARG_ENABLE_PACKAGE([file68],[file68 library],[[$]enable_all])
AC_ARG_ENABLE_PACKAGE([libsc68],[sc68 engine],[[$]enable_all])

# CLI programs
AC_ARG_ENABLE_PACKAGE([info68],[sc68 files info program],[[$]enable_all])
AC_ARG_ENABLE_PACKAGE([sc68],[sc68 command line player],[[$]enable_all])

# DOC
AC_ARG_ENABLE_PACKAGE([sc68-doc],[sc68 documentation],[[$]enable_all])

# ,----------------------------------------------------------------------.
# | Package Dependencies                                                 |
# `----------------------------------------------------------------------'

## Don't check dependencies on the source tree. Some packages may not
## be present here but available by others means (may be installed or
## out of tree sources).
#
# info68 depends on file68
# file68 suggests unice68
# libsc68 depends on file68
# sc68 depends on libsc68

# ,----------------------------------------------------------------------.
# | Subdirs                                                              |
# `----------------------------------------------------------------------'

# # Absolute source
# src_adir=$(cd $srcdir; pwd || echo $PWD)
# AS_IF([test -z "$src_adir" || test ! -d "$src_adir"],
#   [AC_MSG_ERROR([unable to guess absolute source dir])])
# AC_MSG_NOTICE([absolute source dir $src_adir])

# # Absolute built
# blt_adir=$(pwd || echo $PWD)
# AS_IF([test -z "$blt_adir" || test ! -d "$blt_adir"],
#   [AC_MSG_ERROR([unable to guess absolute built dir])])
# AC_MSG_NOTICE([absolute built dir $blt_adir])

AS_IF([test "x${enable_as68}"      = "xyes"],
  [
    AC_CONFIG_SUBDIRS([as68])
  ])

AS_IF([test "x${enable_desa68}"    = "xyes"],
  [
    AC_CONFIG_SUBDIRS([desa68])
  ])

AS_IF([test "x${enable_unice68}"   = "xyes"],
  [
    AC_CONFIG_SUBDIRS([unice68])
  ])

AS_IF([test "x${enable_file68}"    = "xyes"],
  [
    AC_CONFIG_SUBDIRS([file68])
  ])

AS_IF([test "x${enable_info68}"    = "xyes"],
  [
    AC_CONFIG_SUBDIRS([info68])
  ])

AS_IF([test "x${enable_sourcer68}" = "xyes"],
  [
    AC_CONFIG_SUBDIRS([sourcer68])
  ])

AS_IF([test "x${enable_libsc68}"   = "xyes"],
  [
    AC_CONFIG_SUBDIRS([libsc68])
  ])

AS_IF([test "x${enable_sc68}"      = "xyes"],
  [
    AC_CONFIG_SUBDIRS([sc68])
  ])

AS_IF([test "x${enable_debug68}"   = "xyes"],[
    AC_CONFIG_SUBDIRS([debug68])
  ])

AS_IF([test "x${enable_sc68_doc}"   = "xyes"],[
    AC_CONFIG_SUBDIRS([sc68-doc])
  ])

AC_SUBST([subdirs])

# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_CONFIG_FILES([Makefile])
AC_OUTPUT


# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

[subdirs=$(echo $subdirs)]
AC_MSG_NOTICE([])
AC_MSG_NOTICE([,-----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|-----------------------])
AC_MSG_NOTICE([|   debug-mode          : $enable_debug])
AC_MSG_NOTICE([|   release-mode        : $enable_release])
AC_MSG_NOTICE([|   all-static-mode     : $enable_all_static])
AC_MSG_NOTICE([|   sub-dirs            : $subdirs])
AC_MSG_NOTICE([+-----------------------])
AC_MSG_NOTICE([])

dnl#
dnl# End of "$Id$".
dnl#
