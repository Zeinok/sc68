/* $Id$ */

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
/* #include "sc68/istream68.h" */
#include "sc68/sc68.h"

typedef struct dsk_s dsk_t;
struct dsk_s {
  unsigned int modified : 1;            /* disk has been modified */

  char * filename;                      /* filename               */

  disk68_t * disk;                      /* actual sc68 disk       */
  int cur_trk;                          /* current track          */

  tag_t * tags[100];                    /* extra tags             */
};

static dsk_t dsk;
static const int max_tags = sizeof(dsk.tags)/sizeof(dsk.tags[0]);

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
  return dsk.disk ? dsk.disk->nb_six : -1;
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
  if (dsk.disk->nb_six <= 0) {
    msgerr("disk has no track.\n");
    return 0;
  }
  return 1;
}

static int is_valid_track(int trk)
{
  if (!is_valid_disk())
    return 0;
  if (trk <= 0 || trk > dsk.disk->nb_six) {
    msgerr("track number #%d out of range [1..%d]\n", trk, dsk.disk->nb_six);
    return 0;
  }
  return 1;
}

/* static void free_disk_data(disk68_t * disk, char ** ptr) */
/* { */
/*   char * addr; */
/*   assert(disk); assert(ptr); */
/*   addr = *ptr; */

/*   if ( addr && ( addr < disk->data || addr >= disk->data+disk->datasz ) ) { */
/*     free(*ptr); */
/*     *ptr = 0; */
/*   } */
/* } */

/* static void mus_clean(disk68_t * disk, int num) { */
/*   music68_t * mus; */
/*   assert(disk); */
/*   assert(num>=0); */
/*   assert(num<disk->nb_six); */

/*   mus = disk->mus + num; */

/*   free_disk_data(disk, &mus->name); */
/*   free_disk_data(disk, &mus->author); */
/*   free_disk_data(disk, &mus->replay); */
/*   free_disk_data(disk, &mus->converter); */
/*   free_disk_data(disk, &mus->data); */
/* } */

/* static int mus_set_tag(music68_t * mus, const char * tag, const char * val) */
/* { */
/*   assert(mus); */
/*   assert(tag); */
/*   assert(val); */


/*   if (!strcmp(tag,"title") || !strcmp(tag,"name")) { */
/*   } else if (!strcmp(tag,"author")) { */
/*   } else if (!strcmp(tag,"composer")) { */
/*   } else if (!strcmp(tag,"replay") || !strcmp(tag,"player")) { */
/*   } else if (!strcmp(tag,"converter")) { */
/*   } else if (!strcmp(tag,"ripper")) { */
/*   } */

/*   return -1; */
/* } */


/* static int clean_disk(disk68_t * disk) { */
/*   assert(disk); */
/*   return -1; */
/* } */

static int is_var_available(const char *var)
{
  return var && strcmp(var,SC68_NOFILENAME);
}

int dsk_load(const char * url, int merge)
{
  int i, ret = -1;
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

    dsk.cur_trk = dsk.disk->default_six;

    /* Copy disk tags */
    dsk_tag_seti(0, "tracks", dsk.disk->nb_six);
    dsk_tag_seti(0, "deftrk", dsk.disk->default_six+1);
    if (is_var_available(dsk.disk->name))
      dsk_tag_set(0, "title", dsk.disk->name);

    for (i=1; i<=dsk.disk->nb_six; ++i) {
      music68_t * mus = &dsk.disk->mus[i-1];

      dsk_tag_seti(i, "track", i);
      dsk_tag_seti(i, "d0", mus->d0);
      dsk_tag_seti(i, "a0", mus->a0);
      dsk_tag_seti(i, "frq", mus->frq);
      dsk_tag_seti(i, "frames", mus->frames);
      dsk_tag_seti(i, "duration", mus->time_ms);
      dsk_tag_set(i, "replay", mus->replay);
      dsk_tag_set(i, "title",
                  is_var_available(mus->name) ? mus->name : 0);
      dsk_tag_set(i, "artist",
                  is_var_available(mus->author) ? mus->author : 0);
      dsk_tag_set(i, "composer",
                  is_var_available( mus->composer) ?  mus->composer : 0);
      dsk_tag_set(i, "converter",
                  is_var_available( mus->converter) ?  mus->converter : 0);
      dsk_tag_set(i, "ripper",
                  is_var_available( mus->ripper) ?  mus->ripper : 0);
    }


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

int dsk_save(const char * url, int gzip, int version)
{
  msgerr("not implemented\n");
  return -1;
}

static int dsk_alloc(int extra)
{
  int size = sizeof(*dsk.disk) - sizeof(dsk.disk->data) + extra;

  msgdbg("disk allocation: %d (%d)\n", size, extra);
  if ( !(dsk.disk = calloc68(size))) {
    msgerr("disk allocation failed\n");
    return -1;
  }
  msgdbg("disk allocation: %d (%d)\n", size, extra);
  dsk.disk->datasz = extra;
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
  int i;

  if (dsk.modified) {
    msgwrn("destroying a modified disk\n");
  }
  dsk.modified = 0;

  for (i=0; i<max_tags; ++i)
    tag_clr(dsk.tags);

  free68(dsk.disk);
  dsk.disk = 0;

  free(dsk.filename);
  dsk.filename = 0;

  return 0;
}

static char * get_trk_tag(int trk, const char * var)
{
  tag_t * tag = tag_get(&dsk.tags[trk], var);
  return tag ? tag->val : 0;
}

const char * dsk_tag_get(int trk, const char * var)
{
  char * val = 0;
  assert(trk >= 0);
  assert(var);

  if (!is_valid_disk())
    return 0;
  if (trk && !is_valid_track(trk))
    return 0;
  val = get_trk_tag(trk, var);

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
  tag_t * tag;
  if (val)
    msgdbg("set tag %02d:%-16s \"%s\"\n", trk, var, val);
  else
    msgdbg("del tag %02d:%s\n", trk, var);

  tag = tag_set(&dsk.tags[trk], var, val);
  if (!tag && val) {
    msgerr("failed to set tag %02d:%s <= \"%s\"\n", trk, var, val);
  }
  return tag ? tag->val : 0;
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
    ? set_trk_tag(trk, var, val), ival
    : ~ival
    ;
}

void dsk_tag_del(int trk, const char * var)
{
  dsk_tag_set(trk, var, 0);
}

int dsk_trk_get_default(void)
{
  return dsk.disk ? dsk.disk->default_six+1 : -1;
}

int dsk_trk_get_current(void)
{
  return dsk.disk ? dsk.cur_trk+1 : 0;
}

void * dsk_tag_lst(int trk)
{
  if (!has_disk())
    return 0;
  if (trk > 0 && !is_valid_track(trk))
    return 0;
  return dsk.tags[trk];
}

void * dsk_tag_nxt(void * hdl, const char ** var, const char ** val)
{
  tag_t * tag = hdl;

  assert(tag);
  if (tag) {
    hdl = tag->nxt;
    if (var) *var = tag->var;
    if (val) *val = tag->val;
  }
  return hdl;
}

void dsk_tag_clr(int trk)
{
  if (has_disk() && trk >= 0 && trk <= dsk.disk->nb_six) {
    tag_clr(&dsk.tags[trk]);
  }
}
