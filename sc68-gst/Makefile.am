# Time-stamp: <2011-09-05 22:13:00 ben>
# ,----------------------------------------------------------------------.
# |                                                                      |
# | gstsc68 -- Makefile.am -- Use with automake to produce Makefile.in   |
# |                                                                      |
# |                  (C) 2007-2011 Benjamin Gerard                       |
# |           <benjihan -4t- users.sourceforge -d0t- net>                |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

SUBDIRS              = $(subdirs)
ACLOCAL_AMFLAGS      = -I m4 --install
AM_CFLAGS            = $(ALL_CFLAGS) $(LIB_CFLAGS) $(BIN_CFLAGS)
AM_CPPFLAGS          = $(ALL_EFLAGS) $(LIB_EFLAGS) $(BIN_EFLAGS) $(EFLAGS)
AM_LDFLAGS           = $(ALL_LFLAGS) $(LIB_LFLAGS) $(BIN_LFLAGS) $(LFLAGS)
EFLAGS               = $(sc68_inc) $(file68_inc)

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
