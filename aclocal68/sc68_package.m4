dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009 Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# Distribued under the term of the GPL3+
dnl#
dnl# $Id$
dnl#

# serial 20100306

# SC68_PACKAGE([DESC])
# --------------------
# Common package info for sc68 related packages.
AC_DEFUN_ONCE([SC68_PACKAGE],[
    AC_PACKAGE_INFO(
      [$1 It is part of the sc68 project.])
    [PACKAGE_INFOCAT='sc68 - /|\ Atari ST and C= Amiga music player']
    AC_SUBST([PACKAGE_INFOCAT])
    m4_define([PKG],AS_TR_CPP(AC_PACKAGE_NAME))
    m4_define([PKG_ALL_CFLAGS],[ALL_CFLAGS])
    m4_define([PKG_LIB_CFLAGS],[LIB_CFLAGS])
    m4_define([PKG_BIN_CFLAGS],[BIN_CFLAGS])
    m4_define([PKG_ALL_EFLAGS],[ALL_EFLAGS])
    m4_define([PKG_LIB_EFLAGS],[LIB_EFLAGS])
    m4_define([PKG_BIN_EFLAGS],[BIN_EFLAGS])
    m4_define([PKG_ALL_LFLAGS],[ALL_LFLAGS])
    m4_define([PKG_LIB_LFLAGS],[LIB_LFLAGS])
    m4_define([PKG_BIN_LFLAGS],[BIN_LFLAGS])
    PKG_ALL_CFLAGS=''; PKG_BIN_CFLAGS=''; PKG_LIB_CFLAGS=''; PACKAGE_CFLAGS=''
    PKG_ALL_EFLAGS=''; PKG_BIN_EFLAGS=''; PKG_LIB_EFLAGS=''; PACKAGE_EFLAGS=''
    PKG_ALL_LFLAGS=''; PKG_BIN_LFLAGS=''; PKG_LIB_LFLAGS=''; PACKAGE_LFLAGS=''
    ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
