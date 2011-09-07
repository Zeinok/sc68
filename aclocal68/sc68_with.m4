dnl# -*- mode:sh; sh-basic-offset:2; indent-tabs-mode:nil -*-
dnl#
dnl# autoconf macros
dnl#
dnl# (C) 2009-2011 Benjamin Gerard <https://sourceforge.net/users/benjihan>
dnl#
dnl# Distributed under the term of the GPL
dnl#
dnl# $Id$
dnl#

# serial 20090125

# DUMP_SC68_PACKAGE_VARS
# ----------------------
# $1:package var name
# $2:label
m4_define([DUMP_SC68_PACKAGE_VARS],[
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
 --  PACKAGE_EFLAGS=[$]PACKAGE_EFLAGS
 --  PACKAGE_LFLAGS=[$]PACKAGE_LFLAGS
 --  LIBS=[$]LIBS
======================================================================
EOF
  ])


# UNSET_SC68_PACKAGE_VARS
# -----------------------
# $1:var name
m4_define([UNSET_SC68_PACKAGE_VARS],[
    unset $1_req $1_ver $1_fct $1_hdr $1_def $1_inc
    unset $1_lib $1_ldf $1_src $1_abs $1_dir $1_dbn
  ])

# COPY_SC68_PACKAGE_VARS
# ----------------------
# $1:dst var name
# $2:src var name
m4_define([COPY_SC68_PACKAGE_VARS],[
    $1_req="[$]$2_req"; $1_ver="[$]$2_ver"
    $1_fct="[$]$2_fct"; $1_hdr="[$]$2_hdr"
    $1_def="[$]$2_def"; $1_inc="[$]$2_inc"
    $1_lib="[$]$2_lib"; $1_ldf="[$]$2_ldf"
    $1_src="[$]$2_src"; $1_abs="[$]$2_abs"
    $1_dir="[$]$2_dir"; $1_dbn="[$]$2_dbn"
  ])

# RESET_SC68_PACKAGE_VARS
# -----------------------
# $1:package var name
# $2:required           (yes|no)
# $3:function           (function to link)
# $4:header             (header file to test)
# $5:dirbasename
m4_define([RESET_SC68_PACKAGE_VARS],[
    has_$1="no"                 # package found yes/no ?
    $1_req="$2"                 # required yes/no ?
    $1_ver="n/a"                # package version
    $1_def=""                   # package extra cpp definitions (-D)
    $1_inc=""                   # package extra include paths (-I)
    $1_ldf=""                   # package extra library paths (-L)
    $1_lib=""                   # package extra libraries (-l)
    $1_src=""                   # package source (rel. to build)
    $1_abs=""                   # package source absolute
    $1_dir=""                   # package build (rel. to build)
    $1_fct="m4_default([$3],[$1_version])" # function to search
    $1_hdr="m4_default([$4],[$1.h])"       # header to search
    $1_dbn="m4_default([$5],[$1])"         # dir basename
  ])


