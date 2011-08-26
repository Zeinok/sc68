/* $Id$ */

/* Time-stamp: <2009-10-08 19:10:33 ben>  */

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "mksc68_cmd.h"
#include "mksc68_dsk.h"
#include "mksc68_msg.h"
#include "mksc68_opt.h"

static const opt_t longopts[] = {
  {"force",       0, 0, 'f'},
  {"add",         0, 0, 'a'},
  {0,0,0,0}
};


static 
int run_load(cmd_t * cmd, int argc, char ** argv)
{
  char shortopts[(sizeof(longopts)/sizeof(*longopts))*3];
  int ret = -1, force = 0, merge = 0, i;
  char * filename = 0;

  opt_create_short(shortopts, longopts);

  while (1) {
    int longindex;
    int val =
      getopt_long(argc, argv, shortopts, longopts, &longindex);

    switch (val) {
    case  -1: break;                /* Scan finish */
    case 'f': force = 1; break;     /* --force     */
    case 'a': merge = 1; break;     /* --add       */
    case '?':                       /* Unknown or missing parameter */
      goto error;
    default:
      msgerr("unexpected getopt return value (%d)\n", val);
      goto error;
    }
    if (val == -1) break;
  }
  i = optind;

  if (i < argc) {
    filename = argv[i++];
  }
    
  if (i < argc) 
    msgwrn("%d extra parameters ignored\n", argc-i);

  if (force) {
    msgdbg("forcing new disk\n");
    dsk_kill();
  }

  ret = dsk_load(filename, merge);


error:
  return ret;
}

cmd_t cmd_load = {
  /* run */ run_load,
  /* com */ "load",
  /* alt */ "ld",
  /* use */ "[OPTION ...] [--] [URL]",
  /* des */ "load a new disk",
  /* hlp */
  "Load or merge a disk.\n"
  "\n"
  "OPTION\n"
  "   -f --force   force a new disk creation.\n"
  "   -a --add     merge current and loaded disks.\n"
};
