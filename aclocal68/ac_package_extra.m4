dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2007 Benjamin Gerard <benjihan [at] users.sourceforge [dot] net>
dnl#
dnl# Distribued under the term of the GPL
dnl#
dnl# $Id$
dnl#

# serial 20090114

# AC_PACKAGE_INFO([DESCRIPTION],[URL])
# ------------------------------------
# Defines supplemental package info strings
AC_DEFUN_ONCE([AC_PACKAGE_INFO],[
    m4_ifval([$1],
      [
        m4_define([AC_PACKAGE_DESC],
	  [m4_normalize([$1]m4_if([$2],[],[],[ Visit <$2>]))])
        AC_DEFINE_UNQUOTED([PACKAGE_DESC],["AC_PACKAGE_DESC"],
 	  [Package short description])
        AC_SUBST([PACKAGE_DESC],["AC_PACKAGE_DESC"])
	m4_define([AC_PACKAGE_SHORTDESC],
	  m4_bpatsubst(AC_PACKAGE_DESC,[\..*]))

	AC_SUBST([PACKAGE_SHORTDESC],["AC_PACKAGE_SHORTDESC"])
      ])
    m4_ifval([$2],
      [
        m4_define([AC_PACKAGE_URL],[m4_normalize([$2])])
        AC_DEFINE_UNQUOTED([PACKAGE_URL],["AC_PACKAGE_URL"],[Website URL])
        AC_SUBST([PACKAGE_URL],["AC_PACKAGE_URL"])
      ])
    m4_ifval([AC_PACKAGE_VERSION],
      [
        m4_define([AC_PACKAGE_VERNUM],patsubst(AC_PACKAGE_VERSION,[[.]],[]))
        AC_DEFINE_UNQUOTED([PACKAGE_VERNUM],[AC_PACKAGE_VERNUM],
          [Version number])
        AC_SUBST([PACKAGE_VERNUM],[AC_PACKAGE_VERNUM])
      ])
  ])

dnl# ########################################################################
dnl#
dnl# End Of $Id$
dnl#
dnl# ########################################################################
