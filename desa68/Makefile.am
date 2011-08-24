# ,----------------------------------------------------------------------.
# |                                                                      |
# |  desa68 -- Makefile.am -- Use with automake to produce configure.in  |
# |                                                                      |
# |                 (C) 2006-2009 Benjamin Gerard                        |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id$

ACLOCAL_AMFLAGS       = -I m4 --install
AM_CPPFLAGS           = $(ALL_EFLAGS) $(LIB_EFLAGS) -DDESA68_EXPORT=1
AM_CFLAGS             = $(ALL_CFLAGS) $(LIB_CFLAGS)
AM_LDFLAGS            = $(ALL_LFLAGS) $(LIB_LFLAGS) $(LFLAGS)
LFLAGS                = -avoid-version $(NOINSTALL)

# ,----------------------------------------------------------------------.
# | pkg-config                                                           |
# `----------------------------------------------------------------------'

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = desa68.pc

# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

lib_LTLIBRARIES       = libdesa68.la
libdesa68_la_SOURCES  = desa68.c

# ,----------------------------------------------------------------------.
# | Installed headers                                                    |
# `----------------------------------------------------------------------'

include_HEADERS       = desa68.h
