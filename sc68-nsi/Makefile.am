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

MAKENSIS	= @makensis@
installer_exe   = $(PACKAGE_TARNAME)-@sc68_version@.exe
uninstall_exe   = $(PACKAGE_TARNAME)-uninstall.exe

# noinst_DATA = $(installer_exe)

all: $(installer_exe)

$(installer_exe): $(srcdir)/sc68.nsi Makefile
	"$(MAKENSIS)" \
	/DNAME="@PACKAGE_NAME@" \
	/DCAPTION="SC68" \
	/DVERSION="@sc68_version@" \
	/DOUTFILE=`$(CYGPATH_W) "$${PWD}/$@"` \
	/DUNINSTALL="$(uninstall_exe)" \
	/DSRCDIR=`$(CYGPATH_W) "$(srcdir)"` \
	/DWIN32DIR=`$(CYGPATH_W) "$(win32_dir)"` \
	/DWIN64DIR=`$(CYGPATH_W) "$(win64_dir)"` \
	/DDATADIR=`$(CYGPATH_W) "$(sc68_data)"` \
	/DWMPDLL=`$(CYGPATH_W) "$(winamp_dll)"` \
	/DVLCDLL=`$(CYGPATH_W) "$(vlc_dll)"` \
	/DFOODLL=`$(CYGPATH_W) "$(fb2k_dll)"` \
	-- `$(CYGPATH_W) "$<"`

clean-local:
	-rm -f "$(installer_exe)" "$(uninstall_exe)"
