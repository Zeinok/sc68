dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# Time-stamp: <2011-09-08 13:16:55 ben>
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009-2011 Benjamin Gerard
dnl#
dnl# Distribued under the term of the GPL3+

# serial 20110908 cc_declspec.m4

# AC_PROG_CC_DECLSPEC()
# ------------------------------
AC_DEFUN_ONCE([AC_PROG_CC_DECLSPEC],
[
AC_MSG_CHECKING([for cc __declspec()])
AC_COMPILE_IFELSE(
[
__declspec(dllexport) void f_exported (void);
void f_exported (void) {}
__declspec(dllimport) void f_imported (void);
int main (int argc, char **argv)
{f_exported();f_imported();return 0;}
],
[ac_cc_declspec="yes"],
[ac_cc_declspec="no"]
)
AC_MSG_RESULT($ac_cc_declspec)
])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of cc_declspec.m4
dnl#
dnl# ----------------------------------------------------------------------
