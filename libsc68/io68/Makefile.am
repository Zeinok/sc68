# ,----------------------------------------------------------------------.
# |   io68 -- Makefile.am -- Use with automake to produce configure.in   | 
# |                                                                      |
# |                      (C) 2007 Benjamin Gerard                        |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id: Makefile.am,v 1.2 2007/01/12 09:28:14 benjihan Exp $

ACLOCAL_AMFLAGS=-I ../../m4 --install

mysources=\
 io68.c\
 mfp_io.c\
 mfpemul.c\
 mw_io.c\
 mwemul.c\
 paula_io.c\
 paulaemul.c\
 shifter_io.c\
 ym_io.c\
 ymemul.c

myheaders=\
 default_option68.h\
 io68.h\
 mfp_io.h\
 mfpemul.h\
 mw_io.h\
 mwemul.h\
 paula_io.h\
 paulaemul.h\
 shifter_io.h\
 ym_fixed_vol.h\
 ym_io.h\
 ymemul.h\
 ymout1c5bit.h\
 ymoutorg.h

myincludes=\
 ym_atarist_table.c\
 ym_linear_table.c

# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

noinst_LTLIBRARIES  = libio68.la
libio68_la_SOURCES  = $(mysources) $(myheaders)
noinst_HEADERS      = $(myincludes)

libio68_la_CPPFLAGS = -I$(srcdir)/..
libio68_la_LDFLAGS  = -static -avoid-version -no-install
