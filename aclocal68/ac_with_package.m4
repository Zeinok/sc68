dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2007 Benjamin Gerard <benjihan -4t- users.sourceforge -d0t- net>
dnl#
dnl# Distribued under the term of the GPL
dnl#
dnl# $Id$
dnl#

# serial 20090110

# AC_WITH_PACKAGE([PACKAGE],[REQ],[FUNC],[HEADER])
# -----------------------------------------------
# Defines --enable-debug option
AC_DEFUN([AC_WITH_PACKAGE],[
    AC_ARG_WITH(
      [$1],
      [AS_HELP_STRING([--with-$1],[which $1 package])],
      [],[with_$1=check])

    $1_actions=[$](echo [$]with_$1 | sed "s/,/ /g")
    if test "x[$]$1_actions" = "xcheck"; then
      $1_actions="bundle system"
    fi
    
#     echo ========= AT START =====
#     echo -- CPPFLAGS=[$]CPPFLAGS
#     echo -- LDFLAGS=[$]LDFLAGS
#     echo -- LIBS=[$]LIBS

    has_$1=no                   # package found ?
    $1_req=m4_if([yes],m4_default(m4_strip([$2]),[no]),[yes],[no])
    $1_ver="n/a"                # package version
    $1_def=""                   # package extra cpp definitions (-D)
    $1_inc=""                   # package extra include paths (-I)
    $1_ldf=""                   # package extra library paths (-L)
    $1_lib=""                   # package extra libraries (-l)
    $1_src=""                   # package source (rel. to build)
    $1_abs=""                   # package source absolute
    $1_dir=""                   # package build (rel. to build)

    $1_fct="m4_default([$3],[$1_version])"
    $1_hdr="m4_default([$4],[$1.h])"
    for $1_action in [$]$1_actions; do
      case x[$]$1_action in

	xno|xyes) has_$1=[$]$1_action; break;;

	xbundle)
	  AC_MSG_CHECKING([for bundled $1])
	  if $1_abs=[$](cd "[$]{srcdir}/$1" 2>/dev/null && pwd); then
	    test -e "[$]{$1_abs}/configure.gnu" ||
	    test -e "[$]{$1_abs}/configure"
	    if test [$][?] -eq 0; then
	      has_$1=yes
 	      _$1_ver=bundle
	      _$1_def=""
	      _$1_inc=[$](echo [$]$1_inc -I[$]$1_src) # abs ?
              _$1_ldf=""
              _$1_lib=""
              _$1_src="[$]{srcdir}/$1"
              _$1_dir="$1"
              AC_CONFIG_SUBDIRS([$1])
	    fi
	  fi
	  AC_MSG_RESULT([$]{has_$1})
	  ;;

        xsystem)
          AC_PATH_PROG([pkgconfig],["pkg-config"],["false"])
	  AC_MSG_CHECKING([for pkg-config $1 module])
          AS_IF([$][pkgconfig --exists "$1"],[
              _$1_lib=[$]([$]pkgconfig --libs-only-l)
              _$1_ldf=[$]([$]pkgconfig --libs-only-L)
              _$1_inc=[$]([$]pkgconfig --cflags-only-I)
              _$1_def=[$]([$]pkgconfig --cflags-only-other)
              _$1_ver=[$]([$]pkgconfig --modversion||echo [$]$1_ver)
              AC_MSG_RESULT([yes ([$]$1_ver)])
            ],[
              AC_MSG_RESULT([no])
              AC_PATH_PROG([$1_config],["$1-config"],["false"])
              _$1_lib=[$]([$]$1_config --libs-only-l)
              _$1_ldf=[$]([$]$1_config --libs-only-L)
              _$1_inc=[$]([$]$1_config --cflags-only-I)
              _$1_def=[$]([$]$1_config --cflags-only-other)
              _$1_ver=[$]([$]$1_config --version||echo [$]$1_ver)
            ])

#           echo -- $1_lib=[$]$1_lib
#           echo -- [$]prefix/lib
#           echo -- includedir=[$]includedir
#           ac_$1_save_cppflags="[$]CPPFLAGS"
#           ac_$1_save_ldflags="[$]LDFLAGS"
#           eval CPPFLAGS=\"-I[$]includedir [\$]CPPFLAGS\" 
#           eval LDFLAGS=\"-L[$]prefix/lib [\$]LDFLAGS\" 
#           echo -- CPPFLAGS=[$]CPPFLAGS
#           echo -- LDFLAGS=[$]LDFLAGS

#           echo ========= BEFORE CHECK LIB =====
#           echo -- CPPFLAGS=[$]CPPFLAGS
#           echo -- LDFLAGS=[$]LDFLAGS
#           echo -- LIBS=[$]LIBS
          AC_CHECK_HEADER(
            [$][$1_hdr],
            [AC_SEARCH_LIBS(
                [[$]$1_fct],
                [$1],
                [has_$1=yes; _$1_lib="$(echo [$]_$1_lib -l$1)"],
                [],
                [[$]_$1_lib])])
	  AC_MSG_RESULT([$]{has_$1})
#           echo ========= AFTER CHECK LIB =====
#           echo -- CPPFLAGS=[$]CPPFLAGS
#           echo -- LDFLAGS=[$]LDFLAGS
#           echo -- LIBS=[$]LIBS
	  ;;

	*)
	  ;;
		
      esac
      AS_IF([test "x[$]{has_$1}" = "xyes"],[
 	  $1_ver="[$]_$1_ver";
          $1_def="[$]_$1_def"; $1_inc="[$]_$1_inc"
          $1_ldf="[$]_$1_ldf"; $1_lib="[$]_$1_lib"
          $1_src="[$]_$1_src"; $1_dir="[$]_$1_dir"
          break])
      unset _$1_ver _$1_def _$1_inc _$1_ldf _$1_lib _$1_src _$1_dir
    done
    AS_IF([test "[$]{$1_req}x[$]{has_$1}" = "yesxno"],[
	AC_MSG_ERROR([unable to configure a required package ... $1])
      ])

#    AC_SUBST($1[]_dir,[$]$1[]_src)
    
    AM_CONDITIONAL([SOURCE_]m4_toupper($1),[test -n "[$]$1_src"])
    AS_IF([test "x[$]has_$1" = "xyes"],
      [AC_DEFINE([USE_]m4_toupper($1),[1],[Using $1 library])])

#    CPPFLAGS=[$](echo $CPPFLAGS [$]$1_def [$]$1_inc)

# echo ======================================================================
# echo  --  $1_has=[$]has_$1
# echo  --  $1_req=[$]$1_req
# echo  --  $1_ver=[$]$1_ver
# echo  --  $1_fct=[$]$1_fct
# echo  --  $1_hdr=[$]$1_hdr
# echo  --  $1_def=[$]$1_def
# echo  --  $1_inc=[$]$1_inc
# echo  --  $1_lib=[$]$1_lib
# echo  --  $1_ldf=[$]$1_ldf
# echo  --  $1_src=[$]$1_src
# echo  --  $1_dir=[$]$1_dir
# echo ======================================================================
# echo ========= AT END =====
# echo -- CPPFLAGS=[$]CPPFLAGS
# echo -- LDFLAGS=[$]LDFLAGS
# echo -- LIBS=[$]LIBS
  ])

dnl# ----------------------------------------------------------------------
dnl#
dnl# End Of $Id$
dnl#
dnl# ----------------------------------------------------------------------
