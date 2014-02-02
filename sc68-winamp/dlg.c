/*
 * @file    dlg.c
 * @brief   sc68-ng plugin for winamp 5.5 - common dialog functions
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2014-01-20 09:00:14 ben>
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

static int SetText(HWND hdlg, int id, const char * new_text)
{
  return -(SendDlgItemMessage(hdlg, id, WM_SETTEXT, 0, (LPARAM)new_text)
           != TRUE);
}

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

static void SetVisible(HWND hdlg, int id, int visible)
{
  ShowWindow(GetDlgItem(hdlg, id), visible ? SW_SHOW : SW_HIDE);
}

static int SetCombo(HWND hdlg, int idc, const char ** label, const int n)
{
  int i;
  HWND hcb = GetDlgItem(hdlg, idc);
  SendMessage(hcb, (UINT)CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
  for (i = 0; i < n; ++i)
    SendMessage(hcb, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)label[i]);
  return 0;
}
