# ,----------------------------------------------------------------------.
# |                                                                      |
# |  sc68aud -- Makefile.am -- Use with automake to produce Makefile.in  | 
# |                                                                      |
# |                 (C) 2010-2014 Benjamin Gerard                        |
# |                                                                      |
# |             <http://sourceforge.net/users/benjihan>                  |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

SUBDIRS              = $(subdirs)
ACLOCAL_AMFLAGS      = -I m4 --install
AM_CFLAGS            = $(ALL_CFLAGS) $(LIB_CFLAGS) $(BIN_CFLAGS)
AM_CPPFLAGS          = $(sc68_inc) $(file68_inc) 
AM_LDFLAGS           = $(ALL_LFLAGS) $(LIB_LFLAGS) $(BIN_LFLAGS) $(LFLAGS)

# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_SC68
SC68_LA = $(sc68_dir)/libsc68.la
@SET_MAKE@
$(sc68_dir)/libsc68.la:
	cd "$(sc68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libsc68.la
else
SC68_LA = $(sc68_lib) $(file68_lib)
endif

plugin_LTLIBRARIES = sc68.la
sc68_la_SOURCES    = sc68-audacious.c
sc68_la_LIBADD     = $(SC68_LA)
LFLAGS             = -module -avoid-version $(NOINSTALL)
