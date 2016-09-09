# ,----------------------------------------------------------------------.
# |                                                                      |
# |  mksc68 -- Makefile.am -- Use with automake to produce configure.in  |
# |                                                                      |
# |               Copyright (c) 2009-2016 Benjamin Gerard                |
# |                                                                      |
# |               <http://sourceforge.net/users/benjihan>                |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

ACLOCAL_AMFLAGS = -I m4 --install
desa68_dir      = @desa68_builddir@
file68_dir      = @file68_builddir@
libsc68_dir        = @libsc68_builddir@

# ,----------------------------------------------------------------------.
# | Libtool program                                                      |
# `----------------------------------------------------------------------'

if SOURCE_LIBSC68
LIBSC68_LA = $(libsc68_dir)/libsc68.la
@SET_MAKE@
$(libsc68_dir)/libsc68.la:
	cd "$(libsc68_dir)" && $(MAKE) $(AM_MAKEFLAGS)
else
LIBSC68_LA =
endif

bin_PROGRAMS = mksc68
mksc68_SOURCES = src/mksc68.c src/mksc68_cli.c src/mksc68_cli.h		\
 src/mksc68_cmd.c src/mksc68_cmd.h src/mksc68_cmd_extract.c		\
 src/mksc68_cmd_gdb.c src/mksc68_cmd_info.c src/mksc68_cmd_load.c	\
 src/mksc68_cmd_msg.c src/mksc68_cmd_new.c src/mksc68_cmd_play.c	\
 src/mksc68_cmd_save.c src/mksc68_cmd_tag.c src/mksc68_cmd_time.c	\
 src/mksc68_def.h src/mksc68_dsk.c src/mksc68_dsk.h src/mksc68_eva.h	\
 src/mksc68_gdb.c src/mksc68_gdb.h src/mksc68_msg.c src/mksc68_msg.h	\
 src/mksc68_oplen.h src/mksc68_opt.c src/mksc68_opt.h			\
 src/mksc68_snd.c src/mksc68_snd.h src/mksc68_str.c src/mksc68_str.h	\
 src/mksc68_tag.c src/mksc68_tag.h

mksc68_CFLAG    = $(libsc68_CFLAGS) $(gb_CFLAGS)
mksc68_CPPFLAGS = $(libsc68_CPPFLAGS)
mksc68_LDADD    = $(LIBSC68_LA)
mksc68_LDFLAGS  = $(LIBSC68_LIBS) $(gb_LDFLAGS)

# ,----------------------------------------------------------------------.
# | Documentation                                                        |
# `----------------------------------------------------------------------'

# dist_man1_MANS       = mksc68.man

# info_TEXINFOS        = mksc68.texi
# MAINTAINERCLEANFILES = $(srcdir)/mksc68.1

# TEXINFO2MAN          = $(SHELL) $(top_srcdir)/missing --run texinfo2man
# fail                 = { test -e '$@' && rm '$@'; test a = b; }

# $(srcdir)/mksc68.1: mksc68.1.tpl.in $(srcdir)/mksc68.texi
# 	$(TEXINFO2MAN) mksc68.1.tpl $(srcdir)/mksc68.texi >$@ || $(fail)
