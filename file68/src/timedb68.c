/*
 * @file    timedb68.c
 * @brief   music duration database
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-06-03 08:50:34 ben>
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "timedb68.h"

#include <stdlib.h>

#define FBIT 32
#define TBIT 5
#define HBIT (32-TBIT)

#define TIMEDB_ENTRY(HASH,TRACK,FRAMES) { (HASH)>>TBIT, TRACK , FRAMES }

typedef struct {
  unsigned int hash   : HBIT;
  unsigned int track  : TBIT;
  unsigned int frames : FBIT;
} dbentry_t;


#define HAVE_TIMEDB_INC_H 1

#ifdef HAVE_TIMEDB_INC_H
static dbentry_t db[] = {
# include "timedb.inc.h"
};
# define DB_MAX   (sizeof(db)/sizeof(*db))
# define DB_COUNT DB_MAX
# define DB_SORT  1
#else
# define DB_MAX   10000
# define DB_COUNT 0
# define DB_SORT  0
static dbentry_t db[DB_MAX];
#endif

static int dbcount  = DB_COUNT;
static int dbmax    = DB_MAX;
static int dbsort   = DB_SORT;
static int dbchange = 0;

/* static const char * db_uri="rsc68://timedb"; */

static int cmp(const void * ea, const void *eb)
{
  const dbentry_t * a = (const dbentry_t *) ea;
  const dbentry_t * b = (const dbentry_t *) eb;
  int va = a->track | (a->hash << TBIT);
  int vb = b->track | (b->hash << TBIT);
  return va - vb;
}

static dbentry_t * search_for(const dbentry_t * key)
{
  if (!dbsort) {
    qsort(db,dbcount,sizeof(dbentry_t), cmp);
    dbsort = 1;
  }
  return (dbentry_t *) bsearch(key, db, dbcount, sizeof(dbentry_t), cmp);
}


int timedb68_add(int hash, int track, unsigned int frames, int flags)
{
  dbentry_t e, *s;
  e.hash   = hash >> TBIT;
  e.track  = track;
  e.frames = frames;

  s = search_for(&e);
  if (!s && dbcount < dbmax) {
    s = db + dbcount++;
    s->hash  = hash >> TBIT;
    s->track = track;
    dbsort = 0;
  }
  if (s)
    s->frames = frames;
  return s - db;
}

int timedb68_get(int hash, int track, unsigned int * frames, int * flags)
{
  dbentry_t e, *s;
  e.hash   = hash >> TBIT;
  e.track  = track;
  s = search_for(&e);
  if (s) {
    if (frames) *frames = s->frames;
    if (flags)  *flags  = 0;
    return s - db;
  }
  return -1;
}

int timedb68_load(void) {
  return -1;
}

int timedb68_save(void) {
  if (dbchange) {
    dbchange = 0;
  }
  return -1;
}
