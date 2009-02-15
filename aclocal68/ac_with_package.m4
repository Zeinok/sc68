dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2007-2009 Benjamin Gerard
dnl#
dnl# Distributed under the term of the GPL
dnl#
dnl# $Id$
dnl#

# serial 20090125

# DUMP_WITH_PACKAGE_VARS
# ----------------------
# $1:package var name
# $2:label
m4_define([DUMP_WITH_PACKAGE_VARS],[
    cat <<EOF
==$2==========================================================
 --  $1_has=[$]has_$1
 --  $1_req=[$]$1_req
 --  $1_ver=[$]$1_ver
 --  $1_fct=[$]$1_fct
 --  $1_hdr=[$]$1_hdr
 --  $1_def=[$]$1_def
 --  $1_inc=[$]$1_inc
 --  $1_lib=[$]$1_lib
 --  $1_ldf=[$]$1_ldf
 --  $1_src=[$]$1_src
 --  $1_abs=[$]$1_abs
 --  $1_dir=[$]$1_dir
 --  $1_dbn=[$]$1_dbn
======================================================================
EOF
  ])


# UNSET_WITH_PACKAGE_VARS
# -----------------------
# $1:var name
m4_define([UNSET_WITH_PACKAGE_VARS],[
    unset $1_req $1_ver $1_fct $1_hdr $1_def $1_inc
    unset $1_lib $1_ldf $1_src $1_abs $1_dir $1_dbn
  ])

# COPY_WITH_PACKAGE_VARS
# ----------------------
# $1:dst var name
# $2:src var name
m4_define([COPY_WITH_PACKAGE_VARS],[
    $1_req="[$]$2_req"; $1_ver="[$]$2_ver"
    $1_fct="[$]$2_fct"; $1_hdr="[$]$2_hdr"
    $1_def="[$]$2_def"; $1_inc="[$]$2_inc"
    $1_lib="[$]$2_lib"; $1_ldf="[$]$2_ldf"
    $1_src="[$]$2_src"; $1_abs="[$]$2_abs"
    $1_dir="[$]$2_dir"; $1_dbn="[$]$2_dbn"
  ])

# RESET_WITH_PACKAGE_VARS
# -----------------------
# $1:package var name
# $2:required           (yes|no)
# $3:function           (function to link)
# $4:header             (header file to test)
# $5:dirbasename
m4_define([RESET_WITH_PACKAGE_VARS],
  [
    has_$1=no                   # package found ?
    $1_req="$2"
    $1_ver="n/a"                # package version
    $1_def=""                   # package extra cpp definitions (-D)
    $1_inc=""                   # package extra include paths (-I)
    $1_ldf=""                   # package extra library paths (-L)
    $1_lib=""                   # package extra libraries (-l)
    $1_src=""                   # package source (rel. to build)
    $1_abs=""                   # package source absolute
    $1_dir=""                   # package build (rel. to build)
    $1_fct="m4_default([$3],[$1_version])" # fonction to search
    $1_hdr="m4_default([$4],[$1.h])"       # header to search
    $1_dbn="m4_default([$5],[$1])"         # dir basename
  ])


