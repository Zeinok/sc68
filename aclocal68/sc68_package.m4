dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009 Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# Distribued under the term of the GPL3+
dnl#
dnl# $Id$

# serial 20110824

# SC68_PACKAGE([DESC])
# --------------------
# Common package info for sc68 related packages.
AC_DEFUN_ONCE([SC68_PACKAGE],[
    AC_PACKAGE_INFO(
      [$1 It is part of the sc68 project.])
    [PACKAGE_INFOCAT='sc68 - /|\ Atari ST and C= Amiga music player']
    AC_SUBST([PACKAGE_INFOCAT])
    m4_define([PKG],AS_TR_CPP(AC_PACKAGE_NAME))
    m4_define([PAC_ALL_CFLAGS],[ALL_CFLAGS])
    m4_define([PAC_LIB_CFLAGS],[LIB_CFLAGS])
    m4_define([PAC_BIN_CFLAGS],[BIN_CFLAGS])
    m4_define([PAC_ALL_EFLAGS],[ALL_EFLAGS])
    m4_define([PAC_LIB_EFLAGS],[LIB_EFLAGS])
    m4_define([PAC_BIN_EFLAGS],[BIN_EFLAGS])
    m4_define([PAC_ALL_LFLAGS],[ALL_LFLAGS])
    m4_define([PAC_LIB_LFLAGS],[LIB_LFLAGS])
    m4_define([PAC_BIN_LFLAGS],[BIN_LFLAGS])
    PAC_ALL_CFLAGS=''; PAC_BIN_CFLAGS=''; PAC_LIB_CFLAGS=''; PACKAGE_CFLAGS=''
    PAC_ALL_EFLAGS=''; PAC_BIN_EFLAGS=''; PAC_LIB_EFLAGS=''; PACKAGE_EFLAGS=''
    PAC_ALL_LFLAGS=''; PAC_BIN_LFLAGS=''; PAC_LIB_LFLAGS=''; PACKAGE_LFLAGS=''
    ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
