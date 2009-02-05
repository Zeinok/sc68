dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for libsc68
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id$
dnl#


# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT([libsc68],[3.0.0],[benjihan -4t- users.sourcefourge -d0t- net])
AC_CONFIG_SRCDIR([api68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])
AC_CONFIG_HEADERS([config.h])
SC68_PACKAGE([sc68 engine library.])
[LIBSC68_CPPFLAGS="-DSC68_EXPORT=1"]
[LIBSC68_LDFLAGS="-version-info 3:0:0"]

# ,----------------------------------------------------------------------.
# | Optional features                                                    |
# `----------------------------------------------------------------------'

AC_ARG_WITH(
  [ym-engine],
  [AS_HELP_STRING([--with-ym-engine],
      [set default YM-2149 engine (orig or blep) @<:@default=auto-select@:>@])],
  [],[with_ym_engine='x'])

AS_CASE([x-${with_ym_engine}],
  [x-blep],[YM_EMUL='YM_EMUL_BLEP';with_ym_engine='BLEP synthesis'],
  [x-orig],[YM_EMUL='YM_EMUL_ORIG';with_ym_engine='sc68 original'],
  [x-x | x-yes | x-no],[YM_EMUL='x'],[
    YM_EMUL="YM_EMUL_"[]AS_TR_CPP($with_ym_engine)
    with_ym_engine="unknown (3rd party?) $with_ym_engine engine"
    AC_MSG_WARN([Using unknown YM engine may break compilation.])])

AS_IF([test "x-$YM_EMUL" != "x-x"],
  [AC_DEFINE_UNQUOTED([YM_EMUL],[$YM_EMUL],[set default YM engine])],
  [with_ym_engine='auto-select (should be sc68 original)'])

# ,----------------------------------------------------------------------.
# | Sized size integers                                                  |
# `----------------------------------------------------------------------'

AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INTMAX_T
AC_TYPE_LONG_LONG_INT
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINTMAX_T
AC_TYPE_UNSIGNED_LONG_LONG_INT


# ,----------------------------------------------------------------------.
# | Standard functions                                                   |
# `----------------------------------------------------------------------'

AC_CHECK_FUNCS([malloc free vsprintf vsnprintf getenv strtol])


# ,----------------------------------------------------------------------.
# | Required  libraries                                                  |
# `----------------------------------------------------------------------'

AC_WITH_PACKAGE([file68],[yes],[],[sc68/file68.h])


# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'
AC_CONFIG_FILES([
    Makefile
    emu68/Makefile
    io68/Makefile
    libsc68-${VERSION}.ebuild:libsc68.ebuild.in
    libsc68.spec
    sc68-config
  ])
AC_OUTPUT

# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,-----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|-----------------------])
AC_MSG_NOTICE([|   debug-mode          : $enable_debug])
AC_MSG_NOTICE([|   release-mode        : $enable_release])
AC_MSG_NOTICE([|   all-static          : $enable_all_static])
AC_MSG_NOTICE([|   file68              : $has_file68 ($file68_ver)])
AC_MSG_NOTICE([|   default YM engine   : $with_ym_engine])
AC_MSG_NOTICE([+-----------------------])

dnl#
dnl# End of "$Id$".
dnl#
