/*
 * @file    winui_dlg.c
 * @brief   sc68 dialogs for Windows systems
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

/* sc68 */
#include "sc68/sc68.h"                  /* for sc68_dial_f */

/* windows */
/* #include <windef.h> */

/* Needed for UDM32 family defines */
#ifndef WINVER
# define WINVER 0x0505
#endif
#ifndef _WIN32_WINNT
# define _WIN32_WINNT 0x0400
#endif
#ifndef _WIN32_IE
# define _WIN32_IE 0x0500
#endif

/* windows */
#include <windows.h>
#include <commctrl.h>                   /* TBM_* */

/* libc */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* #include "winui_dbg.h" */

/* resource */
#include "resource.h"

/* Fail safe just in case the message are not defined */
#ifndef UDM_SETRANGE32
# define UDM_SETRANGE32 (WM_USER+111)
#endif
#ifndef UDM_GETRANGE32
# define UDM_GETRANGE32 (WM_USER+112)
#endif
#ifndef UDM_SETPOS32
# define UDM_SETPOS32 (WM_USER+113)
#endif
#ifndef UDM_GETPOS32
# define UDM_GETPOS32 (WM_USER+114)
#endif

#if !defined(DWL_USER) && defined(DWLP_USER)
#define USERCOOKIE DWLP_USER
#else
#define USERCOOKIE DWL_USER
#endif

#if !defined(DWL_MSGRESULT) && defined(DWLP_MSGRESULT)
#define USERRESULT DWLP_MSGRESULT
#else
#define USERRESULT DWL_MSGRESULT
#endif

#define GET_USERCOOKIE(H) ((dialog_t *)GetWindowLongPtr(H,USERCOOKIE))
#define SET_USERCOOKIE(H,V) SetWindowLongPtr(H,USERCOOKIE,(LONG_PTR)(V))
#define SET_RESULT(H,V) SetWindowLongPtr(H, USERRESULT, (LONG_PTR)(V))

/* typedef */
typedef struct keydef_s keydef_t;
typedef struct keyval_s keyval_t;
typedef struct dialog_s dialog_t;
typedef struct dlgdef_s dlgdef_t;

struct keydef_s {
  const char * key;                     /* dialog name */
  const int    idc;                     /* dialog resource id */
};

/* struct keyval_s { */
/*   const char * key;               /\* key name also defines its type *\/ */
/*   union { */
/*     int      i;                         /\* integer value       *\/ */
/*     float    f;                         /\* float value         *\/ */
/*     char *   s;                         /\* string value        *\/ */
/*     intptr_t iptr;                      /\* ensure size is good *\/ */
/*   } val; */
/* }; */

typedef sc68_dial_f dlgmsg_f;

struct dlgdef_s {
  const char * ident;
  int          idd;
  int        (*init)(dialog_t*);
};

static const int magic = ('W'<<24)|('D'<<16)|('L'<<8)|'G';

struct dialog_s {
  int          magic;                 /* Magic number               */
  HINSTANCE    hinst;                 /* DLL instance (resource)    */
  HWND         hparent;               /* Parent window              */
  HWND         hdlg;                  /* My window                  */
  int          modal;                 /* created modal or modless   */
  int          retval;                /* dialog return code         */
  int          idd;                   /* dialog resource identifier */
  const char * ident;                 /* dialog string identifier   */
  dlgmsg_f     cntl;                  /* dialog user function       */
  void       * cookie;                /* user provided private data */
};

static int  init_config(dialog_t *);
static int  init_fileinfo(dialog_t *);
static int  init_trackselect(dialog_t *);
static void kill_dialog(dialog_t *);
static dialog_t * new_dialog(void *, dlgmsg_f);

static dlgdef_t dlgdef[3] = {
  { "config",      IDD_DLG_CONFIG,      init_config      },
  { "fileinfo",    IDD_DLG_FILEINFO,    init_fileinfo    },
  { "trackselect", IDD_DLG_TRACKSELECT, init_trackselect },
};

/* Forward declarations. */

/* intptr_t dialog_modless(void * cookie, dlgmsg_f cntl); */
/* intptr_t dialog_modal(void * cookie, dlgmsg_f cntl); */

static intptr_t CALLBACK DialogProc(HWND,UINT,WPARAM,LPARAM);

