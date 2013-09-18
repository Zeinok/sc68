/*
 * @file    fileinfo.c
 * @brief   sc68-ng plugin for winamp 5.5
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-09-18 08:04:17 ben>
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

/* windows */
#include <windows.h>
#include <mmreg.h>
#include <msacm.h>
#include <commctrl.h>

#include "resource.h"

/* Standard */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* sc68 */
#include "sc68/sc68.h"

static BOOL CALLBACK DialogProc(HWND,UINT,WPARAM,LPARAM);

/* Only exported function. */
int fileinfo_dialog(HINSTANCE hinst, HWND hwnd, const char * uri)
{
  HWND hdlg;
  hdlg = CreateDialogParam(hinst, (LPCTSTR)IDD_DLG_FILEINFO,
                           hwnd, (DLGPROC)DialogProc, (LPARAM)uri);
  return hdlg ? 0 : -1;
}

static int SetText(HWND hdlg, int id, const char *new_text)
{
  LRESULT res;
  WPARAM wParam = 0;
  LPARAM lParam = (LPARAM)new_text;
  res = SendDlgItemMessage(hdlg, id, WM_SETTEXT, wParam, lParam);
  return (!!res);
}

static int SetIconText(HWND hdlg, int id, const char *new_text)
{
  return SetText(hdlg, id, new_text);
}

static int SetIconFormat(HWND hdlg, int id, const char *format, int n)
{
  char buffer[128];
  _snprintf(buffer,sizeof(buffer), format, n);
  buffer[sizeof(buffer)-1] = 0;
  return SetText(hdlg, id, buffer);
}


static int SetEnable(HWND hdlg, int id, int enable)
{
  LRESULT res;
  res = SendDlgItemMessage(hdlg, id, BM_SETCHECK, !!enable, 0);
  EnableWindow(GetDlgItem(hdlg,id),!!enable);
  return (!!res);
}

static int SetVisible(HWND hdlg, int id, int visible)
{
  LRESULT res = 0;

  res = ShowWindow(GetDlgItem(hdlg, id), visible ? SW_SHOW : SW_HIDE);
  return (!!res);
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
  SetIconText(hdlg, IDC_INF_ALBUM,     inf->album);
  SetIconText(hdlg, IDC_INF_ARTIST,    inf->artist);
  SetIconText(hdlg, IDC_INF_TITLE ,    inf->title);
  SetIconText(hdlg, IDC_INF_GENRE,     inf->genre);
  SetIconText(hdlg, IDC_INF_FORMAT,    inf->format);
  SetIconText(hdlg, IDC_INF_RIPPER,    inf->ripper);
  SetIconText(hdlg, IDC_INF_CONVERTER, inf->converter);
  SetIconText(hdlg, IDC_INF_YEAR,      inf->year);
  /* SetIconText(hdlg, IDC_INF_HARDWARE,  inf->trk.hw); */
  SetIconText(hdlg, IDC_INF_TIME,      inf->trk.time+3);

  SetVisible(hdlg, IDC_INF_AMIGA, 0 ^ inf->trk.amiga);
  SetVisible(hdlg, IDC_INF_YM,    1 ^ inf->trk.amiga);
  SetVisible(hdlg, IDC_INF_STE,   1 ^ inf->trk.amiga);
  SetVisible(hdlg, IDC_INF_ASID,  1 ^ inf->trk.amiga);

  SetEnable(hdlg,  IDC_INF_AMIGA,inf->trk.amiga);
  SetEnable(hdlg,  IDC_INF_YM,   inf->trk.ym);
  SetEnable(hdlg,  IDC_INF_STE,  inf->trk.ste);
  SetEnable(hdlg,  IDC_INF_ASID, inf->trk.asid);

  /* SetIconText(hdlg, IDC_INFTREPLAY, inf->replay); */
  /* SetIconFormat(hdlg, IDC_INFTRATE, " %d Hz",inf->rate); */
  /* SetIconFormat(hdlg, IDC_INFTLOADADDR, " $%X", inf->addr); */
  return 0;
}

static int NoInfo(HWND hdlg)
{
  sc68_music_info_t info;
  memset(&info,0,sizeof(info));
  info.album = info.artist = info.title = info.genre =
    info.format = info.ripper = info.converter = "N/A";
  strcpy(info.trk.time,"-- --:--");

  int ComboBox_DeleteString(
  HWND hwndCtl,
  int index
);


  return SetInfo(hdlg, &info);
}


// Change cookie, return previous
static sc68_disk_t ChangeDisk(HWND hdlg, sc68_disk_t d)
{
  return (sc68_disk_t) SetWindowLong(hdlg, DWL_USER, (LONG)d);
}

// Get cookie
static sc68_disk_t GetDisk(HWND hdlg)
{
  return (sc68_disk_t) GetWindowLong(hdlg, DWL_USER);
}

static int SetDiskInfo(HWND hdlg, const char * uri)
{
  sc68_disk_t d = GetDisk(hdlg);
  sc68_music_info_t info;

  SetIconText(hdlg,IDC_INF_URI, uri);
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
  case WM_INITDIALOG: {
    ChangeFile(hdlg, (char *)lParam);
    ShowWindow(hdlg, SW_SHOW);
    UpdateWindow(hdlg);
  } return 1;

  case WM_COMMAND: {
    int wNotifyCode = HIWORD(wParam); // notification code
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
        int track = SendMessage((HWND) lParam, (UINT) CB_GETCURSEL,
                                (WPARAM) 0, (LPARAM) 0);
        if (track >= 0 && !sc68_music_info(0, &info, track+1, d))
          SetInfo(hdlg,&info);
        else
          NoInfo(hdlg);
      }
    }
  } return 0;

  case WM_DESTROY:
  {
    sc68_disk_t d = ChangeDisk(hdlg,0);
    sc68_disk_free(d);
  } return 0;

  case WM_CLOSE:
    DestroyWindow (hdlg);
    return 0;
  }

  return 0;
}
