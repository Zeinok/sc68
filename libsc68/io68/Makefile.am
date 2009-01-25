# ,----------------------------------------------------------------------.
# |   io68 -- Makefile.am -- Use with automake to produce configure.in   | 
# |                                                                      |
# |                 (C) 1998-2009 Benjamin Gerard                        |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id$

ACLOCAL_AMFLAGS    = -I ../m4 --install
AM_CFLAGS          = @LIBSC68_CFLAGS@ @LIBLIBSC68_CFLAGS@
AM_CPPFLAGS        = @LIBSC68_CPPFLAGS@ @LIBLIBSC68_CPPFLAGS@ $(SINC)
SINC               = -DSINCSTART -I$(top_srcdir) $(file68_inc) -DSINCEND

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

srcextra=\
 ym_atarist_table.c\
 ym_linear_table.c
myextra=$(srcextra)


# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

noinst_LTLIBRARIES  = libio68.la
libio68_la_SOURCES  = $(mysources) $(myheaders)
libio68_la_LDFLAGS  = -static -avoid-version -no-install
noinst_HEADERS      = $(myextra)
