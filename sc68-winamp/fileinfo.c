/*
 * @file    fileinfo.c
 * @brief   sc68-ng plugin for winamp 5.5 - file info dialog
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
/* #include <mmreg.h> */
/* #include <msacm.h> */
/* #include <commctrl.h> */

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
  const char * uri;                     /*  */
  sc68_disk_t disk;                     /*  */
  sc68_minfo_t info;                    /*  */
  char tstr[4];
};

static void del_cookie(cookie_t * cookie)
{
  DBG("fileinfo\n");
  if (cookie && cookie->me == cookie) {
    free((void*)cookie->uri);
    sc68_disk_free(cookie->disk);
    free(cookie);
  }
}

static const char ident[] = "fileinfo";

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
      int track = (int) *result + 1;
      if (track <= 0 || track > cookie->info.tracks)
        track = cookie->info.dsk.track;
      if (track != cookie->info.trk.track)
        sc68_music_info(0, &cookie->info, track, cookie->disk);
      *result = cookie->info.trk.track - 1;
    } else {
      goto unknown;
    }
    return 0;
  }

  if (key[0] == 's' && key[1] == '_') {
    if (keyis("s_uri"))
      *result = (intptr_t) cookie->uri;
    else if (keyis("s_format"))
      *result = (intptr_t) cookie->info.format;
    else if (keyis("s_genre"))
      *result = (intptr_t) cookie->info.genre;
    else if (keyis("s_title"))
      *result = (intptr_t) cookie->info.title;
    else if (keyis("s_artist"))
      *result = (intptr_t) cookie->info.artist;
    else if (keyis("s_album"))
      *result = (intptr_t) cookie->info.album;
    else if (keyis("s_ripper"))
      *result = (intptr_t) cookie->info.ripper;
    else if (keyis("s_converter"))
      *result = (intptr_t) cookie->info.converter;
    else if (keyis("s_year"))
      *result = (intptr_t) cookie->info.year;
    else {
      goto unknown;
    }
    return 0;
  }

  if (key[0] == 'i' && key[1] == '_') {
    if (keyis("i_time"))
      *result = (intptr_t) ((cookie->info.trk.time_ms+500u)/1000u);
    else if (keyis("i_hw_ym"))
      *result = (intptr_t) cookie->info.trk.ym;
    else if (keyis("i_hw_ste"))
      *result = (intptr_t) cookie->info.trk.ste;
    else if (keyis("i_hw_asid"))
      *result = (intptr_t) cookie->info.trk.asid;
    else {
      goto unknown;
    }
    return 0;
  }

  if (keyis("a_tracklist")) {
    int i = *result;
    if (i == -2)                        /* get current */
      *result = (intptr_t) cookie->info.trk.track - 1;
    else if (i == -1)                   /* get count */
      *result = (intptr_t) cookie->info.tracks;
    else if (i >=0 && i < cookie->info.tracks) {
      ++i;
      cookie->tstr[0] = '0' + (i/10u);
      cookie->tstr[1] = '0' + (i%10u);
      cookie->tstr[2] = 0;
      *result = (intptr_t) cookie->tstr;
    } else {
      DBG("invalid index \"%s[%d]\"", key,i);
      goto error;
    }
    return 0;
  }

unknown:
  DBG("unknown command \"%s\"", key);
error:
  *result = (intptr_t)0;
  return -1;
}


/* Only exported function. */
int fileinfo_dialog(HINSTANCE hinst, HWND hwnd, const char * uri)
{
  sc68_disk_t disk = 0;
  cookie_t * cookie = 0;
  int res = -1;

  if (!dialog_modless || !uri)
    goto error;

  disk = sc68_load_disk_uri(uri);
  if (!disk)
    goto error;

  cookie = malloc(sizeof(*cookie));
  if (!cookie)
    goto error;

  cookie->me    = cookie;
  cookie->hinst = hinst;
  cookie->hwnd  = hwnd;
  cookie->uri   = strdup(uri);
  cookie->disk  = disk;
  sc68_music_info(0, &cookie->info, SC68_DEF_TRACK, cookie->disk);
  disk = 0;
  res = dialog_modless(cookie,getval);
  if (!res)
    goto success;

error:
  sc68_disk_free(disk);
  del_cookie(cookie);
success:
  return res;
}

