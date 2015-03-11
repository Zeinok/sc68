# ,----------------------------------------------------------------------.
# |                                                                      |
# |   winui -- Makefile.am -- Use with automake to produce Makefile.in   | 
# |                                                                      |
# |               Copyright (C) 2007-2015 Benjamin Gerard                |
# |                                                                      |
# |               <http://sourceforge.net/users/benjihan>                |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

ACLOCAL_AMFLAGS = -I m4 --install
AM_CFLAGS       = $(ALL_CFLAGS) $(LIB_CFLAGS) -mwin32
AM_CPPFLAGS     = -I$(top_srcdir)

# ,----------------------------------------------------------------------.
# | Libtool library                                                      |
# `----------------------------------------------------------------------'

noinst_LTLIBRARIES  = libwinui.la
libwinui_la_SOURCES = winui_dlg.c resource.rc
libwinui_la_LDFLAGS = -avoid-version

if dll_winui
EXTRA_DIST          = winui_dll.c
endif

.rc.lo: 
	$(LIBTOOL) --tag=RC --mode=compile $(RC) $< -o $@

noinst_HEADERS      = resource.h