static struct remember_s {
  unsigned int track : 8;
  unsigned int asid  : 2;
  unsigned int set   : 1;
} sel_remember;

/* debug function */
#if defined(DEBUG) || defined(_DEBUG)
#define DBG(F,...) dbg(F, ## __VA_ARGS__)
#define ERR(N,F,...) dbg_error(N,F, ## __VA_ARGS__)

static
void dbg(const char * fmt, ...)
{
  static const char pref[] = "winui  : ";
  char s[256];
  const DWORD max = sizeof(s)-1;
  DWORD i;
  va_list list;
  HANDLE hdl;
  va_start(list,fmt);
  strcpy(s,pref);
  i = sizeof(pref)-1;
  i += vsnprintf(s+i, max-i, fmt, list);
  if (i < max && s[i-1] != '\n')
    s[i++] = '\n';
  if (i > max) i = max;
  s[i] = 0;
  if (hdl = GetStdHandle(STD_ERROR_HANDLE),
      (hdl != NULL && hdl != INVALID_HANDLE_VALUE))
    WriteFile(hdl,s,i,&i,NULL);
  else
    OutputDebugStringA(s);
  va_end(list);
}

static
void dbg_error(int err, const char * fmt, ...)
{
  char str[512];
  int max = sizeof(str), i;
  i = FormatMessageA(
    FORMAT_MESSAGE_FROM_SYSTEM, 0, err,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    str, max, 0);
  if (i<=0)
    i = _snprintf(str,max,"#%d",err);
  else
    while (--i > 0 && isspace(str[i])) ;

  dbg("ERROR: %s\n",str);
  if (fmt) {
    va_list list;
    va_start(list,fmt);
    vsnprintf(str,max,fmt,list);
    va_end(list);
    dbg("ERROR: %s\n", str);
  }
  str[max-1] = 0;
}

#else
# define DBG(F,...) for(;0;)
# define ERR(N,F,...) for(;0;)
#endif

#if 0
static int SetUnsigned(HWND hdlg, int id, unsigned int new_val)
{
  return -!SetDlgItemInt(hdlg, id, new_val, FALSE);
}

static int GetUnsigned(HWND hdlg, int id, unsigned int * ptr_val)
{
  BOOL ok;
  *ptr_val = GetDlgItemInt(hdlg, id, &ok, FALSE);
  return -!ok;
}
#endif

static int SetInt(HWND hdlg, int id, int new_val)
{
  return -!SetDlgItemInt(hdlg, id, new_val, TRUE);
}

static int GetInt(HWND hdlg, int id, int * ptr_val)
{
  BOOL ok;
  *ptr_val = GetDlgItemInt(hdlg, id, &ok, TRUE);
  return -!ok;
}

static int SetText(HWND hdlg, int id, const char * new_text)
{
  return -!SetDlgItemText(hdlg, id, new_text);

}

#if 0
static int GetText(HWND hdlg, int id, char * str, int len)
{
  return -!GetDlgItemText(hdlg, id, str, len);
}
#endif

static int SetFormat(HWND hdlg, int id, const char * fmt, ...)
{
  char buffer[256];
  va_list list;

  va_start(list,fmt);
  _vsnprintf(buffer, sizeof(buffer), fmt, list);
  va_end(list);
  buffer[sizeof(buffer)-1] = 0;
  return SetText(hdlg, id, buffer);
}

static void SetEnable(HWND hdlg, int id, int enable)
{
  EnableWindow(GetDlgItem(hdlg,id),!!enable);
}

static int IsEnabled(HWND hdlg, int id)
{
  return !!IsWindowEnabled(GetDlgItem(hdlg,id));
}

static int GetCheck(HWND hdlg, int id)
{
  switch (SendDlgItemMessage(hdlg, id, BM_GETCHECK, 0, 0)) {
  case BST_CHECKED:   return 1;
  case BST_UNCHECKED: return 0;
  }
  return -1;
}

static void SetCheck(HWND hdlg, int id, int check)
{
  SendDlgItemMessage(hdlg, id, BM_SETCHECK,
                     check ? BST_CHECKED : BST_UNCHECKED, 0);
}

