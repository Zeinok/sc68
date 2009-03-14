dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009 Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# Distribued under the term of the GPL3+
dnl#
dnl# $Id: sc68_package.m4 96 2009-02-15 01:07:39Z benjihan $
dnl#

# serial 20090304

# SC68_OPTIONS([option-list])
# ---------------------------
# Common --enable-options for all sc68 related packages.
AC_DEFUN_ONCE([SC68_OPTIONS],[
    [#]m4_default(m4_strip([$1]),[debug] [all-static])
    SC68_ENABLE_DEBUG
    SC68_ENABLE_ALLSTATIC

    case X[$]enable_debug in
      Xyes);;
      *);;
    esac
    case X[$]enable_release in
      Xyes);;
      *);;
    esac
    case X[$]enable_allstatic in
      Xyes);;
      *);;
    esac
  ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id: sc68_package.m4 96 2009-02-15 01:07:39Z benjihan $
dnl#
dnl# ----------------------------------------------------------------------
