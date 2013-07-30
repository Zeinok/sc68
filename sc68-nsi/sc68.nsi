# -*- mode: conf; indent-tabs-mode: nil -*-
#
# @file   sc68.nsi
# @author https://sourceforge.net/users/benjihan
# @brief  Installer for sc68
#
# Time-stamp: <2013-07-30 06:33:22 ben>
# Init-stamp: <2013-07-29 04:43:45 ben>

#--------------------------------
!include "FileFunc.nsh"
!insertmacro GetParent
#--------------------------------

!Define HKLM_SASHIPA "SOFTWARE\sashipa"
!Define HKLM_SC68    "${HKLM_SASHIPA}\sc68"

Name          "${NAME}"
Caption       "${CAPTION}"
Icon          "${SRCDIR}/sc68-icon-32.ico"
UninstallIcon "${SRCDIR}/sc68-icon-32.ico"
OutFile       "${OUTFILE}"
CRCCheck      On
SetCompress   force
SetCompressor /SOLID lzma
InstProgressFlags smooth colored

Var /GLOBAL VlcDir              # VLC base install dir
Var /GLOBAL WmpDir              # Winamp base install dir
Var /GLOBAL FooDir              # Foobar2000 base install dir

Var /GLOBAL VlcFile             # Installed VLC plugin
Var /GLOBAL WmpFile             # Installed Winamp plugin
Var /GLOBAL FooFile             # Installed Foobar2000 plugin

# InstType "Full"
# InstType "Cli only"
# InstType "Plugins only"

# ----------------------------------------------------------------------
#
# Functions and Macros to locate installation directories
#
# ----------------------------------------------------------------------

Function FindPreviousInstall
    ReadRegStr $0 HKLM ${HKLM_SC68} "Install_Dir"
    StrCMp "" $0 +2 0
    StrCpy $INSTDIR $0
FunctionEnd

!macro FindWinamp
    ReadRegStr $WmpDir HKCU "Software\Winamp" ""
    StrCpy $WmpFile ""
    IfFileExists "$WmpDir\winamp.exe" 0 +2
    StrCpy $WmpFile "$WmpDir\Plugins\in_sc68.dll"
!macroend

!macro FindFoobar
    ReadRegStr $FooDir HKLM "SOFTWARE\foobar2000" "InstallDir"
    StrCpy $FooFile ""
    IfFileExists "$FooDir\foobar2000.exe" 0 +2
    StrCpy $FooFile "$FooDir\components\foo_sc68.dll"
!macroend

!macro FindVlc
    ReadRegStr $VlcDir HKLM "SOFTWARE\VideoLAN\VLC" "InstallDir"
    StrCpy $VlcFile ""
    IfFileExists "$VlcDir\vlc.exe" 0 +2
    StrCpy $VlcFile "$VlcDir\plugins\demux\libsc68_plugin.dll"
!macroend

!macro FindPlugins
    !insertmacro FindWinamp
    !insertmacro FindVlc
    !insertmacro FindFoobar
!macroend

# ----------------------------------------------------------------------
#
# Callbacks
#
# ----------------------------------------------------------------------

Function .onInit
   StrCpy $INSTDIR "$PROGRAMFILES\sc68"
   Call FindPreviousInstall
   !insertmacro FindPlugins
FunctionEnd

Function un.onInit
   !insertmacro FindPlugins
FunctionEnd


# ----------------------------------------------------------------------
#
# Licence
#
# ----------------------------------------------------------------------

LicenseBkColor /windows
LicenseText "license"
LicenseData ${SRCDIR}\..\COPYING
LicenseForceSelection checkbox

#--------------------------------

# ----------------------------------------------------------------------
#
# Pages
#
# ----------------------------------------------------------------------

#Page license
Page components
Page directory
Page instfiles

# ----------------------------------------------------------------------
#
# Plugins Sections
#
# ----------------------------------------------------------------------

SectionGroup "!sc68 plugins"

