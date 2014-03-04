dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for sc68-audacious
dnl#
dnl# by Benjamin Gerard <http://sourceforge.net/users/benjihan>
dnl#

# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT(
  [sc68-audacious],
  [0.1.0],
  [benjihan -4t- users.sourceforge -d0t- net],
  [sc68-audacious],
  [http://sc68.atari.org])
AC_CONFIG_SRCDIR([sc68-audacious.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign 1.10 silent-rules color-tests dist-bzip2])

AC_USE_SYSTEM_EXTENSIONS
LT_INIT([win32-dll])

# ,----------------------------------------------------------------------.
# | sc68                                                                 |
# `----------------------------------------------------------------------'

SC68_PACKAGE([Atari ST and Amiga audio plugin for audacious.])
SC68_SYSTEM
SC68_OPTIONS
SC68_TOOLS
SC68_CHECKS
SC68_WITH([sc68],[yes],[sc68_version],[sc68/sc68.h],[libsc68])
SC68_WITH_HEADER([file68],[yes],[sc68/file68.h])

AC_ARG_WITH(
  [audacious-dir],
  [AS_HELP_STRING([--with-audacious-dir],
      [install in audacious user plugin @<:@default=no@:>@])],
  [],[with_audacious_dir=no])

AC_MSG_CHECKING([for pkg-config audacious module])
if ! [$]pkgconfig --exists audacious; then
  AC_MSG_RESULT([no])
  AC_MSG_ERROR([unable to configure a required package ... audacious])
else
  audacious_ver=[$]([$]pkgconfig audacious --modversion)
  AC_MSG_RESULT([yes ($audacious_ver)])
fi

audacious_min='2.4.0'
AC_MSG_CHECKING([if audacious version >= $audacious_min]) # need more test on version
if [$]pkgconfig audacious --atleast-version=$audacious_min; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
  AC_MSG_ERROR([installed audacious version is too old ($audacious_ver<$audacious_min)])
fi

AC_MSG_CHECKING([for audacious 3])
if [$]pkgconfig audacious --atleast-version='3.0.0'; then
  AC_MSG_RESULT([yes])
  AC_DEFINE([AUDACIOUS3],[1],[audacious >= 3.0.0])
else
  AC_MSG_RESULT([no])
fi

if test x"$with_audacious_dir" = xno; then
  with_audacious_dir=`[$]pkgconfig audacious --variable=input_plugin_dir`
elif test x"$with_audacious_dir" = xuser; then
  with_audacious_dir=$HOME/.local/share/audacious/Plugins
fi
AC_SUBST([plugindir],[$]with_audacious_dir)

aud_cflags=[$]([$]pkgconfig audacious --cflags)
aud_lflags=[$]([$]pkgconfig audacious --libs)

if test -n "$aud_cflags"; then
  SC68_ADD_FLAGS([LIB_CFLAGS],$aud_cflags)
fi
if test -n "$aud_lflags"; then
  SC68_ADD_FLAGS([LIB_LFLAGS],$aud_lflags)
fi


# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_CONFIG_FILES([
    Makefile])
AC_OUTPUT

# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

SC68_DUMPVARS

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,-----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|-----------------------])
AC_MSG_NOTICE([|   compile mode        : $compile_mode])
AC_MSG_NOTICE([|   file68 headers      : $has_file68 ($file68_ver)])
AC_MSG_NOTICE([|   libsc68             : $has_sc68 ($sc68_ver)])
AC_MSG_NOTICE([|   plugin-dir          : $with_audacious_dir])
AC_MSG_NOTICE([+-----------------------])
