/*
 * @file    configdlg.c
 * @brief   sc68-ng plugin for winamp 5.5 - configuration dialog
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2014 Benjamin Gerard
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

/* sc68 */
#include "sc68/sc68.h"
#include "sc68/file68_opt.h"

struct cookie_s {
  cookie_t * me;                        /*  */
  HINSTANCE hinst;                      /* HMOD of sc68cfg DLL */
  HWND hwnd;                            /* Parent window */
};

static void del_cookie(cookie_t * cookie)
{
  DBG("configuration\n");
  if (cookie && cookie->me == cookie) {
    free(cookie);
  }
}

static const char ident[] = "config";
static const char * l_spr[] = {
  "< custom >",
  "11 khz","22 khz", "44.1 khz","48 khz","96 khz"
};
static const int i_spr[] = {
  0,
  11025, 22050, 44100, 48000, 96000
};

static int get_int(const char * key, int def)
{
  const option68_t * opt
    = option68_get(key,opt68_ISSET);
  return opt ? !!opt->val.num : def;
}

static int getopt(cookie_t * cookie, const char * key, intptr_t * result)
{
  const option68_t * opt = option68_get(key+2, opt68_ALWAYS);
  if (opt) {
    const int isset = opt->org != opt68_UDF;

    switch (key[0]) {

    case 'i':
      /*  simple integer option value */
      if (isset && (opt->type == opt68_BOL || opt->type == opt68_INT)) {
        *result = (intptr_t) opt->val.num;
        return 0;
      }
      break;

    case 's':
      /* simple string value */
      if (isset && (opt->type == opt68_STR && !opt->sets)) {
        *result = (intptr_t) opt->val.str;
        return 0;
      }
      break;

    case 'a':
      /* simple string array */
      if (opt->set
          && (opt->type == opt68_STR || opt->type == opt68_ENU)) {
        int i = *result;
        if (i==-2)
          *result = opt->val.num;
        else if (i==-1)
          *result = opt->sets;
        else if (i>=0 && i<opt->sets)
          *result = (intptr_t) i[(const char **)opt->set];
        else {
          DBG("invalid range \"%s[%d]\"\n",key,i);
          return -1;
        };
        return 0;
      }
      break;

    case 'o':
      /* get option detail */
      switch ((int)*result) {
      case 'd': *result = (intptr_t) opt->desc; return 0;
      case 'n': *result = (intptr_t) opt->sets; return 0;
      case '<': *result = (intptr_t) opt->min;  return 0;
      case '>': *result = (intptr_t) opt->max;  return 0;

      case 't':
        if (opt->type == opt68_INT || opt->type == opt68_BOL
            || opt->type == opt68_ENU)
          *result = (intptr_t) 'i';
        else if (opt->type == opt68_STR)
          *result = (intptr_t) 's';
        else
          return -1;
        return 0;

      case 'i':
        if (opt->set) {
          *result = (intptr_t) opt->val.num;
          return 0;
        }
        return -1;

      case 'v':
        if (!opt->set) {
          if (opt->type == opt68_INT || opt->type == opt68_BOL)
            *result = (intptr_t) opt->val.num;
          else if (opt->type == opt68_STR)
            *result = (intptr_t) opt->val.str;
          else
            return -1;
        } else {
          if (opt->type == opt68_INT || opt->type == opt68_BOL)
            *result = (intptr_t) opt->val.num[(int*)opt->set];
          else if (opt->type == opt68_ENU || opt->type == opt68_STR)
            *result = (intptr_t) opt->val.num[(const char **)opt->set];
          else
            return -1;
        }
        return 0;
      }
      DBG("invalid option argument \"%s[%c]\"\n", key, (char)*result);
      return -1;

    }
  }
  DBG("invalid key \"%s\"\n", key);
  return -1;
}

#define keyis(N) !strcmp(key,N)

static int getval(void * _cookie, const char * key, intptr_t * result)
{
  cookie_t * cookie = (cookie_t *) _cookie;
  if (!key || !cookie || cookie->me != cookie || !result)
    return -1;

  if (key[0] == '_') {
    if (keyis("_instance"))
      *result = (intptr_t)cookie->hinst;
    else if (keyis("_parent"))
      *result = (intptr_t)cookie->hwnd;
    else if (keyis ("_identity"))
      *result = (intptr_t) ident;
    else if (keyis("_kill")) {
      del_cookie(cookie);
      *result = (intptr_t)0;
    }
    /* else if (keyis("_settrack")) { */

    /* }  */
    else {
      goto unknown;
    }
    return 0;
  }

  if (keyis("a_sampling-rate")) {
    int j, i = *result;
    const int max = sizeof (l_spr) / sizeof (*l_spr);
    if (i == -2) {
      const option68_t * opt = option68_get(key+2, opt68_ISSET);
      if (opt) {
        for (j = 1; j < max; ++j)
          if (opt->val.num == i_spr[j]) {
            *result = j;
            return 0;
          }
      }
      *result = 0;
      return 0;
    } else if (i == -1) {
      *result = (intptr_t) max;
    } else if (i>=0 && i<max) {
      *result = (intptr_t) l_spr[i];
    } else {
      DBG("invalid range \"%s[%d]\"\n", key, i);
      return -1;
    }
    return 0;
  }

  if (!getopt(cookie,key, result)) {
    DBG("OK getopt \"%s\"\n", key);
    return 0;
  }
  return -1;

/*   if (key[0] == 'a' && key[1] == '_') { */
/*     if (0) { */
/*     } */
/*     else if (keyis("a_sampling-rate")) { */
/*       int i = *result; */
/*       if (i == -2)                        /\* get current *\/ */
/*         *result = (intptr_t) cookie->cfg_spridx; */
/*       else if (i == -1)                   /\* get count *\/ */
/*         *result = (intptr_t) 3; */





unknown:
  DBG("unknown command \"%s\"", key);
error:
  *result = (intptr_t)0;
  return -1;
}

/* Only exported function. */
int config_dialog(HINSTANCE hinst, HWND hwnd)
{
  cookie_t * cookie = 0;
  int res = -1;

  if (!dialog_modless)
    goto error;

  cookie = malloc(sizeof(*cookie));
  if (!cookie)
    goto error;

  cookie->me    = cookie;
  cookie->hinst = hinst;
  cookie->hwnd  = hwnd;
  res = dialog_modless(cookie,getval);
  goto exit;
  DBG("%s() -> %d\n", __FUNCTION__, res);
  return res;

error:
  del_cookie(cookie);
exit:
  DBG("%s() -> %d\n", __FUNCTION__, res);
  return res;
}






/***********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 ***********************************************************************/

#if 0
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

    case IDC_INF_SPR:
      
      /* switch ( SendMessage(hwndCtl, CB_GETCURSEL, 0, 0) ) { */
      /* case 0:                           /\* off *\/ */
      /*   set_asid(SC68_ASID_OFF); break; */
      /* case 1:                           /\* on *\/ */
      /*   set_asid(SC68_ASID_ON); break; */
      /* case 2:                           /\* force *\/ */
      /*   set_asid(SC68_ASID_FORCE); break; */
      /* } break; */




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
#endif
