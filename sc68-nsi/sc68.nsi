; -*- conf -*-
;
; @file   sc68.nsi
; @author https://sourceforge.net/users/benjihan
; @brief  Installer for sc68
;
; Time-stamp: <2013-07-29 12:03:09 ben>
; Init-stamp: <2013-07-29 04:43:45 ben>

;--------------------------------
!include "FileFunc.nsh"
!insertmacro GetParent
;--------------------------------

Name          "${NAME}"
Caption       "${CAPTION}"
Icon          "${SRCDIR}/sc68-icon-32.ico"
UninstallIcon "${SRCDIR}/sc68-icon-32.ico"
OutFile       "${OUTFILE}"
CRCCheck      On
SetCompress   auto

Function CleanString
    ${GetParent} "$0" "$1"
    strcpy $2 $1 1
    StrCmp \" $2 noquote
    StrCpy $1 $1 "" 1
  noquote:
    StrCpy $0 $1
FunctionEnd

Function CheckDisKey
    pop "$3"
    pop "$2"
    pop "$1"
    StrCpy $0 ""
    Strcmp $1 "HKCR" 0 no_hkcr
    ReadRegStr $0 HKCR $2 $3
    Goto continue
  no_hkcr:
    Strcmp $1 "HKLM" 0 no_hklm
    ReadRegStr $0 HKLM $2 $3
  no_hklm:
  
  continue:
    StrCmp "" $0 0 found
    Return
  found:
    Call CleanString
FunctionEnd


Function FindWinamp
;
FunctionEnd

Function FindFoobar
; HKLM SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\foobar2000
FunctionEnd

Function FindVlc
; HKLM SOFTWARE\VideoLAN\VLC InstallDir
FunctionEnd


; Function FindSteamAccount

;     StrCpy $0 ""
    
;     ReadRegStr $0 HKLM \
;         "SOFTWARE\Team-Aces\aces-css-menu" \
;         "Install_Dir"
;     StrCmp "" $0 0 finish
    
;     ReadRegStr $0 HKLM \
;         "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App 240" \
;         "InstallLocation"
;     StrCmp "" $0 0 finish

;     ReadRegStr $0 HKCU \
;         "SOFTWARE\Valve\Steam" \
;         "ModInstallPath"
;     StrCmp "" $0 notfound 0
;     ${GetParent} "$0" "$0"
;     StrCpy $0 "$0\counter-strike source"
;     goto finish

;   notfound:
;     push "HKLM"
;     push "SOFTWARE\Classes\Valve.Source\shell\open\command"
;     push ""
;     Call CheckDisKey
;     StrCmp "" $0 0 finish

;     push "HKLM"
;     push "SOFTWARE\Classes\Applications\hl2.exe\shell\open\command"
;     push ""
;     Call CheckDisKey
;     StrCmp "" $0 0 finish
    
;     push "HKCR"
;     push "Applications\hl2.exe\shell\open\command"
;     push ""
;     Call CheckDisKey
;     StrCmp "" $0 0 finish

;     push "HKCR"
;     push "Valve.Source\shell\open\command"
;     push ""
;     StrCmp "" $0 0 finish

;     ReadRegStr $0 HKLM "SOFTWARE\Valve\Steam" "InstallPath"
    
;   finish:
;     StrCpy $INSTDIR $0
; FunctionEnd

;--------------------------------

;;;
;; Callbacks
;

Function .onInit
;    Call F
FunctionEnd

;--------------------------------

;;;
;; Licence
;
LicenseBkColor /windows
LicenseText "license"
LicenseData ${SRCDIR}\..\COPYING
LicenseForceSelection checkbox

;--------------------------------

;;;
;; Pages
;
Page license
Page directory
Page instfiles

;--------------------------------

;;;
;; Installation
;
Section "Install"
    SetOverwrite on
    SetOutPath $INSTDIR\Replay
    File "${DATADIR}\Replay\*.bin"

    SetOutPath $INSTDIR
    File "${DATADIR}\sc68.cfg"
    File "${BINDIR}\*68.exe"
    File /nonfatal "${BINDIR}\*68.dll"
  
    ;; Write config keys for this programs
    WriteRegStr HKLM \
        "SOFTWARE\sashipa\sc68" \
        "Install_Dir" \
        "$INSTDIR"
    
    ;; Write the uninstall keys for Windows
    
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

;--------------------------------

;;;
;; Uninstall
;
Section "Uninstall"

    ;; Remove registry config keys

    DeleteRegKey HKLM "SOFTWARE\sashipa\sc68"
    DeleteRegKey /ifempty HKLM "SOFTWARE\sashipa"

    ;; Remove registry uninstaller keys
    
    DeleteRegKey HKLM \
        "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\sc68"

    ;; Remove files
    ;Delete $INSTDIR\cstrike\resource\GameMenu.res
    ;Delete $INSTDIR\cstrike\resource\gameui_*.txt
    ;Delete $INSTDIR\cstrike\cfg\aces.cfg
    ;Delete $INSTDIR\aces-css-menu-uninstall.exe

SectionEnd
