dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009 Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# Distribued under the term of the GPL3+
dnl#
dnl# $Id$
dnl#

# serial 20090116

# SC68_PACKAGE([DESC])
# --------------------
# Does common inits for all sc68 related packages.
AC_DEFUN([SC68_PACKAGE],[

# ,----------------------------------------------------------------------.
# | Package info                                                         |
# `----------------------------------------------------------------------'

    AC_PACKAGE_INFO(
      [$1 It is part of the sc68 project.],
      [http://sc68.atari.org])
    [PACKAGE_INFOCAT='sc68 - /|\ Atari ST and C= Amiga music player']
    AC_SUBST([PACKAGE_INFOCAT])
    m4_define([PUP],AS_TR_CPP(AC_PACKAGE_NAME))


# ,----------------------------------------------------------------------.
# | Build and host system                                                |
# `----------------------------------------------------------------------'

    AC_USE_SYSTEM_EXTENSIONS
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

    # libtool
    LT_INIT([win32-dll])
    AC_SUBST([LIBTOOL_DEPS])

    # compiler
    AC_LANG([C])
    AC_PROG_CC
    AM_PROG_CC_STDC
    AM_PROG_CC_C_O
    AC_PROG_CC_DECLSPEC
    AC_C_CONST
    AC_C_INLINE
    AC_C_VISIBILITY_ATTRIBUT([has_visibility])

    # others
    AC_PROG_INSTALL

# ,----------------------------------------------------------------------.
# | Maintainer mode                                                      |
# `----------------------------------------------------------------------'

    AM_MAINTAINER_MODE
    AS_IF([test "x${enable_maintainer_mode}" = "xyes"],
      [
        AC_PATH_PROG([texinfo2man],[texinfo2man],[false])
        AS_IF([test "x${texinfo2man}" = "xfalse"],[
            AC_MSG_NOTICE([texinfo2man is part of GNU/indent package])])
        AC_PATH_PROG([help2man],[help2man],[false])
        AS_IF([test "x${help2man}" = "xfalse"],[
            AC_MSG_NOTICE([help2man is part of GNU/help2man package])])
        AC_MSG_CHECKING([for package.texi.in in srcdir])
        AS_IF([test -e "$srcdir/package.texi.in"],
          [AC_MSG_RESULT([yes])],[
            AC_MSG_RESULT([no])
            AC_MSG_NOTICE([create missing file $srcdir/package.texi.in])
cat <<EOF >$srcdir/package.texi.in
@set PACKAGE     @PACKAGE_NAME@
@set WEBSITE     @PACKAGE_URL@
@set BUGREPORT   @PACKAGE_BUGREPORT@
@set DESCRIPTION @PACKAGE_SHORTDESC@
@set INFOCAT     @PACKAGE_INFOCAT@
EOF
          ])
        ],
      [texinfo2man=false; help2man=false])

# ,----------------------------------------------------------------------.
# | Compiler and companions                                              |
# `----------------------------------------------------------------------'

# win32 specific ... $$$ Is this really neccessary ?
# --------------------------------------------
    AS_IF([test "x$ac_platform_win32" = "xyes"],
      [LIB[]PUP[]_LDFLAGS=$(echo $LIB[]PUP[]_LDFLAGS -no-undefined)])

# Check for __dllspec
# -------------------
    AS_IF([test "x$ac_cc_declspec" = "xyes"],
      [AC_DEFINE([HAVE_DECLSPEC],[1],[Support __declspec()])])


# visibility attribut
# -------------------
    AS_IF([test "x$has_visibility" = "xyes"],
      [
        AC_DEFINE([HAVE_VISIBILITY],[1],[Support visibility __attribute__])
        AX_CHECK_COMPILER_FLAGS(
          [-fvisibility=hidden],
          [fvisibility=yes],
          [fvisibility=no])
        AS_IF(
          [test "x$fvisibility" = "xyes"],
          [PUP[]_CFLAGS="$PUP[]_CFLAGS -fvisibility=hidden"])
      ])

# Check some more (mostly GCC) switch
# -----------------------------------
    AC_FOREACH([FLAG],
      [-std=c99 -pedantic -Wall],
      [AX_CHECK_COMPILER_FLAGS(FLAG,
          [PUP[]_CFLAGS=$(echo ${PUP[]_CFLAGS} FLAG)])])


# ,----------------------------------------------------------------------.
# | Optional Features                                                    |
# `----------------------------------------------------------------------'

    AC_ARG_ENABLE_DEBUG([
        AX_CHECK_COMPILER_FLAGS([-g],
          [PUP[]_CFLAGS=$(echo ${PUP[]_CFLAGS} -g)])])

    # $$$ for program only ? 
    AC_ARG_ENABLE_ALLSTATIC(
      [PUP[]_LDFLAGS=$(echo -all-static ${PUP[]_LDFLAGS})])

    # PUP_FLAGS : common for both libraries and programms
    AC_SUBST(PUP[_CFLAGS])
    AC_SUBST(PUP[_CPPFLAGS])
    AC_SUBST(PUP[_LDFLAGS])

    # LIBPUP_FLAGS : for building libraries
    AC_SUBST([LIB]PUP[_CFLAGS])
    AC_SUBST([LIB]PUP[_CPPFLAGS])
    AC_SUBST([LIB]PUP[_LDFLAGS])

    # BINPUP_FLAGS : for building programms
    AC_SUBST([BIN]PUP[_CFLAGS])
    AC_SUBST([BIN]PUP[_CPPFLAGS])
    AC_SUBST([BIN]PUP[_LDFLAGS])

    # PACKAGE_FLAGS : for others to compile/link against us
    AC_SUBST([PACKAGE_CFLAGS])
    AC_SUBST([PACKAGE_CPPFLAGS])
    AC_SUBST([PACKAGE_LDFLAGS])
    
  ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
