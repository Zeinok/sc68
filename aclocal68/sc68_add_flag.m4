yesdnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009 Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# Distribued under the term of the GPL3+
dnl#
dnl# $Id$
dnl#

# serial 20090305

# SC68_ADD_FLAG([VAR],[FLAG])
# ---------------------------
# Add unique FLAG to VAR
AC_DEFUN([SC68_ADD_FLAG],[
    if test X"[$]$1" = X ; then
      $1="$2"
    else
      for sc68_add_flag in [$]$1 ; do
        test X"[$]sc68_add_flag" = X"$2" && break
      done
      if test [$]? -ne 0; then
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
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
