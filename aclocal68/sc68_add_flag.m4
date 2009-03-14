yesdnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009 Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# Distribued under the term of the GPL3+
dnl#
dnl# $Id: sc68_package.m4 96 2009-02-15 01:07:39Z benjihan $
dnl#

# serial 20090305

# SC68_ADD_FLAG([VAR],[FLAG])
# ---------------------------
# Add unique FLAG to VAR
AC_DEFUN([SC68_ADD_FLAG],[
    if test X"[$]$1" = X"" ; then
      $1="$2"
    else
      for sc68_add_flag in [$]$1 ; do
        if test X"[$]sc68_add_flag" = X"$2"; then
          true
          break
        fi
        false
      done
      if test [$?] -ne 0; then
        $1="[$]$1 $2"
      fi
    fi
  ])

# SC68_ADD_FLAGS([VAR],[FLAGS])
# -----------------------------
# Add unique FLAGS to VAR
AC_DEFUN([SC68_ADD_FLAGS],[
    set -- $2
    while test [$]# -gt 0; do
      SC68_ADD_FLAG($1,[$]1)
      shift
    done
  ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id: sc68_package.m4 96 2009-02-15 01:07:39Z benjihan $
dnl#
dnl# ----------------------------------------------------------------------
