#! /bin/sh
#
# $Id$

svn info >/dev/null || exit 255

# Files eligible for 'Id' keyword property
find . -type f '('  \
    -name '*.[ch]' -o \
    -name '*.in' -o \
    -name '*.m4' -o \
    -name '*.sh' -o \
    -name 'Makefile.am' -o \
    -name 'configure.ac' \
    ')' -execdir svn propset svn:keywords Id {} ';'

# Files for execute 
find . -type f '(' \
    -name '*.sh' \
    ')' -execdir svn propset svn:executable '*' {} ';'
