/* $Id$ */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "mksc68_opt.h"

void opt_create_short(char * shortopts, const opt_t * longopts)
{
  int i, j;
  for (i=j=0; longopts[i].val; ++i) {
    shortopts[j++] = longopts[i].val;
    switch (longopts[i].has_arg) {
    case 2: shortopts[j++] = ':';
    case 1: shortopts[j++] = ':'; break;
    }
  }
  shortopts[j++] = 0;
  optind = 0;
}

int opt_get(int argc, char * const argv[], const char *optstring,
            const opt_t * longopts, int * longindex)
{
  return
    getopt_long(argc, argv, optstring, longopts, longindex);
}
