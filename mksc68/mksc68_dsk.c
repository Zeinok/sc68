/*
 * @file    mksc68_dsk.c
 * @brief   disk functions.
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2011 Benjamin Gerard
 *
 * Time-stamp: <2011-10-31 07:46:58 ben>
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

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "mksc68_dsk.h"
#include "mksc68_msg.h"
#include "mksc68_cmd.h"
#include "mksc68_tag.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include "sc68/file68.h"
#include "sc68/alloc68.h"
#include "sc68/string68.h"
#include "sc68/sc68.h"

typedef struct dsk_s dsk_t;
struct dsk_s {
  unsigned int modified : 1;            /* disk has been modified */
  char * filename;                      /* filename               */
  disk68_t * disk;                      /* actual sc68 disk       */
  int cur_trk;                          /* current track          */
};

static dsk_t dsk;

int dsk_has_disk(void)
{
  return !! dsk.disk;
}

void * dsk_get_disk(void)
{
  return dsk.disk;
}

int dsk_get_tracks(void)
{
  return dsk.disk ? dsk.disk->nb_mus : -1;
}

static int has_disk(void)
{
  int ret = dsk_has_disk();
  if (!ret)
    msgerr("no disk\n");
  return ret;
}

static int is_valid_disk(void)
{
  if (!has_disk())
    return 0;
  if (dsk.disk->nb_mus <= 0) {
    msgerr("disk has no track.\n");
    return 0;
  }
  return 1;
}

static int is_valid_track(int trk)
{
  if (!is_valid_disk())
    return 0;
  if (trk <= 0 || trk > dsk.disk->nb_mus) {
    msgerr("track number #%d out of range [1..%d]\n", trk, dsk.disk->nb_mus);
    return 0;
  }
  return 1;
}

int dsk_load(const char * url, int merge)
{
  int ret = -1;
  disk68_t * newdisk;

  if (!merge && dsk.modified) {
    msgerr("modified data (save or try --force)\n");
    goto error;
  }

  newdisk = file68_load_url(url);
  if (!newdisk) {
    msgerr("failed to load \"%s\"\n",url);
    goto error;
  }

  if (!merge) {
    dsk_kill();
    dsk.filename = strdup(url);
    dsk.disk = newdisk;
    newdisk  = 0;
    dsk.cur_trk = dsk.disk->def_mus;

  } else {
    msgerr("not implemented\n");
    goto error;
  }

  ret = 0;
error:
  if (ret)
    free68(newdisk);
  return ret;
}

int dsk_merge(const char * url)
{
  msgerr("not implemented\n");
  return -1;
}

int dsk_save(const char * url, int version, int gzip)
{
  int err;

  if (!url)
    url = dsk.filename;
  if (!url || !*url) {
    msgerr("missing filename\n");
    return -1;
  }
  msgdbg("saving disk as '%s' gzip:%d version:%d\n", url, gzip, version);

  if (!is_valid_disk())
    return -1;

  if (dsk_validate())
    return -1;

  err = file68_save_url(url, dsk_get_disk(), version, gzip);
  if (!err) {
    dsk.modified = 0;
    msginf("disk saved\n");
  } else
    msgerr("failed to save \"%s\"\n",url);

  return err;
}

static int dsk_alloc(int extra)
{
  msgdbg("disk allocation (extra data %d)\n", extra);
  if (dsk.disk) {
    msgerr("disk already allocated\n");
    return -1;
  }
  if (dsk.disk = file68_new(extra), !dsk.disk) {
    msgerr("disk allocation failed\n");
    return -1;
  }
  msgdbg("disk allocated with %d extra bytes\n", dsk.disk->datasz);
  return 0;
}

int dsk_new(const char * name)
{
  static int cnt = 0;
  char tmp[16];
  const char * n = name;

  if (!name || !name[0]) {
    cnt = (cnt+1) % 100u;
    sprintf(tmp,"unnamed-%02d",cnt);
    n = tmp;
  }

  msgdbg("creating new disk: %s\n", n);
  if (dsk.modified) {
    msgerr("modified data (save or try --force)\n");
    return -1;
  }
  dsk_kill();
  if (dsk_alloc(0) < 0)
    return -1;

  dsk.filename = malloc(strlen(n)+6);
  if (dsk.filename)
    sprintf(dsk.filename,"%s.sc68", n);
  if (name && name[0])
    dsk_tag_set(0,"title",name);

  return 0;
}

int dsk_kill(void)
{
  /* int i; */

  if (dsk.modified) {
    msgwrn("destroying a modified disk\n");
  }
  dsk.modified = 0;

  /* for (i=0; i<max_tags; ++i) */
  /*   tag_clr(dsk.tags); */

  file68_free(dsk.disk);
  dsk.disk = 0;

  free(dsk.filename);
  dsk.filename = 0;

  return 0;
}

extern unsigned int fr2ms(unsigned int fr, unsigned int cpf_or_hz, unsigned int clk);

static unsigned int fr_to_ms(unsigned int fr, unsigned int hz)
{
  return fr2ms(fr,hz,0);
}

int dsk_validate(void)
{
  int t;

  if (!is_valid_disk())
    return -1;

  /* Validate time and loop */
  dsk.disk->time_ms = 0;
  dsk.disk->hwflags.all = 0;
  for (t = 0; t < dsk.disk->nb_mus; ++t) {
    music68_t * m = dsk.disk->mus + t;
    m->loops    = ( m->loops > 0 ) ? m->loops : 1;
    m->first_ms = fr_to_ms(m->first_fr, m->frq);
    m->loops_ms = fr_to_ms(m->loops_fr, m->frq);
    m->total_fr = m->first_fr + (m->loops-1) * m->loops_fr;
    m->total_ms = fr_to_ms(m->total_fr, m->frq);
    m->start_ms = dsk.disk->time_ms;
    dsk.disk->time_ms += m->total_ms;
    dsk.disk->hwflags.all |= m->hwflags.all;
  }

  return 0;
}

const char * dsk_tag_get(int trk, const char * var)
{
  const char * val = 0;
  assert(trk >= 0);
  assert(var);

  if (!is_valid_disk())
    return 0;
  if (trk && !is_valid_track(trk))
    return 0;
  val = tag_get(trk, var);
  return val;
}

static const char * tostr(int v)
{
  static char tmp[64];
  snprintf(tmp,63,"%d",v);
  tmp[63] = 0;
  return tmp;
}

static const char * set_trk_tag(int trk, const char * var, const char * val)
{
  return tag_set(trk, var, val);
}

const char * dsk_tag_set(int trk, const char * var, const char * val)
{
  assert(var);
  return (trk == 0 || is_valid_track(trk))
    ? set_trk_tag(trk, var, val)
    : 0
    ;
}

int dsk_tag_seti(int trk, const char * var, int ival)
{
  const char * val = tostr(ival);
  return
    (trk == 0 || is_valid_track(trk))
    ? set_trk_tag(trk, var, val), 0
    : -1
    ;
}

void dsk_tag_del(int trk, const char * var)
{
  tag_del(trk, var);
}

void dsk_tag_clr(int trk)
{
  tag_del_trk(trk);
}

int dsk_trk_get_default(void)
{
  return dsk.disk ? dsk.disk->def_mus+1 : -1;
}

int dsk_trk_get_current(void)
{
  return dsk.disk ? dsk.cur_trk+1 : 0;
}
