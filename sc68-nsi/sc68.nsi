# -*- mode: conf; indent-tabs-mode: nil -*-
#
# @file   sc68.nsi
# @author https://sourceforge.net/users/benjihan
# @brief  Installer for sc68
#
# Time-stamp: <2013-07-30 19:04:33 ben>
# Init-stamp: <2013-07-29 04:43:45 ben>

#--------------------------------
!include "FileFunc.nsh"
!include "x64.nsh"
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
SetOverwrite  On

Var /GLOBAL VlcDir              # VLC base install dir
Var /GLOBAL WmpDir              # Winamp base install dir
Var /GLOBAL FooDir              # Foobar2000 base install dir

Var /GLOBAL VlcFile             # Installed VLC plugin
Var /GLOBAL WmpFile             # Installed Winamp plugin
Var /GLOBAL FooFile             # Installed Foobar2000 plugin

var /GLOBAL Win                 # either "win32" or "win64"

# ----------------------------------------------------------------------
#
# Functions and Macros to locate installation directories
#
# ----------------------------------------------------------------------

Function FindPreviousInstall
    ReadRegStr $0 HKLM ${HKLM_SC68} "Install_Dir"
    StrCmp "" $0 +2 0
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
# Licence
#
# ----------------------------------------------------------------------

LicenseBkColor /windows
LicenseText "license"
LicenseData ${SRCDIR}\..\COPYING
LicenseForceSelection checkbox

# ----------------------------------------------------------------------
#
# Pages
#
# ----------------------------------------------------------------------

Page license
Page components
Page directory
Page instfiles

# ----------------------------------------------------------------------
#
# Plugins Sections
#
# ----------------------------------------------------------------------

SectionGroup /e "!sc68 plugins"

# WINAMP
Section "sc68 for winamp" s_wmp
    file /oname=$WmpFile ${WMPDLL}
SectionEnd

Section "sc68 for vlc" s_vlc
    file /oname=$VlcFile ${VLCDLL}
SectionEnd

Section "sc68 for foobar2000" s_foo
    file /oname=$FooFile ${FOODLL}
SectionEnd

SectionGroupEnd

# ----------------------------------------------------------------------
#
# Hidden Section to select outpath
#
# ----------------------------------------------------------------------

Section "-outpath"
    SetOutPath $INSTDIR
SectionEnd

# ----------------------------------------------------------------------
#
# CLI Sections
#
# ----------------------------------------------------------------------

SectionGroup "sc68 cli programs (win32)" s0_32

# ----------------------------------------------------------------------

Section "sc68 cli player" s1_32
    File "${WIN32DIR}\bin\sc68.exe"
SectionEnd

Section "ICE packer and depacker" s2_32
    File "${WIN32DIR}\bin\unice68.exe"
SectionEnd

Section "Info on sc68 files" s3_32
    File "${WIN32DIR}\bin\info68.exe"
SectionEnd

Section "Make sc68 file" s4_32
    File "${WIN32DIR}\bin\mksc68.exe"
SectionEnd

SectionGroupEnd

# ----------------------------------------------------------------------

SectionGroup "sc68 cli programs (win64)" s0_64

Section "sc68 cli player (w64)"  s1_64
    File "${WIN64DIR}\bin\sc68.exe"
SectionEnd

Section "ICE packer and depacker (w64)" s2_64
    File "${WIN64DIR}\bin\unice68.exe"
SectionEnd

Section "Info on sc68 files (w64)" s3_64
    File "${WIN64DIR}\bin\info68.exe"
SectionEnd

Section "Make sc68 file (w64)" s4_64
    File "${WIN64DIR}\bin\mksc68.exe"
SectionEnd

SectionGroupEnd

# ----------------------------------------------------------------------

SectionGroup "sc68 config and data"

Section "sc68 music replay"
    File /r "${DATADIR}\Replay"
SectionEnd

Section "sc68 config file"
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

# ----------------------------------------------------------------------
#
# Callbacks
#
# ----------------------------------------------------------------------

Function DisableSection
  push $1
  push $2
  IntOp $2 ${SF_SELECTED} ~
  SectionGetFlags $0 $1
  IntOp $1 $1 & $2
  IntOp $1 $1 | ${SF_RO}
  SectionSetFlags $0 $1
  pop $1
FunctionEnd

Function .onInit
    StrCpy $INSTDIR "$PROGRAMFILES32\sc68"
    StrCpy $Win "win32"
    ${If} ${RunningX64}
        StrCpy $Win "win64"
        StrCpy $INSTDIR "$PROGRAMFILES64\sc68"
    ${EndIf}
    Call FindPreviousInstall
    !insertmacro FindPlugins

    # Check and disable individual plugin sections
    StrCpy $0 ${s_wmp}
    StrCmp "" $WmpFile 0 +2
    call DisableSection

    StrCpy $0 ${s_vlc}
    StrCmp "" $VlcFile 0 +2
    call DisableSection

    StrCpy $0 ${s_foo}
    StrCmp "" $FooFile 0 +2
    call DisableSection

    StrCmp $win "win32" +3 0
    push ${s0_32}
    goto +2
    push ${s0_64}
    pop $0
    call DisableSection
    IntOp $0 $0 + 1
    call DisableSection
    IntOp $0 $0 + 1
    call DisableSection
    IntOp $0 $0 + 1
    call DisableSection
    IntOp $0 $0 + 1
    call DisableSection

FunctionEnd

Function un.onInit
    !insertmacro FindPlugins
FunctionEnd
