#!/bin/sh
#
# by Benjamin Gerard
#
# How it works:
#  - svn revision via svnversion binary
#  - svn revision behind git-svn
#  - git revision
#  - fallbacks to current date YYYY-MM-DD
#
# TODO:
#
#   May be try to determine which source control is in used rather than
#   running the command one by one.
#

export LC_ALL=C
exec 2>/dev/null

base=
rev=

while [ $# -gt 0 ]; do
    case "$1" in
	-h | --help | --usage)
	    cat <<EOF
Usage: vcversion.sh [PREFIX]
 
  Print a version number based on source control revision.
EOF
	    exit 0;;
	*)
	    base="$1" ;;
    esac
    shift
done

if [ -n "${base}" ]; then
    base="${base}."
fi

cleanrev() {
    echo -n `expr "$rev" : '^\([0-9]\+\)'`
}

# svn ? (default sc68 source control)
if [ -z "$rev" ] && rev=`svnversion -nq`; then
    rev=`cleanrev "$rev"`
fi

# git-svn ?
if [ -z "$rev" ] && rev=`git svn find-rev HEAD`; then
    rev=`cleanrev "$rev"`
fi

# git ?
if [ -z "$rev" ] && rev=`git rev-list --count HEAD`; then
    rev=`cleanrev "$rev"`
fi

if [ -n "$rev" ]; then
    echo -n ${base}${rev}
    exit 0
fi

# Last resort: UTC YYYY-MM-DD
echo -n ${base}`date -u +%F`
