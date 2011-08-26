/* $Id$ */

/* Time-stamp: <2009-10-07 18:07:53 ben>  */

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
  int bits = 0;

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
    case 'c': case 's':
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
    msgwrn("%d extra parameters ignored\n", argc-i);

  ret = 0;
error:
  return ret;
}

cmd_t cmd_debug = {
  /* run */ run_debug,
  /* com */ "debug",
  /* alt */ "dbg",
  /* use */ "COMMAND [COMMAND ...]",
  /* des */ "change debug level",
  /* hlp */
  "The `debug' command control debug and message verbosity.\n"
  "Multiple commands are executed sequencially in given order.\n"
  "\n"
  "COMMAND\n"
  "   -l --lst      list existing category.\n"
  "   -s --set=CAT  coma `,' separed list of category to activate.\n"
  "   -c --clr=CAT  coma `,' separed list of category to deactivate.\n"
  "\n"
  "CAT := ALL|CAT1[,CAT2]*\n"
};
