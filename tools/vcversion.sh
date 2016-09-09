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

cleanexpr='s/^[[:space:]]*\([0-9][-_0-9A-Za-z]*\).*/\1/p'

base=
rev=

while test $# -gt 0; do
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

if test X"$base" != X ; then
    base="${base}."
fi

# svn ? (default sc68 source control)
if test X"$rev" = X; then
    rev=`svnversion -nq | sed -ne "$cleanexpr"`
fi 2>/dev/null

# git-svn ?
if test X"$rev" = X; then
   rev=`git svn find-rev HEAD | sed -ne "$cleanexpr"`
fi 2>/dev/null

# git ?
if test X"$rev" = X; then
    rev=`git rev-list --count HEAD | sed -ne "$cleanexpr"`
fi 2>/dev/null

if test "X$rev" != X; then
    # Have something lets print it
    echo -n "${base}${rev}"
else
    # Else use date as default
    today=`date -u "+%Y%m%d" 2>/dev/null || echo 66600666`
    echo -n "${base}${today}"
fi 2>/dev/null
