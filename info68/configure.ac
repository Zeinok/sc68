dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for info68
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id$
dnl#


# ,----------------------------------------------------------------------.
# | Autotools                                                            |
# `----------------------------------------------------------------------'

AC_INIT([info68],[20090117],[benjihan -4t- users.sourceforge -d0t- net])
AC_CONFIG_SRCDIR([info68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])
AM_MAINTAINER_MODE


# ,----------------------------------------------------------------------.
# | Package                                                              |
# `----------------------------------------------------------------------'

AC_PACKAGE_INFO(
    [Utility to get tag info from sc68 music files. This is part of the sc68 project.],
    [http://sc68.atari.org])
[
INFO68_LA_LDFLAGS=""
INFO68_LDFLAGS=""
INFO68_CFLAGS=""
INFO68_CPPFLAGS=""
]

#[PACKAGE_SHORTDESC=$(echo $PACKAGE_DESC|sed 's/\(^[^.]*\).*/\1/')]
[PACKAGE_INFOCAT="sc68 - /|\ Atari ST and C= Amiga music player"]
AC_SUBST([PACKAGE_INFOCAT])

# ,----------------------------------------------------------------------.
# | Build and host system                                                |
# `----------------------------------------------------------------------'

AC_CANONICAL_BUILD
AC_CANONICAL_HOST


# ,----------------------------------------------------------------------.
# | Guess win32 platform                                                 |
# `----------------------------------------------------------------------'

AC_SYS_PLATFORM_WIN32
AC_SYS_NATIVE_WIN32


# ,----------------------------------------------------------------------.
# | Compiler and companions                                              |
# `----------------------------------------------------------------------'

AC_PROG_MAKE_SET
AC_PROG_CC
AM_PROG_CC_STDC
AC_PROG_INSTALL
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_LANG([C])


# ,----------------------------------------------------------------------.
# | External libraries                                                   |
# `----------------------------------------------------------------------'

AC_WITH_PACKAGE([file68],[yes],[file68_version],[sc68/file68.h])

# case "x${with_file68}" in
#     xno|xbundle) actions="${with_file68}";;
#     *)           actions="bundle check"	 ;;
# esac
# m4_define([FILE68_DIR],[../file68])
# file68_req='yes'; file68_version="n/a"; file68_libs=""; has_file68="no"
# file68_dir="FILE68_DIR"
# for action in ${actions}; do
#     case ${action} in
#         no);;
#         finish) break;;
#         bundle)
#             res="not found in ${file68_dir}"
#             AC_MSG_CHECKING([for bundle file68])
#             if test -d "${srcdir}/${file68_dir}"; then
# 	        AC_CONFIG_SUBDIRS(FILE68_DIR)
# 	        file68_version="bundle"; has_file68="yes";
# 	        res="found in ${file68_dir}"
#                 INFO68_CPPFLAGS="${INFO68_CPPFLAGS} -I${file68_dir}"
# 	    fi
#             AC_MSG_RESULT([${res}])
# 	    ;;
#         check)
# 	    AC_PATH_PROG([file68],["file68-config"],["false"])
# 	    file68_version=$(${file68} --version||echo installed)
# 	    file68_libs=$(${file68} --libs)
# 	    AC_SEARCH_LIBS(
# 		[file68_versionstr],[file68],
# 		[AC_CHECK_HEADERS([file68/file68.h],
# 		        [has_file68="yes"])],
#                 [],[${file68_libs}])
# 	    ;;
#     esac
#     test "${action}" = "done" && break
# done
# AS_IF([test "x${file68_req}" = "xyes"],[with_file68='yes'])
# AS_IF([test "${with_file68}x${has_file68}" = "yesxno"],
#     [AC_MSG_ERROR([Unable to configure requested feature: file68])])
# AM_CONDITIONAL([BUNDLE_FILE68],[test "x$file68_version" = "xbundle"])


# ,----------------------------------------------------------------------.
# | Optionnal Features                                                   |
# `----------------------------------------------------------------------'

AC_ARG_ENABLE_DEBUG(
  [AX_CHECK_COMPILER_FLAGS([-g],[INFO68_CFLAGS=$(echo $INFO68_CFLAGS -g)])])

AC_ARG_ENABLE_ALLSTATIC(
  [INFO68_LA_LDFLAGS=$(echo ${INFO68_LA_LDFLAGS} -all-static)])


# Check some more (mostly GCC) switch
# -----------------------------------
AC_FOREACH([FLAG],
  [-std=c99 -pedantic -Wall],
  [AX_CHECK_COMPILER_FLAGS(FLAG,
      [INFO68_CFLAGS=$]([echo $INFO68_CFLAGS ]FLAG))])


# ,----------------------------------------------------------------------.
# | Maintainer mode                                                      |
# `----------------------------------------------------------------------'

AS_IF([test "x${enable_maintainer_mode}" = "xyes"],[
    AC_PATH_PROG([texinfo2man],[texinfo2man],[false])
    AC_PATH_PROG([help2man],[help2man],[false])],
  [texinfo2man=false; help2man=false])


# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_SUBST([INFO68_CFLAGS])
AC_SUBST([INFO68_CPPFLAGS])
AC_SUBST([INFO68_LA_LDFLAGS])
AC_SUBST([INFO68_LDFLAGS])

AC_CONFIG_FILES([
    info68.spec
    info68-${VERSION}.ebuild:info68.ebuild.in
    info68.1.tpl
    ${srcdir}/package.texi:package.texi.in
    Makefile])
AC_OUTPUT


# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,----------------------])
AC_MSG_NOTICE([| Configuration summary])
AC_MSG_NOTICE([|----------------------])
AC_MSG_NOTICE([|   package             : $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|   debug-mode          : $enable_debug])
AC_MSG_NOTICE([|   all-static-mode     : $enable_all_static])
AC_MSG_NOTICE([|   file68              : $has_file68 ($file68_ver)])
AC_MSG_NOTICE([+----------------------])
AC_MSG_NOTICE([])

dnl#
dnl# End of "$Id$".
dnl#
