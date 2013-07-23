# Time-stamp: <2013-07-23 05:59:45 ben>
# ,----------------------------------------------------------------------.
# |                                                                      |
# |  unice68 -- Makefile.am -- Use with automake to produce Makefile.in  | 
# |                                                                      |
# |                 (C) 2007-2013 Benjamin Gerard                        |
# |                                                                      |
# |             <http://sourceforge.net/users/benjihan>                  |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

ACLOCAL_AMFLAGS = -I m4 --install

# ,----------------------------------------------------------------------.
# | pkg-config                                                           |
# `----------------------------------------------------------------------'

pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = unice68.pc

# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

lib_LTLIBRARIES        = libunice68.la
libunice68_la_SOURCES  = unice68_unpack.c unice68_pack.c unice68_version.c
libunice68_la_CFLAGS   = $(ALL_CFLAGS) $(LIB_CFLAGS)
libunice68_la_CPPFLAGS = -DUNICE68_EXPORT=1
libunice68_la_LDFLAGS  = $(ALL_LFLAGS) $(LIB_LFLAGS) $(LFLAGS)
LFLAGS                 = -avoid-version $(NOINSTALL)

# ,----------------------------------------------------------------------.
# | Libtool binary                                                       |
# `----------------------------------------------------------------------'

if UNICE68_CLI

bin_PROGRAMS     = unice68
unice68_SOURCES  = unice68.c
unice68_LDADD    = libunice68.la
unice68_LDFLAGS  = $(ALL_LFLAGS) $(BIN_LFLAGS)
unice68_CFLAGS   = $(ALL_CFLAGS) $(BIN_CFLAGS)

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