# DO_SC68_PACKAGE
# ---------------
# $1:package_var_name  (pack_age)
# $2:package-name      (dirname)
# $3:PACKAGE_DEF_NAME  (PACK_AGE)
# $4:required          (yes/no)
# $5:function          ('-' => ignore; '+' => header only)
# $6:header            ('-' => ignore)
# $7:dirbasename
m4_define([DO_SC68_PACKAGE],[
    # Declare with options 
    AC_ARG_WITH(
      [$2],
      [AS_HELP_STRING([--with-$2],[which $2 package])],
      [],[with_$1=check])

    # Declare with headers
    AC_ARG_WITH(
      [$2],
      [AS_HELP_STRING([--with-$2-headers],[Locate $2 package headers])],
      [],[unset with_$1_headers])

    # Declare with sources
    AC_ARG_WITH(
      [$2],
      [AS_HELP_STRING([--with-$2-sources],[Locate $2 package sources])],
      [],[unset with_$1_sources])
    
    # Reset all package_vars
    RESET_SC68_PACKAGE_VARS([$1],[$4],[$5],[$6],[$7])

    # Verify --with-<package>-sources directory
    if test "x-[$]{with_$1_sources-set}" != "x-set"; then
      AC_MSG_CHECKING([$2 source directory])
      if ! test -d "[$]{with_$1_sources}"; then
        unset with_$1_sources
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([invalid source directory specified for $2])
      fi
      AC_MSG_RESULT([yes ([$]{with_$1_sources})])

      if test "x-[$]{with_$1_headers-set}" = "x-set"; then
        if test -d "[$]{with_$1_sources}/includes"; then
          with_$1_headers="[$]{with_$1_sources}/includes"
        elif  test -d "[$]{with_$1_sources}/include"; then
          with_$1_headers="[$]{with_$1_sources}/include"
        else
          with_$1_headers="[$]{with_$1_sources}"
        fi
      fi
    fi

    # Verify --with-<package>-headers directory
    if test "x-[$]{with_$1_headers-set}" != "x-set"; then
      AC_MSG_CHECKING([$2 header directory])
      if ! test -d "[$]{with_$1_headers}"; then
        unset with_$1_headers
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([invalid headers directory specified for $2])
      fi
      AC_MSG_RESULT([yes ([$]{with_$1_sources})])
    fi
    
    # Set actions
    set -- [$](echo [$]with_$1 | sed "s/,/ /g")
    if test "x[$][1]" = "xcheck"; then
      # Default actions
      if test "x-[$]{with_$1_sources-set}" != "x-set"; then
        # --with-package-source has been set: configure source only
        set -- source
        _$1_src="[$]{with_$1_sources}"
      else
        set -- bundle source system
      fi
    fi

    while test [$]# -gt 0; do
      COPY_SC68_PACKAGE_VARS([_$1],[$1])

      case "x-[$]1" in
        
        # YES or NO: no check; assuming everything is configured.
	x-no | x-yes)
          has_$1="[$]1"; break ;;

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
            _$1_lib="-l$1"   # $$$ Or not ...
          fi

 	  AC_MSG_RESULT([$]has_$1)

          if test "x-[$]has_$1" = "x-maybe"; then
            ac_wp_CPPFLAGS="[$]CPPFLAGS"
            CPPFLAGS="-I[$]_$1_abs [$]CPPFLAGS"
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
          if [$]pkgconfig --exists "$2"; then
            # use pkg-config
            if test "x[$]{_$1_fct}" != "x+"; then
              _$1_lib=[$]([$]pkgconfig "$2" --libs-only-l)
              _$1_ldf=[$]([$]pkgconfig "$2" --libs-only-L)
            fi
            _$1_inc=[$]([$]pkgconfig "$2" --cflags-only-I)
            _$1_def=[$]([$]pkgconfig "$2" --cflags-only-other)
            _$1_ver=[$]([$]pkgconfig "$2" --modversion||echo installed)
            AC_MSG_RESULT([yes ([$]_$1_ver)])
          else
            # use config script
            AC_MSG_RESULT([no])
            AC_PATH_PROG([$1_config],["$2-config"],["false"])
            for ac_flags in : [$]([$]$1_config --libs); do
              case [$]ac_flags in
                :) ;;
                -l*) _$1_lib="[$]_$1_lib [$]ac_flags" ;;
                -L*) _$1_ldf="[$]_$1_ldf [$]ac_flags" ;; 
              esac
            done
            for ac_flags in : [$]([$]$1_config --cflags); do
              case [$]ac_flags in
                :) ;;
                -I*)       _$1_inc="[$]_$1_inc [$]ac_flags" ;;
                *)         _$1_def="[$]_$1_def [$]ac_flags" ;;
                #-D* | -U*) _$1_def="[$]_$1_def [$]ac_flags" ;;
              esac
            done
            _$1_ver=[$]([$]$1_config --version || echo installed)
          fi

          if test "x[$]_$1_hdr" = "x-"; then
            has_$1=maybe
          else
            ac_wp_CPPFLAGS="[$]CPPFLAGS"
            CPPFLAGS="[$]_$1_inc [$]CPPFLAGS"
            AC_CHECK_HEADERS([$][_$1_hdr],[has_$1=maybe])
            CPPFLAGS="[$]ac_wp_CPPFLAGS"
          fi
          
          if test "x[$]has_$1" = "xmaybe"; then
            case "x[$]_$1_fct" in
              x- | x+) has_$1=yes ;;
              *)
                AC_SEARCH_LIBS(
                  [[$]_$1_fctxxx],
                  [$2],
                  [has_$1=yes; _$1_lib="$(echo [$]_$1_lib -l$2)"],
                  [has_$1=no],
                  [[$]_$1_lib])
                ;;
            esac
          fi      
	  ;;
        
	*)
          ;;
      esac
      
      if test "x[$]{has_$1}" = "xyes"; then
        COPY_SC68_PACKAGE_VARS([$1],[_$1])
        break
      fi
      shift
    done
    UNSET_SC68_PACKAGE_VARS([_$1])
    
    AS_IF([test "[$]{$1_req}x[$]{has_$1}" = "yesxno"],
      [AC_MSG_ERROR([unable to configure a required package ... $1])])
    
    AM_CONDITIONAL([SOURCE_]$3,
      [test "x[$]$1_src" != "x" && test "x[$]_$1_fct" != "x+"])

    if test "x[$]has_$1" = "xyes"; then
      AC_DEFINE([USE_]$3,[1],[Using $2 library])
      AC_SUBST($1[_def])
      AC_SUBST($1[_dir])
      AC_SUBST($1[_inc])
      AC_SUBST($1[_lib])
      AC_SUBST($1[_src])
      AC_SUBST($1[_abs])
    fi
    #DUMP_SC68_PACKAGE_VARS([$1],[ END ])
  ])



# SC68_WITH([PACKAGE],[REQ],[FUNC],[HEADER],[DIRNAME])
# ----------------------------------------------------
# 
AC_DEFUN([SC68_WITH],
  [
    DO_SC68_PACKAGE(
      AS_TR_SH(m4_strip[$1]),
      m4_strip([$1]),
      AS_TR_CPP(m4_strip[$1]),
      m4_if([yes],m4_default(m4_strip([$2]),[no]),[yes],[no]),
      m4_strip([$3]),
      m4_strip([$4]),
      m4_strip([$5]))
  ])

# SC68_WITH_HEADER([PACKAGE],[REQ],[HEADER],[DIRNAME])
# ----------------------------------------------------
# 
AC_DEFUN([SC68_WITH_HEADER],
  [
    DO_SC68_PACKAGE(
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
