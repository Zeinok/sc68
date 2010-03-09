dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2010 Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# Distribued under the term of the GPL3+
dnl#
dnl# $Id$
dnl#

# serial 20100906

# SC68_URL([PACKAGE])
# --------------------
# Expand as package website url
AC_DEFUN([SC68_URL],
  [http://sc68.atari.org/][ifdef([AC_PACKAGE_NAME],AC_PACKAGE_NAME)])

# SC68_EMAIL()
# ------------
# Expand as contact email 
AC_DEFUN([SC68_EMAIL],[benjihan -4t- users.sourceforge -d0t- net])