#if 0
static void SetVisible(HWND hdlg, int id, int visible)
{
  ShowWindow(GetDlgItem(hdlg, id), visible ? SW_SHOW : SW_HIDE);
}
#endif

static inline int SetComboPos(HWND hdlg, int idc, int pos)
{
  pos = SendDlgItemMessageA(hdlg, idc, CB_SETCURSEL, pos, 0);
  return pos == CB_ERR ? -1 : pos;
}

static inline int GetComboPos(HWND hdlg, int idc)
{
  int pos = SendDlgItemMessageA(hdlg, idc, CB_GETCURSEL, 0, 0);
  return pos == CB_ERR ? -1 : pos;
}

#if 0
static int init_int(dialog_t * dial, int idc, const char * key)
{
  sc68_dialval_t v;
  if (!dial->cntl(dial->cookie, key, SC68_DIAL_GETI, &v)) {
    DBG("%s(%d,%s) <- %d\n", __FUNCTION__, idc, key, v.i);
    return SetInt(dial->hdlg, idc, v.i);
  }
  return -1;
}
#endif

static int return_int(dialog_t * dial, int idc, const char * key)
{
  sc68_dialval_t v;
  if (!GetInt(dial->hdlg, idc, &v.i)) {
    DBG("%s(%d,%s) -> %d\n", __FUNCTION__, idc, key, v.i);
    return dial->cntl(dial->cookie, key, SC68_DIAL_SETI, &v);
  }
  return -1;
}

static int init_text(dialog_t * dial, int idc, const char * key)
{
  sc68_dialval_t v;
  if (!dial->cntl(dial->cookie, key, SC68_DIAL_GETS, &v)) {
    DBG("%s(%d,%s) <- \"%s\"\n", __FUNCTION__, idc, key, v.s);
    return SetText(dial->hdlg, idc, v.s);
  }
  return -1;
}

#if 0
static int return_text(dialog_t * dial, int idc, const char * key)
{
  char s[128];
  if (!GetText(dial->hdlg, idc, s, sizeof(s))) {
    sc68_dialval_t v;
    v.s = s;
    DBG("%s(%d,%s) -> \"%s\"\n", __FUNCTION__, idc, key, s);
    return dial->cntl(dial->cookie, key, SC68_DIAL_SETS, &v);
  }
  return -1;
}
#endif

static int init_time(dialog_t * dial, int idc, const char * key)
{
  sc68_dialval_t v;
  if (!dial->cntl(dial->cookie, key, SC68_DIAL_GETI, &v))
    return SetFormat(dial->hdlg, idc, "%02u:%02u",
                     (unsigned)v.i/60u,  (unsigned)v.i%60u);
  return -1;
}

static int init_check(dialog_t * dial, int idc, const char * key)
{
  sc68_dialval_t v;
  if (!dial->cntl(dial->cookie, key, SC68_DIAL_GETI, &v)) {
    DBG("%s(%d,%s) <- %s\n", __FUNCTION__, idc, key, v.i?"On":"Off");
    SetCheck(dial->hdlg, idc, v.i);
    return 0;
  }
  return -1;
}

static int return_check(dialog_t * dial, int idc, const char * key)
{
  sc68_dialval_t v;
  v.i = GetCheck(dial->hdlg, idc);
  if (v.i >= 0) {
    DBG("%s(%d,%s) -> %d\n", __FUNCTION__, idc, key, v.i);
    return dial->cntl(dial->cookie, key, SC68_DIAL_SETI, &v);
  }
  return -1;
}

static int init_combo(dialog_t * dial, int idc, const char * key)
{
  int i;
  sc68_dialval_t v;
  HWND hcb = GetDlgItem(dial->hdlg, idc);
  if (!hcb) return -1;

  DBG("%s(%d,%s)\n", __FUNCTION__, idc, key);
  SendMessageA(hcb, CB_RESETCONTENT,  0, 0);
  for (i=0;
       (v.i = i), !dial->cntl(dial->cookie, key, SC68_DIAL_ENUM, &v);
       ++i) {
    DBG("%s(%d) %s[%d] = \"%s\"\n", __FUNCTION__,
        idc, key, i, v.s);
    SendMessageA(hcb, CB_ADDSTRING, 0, (LPARAM)v.s);
  }
  if (!dial->cntl(dial->cookie, key, SC68_DIAL_GETI, &v)) {
    DBG("%s(%d) %s[@] = %d\n", __FUNCTION__, idc, key, v.i);
    SendMessageA(hcb, CB_SETCURSEL, v.i, 0);
  }
  return 0;
}

