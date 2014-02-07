/*
 * @file    configdlg.c
 * @brief   sc68-ng plugin for winamp 5.5 - configuration dialog
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2014 Benjamin Gerard
 *
 * Time-stamp: <2014-02-07 20:05:28 ben>
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
#include "sc68/file68_opt.h"

static BOOL CALLBACK DialogProc(HWND,UINT,WPARAM,LPARAM);

/* Only exported function. */
int config_dialog(HINSTANCE hinst, HWND hwnd)
{
  HWND hdlg
    = CreateDialogParam(hinst, (LPCTSTR)IDD_DLG_CONFIG,
                        hwnd, (DLGPROC)DialogProc, (LPARAM)0);
  return hdlg ? 0 : -1;
}

static int GetOptFromCombo(HWND hdlg, int idc, const char * key)
{
  option68_t * opt = option68_get(key, opt68_ALWAYS);
  int res = -1;
  const char * val = "n/a";

  if (opt && opt->set && opt->sets &&
      (opt->type == opt68_ENU || opt->type == opt68_STR)) {
    res = SendMessage(GetDlgItem(hdlg, idc), CB_GETCURSEL, 0, 0);
    if (res >= 0 && res < opt->sets) {
      val = res[(char **)opt->set];
      option68_iset(opt, res, opt68_ALWAYS, opt68_CFG);
    } else
      res = -1;
  }
  DBG("#%05d set option '%s' = %d \"%s\"\n", idc, key, res, val);
  return res;
}

static int SetComboByOpt(HWND hdlg, int idc, const char * key)
{
  option68_t * opt = option68_get(key, opt68_ALWAYS);
  int res = -1;

  if (opt && opt->set && opt->sets &&
      (opt->type == opt68_ENU || opt->type == opt68_STR)) {
    res = SetCombo(hdlg, idc, (const char **)opt->set, opt->sets);
    res = (!res && option68_isset(opt)) ? opt->val.num : -1;
    SendMessage(GetDlgItem(hdlg, idc), CB_SETCURSEL, res, 0);
  }
  DBG("#%05d item #%d for option '%s'\n", idc, res, key);
  return res;
}

static int GetOptFromSlider(HWND hdlg, int idc, const char * key)
{
  option68_t * opt = option68_get(key, opt68_ALWAYS);
  int res = -1;
  if (opt && opt->type == opt68_INT) {
    res = SendDlgItemMessage(hdlg, idc, TBM_GETPOS, 0, 0);
    option68_iset(opt, res, opt68_ALWAYS, opt68_CFG);
  }
  DBG("#%05d set option '%s' = %d\n", idc, key, res);
  return res;
}

static int SetSliderByOpt(HWND hdlg, int idc, const char * key, int bud,
                          const int * tic, int ntics)
{
  HWND hcb = GetDlgItem(hdlg, idc);
  option68_t * opt = option68_get(key, opt68_ALWAYS);
  if (!opt || opt->type != opt68_INT || opt->min >= opt->max)
    return -1;

  DBG("item=#%05d buddy=#%05d option '%s' tics=%d [%d/%d/%d] \n",
      idc, bud, key, ntics, opt->min, opt->val.num, opt->max);

  SendMessage(hcb, (UINT)TBM_SETRANGE,
              (WPARAM)FALSE /*redraw*/,
              MAKELPARAM(opt->min,opt->max));

  SendMessage(hcb, (UINT)TBM_CLEARTICS,
              (WPARAM)FALSE /*redraw*/, 0);

  if (ntics) {
    int i;
    if (tic) {
      for (i = 0; i < ntics; ++i)
        SendMessage(hcb, (UINT)TBM_SETTIC,
                    (WPARAM)FALSE /*redraw*/, tic[i]);
    } else {
      for (i = 0; i < ntics; ++i) {
        int v = opt->min + (i+1)*(opt->max-opt->min)/(ntics+1);
        SendMessage(hcb, (UINT)TBM_SETTIC,
                    (WPARAM)FALSE /*redraw*/, v);
      }
    }
  }

  if (bud != IDC_STATIC)
    SendMessage(hcb, (UINT)TBM_SETBUDDY,
                (WPARAM)FALSE /*position*/,
                (LPARAM)GetDlgItem(hdlg, bud));


  SendMessage(hcb, (UINT)TBM_SETPOS,
              (WPARAM)TRUE /*redraw*/,
              (LPARAM)opt->val.num /* (opt->min+opt->max)>>1 */);
  return 0;
}

