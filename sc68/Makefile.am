# ,----------------------------------------------------------------------.
# |                                                                      |
# |    sc68 -- Makefile.am -- Use with automake to produce Makefile.in   |
# |                                                                      |
# |                 (C) 2007-2009 Benjamin Gerard                        |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id$

SUBDIRS              = @subdirs@
ACLOCAL_AMFLAGS      = -I m4 --install

AM_CFLAGS            = @SC68_CFLAGS@ @BINSC68_CFLAGS@
AM_CPPFLAGS          = @SC68_CPPFLAGS@ @BINSC68_CPPFLAGS@ $(SINC)
AM_LDFLAGS           = @SC68_LDFLAGS@ @BINSC68_LDFLAGS@
SINC                 = $(sc68_inc) $(file68_inc)


# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_SC68
SC68_LA = $(sc68_dir)/libsc68.la
@SET_MAKE@
$(sc68_dir)/libsc68.la:
	$(MAKE) -C $(sc68_dir) libsc68.la
else
SC68_LA =
endif


bin_PROGRAMS         = sc68
sc68_SOURCES         = sc68.c
sc68_LDADD           = $(SC68_LA)


# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

man_MANS             = sc68.1
info_TEXINFOS        = sc68.texi
sc68_TEXINFOS        = package.texi
MAINTAINERCLEANFILES = \
 $(srcdir)/sc68.info $(srcdir)/package.texi \
 $(srcdir)/sc68.1 $(srcdir)/package.texi.in


if MAINTAINER_MODE
TEXINFO2MAN          = $(SHELL) $(top_srcdir)/missing --run texinfo2man
fail                 = { test -e '$@' && rm '$@'; test a = b; }

$(srcdir)/package.texi: $(srcdir)/package.texi.in
	$(SHELL) $(top_builddir)/config.status --file=$@:$<

$(srcdir)/sc68.1: sc68.1.tpl $(srcdir)/sc68.texi
	$(TEXINFO2MAN) $^ >$@ || $(fail)
endif


# ,----------------------------------------------------------------------.
# | Extra distrib                                                        |
# `----------------------------------------------------------------------'

EXTRA_DIST = sc68.1 sc68.info package.texi
