/*
 * @file    sourcer68.c
 * @brief   a 68K sourcer program
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
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

#include "src68_opt.h"
#include "src68_exe.h"
#include "src68_msg.h"
#include "src68_eva.h"
#include "src68_dis.h"
#include "src68_src.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#ifdef USE_FILE68
# include <sc68/file68.h>
# include <sc68/file68_msg.h>
# include <sc68/file68_opt.h>
# include <sc68/file68_uri.h>
/* static */ int sourcer68_cat = msg68_NEVER;
static vfs68_t * out_hdl;
static void close_hdl(vfs68_t * hdl) {
  vfs68_destroy(hdl);
}
static vfs68_t * open_hdl(const char * path) {
  vfs68_t *hdl =  0;
  if (!strcmp(path,"-"))
    path = "stdout://sourcer68";
  hdl = uri68_vfs(path, 2, 0);
  if (vfs68_open(hdl) == -1) {
    vfs68_destroy(hdl);
    hdl = 0;
  }
  return hdl;
}
#else
static FILE * out_hdl;
static void close_hdl(FILE * hdl) {
  if (hdl && hdl != stderrr && hdl != stdout)
    fclose(hdl);
}
static FILE * open_hdl(const char * path) {
  FILE * hdl = 0;
  if (!strcmp(path,"-") || !strncmp(path,"stdout:",7))
    hdl = stdout;
  else if (!strncmp(path,"stderr:",7))
    hdl = stderr;
  else
    hdl = fopen(path,"wt");
  return hdl;
}
#endif

char *inp_uri = 0, *out_uri = 0, *opt_origin = 0, *opt_format = 0;
const char prg[] = PACKAGE_NAME;

#ifdef HAVE_LIBGEN_H
# include <libgen.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif

/* #ifndef HAVE_BASENAME */
/* extern char * basename(char *); */
/* #endif */

enum {
  ECODE_OK,
  ECODE_ARG,
  ECODE_INP,
  ECODE_OUT,
};

enum {
  SRC68_RELOC = (1<<7), /**< Color relocated memory              */
  SRC68_INST  = (1<<6), /**< Color valid instruction cels        */
  SRC68_ENTRY = (1<<5), /**< Color entry points (need Label def) */
  SRC68_RTS   = (1<<4), /**< Color rts instructions              */
  SRC68_DONE  = (1<<3), /**< Color processed memory              */
  SRC68_SIZE  = (7<<0)  /**< Color opcode size (in word)         */
};


/* static void syserr(char *object) */
/* { */
/*   fprintf(stderr,"%s: %s -- %s",ptrg, strerr(errno), object?object:""); */
/* } */


#if defined(USE_FILE68) && defined(DEBUG)
static
void msg_handler(const int b, void * data, const char * fmt, va_list list)
{
  FILE * out = (FILE *)data;
  if (!out) out = stderr;
  vfprintf(out,fmt,list);
}
#endif

/* Display version number. */
static void print_version(void)
{
  puts
    (PACKAGE_STRING "\n"
     "\n"
     "Copyright (c) 1998-2015 Benjamin Gerard.\n"
     "License GPLv3+ or later <http://gnu.org/licenses/gpl.html>\n"
     "This is free software: you are free to change and redistribute it.\n"
     "There is NO WARRANTY, to the extent permitted by law.\n"
     "\n"
     "Written by Benjamin Gerard <" PACKAGE_BUGREPORT ">"
      );
}

#ifdef USE_FILE68
static void print_option(void * data, const char * option, const char * envvar,
                         const char * values, const char * desc)
{
  fprintf(data,"  %s", option);
  if (values && *values)
    fprintf(data,"=%s", values);
  fprintf(data,"\n"
          "    %-16s %c%s\n", envvar,
          (desc[0]>='a' && desc[0]<='z') ? desc[0]-'a'+'A' : desc[0],
          desc+1);
}
#endif

