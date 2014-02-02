/*
 * @file    tracksel.c
 * @brief   sc68-ng plugin for winamp 5.5 - track selection dialog
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2014 Benjamin Gerard
 *
 * Time-stamp: <2014-01-24 19:25:14 ben>
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

/* winamp 2 */
#include "winamp/in2.h"

#include "resource.h"
#include "dlg.c"

/* libc */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* sc68 */
#include "sc68/sc68.h"

static BOOL CALLBACK DialogProc(HWND,UINT,WPARAM,LPARAM);


static int remember = REMEMBER_NOT_SET;

struct tracksel {
  int select;
  int tracks;
  sc68_t * sc68;
};

/* Only exported function. */
int tracksel_dialog(HINSTANCE hinst, HWND hwnd, sc68_t * sc68)
{
  struct tracksel ts;
  ts.sc68   = sc68;
  ts.tracks = sc68_cntl(sc68, SC68_GET_TRACKS);

  if (remember != REMEMBER_NOT_SET) {
    ts.select = remember;
    if (remember == REMEMBER_ALL_TRACKS || remember == REMEMBER_DEF_TRACK ||
        (remember > 0 && remember <= ts.tracks)) {
      DBG("trackseldlg has been remembered\n");
      goto done;
    }
  }
  ts.select = REMEMBER_NOT_SET;
  DialogBoxParam(hinst, (LPCTSTR)IDD_DLG_TRACKSELECT,
                 hwnd, (DLGPROC)DialogProc, (LPARAM)&ts);
done:
  DBG("trackseldlg -> %d\n", ts.select);
  return ts.select;
}

static int SetTrackList(HWND hdlg, sc68_t * sc68, int sel)
{
  HWND hcb = GetDlgItem(hdlg, IDC_SEL_TRACKLIST);
  int i, tracks = sc68_cntl(sc68,SC68_GET_TRACKS);
  char tmp[256];
  sc68_minfo_t inf;

  DBG("trackseldlg -- SetTrackList tracks=%d sel=%d\n", tracks, sel);

  SendMessage(hcb, (UINT)CB_RESETCONTENT,(WPARAM) 0,(LPARAM) 0);
  SendMessage(hcb,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) "<Default>");
  for (i=1; i <= tracks; ++i)
    if (!sc68_music_info(sc68, &inf, i, 0)) {
      if (!*inf.title || !strcmp(inf.album, inf.title))
        snprintf(tmp,sizeof(tmp),"%02u - %s\n",
                 inf.trk.track, inf.album);
      else
        snprintf(tmp,sizeof(tmp),"%02u - %s\n",
                 inf.trk.track, inf.title);
      SendMessage(hcb,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) tmp);
    }
  if (sel > tracks)
    sel = 0;
  SendMessage(hcb, CB_SETCURSEL, (WPARAM)sel, (LPARAM)0);
  return 0;
}


static int SetInfo(HWND hdlg, sc68_t * sc68)
{
  sc68_minfo_t inf;
  if (!sc68_music_info(sc68, &inf, SC68_DEF_TRACK, 0)) {
    char tmp[512];
    snprintf(tmp,sizeof(tmp),"%s - %s",inf.artist, inf.album);
    SetText(hdlg, IDC_SEL_ALBUM,  tmp);
    return 0;
  }
  return -1;
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
    struct tracksel * ts = (struct tracksel *) lParam;
    SetWindowLong(hdlg, DWL_USER, lParam);
    SetInfo(hdlg, ts->sc68);
    if (remember == REMEMBER_NOT_SET) {
      DBG("trackseldlg -- init -- don't remember\n");
      SetCheck(hdlg,  IDC_SEL_REMEMBER, 0);
      SetCheck(hdlg,  IDC_SEL_ALLTRACKS, 0);
      SetCheck(hdlg,  IDC_SEL_SINGLETRACK, 1);
      SetEnable(hdlg, IDC_SEL_TRACKLIST, 1);
      SetTrackList(hdlg, ts->sc68, 0);
    } else {
      DBG("trackseldlg -- init -- remember %d\n", remember);
      SetCheck(hdlg,  IDC_SEL_REMEMBER, 1);
      SetCheck(hdlg,  IDC_SEL_ALLTRACKS, remember == -1);
      SetCheck(hdlg,  IDC_SEL_SINGLETRACK, remember != -1);
      SetTrackList(hdlg, ts->sc68, remember);
      SetEnable(hdlg, IDC_SEL_TRACKLIST, remember != -1);
    }
    ShowWindow(hdlg, SW_SHOW);
    UpdateWindow(hdlg);
  } return 1;

  case WM_COMMAND: {
    int wNotifyCode = HIWORD(wParam); // notification code
    int wID = LOWORD(wParam);         // item, control, or accelerator identifier
    HWND hwndCtl = (HWND) lParam;     // handle of control
    struct tracksel * ts = (struct tracksel *) GetWindowLong(hdlg, DWL_USER);

    switch(wID) {
    case IDCANCEL:
      DBG("trackseldlg -- CANCEL");
      ts->select = REMEMBER_NOT_SET;
      EndDialog(hdlg, 1);
      return 0;

    case IDOK:
      if (GetCheck(hdlg, IDC_SEL_REMEMBER) == 1)
        remember = ts->select;
      else
        remember = REMEMBER_NOT_SET;
      DBG("trackseldlg -- OK (sel=%d remember=%d)\n", ts->select, remember);
      EndDialog(hdlg, 1);
      return 0;

    case IDC_SEL_SINGLETRACK:
    case IDC_SEL_ALLTRACKS:
      switch (GetCheck(hdlg, IDC_SEL_SINGLETRACK)) {
      case 0:
        SetEnable(hdlg, IDC_SEL_TRACKLIST, 0);
        ts->select = REMEMBER_ALL_TRACKS;
        break;
      case 1:
        SetEnable(hdlg, IDC_SEL_TRACKLIST, 1);
        ts->select = SendDlgItemMessage(hdlg, IDC_SEL_TRACKLIST, CB_GETCURSEL,
                                    (WPARAM)0, (LPARAM)0);
        if (ts->select < 0 || ts->select > ts->tracks)
          ts->select = 0;
        break;
      }
      DBG("trackseldlg -- SINGLETRACK/ALLTRACK -> %d\n", ts->select);
      break;

    case IDC_SEL_TRACKLIST:
      ts->select = SendMessage(hwndCtl, CB_GETCURSEL, 0, 0);
      if (ts->select < 0 || ts->select > ts->tracks)
        ts->select = 0;
      DBG("trackseldlg -- TRACKLIST -> %d\n", ts->select);
      break;
    }
  } return 0;

  case WM_DESTROY:
    return 0;

  case WM_CLOSE:
    EndDialog(hdlg, 0);
      /* DestroyWindow (hdlg); */
    return 0;
  }

  return 0;
}
