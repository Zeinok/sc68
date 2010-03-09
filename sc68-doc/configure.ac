dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for sc68-doc
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id: configure.ac,v 1.3 2007/01/12 09:43:07 benjihan Exp $
dnl#


# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT(
  [sc68-doc],
  [20090110],
  [SC68_EMAIL],,
  [SC68_URL])
AC_CONFIG_SRCDIR([sc68_doc.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])
AM_MAINTAINER_MODE

# ,----------------------------------------------------------------------.
# | Package                                                              |
# `----------------------------------------------------------------------'

AC_PACKAGE_INFO(
  [doc68 is sc68 documentation project. It is part of the sc68 project.],
  [http://sc68.atari.org])

# ,----------------------------------------------------------------------.
# | Maintainer mode                                                      |
# `----------------------------------------------------------------------'

AS_IF([test "x${enable_maintainer_mode}" = "xyes"],
  [],
  [])

AC_PROG_MAKE_SET
AC_PATH_PROG([doxygen],[doxygen],[false])
AS_IF([test "x[$]doxygen" = xfalse],
  [AC_MSG_ERROR([doxygen is required. Visit http://www.doxygen.org])])

# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

INPUTS='$(srcdir)'
m4_foreach_w([dir],[
    sc68-doc/sc68_doc.h as68/as68_doc.h info68/info68_doc.h
    sourcer68/sourcer68_doc.h debug68/debug68_doc.h sc68/sc68_doc.h 
    desa68 unice68 file68 libsc68],[
    AC_MSG_CHECKING([whether dir exists])
    AS_IF([test -e "${srcdir}/../dir"],
      [INPUTS="$INPUTS"' $(srcdir)/../dir'
        AC_MSG_RESULT([yes])],
      [AC_MSG_RESULT([no])])])

AC_SUBST([INPUTS])
AC_CONFIG_FILES([
    doxygen.cfg
    sc68-doc.spec
    sc68-doc-${VERSION}.ebuild:sc68-doc.ebuild.in
    Makefile])
AC_OUTPUT

# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'
  
AC_MSG_NOTICE([])
AC_MSG_NOTICE([,-----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([+-----------------------])
AC_MSG_NOTICE([])

dnl#
dnl# End of "$Id: configure.ac,v 1.2 2006/12/25 22:36:49 benjihan Exp $".
dnl#