static int return_combo(dialog_t * dial, int idc, const char * key)
{
  sc68_dialval_t v;
  v.i = SendDlgItemMessageA(dial->hdlg, idc, CB_GETCURSEL,0,0);
  if (v.i >= 0) {
    DBG("%s(%d,%s) -> %d\n", __FUNCTION__, idc, key, v.i);
    return dial->cntl(dial->cookie, key, SC68_DIAL_SETI, &v);
  }
  return -1;
}

static int init_slide(dialog_t * dial, int idc, const char * key,
                      int bud, const int * tic, int ntics)
{
  HWND hcb = GetDlgItem(dial->hdlg, idc);
  sc68_dialval_t min , max, num;

  DBG("%s(%d,%s,%d,%d)\n", __FUNCTION__, idc, key, bud, ntics);

  if (!dial->cntl(dial->cookie, key, SC68_DIAL_MIN,  &min) &&
      !dial->cntl(dial->cookie, key, SC68_DIAL_MAX,  &max) &&
      !dial->cntl(dial->cookie, key, SC68_DIAL_GETI, &num )) {

    DBG("slide \"%s\" %d<%d<%d\n", key, min.i, num.i, max.i);

    SendMessageA(hcb, (UINT)TBM_SETRANGE,
                 (WPARAM)FALSE /*redraw*/,
                 MAKELPARAM(min.i,max.i));
    SendMessageA(hcb, (UINT)TBM_CLEARTICS,
                 (WPARAM)FALSE /*redraw*/, 0);
    if (ntics) {
      int i;
      if (tic) {
        for (i = 0; i < ntics; ++i)
          SendMessageA(hcb, TBM_SETTIC, TRUE, tic[i]);
      } else {
        for (i = 0; i < ntics; ++i) {
          const int v = min.i + (i+1)*(max.i-min.i)/(ntics+1);
          SendMessageA(hcb, TBM_SETTIC, TRUE, v);
        }
      }
    }
    if (bud != IDC_STATIC) {
      SetInt(dial->hdlg, bud, num.i);
      SendMessageA(hcb, (UINT)TBM_SETBUDDY,
                   (WPARAM)FALSE /*position*/,
                   (LPARAM)GetDlgItem(dial->hdlg, bud));
    }

    SendMessageA(hcb, (UINT)TBM_SETPOS,
                 (WPARAM)TRUE /*redraw*/,
                 (LPARAM)num.i /* (opt->min+opt->max)>>1 */);

    return 0;
  }
  return -1;
}

static int return_slide(dialog_t * dial, int idc, const char * key)
{
  sc68_dialval_t v;
  v.i = SendDlgItemMessageA(dial->hdlg, idc,TBM_GETPOS,0,0);
  if (v.i >= 0) {
    DBG("%s(%d,%s) -> %d\n", __FUNCTION__, idc, key, v.i);
    return dial->cntl(dial->cookie, key, SC68_DIAL_SETI, &v);
  }
  return -1;
}

static int init_spin(dialog_t * dial, int idc, const char * key, int bud)
{
  sc68_dialval_t min , max, num;

  DBG("%s(%d,%s,%d)\n", __FUNCTION__, idc, key, bud);
  if (!dial->cntl(dial->cookie, key, SC68_DIAL_MIN, &min) &&
      !dial->cntl(dial->cookie, key, SC68_DIAL_MAX, &max) &&
      !dial->cntl(dial->cookie, key, SC68_DIAL_GETI, &num)) {
    DBG("spin \"%s\" %d<%d<%d\n", key,min.i,num.i,max.i);

    SendDlgItemMessage(dial->hdlg, idc, UDM_SETRANGE32, min.i, max.i);
    SendDlgItemMessage(dial->hdlg, idc, UDM_SETPOS32, 0, num.i);
    SetInt(dial->hdlg, bud, num.i);
    SendDlgItemMessage(dial->hdlg, idc, UDM_SETBUDDY,
                       (WPARAM)GetDlgItem(dial->hdlg, bud), 0);
    return 0;
  }
  return -1;
}

