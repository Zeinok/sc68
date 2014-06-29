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
SC68_THREADS
SC68_WITH([sc68],[yes],[sc68_version],[sc68/sc68.h],[libsc68])
SC68_WITH_HEADER([file68],[yes],[sc68/file68.h])

AC_ARG_WITH(
  [audacious-dir],
  [AS_HELP_STRING([--with-audacious-dir],
      [audacious imput plugin location @<:@auto*|user|path@:>@])],
  [],[with_audacious_dir=auto])

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

# Convert version string to version number 
set -- `echo [$]audacious_ver | sed 's%\.% %g'`
audvnum=0; audmul=10000
while test [$]# -gt 0; do
  let audvnum=audvnum+[$]1*audmul
  let audmul=audmul/100
  shift
done
AC_DEFINE_UNQUOTED([AUDACIOUS_VERNUM],[$audvnum],[audacious version number])

case x"$with_audacious_dir" in

  xuser)
    with_audacious_dir=$HOME/.local/share/audacious/Plugins
    ;;

  xauto)
    with_audacious_dir=`[$]pkgconfig audacious --variable=input_plugin_dir`
    if test x"$with_audacious_dir" = x; then
      with_audacious_dir=`[$]pkgconfig audacious --variable=plugin_dir`/Input
    fi
    if ! test -d "$with_audacious_dir"; then
      AC_MSG_ERROR([Unable to locate audacious input plugins directory. Try --with-audacious-dir.])
    fi
    ;;

  x/*)
    ;;

  *)
    AC_MSG_ERROR([Invalid value for --with-audacious-dir (auto|user|<path>)])
    ;;
esac
AC_SUBST([plugindir],[$]with_audacious_dir)

aud_cflags="[$]([$]pkgconfig audacious --cflags)"
aud_lflags="[$]([$]pkgconfig audacious --libs)"

if test -n "$aud_cflags"; then
  SC68_ADD_FLAGS([LIB_CFLAGS],$aud_cflags)
fi
if test -n "$aud_lflags"; then
  SC68_ADD_FLAGS([LIB_LFLAGS],$aud_lflags)
fi

# Sometime glib include path is missing.
AC_CHECK_HEADER([glib.h],,
  [
    glib_pkg=glib-2.0
    if glib_version=[$]([$]pkgconfig [$]glib_pkg --modversion); then
      glib_cflags=[$]([$]pkgconfig [$]glib_pkg --cflags)
      SC68_ADD_FLAGS([LIB_CFLAGS],[$]glib_cflags)
    fi
  ])

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
AC_MSG_NOTICE([|   audacious           : $audacious_ver ($audvnum)])
AC_MSG_NOTICE([|   plugin-dir          : $with_audacious_dir])
AC_MSG_NOTICE([+-----------------------])
