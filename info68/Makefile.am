# ,----------------------------------------------------------------------.
# |                                                                      |
# |  info68 -- Makefile.am -- Use with automake to produce Makefile.in   |
# |                                                                      |
# |               Copyright (c) 2006-2016 Benjamin Gerard                |
# |                                                                      |
# |               <http://sourceforge.net/users/benjihan>                |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

ACLOCAL_AMFLAGS = -I m4 --install

# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_FILE68
FILE68_LA = $(file68_builddir)/libfile68.la
@SET_MAKE@
$(FILE68_LA):
	cd "$(file68_builddir)" && $(MAKE) $(AM_MAKEFLAGS)
else
FILE68_LA =
endif

bin_PROGRAMS    = info68
info68_SOURCES  = info68.c
info68_CFLAGS   = $(file68_CFLAGS) $(gb_CFLAGS)
info68_CPPFLAGS = $(file68_CPPFLAGS)
info68_LDFLAGS  = $(file68_LIBS) $(gb_LDFLAGS)
info68_LDADD    = $(FILE68_LA)

# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

# dist_man1_MANS        = info68.man

# info_TEXINFOS        = info68.texi
# MAINTAINERCLEANFILES = $(srcdir)/info68.1

# TEXINFO2MAN          = $(texinfo2man)
# fail                 = { test -e '$@' && rm '$@'; test a = b; }

# $(srcdir)/info68.1: info68.1.tpl.in $(srcdir)/info68.texi
#       $(TEXINFO2MAN) info68.1.tpl $(srcdir)/info68.texi >$@ || $(fail)
