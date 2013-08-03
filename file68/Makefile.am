# Time-stamp: <2013-08-03 14:33:10 ben>
# ,----------------------------------------------------------------------.
# |                                                                      |
# |  file68 -- Makefile.am -- Use with automake to produce configure.in  |
# |                                                                      |
# |                 (C) 2007-2011 Benjamin Gerard                        |
# |                                                                      |
# |            <http://sourceforge.net/users/benjihan>                   |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

SUBDIRS         = data68 $(subdirs)
ACLOCAL_AMFLAGS = -I m4 --install

AM_CFLAGS       = $(ALL_CFLAGS) $(LIB_CFLAGS)
AM_LDFLAGS      = $(ALL_LFLAGS) $(LIB_LFLAGS) -version-info 3:0:0
AM_CPPFLAGS     = -DFILE68_EXPORT=1 -I$(top_srcdir)/sc68 $(unice68_inc)

sdir=src
hdir=sc68
null=

if REPLAY68
replay68_c=$(sdir)/replay68.c
else
replay68_c=
endif

mysources = $(sdir)/error68.c $(sdir)/file68.c $(sdir)/gzip68.c		\
 $(sdir)/ice68.c $(sdir)/init68.c $(sdir)/vfs68.c $(sdir)/vfs68_ao.c	\
 $(sdir)/vfs68_curl.c $(sdir)/vfs68_fd.c $(sdir)/vfs68_file.c		\
 $(sdir)/vfs68_mem.c $(sdir)/vfs68_null.c $(sdir)/vfs68_z.c		\
 $(sdir)/msg68.c $(sdir)/option68.c $(sdir)/registry68.c		\
 $(sdir)/rsc68.c $(sdir)/string68.c $(sdir)/timedb68.c			\
 $(sdir)/uri68.c $(replay68_c) $(null)

apiheaders = $(hdir)/file68_chk.h $(hdir)/file68_ord.h			\
 $(hdir)/file68_err.h $(hdir)/file68.h $(hdir)/file68_api.h		\
 $(hdir)/file68_features.h $(hdir)/file68_vfs.h				\
 $(hdir)/file68_vfs_def.h $(hdir)/file68_msg.h $(hdir)/file68_opt.h	\
 $(hdir)/file68_reg.h $(hdir)/file68_rsc.h $(hdir)/file68_str.h		\
 $(hdir)/file68_tag.h $(hdir)/file68_uri.h $(hdir)/file68_tdb.h		\
 $(null)

otherheaders = $(hdir)/file68_zip.h $(hdir)/file68_ice.h		\
 $(hdir)/file68_vfs_ao.h $(hdir)/file68_vfs_curl.h			\
 $(hdir)/file68_vfs_fd.h $(hdir)/file68_vfs_file.h			\
 $(hdir)/file68_vfs_mem.h $(hdir)/file68_vfs_null.h			\
 $(hdir)/file68_vfs_z.h $(sdir)/timedb.inc.h $(sdir)/replay.inc.h	\
 $(null)

myheaders = $(apiheaders) $(otherheaders)

# ,----------------------------------------------------------------------.
# | pkg-config                                                           |
# `----------------------------------------------------------------------'

pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = file68.pc

# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

if SOURCE_UNICE68
UNICE68_LA           = $(unice68_dir)/libunice68.la
@SET_MAKE@
$(unice68_dir)/libunice68.la:
	$(MAKE) -C $(unice68_dir) $(AM_MAKEFLAGS) libunice68.la
else
UNICE68_LA           =
endif

lib_LTLIBRARIES      = libfile68.la
libfile68_la_SOURCES = $(mysources) $(myheaders)
libfile68_la_LIBADD  = $(UNICE68_LA)

# ,----------------------------------------------------------------------.
# | Install headers                                                      |
# `----------------------------------------------------------------------'

pkginclude_HEADERS   = $(apiheaders)
pkgincludedir        = $(includedir)/sc68