static int return_spin(dialog_t * dial, int idc, const char * key)
{
  sc68_dialval_t v;
  BOOL failed = FALSE;
  v.i = SendDlgItemMessageA(dial->hdlg,idc,UDM_GETPOS32,0,(LPARAM)&failed);
  if (!failed) {
    DBG("%s(%d,%s) -> %d\n", __FUNCTION__, idc, key, v.i);
    return dial->cntl(dial->cookie, key, SC68_DIAL_SETI, &v);
  }
  return -1;
}

static int update_spr(dialog_t * dial)
{
  int v = GetComboPos(dial->hdlg, IDC_CFG_SPR);
  SetEnable(dial->hdlg, IDC_CFG_USRSPR,  !v);
  SetEnable(dial->hdlg, IDC_CFG_SPRSPIN, !v);
  return 0;
}

static int init_config(dialog_t * dial)
{
  int res = 0;
  res |= init_combo(dial,IDC_CFG_YMENGINE,  "ym-engine");
  res |= init_combo(dial,IDC_CFG_YMFILTER,  "ym-filter");
  res |= init_combo(dial,IDC_CFG_YMVOLUME,  "ym-volmodel");
  res |= init_combo(dial,IDC_CFG_ASID,      "asid");
  res |= init_combo(dial,IDC_CFG_SPR,       "sampling");
  res |= init_spin(dial,IDC_CFG_SPRSPIN,    "sampling-rate", IDC_CFG_USRSPR);
  res |= update_spr(dial);
  res |= init_check(dial,IDC_CFG_AGAFILTER, "amiga-filter");
  res |= init_slide(dial,IDC_CFG_AGABLEND,  "amiga-blend", IDC_STATIC, 0, 3);
  res |= init_spin(dial, IDC_CFG_TIMESPIN,  "default-time", IDC_CFG_DEFTIME);
  res |= init_check(dial,IDC_CFG_UFI,       "ufi");
  res |= init_check(dial,IDC_CFG_HOOK,      "hook");
  return -!!res;
}

static int return_config(dialog_t * dial)
{
  int res = 0;

  res |= return_combo(dial,IDC_CFG_YMENGINE,  "ym-engine");
  res |= return_combo(dial,IDC_CFG_YMFILTER,  "ym-filter");
  res |= return_combo(dial,IDC_CFG_YMVOLUME,  "ym-volmodel");
  res |= return_combo(dial,IDC_CFG_ASID,      "asid");
  res |= IsEnabled(dial->hdlg,IDC_CFG_USRSPR)
    ? return_int(dial,IDC_CFG_USRSPR, "sampling-rate")
    : return_combo(dial,IDC_CFG_SPR,  "sampling-rate")
  ;
  res |= return_check(dial,IDC_CFG_AGAFILTER, "amiga-filter");
  res |= return_slide(dial,IDC_CFG_AGABLEND,  "amiga-blend");
  res |= return_spin(dial, IDC_CFG_TIMESPIN,  "default-time");
  res |= return_check(dial,IDC_CFG_UFI,       "ufi");
  res |= return_check(dial,IDC_CFG_HOOK,      "hook");

  if (!res) {
    sc68_dialval_t v;
    if (dial->cntl(dial->cookie,"save",SC68_DIAL_CALL, &v) || v.i)
      res = -1;
  }
  return dial->retval = res ? -3 : 0;
}

static int update_fileinfo(dialog_t * dial, int what)
{
  int res = 0;
  if (what & 1) {
    init_time(dial, IDC_INF_TIME,      "time");
    init_text(dial, IDC_INF_FORMAT,    "format");
    init_text(dial, IDC_INF_GENRE,     "genre");
    init_check(dial, IDC_INF_YM,       "hw_ym");
    init_check(dial, IDC_INF_STE,      "hw_ste");
    init_check(dial, IDC_INF_ASID,     "hw_asid");
    init_text(dial, IDC_INF_TITLE,     "title");
    init_text(dial, IDC_INF_ARTIST,    "artist");
    init_text(dial, IDC_INF_ALBUM,     "album");
    init_text(dial, IDC_INF_RIPPER,    "ripper");
    init_text(dial, IDC_INF_CONVERTER, "converter");
    init_text(dial, IDC_INF_YEAR,      "year");
  }
  if (what & 2) {
    sc68_dialval_t v;
    v.i = GetComboPos(dial->hdlg, IDC_INF_TAGNAME);
    if (v.i >= 0 &&
        !dial->cntl(dial->cookie, "tag-val", SC68_DIAL_ENUM, &v)) {
      SetText(dial->hdlg, IDC_INF_TAGVALUE, v.s);
    }
  }
  InvalidateRect(dial->hdlg,0,TRUE); /* or no erase ... Wiiindooows */
  return res;
}