# DO_WITH_PACKAGE
# ---------------
# $1:package_var_name  (pack_age)
# $2:package-name:dirname
# $3:PACKAGE_DEF_NAME  (PACK_AGE)
# $4:required
# $5:function ('-' =>  ignore; '+' => header only)
# $6:header   ('-' =>  ignore)
# $7:dirbasename
m4_define([DO_WITH_PACKAGE],
  [
    # Declare with options 
    AC_ARG_WITH(
      [$2],
      [AS_HELP_STRING([--with-$2],[which $2 package])],
      [],[with_$1=check])

    # Set actions
    set -- [$](echo [$]with_$1 | sed "s/,/ /g")
    AS_IF([test "x[$][1]" = "xcheck"],
      [set -- bundle source system])

    RESET_WITH_PACKAGE_VARS([$1],[$4],[$5],[$6],[$7])
    while test [$]# -gt 0; do
      COPY_WITH_PACKAGE_VARS([_$1],[$1])

      case "x-[$]1" in
        
        # YES or NO: no check; assume evrything is configured
	x-no | x-yes) has_$1="[$]1"; break
          ;;

        # bundle: source package inside source tree; SRCDIR/PACKAGE
	x-bundle | x-source)
  	  _$1_ver="[$]1"
 	  AC_MSG_CHECKING([for [$]_$1_ver $2])
          _$1_src="[$]srcdir/../[$]_$1_dbn"
          test "x-[$]_$1_ver" = "x-bundle" &&
          _$1_src="[$]srcdir/[$]_$1_dbn"

 	  if ! _$1_abs=[$](cd "[$]_$1_src" 2>/dev/null && pwd); then
            _$1_src=""
          elif test -e "[$]_$1_abs/configure.gnu" ||
              test -e "[$]_$1_abs/configure"; then
            has_$1='maybe'
            test "x[$]_$1_hdr" = "x-" && has_$1=yes
            if test "x-[$]_$1_ver" = "x-bundle"; then
              _$1_dir='[$](top_builddir)/$2'
              _$1_src='[$](top_srcdir)/'"[$]_$1_dbn"
            else
              _$1_dir='[$](top_builddir)/../'"[$]_$1_dbn"
              _$1_src='[$](top_srcdir)/../'"[$]_$1_dbn"
            fi
	    _$1_inc="-I[$]_$1_src"
          fi

 	  AC_MSG_RESULT([$]has_$1)

          if test "x-[$]has_$1" = "x-maybe"; then
            ac_wp_CPPFLAGS="[$]CPPFLAGS"
            CPPFLAGS="-I[$]_$1_abs"
            AC_CHECK_HEADERS([[$]_$1_hdr],[has_$1=yes],[has_$1=no])
            CPPFLAGS="[$]ac_wp_CPPFLAGS"
          fi
          
          if test "x-[$]has_$1" = "x-yes" &&
            test "x-[$]_$1_ver" = "x-bundle" &&
            test "x[$]_$1_fct" != "x+"; then
            AC_CONFIG_SUBDIRS([$1])
          fi
 	  ;;
        
        # system: installed package
        x-system)
          AC_PATH_PROG([pkgconfig],["pkg-config"],["false"])
	  AC_MSG_CHECKING([for pkg-config $2 module])
          AS_IF([$][pkgconfig --exists "$2"],[
              AS_IF([test "x[$]{_$1_fct}" != "x+"],[
                  _$1_lib=[$]([$]pkgconfig "$2" --libs-only-l)
                  _$1_ldf=[$]([$]pkgconfig "$2" --libs-only-L)])
              _$1_inc=[$]([$]pkgconfig "$2" --cflags-only-I)
              _$1_def=[$]([$]pkgconfig "$2" --cflags-only-other)
              _$1_ver=[$]([$]pkgconfig "$2" --modversion||echo installed)
              AC_MSG_RESULT([yes ([$]_$1_ver)])
              ],[
              AC_MSG_RESULT([no])
              AC_PATH_PROG([$1_config],["$2-config"],["false"])
              AS_IF([test "x[$]{_$1_fct}" != "x+"],[
                  _$1_lib=[$]([$]$1_config --libs-only-l)
                  _$1_ldf=[$]([$]$1_config --libs-only-L)])
#               _$1_inc=[$]([$]$1_config --cflags-only-I)
#               _$1_def=[$]([$]$1_config --cflags-only-other)
              _$1_inc=[$]([$]$1_config --pkgincludedir)
              _$1_ver=[$]([$]$1_config --version||echo installed)
            ])
          
          AS_IF([test "x[$]_$1_hdr" = "x-"],
            [has_$1=maybe],
            [AC_CHECK_HEADERS([$][_$1_hdr],[has_$1=maybe])])
          
          AS_IF([test "xhas_$1" = "xmaybe"],
            [AS_CASE(["x[$]_$1_fct"],
                [x- | x+],[has_$1=yes],
                [AC_SEARCH_LIBS(
                    [[$]_$1_fct],
                    [$2],
                    [has_$1=yes; _$1_lib="$(echo [$]_$1_lib -l$2)"],
                    [has_$1=no],
                    [[$]_$1_lib])])])
          
	  AC_MSG_RESULT([$]has_$1)
	  ;;
        
	*)
          ;;
        
      esac
      
      AS_IF([test "x[$]{has_$1}" = "xyes"],[
          COPY_WITH_PACKAGE_VARS([$1],[_$1])
          break
        ])
      shift
    done
    UNSET_WITH_PACKAGE_VARS([_$1])
    
    AS_IF([test "[$]{$1_req}x[$]{has_$1}" = "yesxno"],
      [AC_MSG_ERROR([unable to configure a required package ... $1])])

    AM_CONDITIONAL([SOURCE_]$3,
      [test "x[$]$1_src" != "x" && test "x[$]_$1_fct" != "x+"])

    AS_IF([test "x[$]has_$1" = "xyes"],
      [
        AC_DEFINE([USE_]$3,[1],[Using $2 library])
        AC_SUBST($1[_def])
        AC_SUBST($1[_dir])
        AC_SUBST($1[_inc])
        AC_SUBST($1[_lib])
        AC_SUBST($1[_src])
        AC_SUBST($1[_abs])
      ])

#     [PACKAGE_CFLAGS=$](echo [$PACKAGE_CFLAGS])
#     [PACKAGE_CPPFLAGS=$](echo [$PACKAGE_CPPFLAGS] [$]$1[_def] [$]$1[_inc])
#     [PACKAGE_LDFLAGS=$](echo [$PACKAGE_LDFLAGS] [$]$1[_ldf] [$]$1[_lib])

    DUMP_WITH_PACKAGE_VARS([$1],[ END ])
  ])


# AC_WITH_PACKAGE([PACKAGE],[REQ],[FUNC],[HEADER],[DIRNAME])
# ----------------------------------------------------------
# 
AC_DEFUN([AC_WITH_PACKAGE],
  [
    DO_WITH_PACKAGE(
      AS_TR_SH(m4_strip[$1]),
      m4_strip([$1]),
      AS_TR_CPP(m4_strip[$1]),
      m4_if([yes],m4_default(m4_strip([$2]),[no]),[yes],[no]),
      m4_strip([$3]),
      m4_strip([$4]),
      m4_strip([$5]))
  ])

# AC_WITH_PACKAGE_HEADER([PACKAGE],[REQ],[HEADER],[DIRNAME])
# ----------------------------------------------------------
# 
AC_DEFUN([AC_WITH_PACKAGE_HEADER],
  [
    DO_WITH_PACKAGE(
      AS_TR_SH(m4_strip[$1]),
      m4_strip([$1]),
      AS_TR_CPP(m4_strip[$1]),
      m4_if([yes],m4_default(m4_strip([$2]),[no]),[yes],[no]),
      [+],
      m4_strip([$3]),
      m4_strip([$4]))
  ])


dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