static const char * l_spr[] = {
  "11 khz","22 khz", "44.1 khz","48 khz","96 khz"
};
static const int i_spr[] = {
  11025, 22050, 44100, 48000, 96000
};

static int get_int(const char * key, int def)
{
  const option68_t * opt
    = option68_get(key,opt68_ISSET);
  return opt ? !!opt->val.num : def;
}

/* static int myabs(const int v) { return v < 0 ? -v : v; } */
static int mydiv(const int a, const int b) {
  return a > b ? ((a<<12)/b) : ((b<<12)/a);
}

static int mymin(const int a, const int b) {
  return a < b ? a : b;
}

static BOOL CALLBACK DialogProc(
  HWND hdlg,      // handle to dialog box
  UINT uMsg,      // message
  WPARAM wParam,  // first message parameter
  LPARAM lParam)  // second message parameter
{
  option68_t * opt;

  switch(uMsg)
  {
  case WM_INITDIALOG: {
    SetComboByOpt(hdlg, IDC_INF_YMENGINE, "ym-engine");
    SetComboByOpt(hdlg, IDC_INF_YMFILTER, "ym-filter");
    SetComboByOpt(hdlg, IDC_INF_YMVOLUME, "ym-volmodel");
    SetComboByOpt(hdlg, IDC_INF_ASID,     "asid");
    SetCombo(hdlg, IDC_INF_SPR, l_spr, sizeof(l_spr)/sizeof(*l_spr));
    opt = option68_get("sampling-rate",opt68_ISSET);
    if (opt) {
      int i, idx = 0, best = mydiv(i_spr[0],opt->val.num);
      DBG("looking for %s %dhz\n", opt->name, opt->val.num);
      for (i = 1; i < sizeof(i_spr)/sizeof(*i_spr); ++i) {
        int score = mydiv(i_spr[i],opt->val.num);
        if (score < best) {
          best = score; idx = i;
        }
      }
      DBG("best:#%d/%dhz/%d\n", idx, i_spr[idx], best);
      SendDlgItemMessage(hdlg, IDC_INF_SPR, CB_SETCURSEL, idx, 0);
    }
    SetCheck(hdlg, IDC_INF_AGAFILTER, get_int("amiga-filter",0));
    SetSliderByOpt(hdlg, IDC_INF_AGABLEND, "amiga-blend", IDC_STATIC, 0, 3);

    opt = option68_get("default-time",opt68_ISSET);
    if (opt) {
      DBG("Got a default time: %d/%d/%d\n", opt->min,opt->val.num,opt->max);
      SendDlgItemMessage(hdlg, IDC_SPIN_DEFTIME,
                         UDM_SETRANGE32, opt->min,opt->max);
      SendDlgItemMessage(hdlg, IDC_SPIN_DEFTIME,
                         UDM_SETPOS32, 0, opt->val.num);
      SendDlgItemMessage(hdlg, IDC_SPIN_DEFTIME, UDM_SETBUDDY,
                         (WPARAM)GetDlgItem(hdlg, IDC_INF_DEFTIME), 0);

    }

    opt = option68_get("ufi",opt68_ALWAYS);
    if (opt)
      g_useufi = !!opt->val.num;
    SetCheck(hdlg, IDC_UFI, g_useufi > 0);

    opt = option68_get("hook",opt68_ALWAYS);
    if (opt)
      g_usehook = !!opt->val.num;

    SetCheck(hdlg, IDC_HOOK, g_usehook > 0);
    ShowWindow(hdlg, SW_SHOW);
    UpdateWindow(hdlg);
  } return 1;

  case WM_COMMAND: {
    /* int wNotifyCode = HIWORD(wParam); // notification code */
    int wID = LOWORD(wParam); // item, control, or accelerator identifier
    HWND hwndCtl = (HWND) lParam;       // handle of control

    switch(wID) {

    case IDOK: {
      int v;
      GetOptFromCombo(hdlg, IDC_INF_YMENGINE, "ym-engine");
      GetOptFromCombo(hdlg, IDC_INF_YMFILTER, "ym-filter");
      GetOptFromCombo(hdlg, IDC_INF_YMVOLUME, "ym-volmodel");
      GetOptFromCombo(hdlg, IDC_INF_ASID,     "asid");
      opt = option68_get("sampling-rate",opt68_ALWAYS);
      if (opt) {
        v  = SendDlgItemMessage(hdlg, IDC_INF_SPR, CB_GETCURSEL, 0, 0);
        if (v >= 0 && v < sizeof(i_spr)/sizeof(*i_spr))
          option68_iset(opt, i_spr[v], opt68_ALWAYS, opt68_CFG);
      }
      opt = option68_get("amiga-filter",opt68_ALWAYS);
      if (opt) {
        v = GetCheck(hdlg, IDC_INF_AGAFILTER);
        if (v >= 0)
          option68_iset(opt,v,opt68_ALWAYS,opt68_CFG);
      }
      GetOptFromSlider(hdlg, IDC_INF_AGABLEND, "amiga-blend");

      v = GetCheck(hdlg, IDC_UFI);
      if (v >= 0) {
        g_useufi = v;
        opt = option68_get("ufi",opt68_ALWAYS);
        if (opt)
          option68_iset(opt,v,opt68_ALWAYS,opt68_CFG);
      }

      v = GetCheck(hdlg, IDC_HOOK);
      if (v >= 0) {
        g_usehook = v;
        opt = option68_get("hook",opt68_ALWAYS);
        if (opt)
          option68_iset(opt,v,opt68_ALWAYS,opt68_CFG);
      }

      opt = option68_get("default-time",opt68_ALWAYS);
      if (opt) {
        v = SendDlgItemMessage(hdlg, IDC_SPIN_DEFTIME, UDM_GETPOS32, 0, 0);
        if (v >= opt->min && v < opt->max)
          option68_iset(opt,v,opt68_ALWAYS,opt68_CFG);
      }

      sc68_cntl(0,SC68_CONFIG_SAVE);
      DestroyWindow(hdlg);
    } break;

    case IDCANCEL:
      DBG("#%05d CANCEL\n", wID);
      PostMessage(hdlg, WM_CLOSE,0,0);
      break;

    /* case IDC_SPIN_DEFTIME: { */
    /*   int pos = */
    /*     SendMessage(hwndCtl, UDM_GETPOS, 0, 0); */
    /*   DBG("#%05d pos=%d\n", wID, pos); */
    /* } break; */


    case IDC_INF_ASID:
      switch ( SendMessage(hwndCtl, CB_GETCURSEL, 0, 0) ) {
      case 0:                           /* off */
        set_asid(SC68_ASID_OFF); break;
      case 1:                           /* on */
        set_asid(SC68_ASID_ON); break;
      case 2:                           /* force */
        set_asid(SC68_ASID_FORCE); break;
      } break;

    default:
      DBG("#%05d W=%04x/%04x L=%08x\n",
          wID, HIWORD(wParam), LOWORD(wParam), (unsigned)lParam);
      return 1;
      break;

    }
  } break;

  case WM_CLOSE:
    DBG("CLOSE\n");
    DestroyWindow(hdlg);
    break;
  }

  return 0;
}