static int init_fileinfo(dialog_t * dial)
{
  int res = 0;
  init_text(dial, IDC_INF_URI,     "uri");
  init_combo(dial, IDC_INF_TRACK,  "track");
  init_combo(dial,IDC_INF_TAGNAME, "tag-key");
  res = update_fileinfo(dial,3);
  SetEnable(dial->hdlg, IDC_INF_YM, 0);
  SetEnable(dial->hdlg, IDC_INF_STE, 0);
  SetEnable(dial->hdlg, IDC_INF_ASID, 0);
  return res;
}

static int init_trackselect(dialog_t * dial)
{
  int res = 0;
  HWND hdlg = dial->hdlg;
  void * user = dial->cookie;
  sc68_dialval_t v;

  DBG("remember:%s track:%d asid:%d\n",
      sel_remember.set?"On":"Off", sel_remember.track, sel_remember.asid);

  res |= init_text(dial, IDC_SEL_ALBUM, "album");
  res |= init_combo(dial,IDC_SEL_TRACK, "track");
  res |= init_combo(dial,IDC_SEL_ASID,  "asid");

  if (sel_remember.set) {
    v.i = sel_remember.track;
    dial->cntl(user, "track", SC68_DIAL_SETI, &v);
    SetComboPos(dial->hdlg, IDC_SEL_TRACK, v.i);
    v.i = sel_remember.asid;
    dial->cntl(user, "asid", SC68_DIAL_SETI, &v);
    SetComboPos(dial->hdlg, IDC_SEL_ASID, v.i);
  }
  SetCheck(hdlg, IDC_SEL_REMEMBER, sel_remember.set);
  if (!dial->cntl(dial->cookie,"hw_asid", SC68_DIAL_GETI, &v))
    SetEnable(hdlg,IDC_SEL_ASID,!!v.i);
  DBG("%s() -> %d\n", __FUNCTION__, res);

  return res;
}

static int return_tracksel(dialog_t * dial)
{
  int track = GetComboPos(dial->hdlg, IDC_SEL_TRACK);
  int asid  = GetComboPos(dial->hdlg, IDC_SEL_ASID);
  if (track < 0) track = 0;
  if (asid  < 0) asid = 0;
  dial->retval = track & 255;
  if (IsEnabled(dial->hdlg,IDC_SEL_ASID))
    dial->retval |= asid << 8;
  sel_remember.set = GetCheck(dial->hdlg, IDC_SEL_REMEMBER) == 1;
  sel_remember.track = track;
  sel_remember.asid  = asid;
  DBG("remember:%s track:%d asid:%d\n",
      sel_remember.set?"On":"Off", sel_remember.track, sel_remember.asid);
  return dial->retval;
}

