dnl# -*- m4 -*-
dnl#
dnl# sc68 - autoconf macros
dnl#
dnl# by Benjamin Gerard <ben [at] sashipa [dot] com>
dnl#
dnl# $Id: configure.ac,v 1.3 2007/01/12 09:43:07 benjihan Exp $
dnl#

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

dnl# ########################################################################
dnl#
dnl# End Of $Id: configure.ac,v 1.3 2007/01/12 09:43:07 benjihan Exp $
dnl#
dnl# ########################################################################
