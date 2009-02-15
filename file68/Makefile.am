# ,----------------------------------------------------------------------.
# |  file68 -- Makefile.am -- Use with automake to produce configure.in  | 
# |                                                                      |
# |                 (C) 2007-2009 Benjamin Gerard                        |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id$

SUBDIRS         = data68 @subdirs@
ACLOCAL_AMFLAGS = -I m4 --install

AM_CFLAGS       = @FILE68_CFLAGS@ @LIBFILE68_CFLAGS@
AM_CPPFLAGS     = @FILE68_CPPFLAGS@ @LIBFILE68_CPPFLAGS@ $(SINC)
AM_LDFLAGS      = @FILE68_LDFLAGS@ @LIBFILE68_LDFLAGS@
SINC            = -I$(srcdir)/sc68 $(unice68_inc)

sdir=.
hdir=sc68

mysources=\
 $(sdir)/alloc68.c\
 $(sdir)/endian68.c\
 $(sdir)/error68.c\
 $(sdir)/file68.c\
 $(sdir)/gzip68.c\
 $(sdir)/ice68.c\
 $(sdir)/init68.c\
 $(sdir)/istream68.c\
 $(sdir)/istream68_ao.c\
 $(sdir)/istream68_curl.c\
 $(sdir)/istream68_fd.c\
 $(sdir)/istream68_file.c\
 $(sdir)/istream68_mem.c\
 $(sdir)/istream68_null.c\
 $(sdir)/istream68_pa.c\
 $(sdir)/istream68_z.c\
 $(sdir)/msg68.c\
 $(sdir)/option68.c\
 $(sdir)/registry68.c\
 $(sdir)/rsc68.c\
 $(sdir)/string68.c\
 $(sdir)/url68.c

myheaders=\
 $(hdir)/alloc68.h\
 $(hdir)/audio68.h\
 $(hdir)/endian68.h\
 $(hdir)/error68.h\
 $(hdir)/file68.h\
 $(hdir)/file68_api.h\
 $(hdir)/gzip68.h\
 $(hdir)/ice68.h\
 $(hdir)/init68.h\
 $(hdir)/istream68.h\
 $(hdir)/istream68_ao.h\
 $(hdir)/istream68_curl.h\
 $(hdir)/istream68_def.h\
 $(hdir)/istream68_fd.h\
 $(hdir)/istream68_file.h\
 $(hdir)/istream68_mem.h\
 $(hdir)/istream68_null.h\
 $(hdir)/istream68_pa.h\
 $(hdir)/istream68_z.h\
 $(hdir)/msg68.h\
 $(hdir)/option68.h\
 $(hdir)/registry68.h\
 $(hdir)/rsc68.h\
 $(hdir)/string68.h\
 $(hdir)/url68.h

# ,----------------------------------------------------------------------.
# | script binary                                                        |
# `----------------------------------------------------------------------'

bin_SCRIPTS          = file68-config


# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

if SOURCE_UNICE68
UNICE68_LA           = $(unice68_dir)/libunice68.la
@SET_MAKE@
$(unice68_dir)/libunice68.la:
	$(MAKE) -C $(unice68_dir) libunice68.la
else
UNICE68_LA           =
endif


lib_LTLIBRARIES      = libfile68.la
libfile68_la_SOURCES = $(mysources) $(myheaders)
libfile68_la_LIBADD  = $(UNICE68_LA)


# ,----------------------------------------------------------------------.
# | Install headers                                                      |
# `----------------------------------------------------------------------'

pkginclude_HEADERS   = $(myheaders)
pkgincludedir        = $(includedir)/sc68

# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

dist_man_MANS        = file68-config.1
MAINTAINERCLEANFILES = $(srcdir)/file68-config.1

#if MAINTAINER_MODE
HELP2MAN             = $(SHELL) $(top_srcdir)/missing --run help2man
fail                 = { test -e '$@' && rm '$@'; test a = b; }

$(srcdir)/file68-config.1: ./file68-config.in $(srcdir)/Makefile.am
	$(HELP2MAN) -N  -m 'sc68-tools' \
	'$(SHELL) ./file68-config' -o '$@' || $(fail)
#endif