static void print_usage(void)
{
  printf
    (
      "Usage: source68 [OPTIONS] <input>\n"
      ""
      "  A 68000 disassembler/sourcer\n"
      "\n"
#ifndef USE_FILE68
      "input := binary or tos executable file path\n"
#else
      "input := sc68 or binary or tos executable URI\n"
#endif
      "\n"
      "Options:\n"
      "  -h --help --usage      This message and exit\n"
      "  -V --version           Display version and exit\n"
      "  -t --tab=[STRING]      Set tabulation string\n"
      "  -F --format=FORMAT     Set output format options\n"
      "  -O --org=ADDR          Load address (default: @0)\n"
      "  -E --entry=ENTRY-LIST  Add entry points\n"
      "  -P --part=PART         Add a partition\n"
      );

      /* "  --opcode               Print opcodes\n" */
      /* "  --ascii                Convert immediat to ASCII\n" */
      /* "  --no-symbol            Disable symbol in disassembly output\n", */

#ifdef USE_FILE68
  /* puts */
  /*   ( */
  /*     "  --no-sc68              Disable sc68 mode\n" */
  /*     ); */
  puts("");
  option68_help(stdout,print_option);
#endif
  puts
    (
      "\n"
      "  ENTRY-LIST := ENTRY|RANGE(,ENTRY|RANGE ...)\n"
      "\n"
      "  It consist on a coma ',' separated list of ENTRY or RANGE.\n"
      "\n"
      "  Basically an ENTRY is a number corresponding to an address\n"
      "  where to start a disassemble pass.\n"
      "\n"
      "  RANGE := [ENTRY,ENTRY]\n"
      "\n"
      "  A range starts as many passes needed to disassemble the range.\n"
      "  Using one or more indirection 'l' before a range will use the range\n"
      "  as an indirect table of entry. And using a '+' before a range adds\n"
      "  origin to each value of the range (for all indirections too).\n"
      "  Expression can be used mixing \"C\" like numbers, operators,\n"
      "  and parenthesis '()'. Operators are '+-/*&|^<>' with '<' and '>' for\n"
      "  shifting. Warning: NO PRECEDENCE !!!.\n"
      );
  puts
    ("Copyright (c) 1998-2015 Benjamin Gerard.\n"
     "\n"
     "Visit <" PACKAGE_URL ">\n"
     "Report bugs to <" PACKAGE_BUGREPORT ">");
}

static const opt_t longopts[] = {
  { "help",       0, 0, 'h' },
  { "usage",      0, 0, 'h' },
  { "version",    0, 0, 'V' },
  { "format",     1, 0, 'F' },
  { "origin",     1, 0, 'O' },
  { "entry",      1, 0, 'E' },
  { 0,0,0,0 }
};

static char shortopts[(sizeof(longopts)/sizeof(*longopts))*3+1];

// $$$ TEMP

