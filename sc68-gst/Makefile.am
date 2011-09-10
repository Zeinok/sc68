# Time-stamp: <2011-09-10 13:52:24 ben>
# ,----------------------------------------------------------------------.
# |                                                                      |
# | gstsc68 -- Makefile.am -- Use with automake to produce configure.in  |
# |                                                                      |
# |                 (C) 2010-2011 Benjamin Gerard                        |
# |                                                                      |
# |            <http://sourceforge.net/users/benjihan>                   |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

SUBDIRS              = $(subdirs)
ACLOCAL_AMFLAGS      = -I m4 --install
AM_CFLAGS            = $(ALL_CFLAGS) $(LIB_CFLAGS) $(BIN_CFLAGS)
AM_CPPFLAGS          = $(sc68_inc) $(file68_inc)
AM_LDFLAGS           = $(ALL_LFLAGS) $(LIB_LFLAGS) $(BIN_LFLAGS) $(LFLAGS)

# ,----------------------------------------------------------------------.
# | Libtool plugin                                                       |
# `----------------------------------------------------------------------'

if SOURCE_SC68
SC68_LA = $(sc68_dir)/libsc68.la
@SET_MAKE@
$(sc68_dir)/libsc68.la:
	cd "$(sc68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libsc68.la
else
SC68_LA = $(sc68_lib) $(file68_lib)
endif

plugin_LTLIBRARIES         = libgstsc68.la
libgstsc68_la_SOURCES      = gstsc68.c gstsc68_core.c
noinst_HEADERS             = gstsc68.h
libgstsc68_la_LIBADD       = $(SC68_LA)
libgstsc68_la_LIBTOOLFLAGS = --tag=disable-static
LFLAGS                     = -module -avoid-version -export-symbols-regex [_]*\(gst_\|Gst\|GST_\).* $(NOINSTALL)
