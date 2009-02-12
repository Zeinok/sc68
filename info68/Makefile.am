# ,----------------------------------------------------------------------.
# |                                                                      |
# | info68 -- Makefile.am -- Use with automake to produce Makefile.in    |
# |                                                                      |
# |                 (C) 2006-2009 Benjamin Gerard                        |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# $Id$
# @configure_input@

SUBDIRS              = @subdirs@
ACLOCAL_AMFLAGS      = -I m4 --install

AM_CFLAGS            = @INFO68_CFLAGS@ @BININFO68_CFLAGS@
AM_CPPFLAGS          = @INFO68_CPPFLAGS@ @BININFO68_CPPFLAGS@ $(SINC)
AM_LDFLAGS           = @INFO68_LDFLAGS@ @BININFO68_LDFLAGS@
SINC                 = $(file68_inc)


# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_FILE68
FILE68_LA = $(file68_dir)/libfile68.la
@SET_MAKE@
$(file68_dir)/libfile68.la:
	cd "$(file68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libfile68.la
else
FILE68_LA            =
endif

bin_PROGRAMS         = info68
info68_SOURCES       = info68.c
info68_LDADD         = $(FILE68_LA) $(DESA68_LA)
noinst_HEADERS       = info68_doc.h


# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

dist_man_MANS        = info68.1
info_TEXINFOS        = info68.texi
#info68_TEXINFOS      = package.texi
MAINTAINERCLEANFILES = $(srcdir)/info68.1
# $(srcdir)/info68.info $(srcdir)/package.texi


#if MAINTAINER_MODE
TEXINFO2MAN          = $(SHELL) $(top_srcdir)/missing --run texinfo2man
fail                 = { test -e '$@' && rm '$@'; test a = b; }

$(srcdir)/info68.1: info68.1.tpl.in $(srcdir)/info68.texi
	$(TEXINFO2MAN) info68.1.tpl $(srcdir)/info68.texi >$@ || $(fail)
#endif


# ,----------------------------------------------------------------------.
# | Extra distrib                                                        |
# `----------------------------------------------------------------------'

#EXTRA_DIST = info68.1 info68.info package.texi
