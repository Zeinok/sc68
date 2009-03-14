# ,----------------------------------------------------------------------.
# | unice68 -- Makefile.am -- Use with automake to produce configure.in  | 
# |                                                                      |
# |                 (C) 2007-2009 Benjamin Gerard                        |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id$

ACLOCAL_AMFLAGS = -I m4 --install

# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

lib_LTLIBRARIES        = libunice68.la
libunice68_la_SOURCES  = unice68_native.c
libunice68_la_CFLAGS   = $(ALL_EFLAGS) $(LIB_CFLAGS)
libunice68_la_CPPFLAGS = $(ALL_EFLAGS) $(LIB_EFLAGS) -DUNICE68_EXPORT=1
libunice68_la_LDFLAGS  = $(ALL_LFLAGS) $(LIB_LFLAGS) $(LFLAGS)
LFLAGS                 = -avoid-version $(NOINSTALL)

# ,----------------------------------------------------------------------.
# | Libtool binary                                                       |
# `----------------------------------------------------------------------'

if UNICE68_CLI

bin_PROGRAMS     = unice68
unice68_SOURCES  = unice68.c
unice68_LDADD    = libunice68.la
unice68_LDFLAGS  = $(ALL_EFLAGS) $(BIN_LFLAGS)
unice68_CFLAGS   = $(ALL_EFLAGS) $(BIN_CFLAGS)
unice68_CPPFLAGS = $(ALL_LFLAGS) $(BIN_EFLAGS)

endif

# ,----------------------------------------------------------------------.
# | Installed headers                                                    |
# `----------------------------------------------------------------------'

include_HEADERS      = unice68.h

# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

dist_man_MANS        = unice68.1
info_TEXINFOS        = unice68.texi
MAINTAINERCLEANFILES = $(srcdir)/unice68.1

TEXINFO2MAN          = $(texinfo2man)
fail                 = { test -e '$@' && rm '$@'; test a = b; }

$(srcdir)/unice68.1: unice68.1.tpl.in $(srcdir)/unice68.texi
	$(TEXINFO2MAN) unice68.1.tpl $(srcdir)/unice68.texi >$@ || $(fail)
