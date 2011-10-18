/*
 * @file    mksc68_cmd_save.c
 * @brief   the "save" command
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2011 Benjamin Gerard
 *
 * Time-stamp: <2011-10-18 02:15:48 ben>
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

#include <ctype.h>

#include "mksc68_cmd.h"
#include "mksc68_dsk.h"
#include "mksc68_msg.h"
#include "mksc68_opt.h"

static const opt_t longopts[] = {
  {"gzip",    1, 0, 'z'},
  {0,0,0,0}
};


static
int run_save(cmd_t * cmd, int argc, char ** argv)
{
  char shortopts[(sizeof(longopts)/sizeof(*longopts))*3];
  int ret = -1, gzip = 0, version = 0, i;
  char * filename = 0;

  opt_create_short(shortopts, longopts);

  while (1) {
    int longindex;
    int val =
      getopt_long(argc, argv, shortopts, longopts, &longindex);

    switch (val) {
    case  -1: break;                /* Scan finish  */
    case 'z':                       /* --gzip=[0-9] */
      if (!isdigit(*optarg) || optarg[1]) {
        msgerr("invalid compression level \"%s\"\n", optarg);
        goto error;
      }
      gzip = *optarg - '0';
      break;

    case '?':                       /* Unknown or missing parameter */
      goto error;
    default:
      msgerr("unexpected getopt return value (%d)\n", val);
      goto error;
    }
    if (val == -1) break;
  }
  i = optind;

  if (i < argc)
    filename = argv[i++];
  if (i < argc)
    msgwrn("%d extra parameters ignored\n", argc-i);

  ret = dsk_save(filename, version, gzip);

error:
  return ret;
}

cmd_t cmd_save = {
  /* run */ run_save,
  /* com */ "save",
  /* alt */ "sd",
  /* use */ "[opts] [<url>]",
  /* des */ "save disk",
  /* hlp */
  "The `save' command saves the disk.\n"
  "\n"
  "OPTION\n"
  "   -z --gzip=#  Set compression level [0..9]."
};
