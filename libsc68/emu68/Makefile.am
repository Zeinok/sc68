# ,----------------------------------------------------------------------.
# |  emu68 -- Makefile.am -- Use with automake to produce configure.in   | 
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
 cc68.c\
 emu68.c\
 error68.c\
 getea68.c\
 inst68.c\
 ioplug68.c\
 line0_68.c line1_68.c line2_68.c line3_68.c\
 line4_68.c line5_68.c line6_68.c line7_68.c\
 line8_68.c line9_68.c lineA_68.c lineB_68.c\
 lineC_68.c lineD_68.c lineE_68.c lineF_68.c\
 mem68.c\
 table68.c

myheaders=\
 cc68.h\
 emu68.h\
 error68.h\
 excep68.h\
 getea68.h\
 inst68.h\
 ioplug68.h\
 macro68.h\
 mem68.h\
 srdef68.h\
 struct68.h\
 type68.h

srcextra=\
 lines/line0.c lines/line1.c lines/line2.c lines/line3.c\
 lines/line4.c lines/line5.c lines/line6.c lines/line7.c\
 lines/line8.c lines/line9.c lines/lineA.c lines/lineB.c\
 lines/lineC.c lines/lineD.c lines/lineE.c lines/lineF.c\
 lines/table.c
myextra=$(srcextra)


# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

noinst_LTLIBRARIES  = libemu68.la
libemu68_la_SOURCES = ${mysources} ${myheaders}
libemu68_la_LDFLAGS = -avoid-version -no-install
noinst_HEADERS      = $(myextra)
