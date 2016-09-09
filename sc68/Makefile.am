# ,----------------------------------------------------------------------.
# |                                                                      |
# |   sc68 -- Makefile.am -- Use with automake to produce Makefile.in    |
# |                                                                      |
# |               Copyright (c) 2007-2016 Benjamin Gerard                |
# |                                                                      |
# |               <http://sourceforge.net/users/benjihan>                |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

ACLOCAL_AMFLAGS = -I m4 --install
libsc68_dir     = @libsc68_builddir@

# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

LIBSC68_LA =
if SOURCE_LIBSC68
LIBSC68_LA += $(libsc68_dir)/libsc68.la
@SET_MAKE@
$(libsc68_dir)/libsc68.la:
	cd "$(libsc68_dir)" && $(MAKE) $(AM_MAKEFLAGS)
endif

FILE68_LA =
# if SOURCE_FILE68
# FILE68_LA += $(file68_dir)/libfile68.la
# @SET_MAKE@
# $(file68_dir)/libfile68.la:
#       $(MAKE) -C "$(file68_dir)" $(AM_MAKEFLAGS)
# endif

bin_PROGRAMS  = sc68
sc68_SOURCES  = sc68.c
sc68_CFLAGS   = $(libsc68_CFLAGS) $(gb_CFLAGS)
sc68_CPPFLAGS = $(libsc68_CPPFLAGS)
sc68_LDFLAGS  = $(libsc68_LIBS) $(gb_LDFLAGS)
sc68_LDADD    = $(LIBSC68_LA) $(FILE68_LA)

# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

# dist_man1_MANS       = sc68.man
# info_TEXINFOS        = sc68.texi
# MAINTAINERCLEANFILES = $(srcdir)/sc68.1

# TEXINFO2MAN          = $(texinfo2man)
# fail                 = { test -e '$@' && rm '$@'; test a = b; }

# $(srcdir)/sc68.1: sc68.1.tpl.in $(srcdir)/sc68.texi
#       $(TEXINFO2MAN) sc68.1.tpl $(srcdir)/sc68.texi >$@ || $(fail)
