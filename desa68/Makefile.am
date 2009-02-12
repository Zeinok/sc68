# ,----------------------------------------------------------------------.
# |                                                                      |
# |  desa68 -- Makefile.am -- Use with automake to produce configure.in  | 
# |                                                                      |
# |		    (C) 2006-2009 Benjamin Gerard                        |
# |	     <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id$

ACLOCAL_AMFLAGS = -I m4 --install
AM_CPPFLAGS     = @DESA68_CPPFLAGS@ @LIBDESA68_CPPFLAGS@
AM_CFLAGS       = @DESA68_CFLAGS@ @LIBDESA68_CFLAGS@ 
AM_LDFLAGS      = @DESA68_LDFLAGS@ @LIBDESA68_LDFLAGS@

# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

lib_LTLIBRARIES       = libdesa68.la
libdesa68_la_SOURCES  = desa68.c

# ,----------------------------------------------------------------------.
# | Installed headers                                                    |
# `----------------------------------------------------------------------'

include_HEADERS       = desa68.h
