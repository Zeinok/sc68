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
AM_CPPFLAGS          = @INFO68_CPPFLAGS@ @BININFO68_CPPFLAGS@ $(SINCS)
AM_LDFLAGS           = @INFO68_LDFLAGS@ @BININFO68_LDFLAGS@


# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_FILE68
FILE68_LA = $(file68_dir)/libfile68.la
@SET_MAKE@
$(file68_dir)/libfile68.la:
	$(MAKE) -C $(file68_dir) libfile68.la
else
FILE68_LA            =
endif
SINCS                = $(file68_inc)

bin_PROGRAMS         = info68
info68_SOURCES       = info68.c
info68_LDADD         = $(FILE68_LA) $(DESA68_LA)
noinst_HEADERS       = info68_doc.h


# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

man_MANS             = info68.1
info_TEXINFOS        = info68.texi
info68_TEXINFOS      = package.texi
MAINTAINERCLEANFILES = \
 $(srcdir)/info68.info $(srcdir)/package.texi \
 $(srcdir)/info68.1 $(srcdir)/package.texi.in


if MAINTAINER_MODE
fail                 = { test -e '$@' && rm '$@'; test a = b; }

$(srcdir)/package.texi: $(srcdir)/package.texi.in
	$(SHELL) $(top_builddir)/config.status --file=$@:$<

$(srcdir)/info68.1: info68.1.tpl $(srcdir)/info68.texi
	@texinfo2man@ $^ >$@ || $(fail)

endif


# ,----------------------------------------------------------------------.
# | Extra distrib                                                        |
# `----------------------------------------------------------------------'

EXTRA_DIST = info68.1 info68.info package.texi
