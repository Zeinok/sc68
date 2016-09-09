dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl# 
dnl# the "configure" script is made from this by running GNU "autoconf"
dnl#
dnl# Configuration script for file68
dnl#
dnl# by Benjamin Gerard <http://sourceforge.net/users/benjihan>
dnl#

# ,----------------------------------------------------------------------.
# | Autotools and Package                                                |
# `----------------------------------------------------------------------'

define([sc_version], esyscmd([sh vcversion.sh 3.0.0]))dnl

AC_INIT(
  [file68],
  sc_version,
  [http://sourceforge.net/projects/sc68/],
  [libfile68],
  [http://sc68.atari.org])
AC_CONFIG_SRCDIR([src/file68.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign 1.11 silent-rules dist-xz subdir-objects
                  no-installman no-installinfo])
AC_CONFIG_HEADERS([config.h sc68/file68_features.h])
AC_USE_SYSTEM_EXTENSIONS
LT_INIT([win32-dll])

# ,----------------------------------------------------------------------.
# | sc68                                                                 |
# `----------------------------------------------------------------------'

AC_SUBST([PACKAGE_SHORTDESC],["sc68 file and utility library"])
SC68_CC
AC_PROG_INSTALL

# ,----------------------------------------------------------------------.
# | Optional Features                                                    |
# `----------------------------------------------------------------------'

AC_ARG_ENABLE(
  [file68-data],
  [AS_HELP_STRING([--enable-file68-data],
      [file68 shared data to be installed @<:@default=check@:>@])],
  [],[enable_file68_data=check])

AC_ARG_ENABLE(
  [file],
  [AS_HELP_STRING([--enable-file],
      [FILE stream support @<:@default=check@:>@])],
  [],[enable_file=check])

AC_ARG_ENABLE(
  [fd],
  [AS_HELP_STRING([--enable-fd],
      [file descriptor stream support @<:@default=check@:>@])],
  [],[enable_fd=check])

AC_ARG_ENABLE(
  [mem],
  [AS_HELP_STRING([--enable-mem],
      [memory stream support @<:@default=yes@:>@])],
  [],[enable_mem=yes])

AC_ARG_ENABLE(
  [registry],
  [AS_HELP_STRING([--enable-registry],
      [windows registry support @<:@default=check@:>@])],
  [],[enable_reg=check])

AC_ARG_ENABLE(
  [replay-rom],
  [AS_HELP_STRING([--enable-replay-rom],
      [add built-in replay (~130kB) @<:@default=no@:>@])],
  [],[enable_replay_rom=no])

# ,----------------------------------------------------------------------.
# | External libraries                                                   |
# `----------------------------------------------------------------------'

unset PAC_REQUIRES

# ------------------------------------------------------------------------
# unice68

has_unice68=no; unice68_ver=n/a
AC_ARG_WITH(
  [unice68],
  [AS_HELP_STRING(
      [--with-unice68],
      [Select unice68 @<:@check*,source,pkgconfig,system@:>@])],
  [],[with_unice68=check])

AS_IF(
  [test "x$with_unice68" = "xcheck" || test "x$with_unice68" = "xsource"],
  [AC_MSG_CHECKING([unice68 build dir])
   AS_IF([test x"$unice68_dir" = x],[unice68_dir="$builddir/../unice68"])
   AS_IF(
     [test -e "$unice68_dir/config.h"],
     [unice68_ver=`sed -ne 's/^#define PACKAGE_STRING "unice68 \(@<:@^"@:>@\+\).*/\1/p' "$unice68_dir/config.h"`
      AS_IF([test "x$unice68_ver" = x],[unice68_ver=n/a])
     ])
   AS_IF([test "x$unice68_ver" = xn/a],
         [
           AC_MSG_RESULT([no])
           unice68_ver=n/a
         ],
         [
           AC_MSG_RESULT([$unice68_ver])
           has_unice68=source
           with_unice68=$has_unice68
           PAC_REQUIRES="${PAC_REQUIRES-}${PAC_REQUIRES+,}unice68"
         ])
  ])

AS_IF(
  [test "x$with_unice68" = "xcheck" || test "x$with_unice68" = "xpkgconfig"],
  [PKG_CHECK_MODULES(
      [unice68],[unice68],
      [has_unice68=pkgconfig; with_unice68=[$]has_unice68
       unice68_ver=`$PKG_CONFIG --modversion unice68 2>/dev/null || echo n/a`
       PAC_REQUIRES="${PAC_REQUIRES-}${PAC_REQUIRES+,}unice68"
      ],
      [AC_MSG_NOTICE([unice68: $unice68_PKG_ERRORS])]
    )
  ])

AS_CASE(
  ["x$with_unice68"],
  [xyes],[],
  [xno|xcheck],[unice68_LIBS=''; unice68_CFLAGS=''],
  [AS_IF([test "x$has_unice68" != "x$with_unice68"],
         [AC_MSG_ERROR([could not satisfy --with-unice68 option.])])])

AS_IF(
  [test "X$has_unice68" != Xno],
  [save_unice68_CPPFLAGS="$CPPFLAGS"; set -- ${unice68_CFLAGS}
   while test $# -ne 0; do
     AS_CASE([$1],
             [-I|-D|-U],[CPPFLAGS="$CPPFLAGS $1$2"; shift 2],
             [-I*|-D*|-U*],[CPPFLAGS="$CPPFLAGS $1"; shift],
             [shift])
   done
   AC_MSG_NOTICE([CPPFLAGS: $CPPFLAGS])
   AC_CHECK_HEADER([unice68.h],,[AC_MSG_WARN([could not include unice.h])])
   CPPFLAGS="$save_unice68_CPPFLAGS"; unset save_unice68_CPPFLAGS])

AM_CONDITIONAL([SOURCE_UNICE68],[test "X$has_unice68" = Xsource])

# ------------------------------------------------------------------------
# zlib

PKG_CHECK_MODULES(
  [zlib],[zlib],
  [
    has_z=pkgconfig
    AC_MSG_CHECKING([for zlib version])
    z_ver=`$PKG_CONFIG --modversion zlib 2>/dev/null || echo n/a`
    AC_MSG_RESULT([$z_ver])
    PAC_REQUIRES="${PAC_REQUIRES-}${PAC_REQUIRES+,}zlib"
  ],
  [
    has_z=no
    z_ver=n/a
    AC_MSG_NOTICE([zlib: $zlib_PKG_ERRORS])
  ])

AS_IF(
  [test "X$has_z" != Xno],
  [save_zlib_LIBS="$LIBS"; LIBS="$LIBS $zlib_LIBS"
   save_zlib_CFLAGS="$CFLAGS"; CFLAGS="$CFLAGS $zlib_CFLAGS"
   AC_CHECK_FUNC([gzopen],,[AC_MSG_WARN([unable to link against zlib])])
   AC_CHECK_HEADER([zlib.h],,[AC_MSG_WARN([could not include zlib.h])])
   LIBS="$save_zlib_LIBS"; unset save_zlib_LIBS
   CFLAGS="$save_zlib_CFLAGS"; unset save_zlib_CFLAGS
  ])

# ------------------------------------------------------------------------
# libao

has_ao=no; ao_ver=n/a
PKG_CHECK_MODULES(
  [ao],[ao],
  [
    has_ao=pkgconfig
    ao_ver=`$PKG_CONFIG --modversion ao 2>/dev/null || echo n/a`
    PAC_REQUIRES="${PAC_REQUIRES-}${PAC_REQUIRES+,}ao"
  ],
  [
    has_ao=no
    AC_MSG_NOTICE([ao: $ao_PKG_ERRORS])
  ])

AS_IF(
  [test "X$has_ao" != Xno],
  [save_ao_LIBS="$LIBS"; LIBS="$LIBS $ao_LIBS"
   save_ao_CFLAGS="$CFLAGS"; CFLAGS="$CFLAGS $ao_CFLAGS"
   AC_CHECK_HEADER([ao/ao.h])
   AC_CHECK_FUNCS([ao_initialize ao_file_extension])
   LIBS="$save_ao_LIBS"; unset save_ao_LIBS
   CFLAGS="$save_ao_CFLAGS"; unset save_ao_CFLAGS
  ])

# ------------------------------------------------------------------------
# cURL

has_curl=no; curl_ver=n/a
PKG_CHECK_MODULES(
  [curl],[libcurl],
  [
    has_curl=pkgconfig
    curl_ver=`$PKG_CONFIG --modversion libcurl 2>/dev/null || echo n/a`
    PAC_REQUIRES="${PAC_REQUIRES-}${PAC_REQUIRES+,}libcurl"
  ],
  [
    has_curl=no
    AC_MSG_NOTICE([curl: $curl_PKG_ERRORS])
  ])

AS_IF(
  [test "X$has_curl" != Xno],
  [save_curl_LIBS="$LIBS"; LIBS="$LIBS $curl_LIBS"
   save_curl_CFLAGS="$CFLAGS"; CFLAGS="$CFLAGS $curl_CFLAGS"
   AC_CHECK_HEADER([curl/curl.h])
   AC_CHECK_FUNC([curl_global_init])
   LIBS="$save_curl_LIBS"; unset save_curl_LIBS
   CFLAGS="$save_curl_CFLAGS"; unset save_curl_CFLAGS
  ])

AS_IF(
  [test X${curl_SCHEMES+set} != Xset],
  [
    curl_relevant_schemes='FILE HTTP HTTPS FTP FTPS HTTP SFTP SMB SMBS'
    AS_CASE(
      ["x/$has_curl"],
      [x/no],[has_curl=no],
      [x/pkgconfig],
      [
        PKG_CHECK_VAR(
          [curl_supported_schemes],[libcurl],[supported_protocols],
          [set -- $curl_supported_schemes
           while test $# -ne 0; do
               for curl_scheme in $curl_relevant_schemes; do
                 if test "X$curl_scheme" = "X$1"; then
                     curl_SCHEMES="${curl_SCHEMES-}${curl_SCHEMES+ }$1"
                     break
                 fi
               done
               shift
           done],
          [curl_SCHEMES="$curl_relevant_schemes"])
      ],
      [curl_SCHEMES="$curl_relevant_schemes"]
    )
  ])

AS_IF(
  [test "x$has_curl" = xno],
  [unset curl_SCHEMES],
  [
    AC_DEFINE_UNQUOTED(
      [CURL_SCHEMES],
      [`echo "$curl_SCHEMES"|sed -e 's/ \+/,/g; s/\(\w\+\)/"\1"/g'`],
      [Supported curl schemes])
   ])

# Registry68 support
# ------------------
#
# Don't test for a specific function. Should not be too much of a problem.
#
AS_IF([test "X$enable_reg" = Xcheck],
      [AS_CASE(["X$host_os"],[Xmingw*],
               [AC_CHECK_HEADERS([windows.h])
                AC_CHECK_HEADERS(
                  [winreg.h],[enable_reg=yes],[],
                  [#ifdef HAVE_WINDOWS_H]
                  [#include <windows.h>]
                  [#endif])
               ])
      ])
                
AS_IF([test "X$enable_reg" = Xyes],
      [AC_DEFINE([USE_REGISTRY68],[1],[Using Windows registry])],
      [enable_reg=no])
AC_MSG_CHECKING([for Windows registry])
AC_MSG_RESULT([$enable_reg])

# FILE stream support
#--------------------
AS_IF([test "X$enable_file" = Xcheck],
      [AC_CHECK_HEADERS([stdio.h],[enable_file=yes],[enable_file=no])])
AS_IF([test "X$enable_file" = Xno],
      [AC_DEFINE([ISTREAM68_NO_FILE],[1],[Disable FILE stream support])])

# FD stream support
#------------------
AS_IF([test "X$enable_fd" = Xcheck],
      [AC_CHECK_HEADERS([unistd.h],[enable_fd=yes],[enable_fd=no])])
AS_IF([test "X$enable_fd" = Xno],
      [AC_DEFINE([ISTREAM68_NO_FD],[1],
                 [Disable file decriptor stream support])])

# MEM stream support
#-------------------
AS_IF([test "X$enable_mem" = Xno],
      [AC_DEFINE([ISTREAM68_NO_MEM],[1],[Disable memory stream support])])

# DATA for file68
#----------------
sc68datadir='${datadir}/sc68'
AC_SUBST([sc68datadir])

AC_MSG_CHECKING([whether file68 data will be installed])
AS_CASE(["X$enable_file68_data/$host_os"],
        [Xno/* | Xcheck/mingw*],
        [enable_file68_data=no
         AC_MSG_RESULT([$enable_file68_data])],
        [enable_file68_data=yes
         AC_MSG_RESULT([$enable_file68_data @<:@$sc68datadir@:>@])])
AM_CONDITIONAL([FILE68_DATA],[test "X$enable_file68_data" = Xyes])

dnl # This can not be in config.h because we need the variable
dnl # substitutions to happen at compile time.

AS_CASE(["X$host_os"],
        [Xmingw*],[file68_datadir=0],
        [file68_datadir='\"$(datadir)/sc68\"'])
CPPFLAGS="-DFILE68_SHARED_PATH=${file68_datadir}${CPPFLAGS:+ }${CPPFLAGS}"

# Built-in replay rom
#--------------------
AM_CONDITIONAL([REPLAY68],[test "X$enable_replay_rom" = Xyes])
AC_MSG_CHECKING([whether external replays are built-in])
AS_IF([test "X$enable_replay_rom" = Xyes],
      [AC_DEFINE([USE_REPLAY68],[1],[Built-in replays])],
      [enable_replay_rom=no])
AC_MSG_RESULT([$enable_replay_rom])

# ,----------------------------------------------------------------------.
# | Standard functions                                                   |
# `----------------------------------------------------------------------'

AC_CHECK_FUNCS([malloc free getenv])
AC_CHECK_FUNCS([sleep usleep])
AC_CHECK_FUNCS([vsprintf vsnprintf])
AC_CHECK_FUNCS([fsync fdatasync])

# ,----------------------------------------------------------------------.
# | File68 features
# `----------------------------------------------------------------------'

unset file68_features
AS_IF(
  [test "X$has_curl" != Xno],
  [file68_features="${file68_features-}${file68_features+ }curl"
   AC_DEFINE([FILE68_CURL],1,
             [file68 supports remote files (via libcurl)])
  ])

AS_IF(
  [test "X$has_ao" != Xno],
  [file68_features="${file68_features-}${file68_features+ }ao"
   AC_DEFINE([FILE68_AO],1,[file68 supports audio (via libao)])
  ])

AS_IF(
  [test "X$has_z" != Xno],
  [file68_features="${file68_features-}${file68_features+ }z"
   AC_DEFINE([FILE68_Z],[1],[file68 supports inflale/deflate (via zlib)])
  ])

AS_IF(
  [test "X$has_unice68" != Xno],
  [file68_features="${file68_features-}${file68_features+ }ice"
   AC_DEFINE([FILE68_UNICE68],[1],[file68 supports !ICE packer (via unice68)])
  ])

AC_SUBST([file68_features])

# ,----------------------------------------------------------------------.
# | Output                                                               |
# `----------------------------------------------------------------------'

AC_SUBST([PAC_REQUIRES])
AC_SUBST(lib_LDFLAGS)
AC_ARG_VAR([unice68_dir],
           [Directory for unice68 build.])
AC_ARG_VAR([lib_LDFLAGS],
           [Supplemental LDFLAGS for building the library.
            Typically libtool flags such as -no-undefined.])
AC_ARG_VAR([curl_SCHEMES],
           [List of space separated relevant schemes (protocols)
            supported by the cURL library.])


AC_CONFIG_FILES(
  [file68.pc
   Makefile
   data68/Makefile
  ])

AC_OUTPUT

# ,----------------------------------------------------------------------.
# | Summary                                                              |
# `----------------------------------------------------------------------'

unset vfs
AS_IF([test "X$enable_file" = Xyes],[vfs="${vfs-}${vfs+,}file"])
AS_IF([test "X$enable_fd"   = Xyes],[vfs="${vfs-}${vfs+,}fd"])
AS_IF([test "X$enable_mem"  = Xyes],[vfs="${vfs-}${vfs+,}memory"])

AC_MSG_NOTICE([])
AC_MSG_NOTICE([,----------------------])
AC_MSG_NOTICE([| $PACKAGE_NAME ($PACKAGE_VERSION)])
AC_MSG_NOTICE([|----------------------])
AC_MSG_NOTICE([|   supported VFS       : ${vfs-none}])
AC_MSG_NOTICE([|   windows registry    : $enable_reg])
AC_MSG_NOTICE([|   built-in replays    : $enable_replay_rom])
AC_MSG_NOTICE([|   zlib support        : $has_z ($z_ver)])
AC_MSG_NOTICE([|   curl support        : $has_curl ($curl_ver)])
AC_MSG_NOTICE([|   curl schemes        : ${curl_SCHEMES-n/a}])
AC_MSG_NOTICE([|   ICE! support        : $has_unice68 ($unice68_ver)])
AC_MSG_NOTICE([|   ao support          : $has_ao ($ao_ver)])
AC_MSG_NOTICE([|   sc68 data dir       : $sc68datadir])
AC_MSG_NOTICE([+----------------------])
