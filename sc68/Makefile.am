# Time-stamp: <2011-08-23 02:56:07 ben>
# ,----------------------------------------------------------------------.
# |                                                                      |
# |    sc68 -- Makefile.am -- Use with automake to produce Makefile.in   |
# |                                                                      |
# |                  (C) 2007-2011 Benjamin Gerard                       |
# |           <benjihan -4t- users.sourceforge -d0t- net>                |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

SUBDIRS              = $(subdirs)
ACLOCAL_AMFLAGS      = -I m4 --install

AM_CFLAGS            = $(ALL_CFLAGS) $(BIN_CFLAGS)
AM_CPPFLAGS          = $(ALL_EFLAGS) $(BIN_EFLAGS) $(EFLAGS)
AM_LDFLAGS           = $(ALL_LFLAGS) $(BIN_LFLAGS)
EFLAGS               = $(sc68_inc) $(file68_inc)

# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_SC68
SC68_LA = $(sc68_dir)/libsc68.la
@SET_MAKE@
$(sc68_dir)/libsc68.la:
	cd "$(sc68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libsc68.la
else
SC68_LA =
endif

bin_PROGRAMS         = sc68
sc68_SOURCES         = sc68.c
sc68_LDADD           = $(SC68_LA)

# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

dist_man_MANS        = sc68.1
info_TEXINFOS        = sc68.texi
MAINTAINERCLEANFILES = $(srcdir)/sc68.1

TEXINFO2MAN          = $(texinfo2man)
fail                 = { test -e '$@' && rm '$@'; test a = b; }

$(srcdir)/sc68.1: sc68.1.tpl.in $(srcdir)/sc68.texi
	$(TEXINFO2MAN) sc68.1.tpl $(srcdir)/sc68.texi >$@ || $(fail)