int main(int argc, char **argv)
{
  int i;
  uint_t origin = EXE_DEFAULT;

#if defined(USE_FILE68)
  argc = file68_init(argc, argv);
  sourcer68_cat = msg68_cat("src","sourcer68 specific message",1);
  msg68_set_cookie(stderr);
# ifdef DEBUG
  msg68_set_handler(msg_handler);
# else
  msg68_set_handler(0);
# endif
#endif


  dmsg("Debug messages are ON\n");

  /* create the short options from long */
  shortopts[0] = '-';                   /* preserve option ordering. */
  opt_new(shortopts+1, longopts);

  /* */
  while (1) {
    int longindex = 0;
    int val =
      opt_get(argc, argv, shortopts, longopts, &longindex);
    char ** gotarg = 0;

    switch (val) {
    case  -1: break;                /* Scan finish */
    case 1:                         /* when preserving ordering */
      if (!inp_uri)
        inp_uri = opt_arg();
      else if (!out_uri)
        out_uri = opt_arg();
      else {
        emsg(-1, "too many arguments -- `%s'\n", opt_arg());
        return ECODE_ARG;
      }
      break;

    case ':': case '?':             /* Unknown or missing parameter */
      val = opt_opt();
      if (val)
        emsg(-1, "unknown option -- `%c'\n", val);
      else {
        val = opt_ind() - 1;
        emsg(-1, "unknown option -- `%s'\n",
             (val>0 && val<argc) ?
             argv[val] + (argv[val][0]=='-') + (argv[val][1]=='-') : "?");
      }
      return ECODE_ARG;

    case 'h':                           /* --help */
      print_usage();
      return ECODE_OK;

    case 'V':                           /* --version */
      print_version();
      return ECODE_OK;

    case 'F':                           /* --format= */
      gotarg = opt_arg() ? &opt_format : 0;
      break;

    case 'O':
      gotarg = opt_arg() ? &opt_origin : 0;
      break;

    default:
      emsg(-1, "unhandled option -- `%c' (%d)\n", isgraph(val)?val:'-',val);
      return ECODE_ARG;
    }
    if (val == -1) break;

    if (gotarg) {
      if (*gotarg) {
        if (isalpha(val))
          emsg(-1,"duplicated option -- `%c'\n", val);
        else {
          val = opt_ind() - 1;
          emsg(-1,"duplicated option -- `%s'\n",
               (val>0 && val<argc) ?
               argv[val]+(argv[val][0]=='-')+(argv[val][1]=='-'):"?");
        }
        return ECODE_ARG;
      }
      *gotarg = opt_arg();
      gotarg = 0;
    }
  }

  i = opt_ind();
  if (!inp_uri && i < argc)
    inp_uri = argv[i++];
  if (!out_uri && i < argc)
    out_uri = argv[i++];
  if (i != argc) {
    emsg(-1,"too many arguments. Try --help.\n");
    return ECODE_ARG;
  }

  /* Eval --origin */
  if (opt_origin) {
    const char * argstr = opt_origin, *errstr;
    int tmporg;
    errstr = eva_expr(&tmporg, &argstr,0, 0,0,0);
    if (errstr || *argstr) {
      emsg(-1,"failed to evaluate origin -- \"%s\"\n", opt_origin);
      dmsg("> %s (%d)\n",opt_origin, (argstr - opt_origin));
      dmsg("> %+*s^_ %s\n",
           (argstr - opt_origin),"",  errstr?errstr:"trailing char");
      return ECODE_ARG;
    }
    if ( (uint_t)tmporg > 0x1000000u &&
         tmporg != EXE_DEFAULT) {
      emsg(-1,"invalid org -- %s (%s$%x)\n", opt_origin,
           tmporg<0?"-":"", tmporg<0 ? -tmporg : tmporg);
      return ECODE_ARG;
    }
    origin = tmporg;
    dmsg("** ORIGIN is $%x\n", origin);
  }

  /* Eval format */
  if (opt_format) {
    /* ascii mode */
    /* no symbol */
    /* opcodes */
  }

  /* Check if input filename was given */
  if (!inp_uri) {
    emsg(-1,"missing input. Try --help.\n");
    return ECODE_ARG;
  }

  dmsg("** LOADING \"%s\" at %x\n", inp_uri, origin);

  exe_t * exe = exe_load(inp_uri, origin, LOAD_AS_AUTO);
  if (!exe)
    return ECODE_INP;

  if (exe->sections) {
    uint_t i;


    dmsg("sections: %d\n", exe->sections->cnt);
    for (i=0; i<exe->sections->cnt; ++i) {
      sec_t * sec = section_get(exe->sections, i);
      dmsg("section #%d \"%s\" @$%06x + %u [$%x]\n", i,
           sec->name, sec->addr, sec->size, sec->flag);
    }

    for (i=0; i<exe->sections->cnt; ++i) {
      uint_t a;
      sec_t * sec = section_get(exe->sections, i);
      if ( ! (sec->flag & SECTION_X ))
        continue;
      for (a=0; a<sec->size; a += 2) {
        const uint_t adr = sec->addr + a;
        /* const uint_t off = adr - exe->mbk->org; */
        if (mbk_getmib(exe->mbk, adr) & MIB_ENTRY) {
          walk_t walk;
          memset(&walk,0,sizeof(walk));
          walkopt_set_default(&walk.opt);
          walk.adr = adr;
          walk.exe = exe;
          dis_walk(&walk);
        }
      }
    }


  }

  if (exe) {
    fmt_t * fmt = 0;
    if (!out_uri)
      out_uri = "-";
    out_hdl = open_hdl(out_uri);
    fmt = fmt_new(out_hdl);
    src_exec(fmt, exe/* , inp_uri */);
    close_hdl(out_hdl);
    fmt_del(fmt);
    exe_del(exe);
  }

  /* Verify if input is an sc68 file */
  return ECODE_OK;
}
