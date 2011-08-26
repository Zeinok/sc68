
/* $Id$ */

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif


#include "mksc68_emu.h"
#include "mksc68_msg.h"

#include <assert.h>
/* #include <stdlib.h> */
/* #include <stdio.h> */
#include <string.h>

#include "sc68/sc68.h"
#include "emu68/emu68.h"
#include "io68/io68.h"

sc68_t   * sc68;
emu68_t  * emu68;
io68_t  ** ios68;

extern int sc68_emulators(sc68_t *, emu68_t **, io68_t  ***);

int emu_init(void)
{
  int ret = -1;
  sc68_create_t create68;

  memset(&create68,0,sizeof(create68));
  create68.log2mem     = 19;
  create68.emu68_debug = 1;
  sc68 = sc68_create(&create68);
  if (!sc68)
    goto error;
  
  sc68_emulators(sc68, &emu68, &ios68);
  msgdbg("emulators: emu68 %p\n", emu68);
  msgdbg("emulators: io68 %p '%s'\n", ios68[0], ios68[0]->name);
  msgdbg("emulators: io68 %p '%s'\n", ios68[1], ios68[1]->name);
  msgdbg("emulators: io68 %p '%s'\n", ios68[2], ios68[2]->name);
  msgdbg("emulators: io68 %p '%s'\n", ios68[3], ios68[3]->name);
  msgdbg("emulators: io68 %p '%s'\n", ios68[4], ios68[4]->name);

  ret = 0;
 error:
  if (ret)
    emu_shutdown();

  return ret;
}

void emu_shutdown(void)
{
  if (sc68) {
    sc68_close(sc68);
    sc68_destroy(sc68);
    sc68 = 0;
  }
}

int emu_load_sc68(const char * url)
{
  sc68_disk_t disk;

  assert(url);
  assert(sc68);
  disk = sc68_load_disk_url(url);


  if (!sc68) {
    return -1;
  }
/*   sc68_close(sc68); */

  msgdbg("loading sc68 [%s]\n",url);


  if (sc68_load_url(sc68,url))
    return -1;

  return 0;
}


/* static */
/* disk68_t * merge_disk(disk68_t * diska, disk68_t *  diskb) */
/* { */
/*   return 0; */
/* } */