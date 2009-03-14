# ,----------------------------------------------------------------------.
# |                                                                      |
# | sc68-doc -- Makefile.am -- Use with automake to produce Makefile.in  |
# |                                                                      |
# |                 (C) 2007-2009 Benjamin Gerard                        |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# $Id: Makefile.am 60 2009-02-01 13:01:52Z benjihan $

ACLOCAL_AMFLAGS      = -I m4 --install

# ,----------------------------------------------------------------------.
# | Doxygen                                                              |
# `----------------------------------------------------------------------'

all: all-am dox-all

include Makefile-doc.in 

# CLEANFILES           =
# DISTCLEANFILES       =
# MAINTAINERCLEANFILES =

clean-local: dox-clean-local

# ,----------------------------------------------------------------------.
# | Extra distrib                                                        |
# `----------------------------------------------------------------------'

EXTRA_DIST           = sc68_doc.h
