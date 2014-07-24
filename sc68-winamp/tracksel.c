/*
 * @file    tracksel.c
 * @brief   sc68-ng plugin for winamp 5.5 - track selection dialog
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

/* winamp 2 */
#include "winamp/in2.h"

/* libc */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* sc68 */
#include "sc68/sc68.h"

struct cookie_s {
  cookie_t * me;                        /*  */
  HINSTANCE hinst;                      /* HMOD of sc68cfg DLL */
  HWND hwnd;                            /* Parent window */
  sc68_disk_t disk;                     /*  */
  int track;                            /* 0:all >0 track# */
  int asid;                             /* 0:off 1:on 2:force */
  sc68_minfo_t info;                    /*  */
  char tstr[128];
};

static void del_cookie(cookie_t * cookie)
{
  DBG("trackselect\n");
  if (cookie && cookie->me == cookie) {
    free(cookie);
  }
}

static const char ident[] = "trackselect";

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
    else if (keyis("_settrack")) {
      int track = (int) *result;
      if (track < 0 || track > cookie->info.tracks)
        track = 0;
      if (track >0 && track != cookie->info.trk.track) {
        sc68_music_info(0, &cookie->info, track, cookie->disk);
        track = cookie->info.trk.track;
      }
      *result = cookie->track = track;
    } else {
      DBG("unknown command \"%s\"", key);
      *result = (intptr_t)0;
      return -1;
    }
    return 0;
  }

  if (key[0] == 's' && key[1] == '_') {
    if (keyis("s_album"))
      *result = (intptr_t) cookie->info.album;
    else {
      DBG("unknown command \"%s\"", key);
      *result = (intptr_t)0;
      return -1;
    }
    return 0;
  }

  if (key[0] == 'i' && key[1] == '_') {
    if (keyis("i_hw_asid"))
      *result = (intptr_t) cookie->info.trk.asid;
    else {
      DBG("unknown command \"%s\"", key);
      *result = (intptr_t)0;
      return -1;
    }
    return 0;
  }

  if (keyis("a_tracklist")) {
    int i = *result;
    if (i == -2)                        /* get current */
      *result = (intptr_t) cookie->track;
    else if (i == -1)                   /* get count */
      *result = (intptr_t) cookie->info.tracks + 1;
    else if (i >= 0 && i <= cookie->info.tracks) {
      if (!i)
        snprintf(cookie->tstr,sizeof(cookie->tstr),"ALL - %s",
                 cookie->info.album);
      else {
        sc68_music_info_t info;
        sc68_music_info(0, &info, i, cookie->disk);
        snprintf(cookie->tstr,sizeof(cookie->tstr),"%02u - %s",
                 info.trk.track, info.title);
      }
      cookie->tstr[sizeof(cookie->tstr)-1] = 0;
      *result = (intptr_t) cookie->tstr;
    } else {
      *result = (intptr_t)0;
      return -1;
    }
    return 0;
  } else if (keyis("a_asid")) {
    int i = *result;
    if (i == -2)                        /* get current */
      *result = (intptr_t) cookie->asid;
    else if (i == -1)                   /* get count */
      *result = (intptr_t) 3;
    else if (i >= 0 && i < 3) {
      static const char * asid[3] = { "Off", "On", "Force" };
      *result = (intptr_t) asid[i];
    } else {
      *result = (intptr_t)0;
      return -1;
    }
    return 0;
  }

  DBG("unknown command \"%s\"", key);
  *result = (intptr_t)0;
  return -1;
}

/* Only exported function. */
int tracksel_dialog(HINSTANCE hinst, HWND hwnd, sc68_disk_t disk)
{
  cookie_t * cookie = 0;
  int res = -1;

  if (!dialog_modal || !disk)
    goto error;

  cookie = malloc(sizeof(*cookie));
  if (!cookie)
    goto error;

  cookie->me    = cookie;
  cookie->hinst = hinst;
  cookie->hwnd  = hwnd;
  cookie->disk  = disk;
  sc68_music_info(0, &cookie->info, SC68_DEF_TRACK, cookie->disk);
  cookie->track = 0;
  cookie->asid  = 0;
  res = dialog_modal(cookie,getval);
  goto exit;
  DBG("%s() -> %d\n", __FUNCTION__, res);
  return res;

error:
  del_cookie(cookie);
exit:
  DBG("%s() -> %d\n", __FUNCTION__, res);
  return res;
}
