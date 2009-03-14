# ,----------------------------------------------------------------------.
# |                                                                      |
# |  debug68 -- Makefile.am -- Use with automake to produce Makefile.in  |
# |                                                                      |
# |                 (C) 2006-2009 Benjamin Gerard                        |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# $Id: Makefile.am 26 2009-01-28 08:11:43Z benjihan $
# @configure_input@

SUBDIRS              = $(subdirs)
ACLOCAL_AMFLAGS      = -I m4 --install

AM_CFLAGS            = $(ALL_CFLAGS) $(BIN_CFLAGS)
AM_CPPFLAGS          = $(ALL_EFLAGS) $(BIN_EFLAGS) $(EFLAGS)
AM_LDFLAGS           = $(ALL_LFLAGS) $(BIN_LFLAGS)
EFLAGS               = $(sc68_inc) $(file68_inc) $(desa68_inc)

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

if SOURCE_SC68
SC68_LA = $(sc68_dir)/libsc68.la
@SET_MAKE@
$(sc68_dir)/libsc68.la:
	cd "$(sc68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libsc68.la
else
SC68_LA =
endif

sources =\
 debug68.c\
 debug68_breakpoint.c\
 debug68_cli.c\
 debug68_debugcom.c\
 debug68_error.c\
 debug68_eval.c\
 debug68_io.c

header =\
 debug68_breakpoint.h\
 debug68_cli.h\
 debug68_debugcom.h\
 debug68_doc.h\
 debug68_error.h\
 debug68_eval.h\
 debug68_io.h

bin_PROGRAMS         = debug68
debug68_SOURCES      = $(sources) $(headers) 
debug68_LDADD        = $(SC68_LA) $(DESA68_LA)
noinst_HEADERS       = debug68_doc.h 

# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

dist_man_MANS        = debug68.1
info_TEXINFOS        = debug68.texi
MAINTAINERCLEANFILES = $(srcdir)/debug68.1

TEXINFO2MAN          = $(SHELL) $(top_srcdir)/missing --run texinfo2man
fail                 = { test -e '$@' && rm '$@'; test a = b; }

$(srcdir)/debug68.1: debug68.1.tpl.in $(srcdir)/debug68.texi
	$(TEXINFO2MAN) debug68.1.tpl $(srcdir)/debug68.texi >$@ || $(fail)