# WINAMP
Section "sc68 for winamp"
    SectionIn 1 3
    SetOverwrite on
    SetOutPath "${WmpDir}\Plugins"
    File /nonfatal "${WMPFILE}"
    StrCpy $0 "${WmpDir}\Plugins\${WMPFILE}"
    IfFileExists $0 0 +2
    WriteRegStr HKLM ${HKLM_SC68} "winamp_plugin" $0
SectionEnd

Section "sc68 for vlc"
    SectionIn 1 3
    SetOverwrite on
    SetOutPath $VlcDir
    SetOutPath "$WmpDir\plugins\demux"
    File /nonfatal "${VLCFILES}"
SectionEnd

Section "sc68 for foobar2000"
    SetOverwrite on
    SetOutPath $FooDir
    SetOutPath "$FooDir\components"
    File /nonfatal "${FOOFILES}"
SectionEnd

SectionGroupEnd

# ----------------------------------------------------------------------
#
# CLI Sections
#
# ----------------------------------------------------------------------

SectionGroup "sc68 command line programs"

Section "sc68 command line player"
    SetOutPath $INSTDIR
    File "${BINDIR}\sc68.exe"
    File /nonfatal "${BINDIR}\*68.dll"
SectionEnd

Section "ICE packer and depacker"
    SetOutPath $INSTDIR
    File "${BINDIR}\unice68.exe"
    File /nonfatal "${BINDIR}\unice68.dll"
SectionEnd

Section "Make sc68 file"
    SetOutPath $INSTDIR
    File /nonfatal "${BINDIR}\mksc68.exe"
    File /nonfatal "${BINDIR}\*68.dll"
SectionEnd

SectionGroupEnd

SectionGroup "sc68 config and data"

Section "sc68 music replay"
    SetOutPath $INSTDIR
    File /r "${DATADIR}\Replay"
SectionEnd

Section "sc68 config file"
    SetOutPath $INSTDIR
    File "${DATADIR}\sc68.cfg"
SectionEnd

SectionGroupEnd


# ----------------------------------------------------------------------
#
# Registry Section
#
# ----------------------------------------------------------------------

Section "-registry"
    SectionIn  1 2 3 RO

    DetailPrint "VLC plugin is  '$VlcFile'"
    DetailPrint "Winamp plugin is '$WmpFile'"
    DetailPrint "Foobar2000 plugin in '$FooFile'"

    ## Write config keys for this programs
    WriteRegStr HKLM ${HKLM_SC68} "Install_Dir" "$INSTDIR"

    ## Write the uninstall keys for Windows
    
    WriteRegStr HKLM \
        "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\sc68" \
        "DisplayName" \
        "sc68"

    WriteRegStr HKLM \
        "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\sc68" \
        "UninstallString" \
        '"$INSTDIR\${UNINSTALL}"'

    WriteRegStr HKLM \
        "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\sc68" \
        "DisplayIcon" \
        '"$INSTDIR\${UNINSTALL}"'
        
    WriteRegDWORD HKLM \
        "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\sc68" \
        "NoModify" 1
        
    WriteRegDWORD HKLM \
        "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\sc68" \
        "NoRepair" 1
    
    WriteUninstaller "${UNINSTALL}"
  
SectionEnd

# ----------------------------------------------------------------------
#
# Uninstall Section
#
# ----------------------------------------------------------------------

Section "Uninstall"

    ## Remove registry config keys

    DeleteRegKey HKLM ${HKLM_SC68}
    DeleteRegKey /ifempty HKLM ${HKLM_SASHIPA}

    ## Remove registry uninstaller keys
    
    DeleteRegKey HKLM \
        "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\sc68"

    ## Remove files

    StrCmp "" $VlcFile +2 0
    Delete $VlcFile

    StrCmp "" $WmpFile +2 0
    Delete $WmpFile

    StrCmp "" $FooFile +2 0
    Delete $FooFile

    StrCmp "" $INSTDIR +2 0
    RMDir /r $INSTDIR

SectionEnd