static intptr_t CALLBACK DialogProc(
  HWND hdlg,      // handle to dialog box
  UINT umsg,      // message
  WPARAM wparam,  // first message parameter
  LPARAM lparam)  // second message parameter
{
  dialog_t * dial = GET_USERCOOKIE(hdlg);
  int i;

  /* To set a result : */
  /* SetWindowLong(hdlg, DWL_MSGRESULT, lResult); return TRUE; */

  switch(umsg)
  {
  case WM_INITDIALOG:
    DBG("%s() -- WM_INITDIALOG:\n", __FUNCTION__);
    dial = (dialog_t *)lparam;
    if (!dial || dial->magic != magic)  {
      DBG("%s() WARNING user cookie corrupted\n", __FUNCTION__);
      dial = 0;
    } else {
      dial->hdlg = hdlg;
    }

    SET_USERCOOKIE(hdlg, dial);
    for (i=0; i<sizeof(dlgdef)/sizeof(*dlgdef); ++i) {
      if (!strcmp(dial->ident, dlgdef[i].ident )) {
        dlgdef[i].init(dial);
      }
    }
    return TRUE;

  case WM_COMMAND: {
    /* int wNotifyCode = HIWORD(wparam); // notification code */
    int wID = LOWORD(wparam); // item, control, or accelerator identifier
    HWND hwndCtl = (HWND) lparam;       // handle of control

    switch(wID) {

      /* ----------------------------------------------------------------
       * Generic
       * ---------------------------------------------------------------- */

    case IDOK:
      switch (dial->idd) {

        /* OK on trackselect */
      case IDD_DLG_TRACKSELECT:
        return_tracksel(dial); break;

        /* OK on config */
      case IDD_DLG_CONFIG:
        return_config(dial); break;

        /* OK on others */
      default:
        dial->retval = 0; break;
      }
      PostMessage(hdlg, WM_CLOSE, 0,0);
      return TRUE;

    case IDCANCEL:
      PostMessage(hdlg, WM_CLOSE, 0,0);
      return TRUE;

    case IDC_SEL_ASID:
    case IDC_CFG_ASID: {
      sc68_dialval_t asid;
      asid.i = SendMessageA(hwndCtl, CB_GETCURSEL, 0, 0);
      if (asid.i >=0 && asid.i<3) {
        dial->cntl(dial->cookie,"asid", SC68_DIAL_SETI, &asid);
        if (wID == IDC_CFG_ASID)
          dial->cntl(dial->cookie,"asid", SC68_DIAL_CALL, &asid);
      }
    } return TRUE;

    case IDC_SEL_REMEMBER:
      return TRUE;

    case IDC_SEL_TRACK:
    case IDC_INF_TRACK:
      if (HIWORD(wparam) == CBN_SELCHANGE) {
        sc68_dialval_t v;
        v.i = SendMessage(hwndCtl,CB_GETCURSEL,0,0);
        if (v.i != CB_ERR) {
          dial->cntl(dial->cookie,"track", SC68_DIAL_SETI, &v);
          if (wID == IDC_INF_TRACK) {
            update_fileinfo(dial,1);
          } else /* if (wID == IDC_SEL_TRACK) */ {
            if (!dial->cntl(dial->cookie,"hw_asid", SC68_DIAL_GETI, &v))
              SetEnable(hdlg,IDC_SEL_ASID,!!v.i);
          }
        }
      }
      return TRUE;

    case IDC_INF_TAGNAME:
      if (HIWORD(wparam) == CBN_SELCHANGE)
        update_fileinfo(dial,2);
      return TRUE;

    case IDC_CFG_SPR:
      update_spr(dial);
      return TRUE;

    default:
      DBG("#%05d W=%04x/%04x L=%08x\n",
          wID, HIWORD(wparam), LOWORD(wparam), (unsigned)lparam);
      break;
    }
  } return TRUE;

  case WM_CLOSE:
    DBG("%s(%08x) CLOSE(%s)\n", __FUNCTION__,
        (unsigned)hdlg, dial->modal?"modal":"modless");
    if (dial->modal) {
      EndDialog(hdlg,1);
    } else {
      SET_RESULT(hdlg,1);
      DestroyWindow(hdlg);
    }
    return TRUE;

  case WM_DESTROY: {
    DBG("WM_DESTROY retval = %d\n", dial->retval);
    if (!dial->modal)
      kill_dialog(dial);
    SET_USERCOOKIE(hdlg, 0);
    dial->hdlg = 0;
  } break;

  }

  return FALSE;
}

/* ----------------------------------------------------------------------
   Export
   ---------------------------------------------------------------------- */

static void kill_dialog(dialog_t * dial)
{
  if (dial) {
    DBG("kill dialog \"%s\"\n",dial->ident?dial->ident:"(nil)");
    if (dial->cntl) {
      sc68_dialval_t v;
      v.i = dial->retval;
      dial->cntl(dial->cookie,SC68_DIAL_KILL, SC68_DIAL_CALL, &v);
    }
    free(dial);
  }
}

