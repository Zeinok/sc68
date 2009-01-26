dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl# 
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for file68
dnl#
dnl# by Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# $Id$
dnl#


# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

AC_INIT([file68],[3.0.0],[benjihan -4t- users.sourcefourge -d0t- net])
AC_CONFIG_SRCDIR([file68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu 1.10 check-news])
AC_CONFIG_HEADERS([config.h])
SC68_PACKAGE([sc68 file library.])
[LIBFILE68_CPPFLAGS="-DFILE68_EXPORT=1"]
[LIBFILE68_LDFLAGS="-version-info 2:0:0"]


# ,----------------------------------------------------------------------.
# | Optional Packages                                                    |
# `----------------------------------------------------------------------'

AC_ARG_WITH(
  [zlib],
  [AS_HELP_STRING([--with-zlib],
      [support zlib @<:@default=check@:>@])],
  [],[with_zlib="check"])

AC_ARG_WITH(
  [curl],
  [AS_HELP_STRING([--with-curl],
      [support curl @<:@default=check@:>@])],
  [],[with_curl="check"])


# ,----------------------------------------------------------------------.
# | Optional Features                                                    |
# `----------------------------------------------------------------------'

AC_ARG_ENABLE(
  [file],
  [AS_HELP_STRING([--enable-file],
      [FILE stream support @<:@default=check@:>@])],
  [],[enable_file="check"])

AC_ARG_ENABLE(
  [fd],
  [AS_HELP_STRING([--enable-fd],
      [file descriptor stream support @<:@default=check@:>@])],
  [],[enable_fd="check"])

AC_ARG_ENABLE(
  [mem],
  [AS_HELP_STRING([--enable-mem],
      [memory stream support @<:@default=yes@:>@])],
  [],[enable_mem="yes"])

AC_ARG_ENABLE(
  [registry],
  [AS_HELP_STRING([--enable-registry],
      [windows registry support @<:@default=check@:>@])],
  [],[enable_reg="check"])


# ,----------------------------------------------------------------------.
# | External libraries                                                   |
# `----------------------------------------------------------------------'

AC_WITH_PACKAGE([unice68])

bundle_libs="curl zlib libao"
AC_ARG_WITH(
  [bundle-libs],
  [AS_HELP_STRING(
      [--with-bundle-libs],
      [compile and link given bundle libs @<:@default=none@:>@])],
  [],[with_bundle_libs="no"])

AS_CASE([${with_bundle_libs}],
  [yes],[with_bundle_libs="${bundle_libs}"],
  [no],[with_bundle_libs=""],
  [with_bundle_libs="${with_bundle_libs//,/ }"])

found_bundle_libs=""
not_found_bundle_libs=""
for xdir in : ${with_bundle_libs} ; do
  case ${xdir} in
    :);;
    curl|zlib|libao)
      AS_IF([test ! -d $srcdir/${xdir}68],
        [
          AC_MSG_WARN([$srcdir/${xdir}68 missing directory])
          not_found_bundle_libs="${xdir} ${not_found_bundle_libs}"
          xdir=":"
        ])
      ;;
    *)
      AC_MSG_ERROR([${xdir} is not a bundle library])
      ;;
  esac
  AS_IF([test "x${xdir}" != "x:"],
    [
      case ${xdir} in
        curl)
          AC_CONFIG_SUBDIRS([curl68])
          ;;
        zlib)
          AC_CONFIG_SUBDIRS([zlib68])
          zlib_version="bundle"
          ;;
        libao)
          AC_CONFIG_SUBDIRS([libao68])
          ;;
      esac
      found_bundle_libs="${xdir} ${found_bundle_libs}"
    ])
done
[
  found_bundle_libs=$(echo $found_bundle_libs)
  not_found_bundle_libs=$(echo $not_found_bundle_libs)
]


# Zlib support
# ------------
[has_zlib="no";zlib_version=""]
AS_IF([test "x${with_zlib}" != "xno"],
  [
    AC_SEARCH_LIBS(
      [gzdopen],[z],
      [AC_CHECK_HEADERS([zlib.h],[has_zlib="yes"])])
  ])
AS_IF([test "${with_zlib}x${has_zlib}" = "yesxno"],
  [AC_MSG_ERROR([z library and/or header file not found])])
AS_IF([test "x${has_zlib}" = "xyes"],
  [AC_DEFINE([USE_ZLIB],[1],[zlib support])])

# XIPH-AO support
# ---------------
AC_ARG_WITH(
  [ao],
  [AS_HELP_STRING([--with-ao],
      [support ao @<:@default=check@:>@])],
  [],[with_ao="check"])
[has_ao="no";ao_version="n/a"]
AS_IF([test "x${with_ao}" != "xno"],
  [
    ao_libs=$(${pkgconfig-false} --libs ao||echo -lao)
    AC_SEARCH_LIBS(
      [ao_initialize],[ao],
      [AC_CHECK_HEADERS([ao/ao.h],
	  [ao_version=$(${pkgconfig-false} --modversion ao||echo n/a)
	    has_ao=yes])],
      [],[${ao_libs}])
  ])
AS_IF([test "${with_ao}x${has_ao}" = "yesxno"],
  [AC_MSG_ERROR([ao library and/or header file not found])])
