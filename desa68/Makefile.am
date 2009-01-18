# ,----------------------------------------------------------------------.
# |                                                                      |
# |  desa68 -- Makefile.am -- Use with automake to produce configure.in  | 
# |                                                                      |
# |		    (C) 2006-2009 Benjamin Gerard                        |
# |	     <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id: Makefile.am,v 1.2 2007/01/12 09:28:14 benjihan Exp $

ACLOCAL_AMFLAGS = -I m4 --install
AM_CPPFLAGS     = @DESA68_CPPFLAGS@ @LIBDESA68_CPPFLAGS@
AM_CFLAGS       = @DESA68_CFLAGS@ @LIBDESA68_CFLAGS@ 
AM_LDFLAGS      = @DESA68_LDFLAGS@ @LIBDESA68_LDFLAGS@

# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

lib_LTLIBRARIES       = libdesa68.la
libdesa68_la_SOURCES  = desa68.c
libdesa68_la_LDFLAGS  = $(AM_LDFLAGS)
libdesa68_la_CPPFLAGS = $(AM_CPPFLAGS)


# ,----------------------------------------------------------------------.
# | Installed headers                                                    |
# `----------------------------------------------------------------------'

include_HEADERS   = desa68.h
