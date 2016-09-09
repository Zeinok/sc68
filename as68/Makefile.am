# ,----------------------------------------------------------------------.
# |                                                                      |
# |   as68 -- Makefile.am -- Use with automake to produce Makefile.in    |
# |                                                                      |
# |               Copyright (c) 1998-2016 Benjamin Gerard                |
# |                                                                      |
# |               <http://sourceforge.net/users/benjihan>                |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

ACLOCAL_AMFLAGS = -I m4 --install

# ,----------------------------------------------------------------------.
# | Program                                                              |
# `----------------------------------------------------------------------'

mysources    = as68.c error.c expression.c opcode.c word.c
myheaders    = as68.h error.h
bin_PROGRAMS = as68
as68_SOURCES = $(mysources) $(myheaders)
as68_CFLAGS  = $(gb_CFLAGS)
as68_LDFLAGS = $(gb_LDFLAGS)

# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

# dist_man_MANS        = as68.1
# info_TEXINFOS        = as68.texi
# MAINTAINERCLEANFILES = $(srcdir)/as68.1

# TEXINFO2MAN          = $(texinfo2man)
# fail                 = { test -e '$@' && rm '$@'; test a = b; }

# $(srcdir)/as68.1: as68.1.tpl.in $(srcdir)/as68.texi
#       $(TEXINFO2MAN) as68.1.tpl $(srcdir)/as68.texi >$@ || $(fail)
