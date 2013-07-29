# ,----------------------------------------------------------------------.
# |                                                                      |
# | sc68-nsi -- Makefile.am -- Use with automake to produce configure.in | 
# |                                                                      |
# |		    (C) 2007-2013 Benjamin Gerard                        |
# | 	     <benjihan -4t- users.sourceforge -d0t- net>                 |
# |                                                                      |
# `----------------------------------------------------------------------'
# @configure_input@
# Time-stamp: <2013-06-08 13:00:29 ben>

ACLOCAL_AMFLAGS = -I ../m4 --install
installer_exe   = $(PACKAGE_TARNAME)-$(PACKAGE_VERSION)$(EXEEXT)
uninstall_exe   = $(PACKAGE_TARNAME)-uninstall$(EXEEXT)

# noinst_DATA = $(installer_exe)

installer: $(installer_exe)

$(installer_exe): $(srcdir)/sc68.nsi
	"$(MAKENSIS)" \
	/DNAME="@PACKAGE_NAME@" \
	/DCAPTION="SC68" \
	/DVERSION="@PACKAGE_VERSION@" \
	/DOUTFILE=`$(CYGPATH_W) "$${PWD}/$@"` \
	/DUNINSTALL="$(uninstall_exe)" \
	/DSRCDIR=`$(CYGPATH_W) "$(srcdir)"` \
	/DBINDIR=`$(CYGPATH_W) "$(bindir)"` \
	/DLIBDIR=`$(CYGPATH_W) "$(libdir)"` \
	/DDATADIR=`$(CYGPATH_W) "$(datadir)/sc68"` \
	-- `$(CYGPATH_W) "$<"`

clean-local:
	-rm -f "$(installer_exe)" "$(uninstall_exe)"
