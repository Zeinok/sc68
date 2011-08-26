/* $Id$ */

#ifndef _MKSC68_DSK_H_
#define _MKSC68_DSK_H_

#include "mksc68_def.h"

EXTERN68 int dsk_new(const char * name);
EXTERN68 int dsk_load(const char * url, int merge);
EXTERN68 int dsk_merge(const char * url);
EXTERN68 int dsk_save(const char * url, int gzip, int version);
EXTERN68 int dsk_kill(void);

EXTERN68 int    dsk_has_disk(void);
EXTERN68 void * dsk_get_disk(void);
EXTERN68 int    dsk_get_tracks(void);

EXTERN68 int dsk_trk_get_current(void);
EXTERN68 int dsk_trk_set_current(int track);
EXTERN68 int dsk_trk_get_default(void);
EXTERN68 int dsk_trk_set_default(int track);

EXTERN68 void * dsk_tag_lst(int trk);
EXTERN68 void * dsk_tag_nxt(void * hdl, const char ** var, const char ** val);

EXTERN68 const char * dsk_tag_get(int trk, const char * var);
EXTERN68 const char * dsk_tag_set(int trk, const char * var, const char * val);

EXTERN68 int  dsk_tag_geti(int trk, const char * var);
EXTERN68 int  dsk_tag_seti(int trk, const char * var, int val);
EXTERN68 void dsk_tag_del(int trk, const char * var);
EXTERN68 void dsk_tag_clr(int trk);

EXTERN68 int dsk_play(int trk, int loop, int start, int len, int bg);
EXTERN68 int dsk_stop(void);

#endif /* ifndef _MKSC68_DSK_H_ */
