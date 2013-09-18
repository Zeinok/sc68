/*
 * @file    fileinfo.c
 * @brief   sc68-ng plugin for winamp 5.5
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-09-18 05:01:39 ben>
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

static int SetInfo(HWND hdlg, const sc68_music_info_t * inf)
{
  SetIconText(hdlg, IDC_INF_ALBUM,     inf->album);
  SetIconText(hdlg, IDC_INF_ARTIST,    inf->artist);
  SetIconText(hdlg, IDC_INF_TITLE ,    inf->title);
  SetIconText(hdlg, IDC_INF_GENRE,     inf->genre);
  SetIconText(hdlg, IDC_INF_FORMAT,    inf->format);
  SetIconText(hdlg, IDC_INF_RIPPER,    inf->ripper);
  SetIconText(hdlg, IDC_INF_CONVERTER, inf->converter);
  SetIconText(hdlg, IDC_INF_HARDWARE,  inf->trk.hw);
  SetIconText(hdlg, IDC_INF_TIME,      inf->trk.time+3);

  /* SetIconText(hdlg, IDC_INFTTIME, inf->time); */
  /* SetIconText(hdlg, IDC_INFTREPLAY, inf->replay); */
  /* SetIconFormat(hdlg, IDC_INFTRATE, " %d Hz",inf->rate); */
  /* SetIconFormat(hdlg, IDC_INFTLOADADDR, " $%X", inf->addr); */
  /* SetEnable(hdlg, IDC_INFHWYM,  inf->hw.ym); */
  /* SetEnable(hdlg, IDC_INFHWMW,  inf->hw.ste); */
  /* SetEnable(hdlg, IDC_INFHWAGA, inf->hw.amiga); */
  return 0;
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
  static const char error[] = "Not an sc68 file";
  sc68_disk_t d;
  int min = 0, max = 0, pos = 0;

  SetIconText(hdlg,IDC_INF_URI, uri);
  d = GetDisk(hdlg);

  if (d) {
    sc68_music_info_t info;
    if (sc68_music_info(0, &info, 1, d) < 0) {
      return -1;
    }
    SetInfo(hdlg,&info);
    /* SetIconText(hdlg,IDC_DNAME, info.title); */
    /* SetIconText(hdlg,IDC_DTIME, info.time); */
    /* SetTrackInfo(hdlg, d, pos = 0); */
    /* max = info.tracks-1; */
  } else {
    /* SetIconText(hdlg,IDC_DNAME, error); */
    /* SetIconText(hdlg,IDC_DTIME, ""); */
    /* SetTrackInfo(hdlg, 0, 0); */
  }
  /* SendDlgItemMessage(hdlg, IDC_TUP,UDM_SETRANGE32, min, max); */
  /* SendDlgItemMessage(hdlg, IDC_TUP,UDM_SETPOS, 0, (LPARAM) MAKELONG((short)pos, 0)); */

  /* SetIconText(hdlg, IDOK, endMsg[rand() % (sizeof(endMsg)/ sizeof(*endMsg))]); */
  /* SetFocus(GetDlgItem(hdlg, IDC_TUP)); */
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
  LPARAM lParam   // second message parameter
  )
{

  switch(uMsg)
  {
  case WM_INITDIALOG:
  {
    ChangeFile(hdlg, (char *)lParam);
    ShowWindow(hdlg, SW_SHOW);
    UpdateWindow(hdlg);
  } return 1;

  case WM_COMMAND:
  {
    int wNotifyCode = HIWORD(wParam); // notification code
    int wID = LOWORD(wParam);         // item, control, or accelerator identifier
    HWND hwndCtl = (HWND) lParam;     // handle of control

    switch(wID) {
    case IDOK:
      DestroyWindow (hdlg);
      break;
    }
  } return 0;

#if 0
  case WM_VSCROLL:
  {
    int nScrollCode = (int) LOWORD(wParam);   // scroll bar value
    short nPos = (short int) HIWORD(wParam);  // scroll box position
    HWND hwndScrollBar = (HWND) lParam;       // handle to scroll bar
    if (hwndScrollBar != GetDlgItem(hdlg, IDC_TUP)) {
      return 0; // Safe net, should not happen.
    } else {
      api68_disk_t d = GetDisk(hdlg);
      if (!d) {
        return 0;
      }

      switch (nScrollCode) {
      case SB_THUMBPOSITION:
        SetTrackInfo(hdlg, d, nPos);
        break;
      }
    }
  } return 0;
#endif

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
