# ,----------------------------------------------------------------------.
# |                                                                      |
# | sc68-doc -- Makefile.am -- Use with automake to produce Makefile.in  |
# |                                                                      |
# |                 (C) 2007-2013 Benjamin Gerard                        |
# |          <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@

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
