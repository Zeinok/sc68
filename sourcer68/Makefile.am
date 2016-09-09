# ,----------------------------------------------------------------------.
# |                                                                      |
# | sourcer68 -- Makefile.am -- Use with automake to produce configure.in|
# |                                                                      |
# |               Copyright (c) 2006-2016 Benjamin Gerard                |
# |                                                                      |
# |               <http://sourceforge.net/users/benjihan>                |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

ACLOCAL_AMFLAGS = -I m4 --install
desa68_dir      = @desa68_builddir@
file68_dir      = @file68_builddir@

# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

DESA68_LA =
if SOURCE_DESA68
DESA68_LA += $(desa68_dir)/libdesa68.la
@SET_MAKE@
$(desa68_dir)/libdesa68.la:
	cd "$(desa68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libdesa68.la
endif

FILE68_LA =
if SOURCE_FILE68
FILE68_LA += $(file68_dir)/libfile68.la
@SET_MAKE@
$(file68_dir)/libfile68.la:
	 cd "$(file68_dir)" && $(MAKE) $(AM_MAKEFLAGS) libfile68.la
endif

bin_PROGRAMS = sourcer68

sourcer68_SOURCES = src/sourcer68.c src/src68_adr.c src/src68_adr.h     \
 src/src68_def.h src/src68_dis.c src/src68_dis.h src/src68_eva.c        \
 src/src68_eva.h src/src68_exe.c src/src68_exe.h src/src68_fmt.c        \
 src/src68_fmt.h src/src68_mbk.c src/src68_mbk.h src/src68_msg.c        \
 src/src68_msg.h src/src68_opt.c src/src68_opt.h src/src68_rel.c        \
 src/src68_rel.h src/src68_sec.c src/src68_sec.h src/src68_src.c        \
 src/src68_src.h src/src68_sym.c src/src68_sym.h src/src68_tos.c        \
 src/src68_tos.h src/src68_vec.c src/src68_vec.h
sourcer68_CFLAGS   = $(desa68_CFLAGS) $(file68_CFLAGS) $(gb_CFLAGS)
sourcer68_CPPFLAGS = $(desa68_CPPFLAGS) $(file68_CPPFLAGS)
sourcer68_LDFLAGS  = $(desa68_LIBS) $(file68_LIBS) $(gb_LDFLAGS)
sourcer68_LDADD    = $(FILE68_LA) $(DESA68_LA)

# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

# dist_man1_MANS       = sourcer68.man
# info_TEXINFOS        = sourcer68.texi
# MAINTAINERCLEANFILES = $(srcdir)/sourcer68.1

# TEXINFO2MAN          = $(texinfo2man)
# fail                 = { test -e '$@' && rm '$@'; test a = b; }

# $(srcdir)/sourcer68.1: sourcer68.1.tpl.in $(srcdir)/sourcer68.texi
#       $(TEXINFO2MAN) sourcer68.1.tpl $(srcdir)/sourcer68.texi >$@ || $(fail)
