# ,----------------------------------------------------------------------.
# |                                                                      |
# |  desa68 -- Makefile.am -- Use with automake to produce Makefile.in   | 
# |                                                                      |
# |               Copyright (c) 2007-2016 Benjamin Gerard                |
# |                                                                      |
# |               <http://sourceforge.net/users/benjihan>                |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

ACLOCAL_AMFLAGS       = -I m4 --install

# ,----------------------------------------------------------------------.
# | pkg-config                                                           |
# `----------------------------------------------------------------------'

pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = desa68.pc

# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

lib_LTLIBRARIES      = libdesa68.la
libdesa68_la_SOURCES = desa68.c
libdesa68_la_LDFLAGS = -avoid-version $(lib_LDFLAGS)

# ,----------------------------------------------------------------------.
# | Headers                                                              |
# `----------------------------------------------------------------------'

include_HEADERS = desa68.h
noinst_HEADERS  = desa68_private.h

# ,----------------------------------------------------------------------.
# | Distrib                                                              |
# `----------------------------------------------------------------------'

EXTRA_DIST = CMakeLists.txt cm_config.h.in