static dialog_t * new_dialog(void * cookie, dlgmsg_f cntl)
{
  sc68_dialval_t v;
  dialog_t tmp, *dial = 0;
  int i;
  const int maxdef = sizeof(dlgdef)/sizeof(*dlgdef);

  if (!cntl)
    return 0;

  ZeroMemory(&tmp,sizeof(tmp));
  tmp.magic   = magic;
  tmp.idd     = IDC_STATIC;
  tmp.cookie  = cookie;
  tmp.cntl    = cntl;
  tmp.retval  = -2;

  /* Introducing myself  */
  v.s =  "win32/native";
  if (cntl(cookie,SC68_DIAL_HELLO,SC68_DIAL_CALL,&v ))
    goto failed;
  DBG("connected to \"%s\"\n", v.s);

  /* Do I know you ? */
  for (i=0; i<maxdef; ++i) {
    if (!strcmp(dlgdef[i].ident,v.s)) {
      tmp.idd = dlgdef[i].idd;
      tmp.ident = dlgdef[i].ident;
      break;
    }
  }
  if (i == maxdef) {
    DBG("don't know a dialog named \"%s\"\n", v.s);
    goto failed;
  }

  /* $$$ the "instance" and "parent" commands need to be changed to
   * something more system independant. */

  if (cntl(cookie, "instance", SC68_DIAL_CALL, &v) || !v.s) {
    v.s = (void*)GetModuleHandleA(0);
    DBG("User did not provide a instance handle -> default is %p\n",v.s);
    if (!v.s)
      goto failed;
  }
  tmp.hinst = (HINSTANCE) v.s;
  DBG("win32 instance -> %p\n", (void*) tmp.hinst);

  if (cntl(cookie, "parent", SC68_DIAL_CALL, &v))
    v.s = 0;                            /* $$TEMP */
    /* goto failed; */
  tmp.hparent = (HWND) v.s;
  DBG("win32 parent window -> %p\n", (void*) tmp.hparent);

  dial = malloc(sizeof(dialog_t));
  if (dial) {
    *dial = tmp;
    return dial;
  }


failed:
  DBG("%s -- failed to create dialog\n", __FUNCTION__);
  if (cntl) {
    v.i = -1;
    cntl(cookie,SC68_DIAL_KILL, SC68_DIAL_CALL, &v);
  }
  return 0;
}

static
intptr_t dialog_modal(void * cookie, dlgmsg_f cntl)
{
  intptr_t ret;
  dialog_t * dial = new_dialog(cookie, cntl);

  DBG("%s -- new_dialog() -> %p\n", __FUNCTION__, dial);

  if (!dial)
    ret = -1;
  else {
    dial->modal = 1;
    /* return the result of EndDialog() */
    ret = DialogBoxParam(dial->hinst,
                         MAKEINTRESOURCE(dial->idd),
                         dial->hparent,
                         (DLGPROC) DialogProc,
                         (LPARAM)dial);
    if (ret <= 0) {
      ERR(GetLastError(), "%s(\"%s\")", __FUNCTION__, dial->ident);
    } else {
      ret = dial->retval;
    }
    kill_dialog(dial);
  }
  DBG("%s -> %d\n", __FUNCTION__, (int) ret);
  return ret;
}

static
intptr_t dialog_modless(void * cookie, dlgmsg_f cntl)
{
  intptr_t ret;
  dialog_t * dial = new_dialog(cookie, cntl);

  DBG("%s -- new_dialog() -> %p\n", __FUNCTION__, dial);


  if (!dial)
    ret = -1;
  else {
    dial->modal = 0;
    dial->hdlg =
      CreateDialogParam(dial->hinst,
                        MAKEINTRESOURCE(dial->idd),
                        dial->hparent,
                        (DLGPROC) DialogProc,
                        (LPARAM)dial);
    if (!dial->hdlg) {
      ERR(GetLastError(), "%s(\"%s\")\n", __FUNCTION__, dial->ident);
      kill_dialog(dial);
      ret = -1;
    } else
      ret = 0;
  }
  DBG("%s -> %d\n", __FUNCTION__, (int) ret);
  return ret;
}

int dial68_frontend(void * data, sc68_dial_f cntl)
{
  sc68_dialval_t v;
  return !cntl(data,SC68_DIAL_WAIT,SC68_DIAL_CALL,&v) && v.i
    ? dialog_modal(data,cntl)
    : dialog_modless(data,cntl)
    ;
}
