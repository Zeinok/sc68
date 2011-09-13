dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for desa68
dnl#
dnl# by Benjamin Gerard <http://sourceforge.net/users/benjihan>
dnl#

# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT(
  [desa68],
  [20110909],
  [benjihan -4t- sourceforge],
  [libdesa68],
  [http://sc68.atari.org])
AC_CONFIG_SRCDIR([desa68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign 1.10 silent-rules dist-bzip2])

AC_USE_SYSTEM_EXTENSIONS
LT_INIT([win32-dll])

# ,----------------------------------------------------------------------.
# | sc68                                                                 |
# `----------------------------------------------------------------------'

SC68_PACKAGE([Motoral MC68000 disassembler library.])
SC68_SYSTEM
SC68_OPTIONS
SC68_TOOLS
SC68_CHECKS

AX_CHECK_COMPILER_FLAGS([-Wno-multichar],
  SC68_ADD_FLAG(ALL_CFLAGS,[-Wno-multichar]))

PAC_CFLAGS="-I[$]includedir"
PAC_LFLAGS="-L[$]libdir -ldesa68"

# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_CONFIG_FILES([
    libdesa68.spec
    libdesa68-${VERSION}.ebuild:libdesa68.ebuild.in
    desa68.pc
    Makefile])
AC_OUTPUT

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,-----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|-----------------------])
AC_MSG_NOTICE([|   debug-mode          : $enable_sc68_debug])
AC_MSG_NOTICE([|   release-mode        : $enable_sc68_release])
AC_MSG_NOTICE([|   all-static-mode     : $enable_sc68_static])
AC_MSG_NOTICE([+-----------------------])
AC_MSG_NOTICE([])

dnl#
dnl# End of desa68/configure.ac
dnl#