AS_IF([test "x${has_ao}" = "xyes"],
  [AC_DEFINE([USE_AO],[1],[Xiph libao support])])


# Curl support
# ------------
[has_curl="no";curl_version="n/a"]
AS_IF([test "x${with_curl}" != "xno"],
  [
    AC_PATH_PROG([curlconfig],["curl-config"],["false"])
    curl_libs=$($curlconfig --libs||echo -lcurl)
    AC_SEARCH_LIBS([curl_version],[curl],
      [AC_CHECK_HEADERS([curl/curl.h],
	  [curl_version=$($curlconfig --version||echo n/a);
	    has_curl="yes"])],
      [],
      [${curl_libs}])
  ])
AS_IF([test "${with_curl}x${has_curl}" = "yesxno"],
  [AC_MSG_ERROR([curl library and/or header file not found])])
AS_IF([test "x${has_curl}" = "xyes"],
  [
    AC_DEFINE([USE_CURL],[1],[curl support])
    
	# Use STATIC curl when compiling for win32 native platform
	# (mingw) because it's more convenient and currently it does
	# not link without it. The point is that there is no easy way
	# to determine whether we are linking against curl static or
	# dynamic library and whether it does matter or not (it does
	# on win32 platform).
    
    AS_IF([test "x$ac_native_win32" = "xyes"],
      [AC_DEFINE([CURL_STATICLIB],[1],[use curl static])])
  ]
# !!! HAXX !!! 
  AS_IF([test "x$curlconfig" != "xfalse"],
    [LIBS="$LIBS $($curlconfig --libs)" ])
  )

# Registry68 support
# ------------------
AS_IF([test "x${enable_reg}"  = "xcheck"],
  [AC_CHECK_HEADERS([windows.h],
      [AC_CHECK_HEADERS([winreg.h],
          [enable_reg="yes"],[],
          [#include <windows.h>])])])
AS_IF([test "x${enable_reg}"  = "xyes"],
  [AC_DEFINE([USE_REGISTRY68],[1],[Enable use of windows registry])],
  [enable_reg="no"])

# FILE stream support
#--------------------
AS_IF([test "x${enable_file}"  = "xcheck"],
  [AC_CHECK_HEADERS([stdio.h],[enable_file="yes"],[enable_file="no"])])
AS_IF([test "x${enable_file}"  = "xno"],
  [AC_DEFINE([ISTREAM68_NO_FILE],[1],[Disable FILE* stream support])])

# FD stream support
#------------------
AS_IF([test "x${enable_fd}"  = "xcheck"],
  [AC_CHECK_HEADERS([unistd.h],[enable_fd="yes"],[enable_fd="no"])])
AS_IF([test "x${enable_fd}"  = "xno"],
  [AC_DEFINE([ISTREAM68_NO_FD],[1],[Disable file decriptor stream support])])

# MEM stream support
#-------------------
AS_IF([test "x${enable_mem}"  = "xno"],
  [AC_DEFINE([ISTREAM68_NO_MEM],[1],[Disable memory stream support])])


# ,----------------------------------------------------------------------.
# | Specific flags                                                       |
# `----------------------------------------------------------------------'

AS_IF([test "x$ac_platform_win32" = "xyes"],
  [LIBFILE68_LA_LDFLAGS="-no-undefined $LIBFILE68_LA_LDFLAGS"])
AS_IF([test "x$ac_native_win32" = "xyes"],
  [LIBFILE68_CPPFLAGS="$LIBFILE68_CPPFLAGS -DFILE68_SHARED_PATH=0"],
  [LIBFILE68_CPPFLAGS="$LIBFILE68_CPPFLAGS"' -DFILE68_SHARED_PATH=\"$(sc68datadir)\"'])
sc68datadir='$(datadir)/sc68'
AC_SUBST([sc68datadir])


# ,----------------------------------------------------------------------.
# | Standard functions                                                   |
# `----------------------------------------------------------------------'

AC_CHECK_FUNCS([malloc free sleep usleep vsprintf vsnprintf getenv])



# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_CONFIG_FILES([
    file68-${VERSION}.ebuild:file68.ebuild.in
    file68-config
    file68.spec
    Makefile
    data68/Makefile])

AC_OUTPUT


# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|----------------------])
AC_MSG_NOTICE([|   debug-mode          : $enable_debug])
AC_MSG_NOTICE([|   file-stream         : $enable_file])
AC_MSG_NOTICE([|   fd-stream           : $enable_fd])
AC_MSG_NOTICE([|   memory-stream       : $enable_mem])
AC_MSG_NOTICE([|   windows registry    : $enable_reg])
AC_MSG_NOTICE([|   zlib support        : $has_zlib ($zlib_version)])
AC_MSG_NOTICE([|   curl support        : $has_curl ($curl_version)])
AC_MSG_NOTICE([|   ICE! support        : $has_unice68 ($unice68_ver)])
AC_MSG_NOTICE([|   ao support          : $has_ao ($ao_version)])
AC_MSG_NOTICE([|   sc68 data dir       : $sc68datadir])
AC_MSG_NOTICE([+----------------------])

dnl#
dnl# End of "$Id$".
dnl#
