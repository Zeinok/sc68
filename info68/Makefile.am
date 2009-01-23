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

BN              = info68
ACLOCAL_AMFLAGS = -I m4 --install
SUBDIRS         = @subdirs@


# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_FILE68
FILE68_LA = $(file68_dir)/libfile68.la
@SET_MAKE@
$(file68_dir)/libfile68.la:
        $(MAKE) -C $(file68_dir) libfile68.la
else
FILE68_LA =
endif

AM_CFLAGS      = @INFO68_CFLAGS@
AM_CPPFLAGS    = @INFO68_CPPFLAGS@

bin_PROGRAMS   = info68
info68_SOURCES = info68.c
info68_LDFLAGS = @INFO68_LA_LDFLAGS@ @INFO68_LDFLAGS@
info68_LDADD   = $(FILE68_LA)
noinst_HEADERS = info68_doc.h


# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

man_MANS        = $(BN).1
info_TEXINFOS   = $(BN).texi
info68_TEXINFOS	= package.texi options68.texi

EXTRA_DOC       = $(BN).1 $(BN).info
MAINTAINERCLEANFILES =\
 $(srcdir)/$(BN).1 $(srcdir)/$(BN).info

if MAINTAINER_MODE
$(srcdir)/$(BN).1: $(BN).1.tpl $(srcdir)/$(BN).texi $(srcdir)/Makefile.am
	if ! @texinfo2man@ $(BN).1.tpl $(srcdir)/$(BN).texi>$@; then rm -f $@; exit 255; fi
endif


# ,----------------------------------------------------------------------.
# | Extra distrib                                                        |
# `----------------------------------------------------------------------'

EXTRA_DIST =\
 $(EXTRA_DOC)\
 $(PACKAGE)-$(PACKAGE_VERSION).ebuild\
 $(PACKAGE).spec
