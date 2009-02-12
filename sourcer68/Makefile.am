# ,----------------------------------------------------------------------.
# |                                                                      |
# | sourcer68 -- Makefile.am -- Use with automake to produce Makefile.in |
# |                                                                      |
# |            Copyright (C) 2006-2009 Benjamin Gerard                   |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# $Id$
# @configure_input@

SUBDIRS              = @subdirs@
ACLOCAL_AMFLAGS      = -I m4 --install

AM_CFLAGS            = @SOURCER68_CFLAGS@ @BINSOURCER68_CFLAGS@
AM_CPPFLAGS          = @SOURCER68_CPPFLAGS@ @BINSOURCER68_CPPFLAGS@ $(SINC)
AM_LDFLAGS           = @SOURCER68_LDFLAGS@ @BINSOURCER68_LDFLAGS@
SINC                 = $(desa68_inc) $(file68_inc)


# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_DESA68
DESA68_LA  = $(desa68_dir)/libdesa68.la
@SET_MAKE@
$(desa68_dir)/libdesa68.la:
	cd "$(desa68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libdesa68.la
else
DESA68_LA  = 
endif

if SOURCE_FILE68
FILE68_LA = $(file68_dir)/libfile68.la
@SET_MAKE@
$(file68_dir)/libfile68.la:
	$(MAKE) -C $(file68_dir) libfile68.la
else
FILE68_LA =
endif


bin_PROGRAMS          = sourcer68
sourcer68_SOURCES     = sourcer68.c
sourcer68_LDADD       = $(FILE68_LA) $(DESA68_LA)
noinst_HEADERS        = sourcer68_doc.h


# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

dist_man_MANS        = sourcer68.1
info_TEXINFOS        = sourcer68.texi
#sourcer68_TEXINFOS   = package.texi
MAINTAINERCLEANFILES = $(srcdir)/sourcer68.1
# $(srcdir)/sourcer68.info $(srcdir)/package.texi


#if MAINTAINER_MODE
TEXINFO2MAN          = $(SHELL) $(top_srcdir)/missing --run texinfo2man
fail                 = { test -e '$@' && rm '$@'; test a = b; }

$(srcdir)/sourcer68.1: sourcer68.1.tpl.in $(srcdir)/sourcer68.texi
	$(TEXINFO2MAN) sourcer68.1.tpl $(srcdir)/sourcer68.texi >$@ || $(fail)
#endif


# ,----------------------------------------------------------------------.
# | Extra distrib                                                        |
# `----------------------------------------------------------------------'

#EXTRA_DIST = sourcer68.1 sourcer68.info package.texi
