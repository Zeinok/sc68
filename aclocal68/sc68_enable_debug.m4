dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2007-2009 Benjamin Gerard
dnl#
dnl# Distributed under the term of the GPL
dnl#
dnl# $Id$
dnl#

# serial 20090304

# SC68_ENABLE_DEBUG([DEBUG-ACTION],[NO-DEBUG-ACTION],
#                   [RELEASE-ACTION],[NO-RELEASE-ACTION])
# -------------------------------------------------------
# Defines --enable-sc68-debug and --enable-sc68-release options
AC_DEFUN_ONCE([SC68_ENABLE_DEBUG],[
    # --enable-sc68-debug
    AC_ARG_ENABLE(
      [sc68-debug],
      [AS_HELP_STRING([--enable-sc68-debug],
          [debug facilities @<:@default=no@:>@])],
      [],[enable_sc68_debug=no])
    SC68_ENABLE_THIS([enable_sc68_debug])
    if test X"$enable_sc68_debug" = X"yes"; then
      AC_DEFINE([DEBUG],[1],[debug facilities])
      AC_DEFINE([DEBUG_]AS_TR_CPP(AC_PACKAGE_NAME),[1],
 	[debug facilities for ]AC_PACKAGE_NAME)
      [$1]
    else
      enable_sc68_debug=no
      [$2]
    fi

    # --enable-sc68-release (depend on debug)
    AC_ARG_ENABLE(
      [sc68-release],
      [AS_HELP_STRING([--enable-sc68-release],
          [remove debug facilities @<:@default=not-enable-debug@:>@])],
      [],[enable_sc68_release='_'])

    if test X"[$]enable_sc68_release" = X_ ; then
      enable_sc68_release=yes
      if test X"[$]enable_sc68_debug" = Xyes; then enable_sc68_release=no; fi
    fi
    SC68_ENABLE_THIS([enable_sc68_release])
    if test X"[$]enable_sc68_release" = X"yes"; then
      # $$$ NDEBUG is handled by --enable-assert
      #AC_DEFINE([NDEBUG],[1],[suppress all debug facilities])
      AC_DEFINE([NDEBUG_]AS_TR_CPP(AC_PACKAGE_NAME),[1],
 	[suppress all debug facilities for ]AC_PACKAGE_NAME)
      [$3]
    else
      enable_sc68_release=no
      [$4]
    fi

    # HAXXX: try to force assertion depending on debug/release modes
    if test X"[$]{enable_assert+set}" != Xset; then
      if test X"[$]enable_sc68_release" = Xyes; then
        enable_assert=no
      elif test X"[$]enable_sc68_debug" = Xyes; then
        enable_assert=yes
      fi
    fi

  ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
