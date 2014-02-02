/*
 * @file    fileinfo.c
 * @brief   sc68-ng plugin for winamp 5.5 - file info dialog
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2014 Benjamin Gerard
 *
 * Time-stamp: <2014-01-25 13:24:49 ben>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* generated config header */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* winamp sc68 declarations */
#include "wasc68.h"

/* windows */
#include <windows.h>
#include <mmreg.h>
#include <msacm.h>
#include <commctrl.h>

/* libc */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* resource */
#include "resource.h"
#include "dlg.c"

/* sc68 */
#include "sc68/sc68.h"

static BOOL CALLBACK DialogProc(HWND,UINT,WPARAM,LPARAM);

/* Only exported function. */
int fileinfo_dialog(HINSTANCE hinst, HWND hwnd, const char * uri)
{
  HWND hdlg
    = CreateDialogParam(hinst, (LPCTSTR)IDD_DLG_FILEINFO,
                        hwnd, (DLGPROC)DialogProc, (LPARAM)uri);
  return hdlg ? 0 : -1;
}

static int SetTrackList(HWND hdlg, const int t, const int n)
{
  HWND hcb = GetDlgItem(hdlg, IDC_INF_TRACK);
  int i;

  SendMessage(hcb, (UINT)CB_RESETCONTENT,(WPARAM) 0,(LPARAM) 0);
  for (i = 1; i <= n; ++i) {
    char t[3];
    t[0] = '0' + i / 10; t[1] = '0' + i % 10; t[2] = 0;
    SendMessage(hcb,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) t);
  }
  if (t > 0 && t <= n)
    SendMessage(hcb, CB_SETCURSEL, (WPARAM)t-1, (LPARAM)0);
  return 0;
}

static int SetInfo(HWND hdlg, const sc68_music_info_t * inf)
{
  SetText(hdlg, IDC_INF_ALBUM,     inf->album);
  SetText(hdlg, IDC_INF_ARTIST,    inf->artist);
  SetText(hdlg, IDC_INF_TITLE ,    inf->title);
  SetText(hdlg, IDC_INF_GENRE,     inf->genre);
  SetText(hdlg, IDC_INF_FORMAT,    inf->format);
  SetText(hdlg, IDC_INF_RIPPER,    inf->ripper);
  SetText(hdlg, IDC_INF_CONVERTER, inf->converter);
  SetText(hdlg, IDC_INF_YEAR,      inf->year);
  SetText(hdlg, IDC_INF_TIME,      inf->trk.time+3);

  SetVisible(hdlg, IDC_INF_AMIGA, 0 ^ inf->trk.amiga);
  SetVisible(hdlg, IDC_INF_YM,    1 ^ inf->trk.amiga);
  SetVisible(hdlg, IDC_INF_STE,   1 ^ inf->trk.amiga);
  SetVisible(hdlg, IDC_INF_ASID,  1 ^ inf->trk.amiga);

  SetEnable(hdlg,  IDC_INF_AMIGA,0);
  SetEnable(hdlg,  IDC_INF_YM,   0);
  SetEnable(hdlg,  IDC_INF_STE,  0);
  SetEnable(hdlg,  IDC_INF_ASID, 0);

  SetCheck(hdlg,  IDC_INF_AMIGA,inf->trk.amiga);
  SetCheck(hdlg,  IDC_INF_YM,   inf->trk.ym);
  SetCheck(hdlg,  IDC_INF_STE,  inf->trk.ste);
  SetCheck(hdlg,  IDC_INF_ASID, inf->trk.asid);
  return 0;
}

static int NoInfo(HWND hdlg)
{
  sc68_music_info_t info;
  memset(&info,0,sizeof(info));
  info.album = info.artist = info.title = info.genre =
    info.format = info.ripper = info.converter = "N/A";
  strcpy(info.trk.time,"-- --:--");
  return SetInfo(hdlg, &info);
}

/* Change cookie, return previous */
static sc68_disk_t ChangeDisk(HWND hdlg, sc68_disk_t d)
{
  return (sc68_disk_t) SetWindowLong(hdlg, DWL_USER, (LONG)d);
}

/* Get cookie */
static sc68_disk_t GetDisk(HWND hdlg)
{
  return (sc68_disk_t) GetWindowLong(hdlg, DWL_USER);
}

static int SetDiskInfo(HWND hdlg, const char * uri)
{
  sc68_disk_t d = GetDisk(hdlg);
  sc68_music_info_t info;

  SetText(hdlg,IDC_INF_URI, uri);
  if (d && !sc68_music_info(0, &info, 1, d)) {
    SetTrackList(hdlg, info.trk.track, info.tracks);
    SetInfo(hdlg,&info);
  } else {
    SetTrackList(hdlg, 0, 0);
    NoInfo(hdlg);
  }
  SetFocus(GetDlgItem(hdlg, IDOK));
  return 0;
}

static int ChangeFile(HWND hdlg, const char * uri)
{
  sc68_disk_t d, prev;

  d = sc68_load_disk_uri(uri);
  if (!d)
    return -1;
  prev = ChangeDisk(hdlg, d);
  if (prev)
    sc68_disk_free(prev);
  SetDiskInfo(hdlg, uri);
  return 0;
}

static BOOL CALLBACK DialogProc(
  HWND hdlg,      // handle to dialog box
  UINT uMsg,      // message
  WPARAM wParam,  // first message parameter
  LPARAM lParam)  // second message parameter
{

  switch(uMsg)
  {

  case WM_INITDIALOG:
    ChangeFile(hdlg, (char *)lParam);
    ShowWindow(hdlg, SW_SHOW);
    UpdateWindow(hdlg);
    return 1;

  case WM_COMMAND: {
    /* int wNotifyCode = HIWORD(wParam); // notification code */
    int wID = LOWORD(wParam);         // item, control, or accelerator identifier
    HWND hwndCtl = (HWND) lParam;     // handle of control

    switch(wID) {

    case IDOK:
      DestroyWindow (hdlg);
      break;

    case IDC_INF_TRACK:
      if (HIWORD(wParam) == CBN_SELCHANGE) {
        sc68_music_info_t info;
        sc68_disk_t d = GetDisk(hdlg);
        int track = SendMessage(hwndCtl, (UINT) CB_GETCURSEL,
                                (WPARAM) 0, (LPARAM) 0);
        if (track >= 0 && !sc68_music_info(0, &info, track+1, d))
          SetInfo(hdlg,&info);
        else
          NoInfo(hdlg);
      }
    }
  } break;

  case WM_DESTROY:
    sc68_disk_free(ChangeDisk(hdlg,0));
    break;

  case WM_CLOSE:
    DestroyWindow (hdlg);
    break;
  }

  return 0;
}
