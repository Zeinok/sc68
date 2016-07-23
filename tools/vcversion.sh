#!/bin/sh
#
# Guess version control version or something
#

export LC_ALL=C

exec 2>/dev/null

cleanrev() {
    echo -n `expr "$rev" : '^\([0-9]\+\)'`
}
    
rev=''

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
    echo -n $rev
    exit $?
fi

# Last resort: UTC YYYY-MM-DD
echo -n xxx `date -u +%F`
