# ,----------------------------------------------------------------------.
# |                                                                      |
# |      as68 -- Makefile.am -- Use automake to create Makefile.in       |
# |                                                                      |
# |            Copyright (C) 1998-2009 Benjamin Gerard                   |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# $Id$

ACLOCAL_AMFLAGS      = -I m4 --install
AM_CFLAGS            = @AS68_CFLAGS@ @BINAS68_CFLAGS@
AM_CPPFLAGS          = @AS68_CPPFLAGS@ @BINAS68_CPPFLAGS@
AM_LDFLAGS           = @AS68_LDFLAGS@ @BINAS68_LDFLAGS@

# ,----------------------------------------------------------------------.
# | Program                                                              |
# `----------------------------------------------------------------------'

mysources            = as68.c error.c expression.c opcode.c word.c
myheaders            = as68.h error.h

bin_PROGRAMS         = as68
as68_LDFLAGS         = $(AM_LDFLAGS)
as68_SOURCES         = $(mysources) $(myheaders)


# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

dist_man_MANS        = as68.1
info_TEXINFOS        = as68.texi
#as68_TEXINFOS        = package.texi
MAINTAINERCLEANFILES = $(srcdir)/as68.1
# $(srcdir)/as68.info $(srcdir)/package.texi

#if MAINTAINER_MODE
TEXINFO2MAN          = $(SHELL) $(top_srcdir)/missing --run texinfo2man
fail                 = { test -e '$@' && rm '$@'; test a = b; }

$(srcdir)/as68.1: as68.1.tpl.in $(srcdir)/as68.texi
	$(TEXINFO2MAN) as68.1.tpl $(srcdir)/as68.texi >$@ || $(fail)
#endif


# ,----------------------------------------------------------------------.
# | Extra distrib                                                        |
# `----------------------------------------------------------------------'

# EXTRA_DIST = \
# as68.1 as68.1.tpl as68.1.tpl.in \
# package.texi.in
