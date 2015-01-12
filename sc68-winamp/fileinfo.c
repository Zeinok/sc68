/*
 * @file    fileinfo.c
 * @brief   sc68-ng plugin for winamp 5.5 - file info dialog
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
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

/* libc */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <time.h>

/* sc68 */
#include "sc68/sc68.h"

static const int magic = ('F'<<24)|('I'<<16)|('N'<<8)|'F';

struct cookie_s {
  int          magic;                   /* Points on me */
  HINSTANCE    hinst;                   /* HMOD of sc68cfg DLL */
  HWND         hwnd;                    /* Parent window */
  const char * uri;                     /* Uri of the file */
  sc68_disk_t  disk;                    /* sc68 disk */
};

static inline int ismagic(cookie_t * cookie) {
  const int res = cookie && cookie->magic == magic;
  if (!res) {
    DBG(" !!! NOT MAGIC !!!\n");
  }
  return res;
}

static void del_cookie(cookie_t * cookie)
{
  DBG("fileinfo\n");
  if (ismagic(cookie)) {
    free((void*)cookie->uri);
    sc68_disk_free(cookie->disk);
    free(cookie);
  }
}

#define keyis(N) !strcmp(key,N)

static int cntl(void * _cookie, const char * key, int op, sc68_dialval_t *val)
{
  cookie_t * cookie = (cookie_t *) _cookie;

  if (!key || !ismagic(cookie))
    return -1;

  switch (op) {
  case SC68_DIAL_CALL:
    if (keyis(SC68_DIAL_KILL))
      del_cookie(cookie);
    else if (keyis(SC68_DIAL_HELLO))
      val->s = "fileinfo";
    else if (keyis(SC68_DIAL_WAIT))
      val->i = 0;
    else if (keyis("instance"))
      val->s = (const char *) cookie->hinst;
    else if (keyis("parent"))
      val->s = (const char *) cookie->hwnd;
    else if (keyis("disk"))
      val->s = (const char *) cookie->disk;
    else break;
    return 0;
  case SC68_DIAL_GETS:
    if (keyis("uri"))
      val->s = cookie->uri;
    else break;
    return 0;
  }
  DBG("\"%s\" unknown command #%02d \"%s\"",  "fileinfo", op, key);
  return 1;
}

/* Only exported function. */
int fileinfo_dialog(HINSTANCE hinst, HWND hwnd, const char * uri)
{
  int res = -1;
  cookie_t * cookie = malloc(sizeof(cookie_t));
  if (cookie) {
    cookie->magic = magic;
    cookie->hinst = hinst;
    cookie->hwnd  = hwnd;
    cookie->uri   = strdup(uri);
    if (!cookie->uri ||
        !(cookie->disk = sc68_load_disk_uri(uri)))
      del_cookie(cookie);
    else
      res = sc68_cntl(0, SC68_DIAL, cookie, cntl);

    if (res == -1) {
        /* fileinfo dialog has failed, notify so that next time winamp
         * used the built-in unified fileinfo instead of this one.
         */
      g_useufi = 1;
    }
  }
  DBG("%s() -> %d\n", __FUNCTION__, res);
  return res;
}
