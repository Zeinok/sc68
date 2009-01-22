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
AM_CFLAGS       = @UNICE68_CFLAGS@
AM_CPPFLAGS     = @UNICE68_CPPFLAGS@
AM_LDFLAGS      = @UNICE68_LDFLAGS@


# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

lib_LTLIBRARIES        = libunice68.la
libunice68_la_SOURCES  = unice68_native.c
libunice68_la_LDFLAGS  = @LIBUNICE68_LDFLAGS@ $(AM_LDFLAGS)
libunice68_la_CFLAGS   = @LIBUNICE68_CFLAGS@ $(AM_CFLAGS)
libunice68_la_CPPFLAGS = @LIBUNICE68_CPPFLAGS@ $(AM_CPPFLAGS)


# ,----------------------------------------------------------------------.
# | Libtool binary                                                       |
# `----------------------------------------------------------------------'

bin_PROGRAMS     = unice68
unice68_SOURCES  = unice68.c
unice68_LDADD    = libunice68.la
unice68_LDFLAGS  = @BINUNICE68_LDFLAGS@ $(AM_LDFLAGS)
unice68_CFLAGS   = @BINUNICE68_CFLAGS@ $(AM_CFLAGS)
unice68_CPPFLAGS = @BINUNICE68_CPPFLAGS@ $(AM_CPPFLAGS)


# ,----------------------------------------------------------------------.
# | Installed headers                                                    |
# `----------------------------------------------------------------------'

include_HEADERS      = unice68.h


# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

man_MANS             = unice68.1
info_TEXINFOS        = unice68.texi
unice68_TEXINFOS     = package.texi
MAINTAINERCLEANFILES = \
 $(srcdir)/unice68.info $(srcdir)/package.texi \
 $(srcdir)/unice68.1


if MAINTAINER_MODE
fail                 = { test -e '$@' && rm '$@'; test a = b; }

$(srcdir)/package.texi: $(srcdir)/package.texi.in
	$(SHELL) $(top_builddir)/config.status --file=$@:$<

$(srcdir)/unice68.1: unice68.1.tpl $(srcdir)/unice68.texi
	@texinfo2man@ $^ >$@ || $(fail)

endif


# ,----------------------------------------------------------------------.
# | Extra distrib                                                        |
# `----------------------------------------------------------------------'

EXTRA_DIST = unice68.1 unice68.info package.texi
