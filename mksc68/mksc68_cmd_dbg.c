/*
 * @file    mksc68_cmd_dbg.c
 * @brief   the "debug" command
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2011 Benjamin Gerard
 *
 * Time-stamp: <2011-10-10 18:29:15 ben>
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

#include "mksc68_cmd.h"
#include "mksc68_msg.h"
#include "mksc68_opt.h"

#include "sc68/msg68.h"

#include <stdio.h>
#include <string.h>

static const opt_t longopts[] = {
  {"list",  0, 0, 'l'},
  {"lst",   0, 0, 'l'},
  {"set",   1, 0, 's'},
  {"clr"  , 1, 0, 'c'},
  {"clear", 1, 0, 'c'},
  {0,0,0,0}
};

/* Callback for message category printing. */
static void
print_cat(void * data,
          const int bit, const char * name, const char * desc)
{
  const char * fmt = "%02d | %-10s | %-40s | %-3s\n";
  const int mask = (msg68_cat_filter(0,0) >> bit) & 1;
  fprintf(data,fmt, bit, name, desc, mask?"ON":"OFF");
}

/* Print message category. */
static int print_cats(void)
{
  printf("message category: current mask is %08X\n",msg68_cat_filter(0,0));
  msg68_cat_help(stdout,print_cat);
  return 0;
}

static
int run_debug(cmd_t * cmd, int argc, char ** argv)
{
  char shortopts[(sizeof(longopts)/sizeof(*longopts))*3];
  int ret = -1, i;
  char * tok = 0;
  const char * delim = "|,:/";
  int command = 0;

  opt_create_short(shortopts, longopts);

  if (argc == 1) {
    print_cats();
    return 0;
  }

  while (1) {
    int longindex;
    int val =
      getopt_long(argc, argv, shortopts, longopts, &longindex);

    switch (val) {
    case  -1: break;                /* Scan finish */
    case 'l':                       /* --lst */
      print_cats();
      break;
    case 'c': case 's': {
      int bits = 0; /* msg68_cat_filter(0, 0); */

      command = val;
      for (tok=strtok(optarg,delim); tok; tok=strtok(0,delim)) {
        msgdbg("token for '%c' '%s'\n",val,tok);
        if (!strcasecmp(tok,"all")) {
          bits = -1;
        } else {
          int bit = msg68_cat_bit(tok);
          if (bit < 0) {
            msgwrn("unknown categorie \"%s\". Try --list.\n",tok);
          } else {
            bits |= 1 << bit;
          }
        }
      }
      msgdbg("bits for '%c' := %08X\n",val,bits);
      if (val == 'c') {
        msg68_cat_filter(bits, 0);
      } else {
        msg68_cat_filter(0, bits);
      }
    } break;
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
    msgwrn("%d extra parameters ignored\n", argc-i);

  ret = 0;
error:
  return ret;
}

cmd_t cmd_debug = {
  /* run */ run_debug,
  /* com */ "debug",
  /* alt */ "dbg",
  /* use */ "[cmd ...]",
  /* des */ "change debug level",
  /* hlp */
  "The `debug' command control debug and message verbosity.\n"
  "Multiple commands are executed sequencially in given order.\n"
  "\n"
  "COMMANDS\n"
  "   -l --lst      list existing category.\n"
  "   -s --set=CAT  coma `,' separed list of category to activate.\n"
  "   -c --clr=CAT  coma `,' separed list of category to deactivate.\n"
  "\n"
  "CAT := ALL|CAT1[,CAT2]*"
};
