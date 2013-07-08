/*
 * @file    sc68.c
 * @brief   command line player
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-07-08 07:31:25 ben>
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

/* need this before sc68.h to have all features defined. */
#include <sc68/istream68.h>
#include <sc68/msg68.h>
#include <sc68/option68.h>
#include <sc68/url68.h>

/* sc68 includes */
#include <sc68/sc68.h>
#include <sc68/string68.h>

/* Standard Includes */
#ifdef HAVE_STDARG_H
# include <stdarg.h>
#endif
#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#else
# ifdef HAVE_MEMORY_H
#  include <memory.h>
# endif
# ifdef HAVE_STRINGS_H
#  include <strings.h>
# endif
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif
#ifdef HAVE_CTYPE_H
# include <ctype.h>
#endif
#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif


#ifdef HAVE_STRUCT_OPTION
typedef struct option my_option_t;
#else
struct my_option_s {
  const char *name;
  int         has_arg;
  int        *flag;
  int         val;
};
typedef struct my_option_s my_option_t;
#endif

static sc68_t * sc68 = 0;
static const int sc68_cat = msg68_DEBUG;
static int opt_verb = msg68_WARNING;
static int opt_vers = 0;
static int opt_help = 0;
static int opt_list = 0;
static int opt_owav = 0;

struct sc68_debug_data_s {
  FILE * out;
  FILE * err;
};
static struct sc68_debug_data_s sc68_debug_data;

static void
sc68_debug_cb(const int bit, sc68_t * sc68, const char *fmt, va_list list)
{
  FILE * out;

  /* select output: always error output except for INFO messages */
  out = bit == msg68_INFO
    ? sc68_debug_data.out
    : sc68_debug_data.err
    ;

  vfprintf(out,fmt,list);
  fflush(out);
}

/* Debug message. */
static void Debug(const char * fmt, ...)
{
  va_list list;
  va_start(list,fmt);
  msg68_va(sc68_cat,fmt,list);
  va_end(list);
}

/* Info message. */
void Info(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_INFO,fmt,list);
  va_end(list);
}

/* Standard message. */
void Print(const char * fmt, ...)
{
  if (opt_verb >= msg68_WARNING) {
    va_list list;
    va_start(list, fmt);
    vfprintf(sc68_debug_data.out,fmt,list);
    va_end(list);
  }
}

/* Warning message. */
void Warning(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_WARNING,fmt,list);
  va_end(list);
}

/* Error message. */
void Error(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_ERROR,fmt,list);
  va_end(list);
}

/* Display version number. */
static int print_version(void)
{
  puts
    (PACKAGE_STRING "\n"
     "\n"
     "Copyright (C) 2001-2009 Benjamin Gerard.\n"
     "License GPLv3+ or later <http://gnu.org/licenses/gpl.html>\n"
     "This is free software: you are free to change and redistribute it.\n"
     "There is NO WARRANTY, to the extent permitted by law.\n"
     "\n"
     "Written by Benjamin Gerard <" PACKAGE_BUGREPORT ">"
      );
  return 0;
}

/* Callback for option printing. */
static void
print_option(void * data,
             const char * option, const char * envvar, const char * desc)
{
  fprintf(data,
          "  %s or `$%s'\n"
          "                      %c%s\n",
          option, envvar,
          (desc[0]>='a' && desc[0]<='z') ? desc[0]-'a'+'A' : desc[0],
          desc+1);
}

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

/* Print usage. */
static int print_usage(void)
{
  puts
    (
      "Usage: sc68 [OPTION ...] <URI>\n"
      "\n"
      "  An /!\\ Atari ST and C= Amiga music player.\n"
      "\n"
      "Options:\n"
      "  -h --help           Print this message and exit\n"
      "  -v --version        Print sc68 version x.y.z and licence and exit\n"
      "  -D --debug-list     Print debug categories and exit\n"
      "  -v --verbose        Print more info\n"
      "  -q --quiet          Do not display music info\n"
      "  -r --rate=<val>     Sampling rate (in hz)\n"
      "  -t --track=<val>    Choose track to play [\"all\"|\"def\"|track-#]\n"
      "  -l --loop=<val>     Track loop [\"def\"|\"inf\"|loop-#]\n"
      "  -o --output=<URI>   Set output\n"
      "  -c --stdout         Output raw to stdout (--output=stdout://)\n"
      "  -n --null           No output (--output=null://)\n"
      "  -w --wav            Riff Wav output. Use in combination with -o.\n"
      );

  option68_help(stdout,print_option);

  puts
    (
      "\n"
      "URI:\n"
      "  stdin://<name>      Standard input\n"
      "  stdout://<name>     Standard output\n"
      "  sterr://<name>      Standard error\n"
      "  null://<name>       Null/Zero\n"
      "  <path> or file://path\n"
      "   or local://path    Local file\n"
      "  http://path or ftp://path\n"
      "   or others          Remote protocol (see curl)\n"
      "  sc68://author/hw/title[/:track[:loop:[time]]]\n"
      "                      Access sc68 music database. The music file is\n"
      "                      searched in `sc68-music' music path, then in\n"
      "                      `sc68-rmusic' music path.\n"
      "\n"
      "Copyright (C) 1998-2009 Benjamin Gerard\n"
      "Visit <" PACKAGE_URL ">\n"
      "Report bugs to <" PACKAGE_BUGREPORT ">"
      );
  return 1;
}

/* Get filename extension */
static char * myext(char * fname)
{
  static const char delims[] = "\\/:";
  int pos, len = strlen(fname);
  /* pos > 0 is correct : don't want '^.ext$' */
  for (pos = len-1; pos > 0; --pos) {
    if (fname[pos] == '.') return fname+pos;
    if (strchr(delims,fname[pos])) break;
  }
  return fname+len;
}

/* Get basename */
static char * mybasename(char * fname)
{
#ifdef HAVE_BASENAME
  fname = basename(fname);
#else
  int pos, len = strlen(fname);
  for (pos = len-1; pos >= 0; --pos) {
    switch (fname[pos]) {
    case '/': case '\\':
      return fname+pos+1;
    }
  }
#endif
  return fname;
}

/** Display to output debug statcked error messages.
 */
static void spool_error_message(sc68_t * sc68)
{
  const char * s;

  if (s = sc68_error_get(sc68), s) {
    msg68_error("%s\n","sc68: stacked error message:");
    do {
      msg68_error("      - %s\n",s);
    } while (s = sc68_error_get(sc68), s);
  }
}

static void DisplayInfo(int track)
{
  sc68_music_info_t Info, * info = &Info;

  if (!sc68_music_info(sc68,info,track,0)) {
    int j, len = 11;

    Print("%-*s : %s %s\n", len, "Disk"   , info->dsk.time, info->album);
    Print("%-*s : %s %s\n", len, "Track"  , info->trk.time, info->title);
    Print("%-*s : %s\n",    len, "Artist" , info->artist);
    Print("%-*s : %02u:%02u,%02u\n", len, "Loop-Len",
          info->loop_ms / 60000u,
          info->loop_ms / 1000u % 60u,
          info->loop_ms % 1000u / 100);

    Print("Disk tags:\n");
    for (j=0; j<info->dsk.tags; ++j)
      Print("* %c%-*s : %s\n",
            toupper((int)*info->dsk.tag[j].key),
            len-3,
            info->dsk.tag[j].key+1,
            info->dsk.tag[j].val);

    Print("Tracks tags:\n");
    for (j=0; j<info->trk.tags; ++j)
      Print("* %c%-*s : %s\n",
            toupper((int)*info->trk.tag[j].key),
            len-3,
            info->trk.tag[j].key+1,
            info->trk.tag[j].val);
  }
}

static char * codestr(int code) {
  static char s[8];
  if (code == SC68_ERROR)
    return "ERROR";
  s[0] = (code & SC68_IDLE )   ? 'I' : 'i';
  s[1] = (code & SC68_CHANGE ) ? 'C' : 'c';
  s[2] = (code & SC68_LOOP )   ? 'L' : 'l';
  s[3] = (code & SC68_END )    ? 'E' : 'e';
  s[4] = (code & SC68_SEEK )   ? 'S' : 's';
  s[5] = 0;
  return s;
}

/* track:  0:all -1:default */
static int PlayLoop(istream68_t * out, int track, int loop)
{
  static char buffer[512 << 2];
  const int max = sizeof(buffer) >> 2;
  int all = 0;
  int code;

  Debug("PlayLoop:\n"
        " track  : %d\n"
        " loop   : %d\n"
        " output : '%s'\n",
        track,loop,istream68_filename(out));

  if (loop == 0) {
    Debug("PlayLoop: default loop resquested\n");
    loop = SC68_DEF_LOOP;
  } else if (loop == -1) {
    Debug("PlayLoop: infinite loop resquested\n");
    loop = SC68_INF_LOOP;
  }

  if (track == 0) {
    Debug("PlayLoop: default track resquested\n");
    track = SC68_DEF_TRACK;
  } else if (track == -1) {
    Debug("PlayLoop: all tracks requested\n");
    track = 1;
    all   = 1;
  } else {
    Debug("PlayLoop: track #%d resquested\n",track);
  }
  Debug(" all    : %s\n",all?"yes":"no");

  /* Set track. */
  code = sc68_play(sc68, track, loop);
  if (code != SC68_ERROR) {
    /* Update return code (load the track) */
    code = sc68_process(sc68, 0, 0);
    Debug("Pass: #%5d, PCM: %4d/%4d, Code: %s,(%02x)\n",
          0, 0, 0, codestr(code), code);
    if (code != SC68_ERROR)
      DisplayInfo(-1);
  }

  while ( ! (code & SC68_END) ) {
    int n = max;

    code = sc68_process(sc68, buffer, &n);

    if (0) {
      static int pass;
      if (++pass < 10 || n != max || (code & ~SC68_IDLE) )
        Debug("Pass: #%5d, PCM: %4d/%4d, Code: %s,(%02x)\n",
              pass, n, max, codestr(code), code);
    }

    if (code == SC68_ERROR)
      break;

    if (code & SC68_LOOP)
      Debug("Loop: #%d\n", sc68_play(sc68, SC68_GET_TRACK, SC68_GET_LOOP));

    if (code & SC68_CHANGE) {
      if (!all)
        break;
      else
        DisplayInfo(-1);
    }

    /* if (n > 0) */
    /* Send audio PCM to stdout. */
    if (istream68_write(out, buffer, n<<2) != (n<<2))
      return -1;
  }
  return -(code == SC68_ERROR);
}

/* Build output URI for wav output */
static char * build_output_uri(char * inname, char * outname)
{
  static char prefix[] = "audio://ao/driver=wav/output=";
  char protocol[32];
  char * namebuf = 0, * ext = 0;
  int len;

  Debug("sc68: create output URI in:'%s' out:'%s'\n",inname,outname);

  if (outname) {
    if (!url68_get_protocol(protocol, 32, outname)) {
      fprintf(stderr,"sc68: can't create wav here '%s://'\n", protocol);
      return 0;
    }
    Debug("sc68: don't have a protocol\n");
  } else {
    /* no output given, make it from inname */
    outname = mybasename(inname);
    ext = ".wav";
    Debug("sc68: basename '%s'\n",outname);
  }

  len  = sizeof(prefix) + strlen(outname) + 8;
  namebuf = malloc(len);
  if (!namebuf) {
    fprintf(stderr,"sc68: %s\n", strerror(errno));
    return 0;
  }
  memcpy(namebuf,prefix,sizeof(prefix));
  strcpy(namebuf+sizeof(prefix)-1, outname);
  if (ext)
    strcpy(myext(namebuf),ext);

  return namebuf;
}

int main(int argc, char *argv[])
{
  char *namebuf = 0, *outname = 0, *inname  = 0;
  const char * tracks  = "def";
  const char * loops   = "def";
  const char * rates   = "def";

  int i,j;
  int track = 0;
  int loop  = 0;
  int rate  = 0;
  int err   = 1;
  sc68_init_t init68;
  sc68_create_t create68;
  istream68_t * out = 0;

  /* Force program name */
  argv[0] = "sc68";

  const my_option_t longopts[] = {
    {"help",       0, 0, 'h'},
    {"version",    0, 0, 'V'},
    {"debug-list", 0, 0, 'D'},
    {"quiet",      0, 0, 'q'},
    {"verbose",    0, 0, 'v'},
    {"stdout",     0, 0, 'c'},
    {"null",       0, 0, 'n'},
    {"wav",        0, 0, 'w'},
    {"output",     1, 0, 'o'},
    {"track",      1, 0, 't'},
    {"loop",       1, 0, 'l'},
    {"rate",       1, 0, 'r'},
    {0,0,0,0}
  };
  char shortopts[(sizeof(longopts)/sizeof(*longopts))*3];

  /* Build short options list from long */
  for (i=j=0; longopts[i].val; ++i) {
    shortopts[j++] = longopts[i].val;
    switch (longopts[i].has_arg) {
    case 2: shortopts[j++] = ':';
    case 1: shortopts[j++] = ':'; break;
    }
  }
  shortopts[j++] = 0;

  /* Initialize sc68 api. */
  sc68_debug_data.out = stdout;
  sc68_debug_data.err = stderr;
  memset(&init68, 0, sizeof(init68));
  init68.argc = argc;
  init68.argv = argv;
  init68.msg_handler = sc68_debug_cb;
  if (sc68_init(&init68)) {
    goto error;
  }
  argc = init68.argc;
  argv = init68.argv;

#if defined(HAVE_GETOPT_LONG) || defined(HAVE_GETOPT)
  while (1) {
# if defined(HAVE_GETOPT_LONG)
    int longindex;
# endif
    int val =
      getopt_long(argc, argv, shortopts, longopts, &longindex);

    switch (val) {
    case  -1: break;                /* Scan finish   */
    case 'h': opt_help = 1; break;  /* --help        */
    case 'V': opt_vers = 1; break;  /* --version     */
    case 'D': opt_list = 1; break;  /* --debug-list  */
    case 'v': ++opt_verb; break;    /* --verbose     */
    case 'q': --opt_verb; break;    /* --quiet       */
    case 'n': case 'c': case 'o':
      if (outname) {
        fprintf(stderr,"%s: output already setted -- '%c'\n", argv[0], val);
        goto error;
      }
      switch (val) {
      case 'n':
        outname = "null://"; break; /* --null        */
      case 'c':
        outname = "-"; break;       /* --stdout      */
      case 'o':
        outname = optarg; break;    /* --output=     */
      }
      break;
    case 't':
      tracks = optarg; break;       /* --track=      */
    case 'l':
      loops = optarg; break;        /* --loop=       */
    case 'r':
      rates = optarg; break;        /* --rate=       */
    case 'w':
      opt_owav = 1; break;          /* --wav         */

    case '?':                       /* Unknown or missing parameter */
      goto error;
    default:
      fprintf(stderr,"%s: unexpected getopt return value (%d)\n", argv[0], val);
      goto error;
    }
    if (val == -1) break;
  };
#else
  fprintf(stderr,"sc68: getopt() need to be implemented\n");
  err = -1;
  goto error;
#endif
  i = optind;

  /* Special program mode for --help --version and --debug-list */

  if (opt_help) {
    err = print_usage();
    goto exit;
  }

  if (opt_vers) {
    err = print_version();
    goto exit;
  }

  if (opt_verb < msg68_CRITICAL) opt_verb = msg68_CRITICAL;
  if (opt_verb > msg68_TRACE)    opt_verb = msg68_TRACE;
  msg68_cat_level(opt_verb);

  if (opt_list) {
    err = print_cats();
    goto exit;
  }


  /* Select input */

  if (!inname && i<argc) {
    inname = argv[i];
  }
  if (!inname) {
    fprintf (stderr, "%s: missing input file. Try --help.\n",argv[0]);
    goto error;
  }
  if (!strcmp(inname,"-")) {
    inname = "stdin://sc68";
  }

  /* Select output
   *
   *  - Doing this earlier so that proper message handler can be set.
   *  - Default output is audio:// even if libsc68 does not have audio
   *    backend.
   */
  if (outname && !strcmp(outname,"-")) {
    outname = "stdout://sc68";
  }

  if (opt_owav) {
    outname = namebuf = build_output_uri(inname, outname);
    if (!outname) goto error;
  } else if (!outname) {
    outname = "audio://default";
  }

  /* Output message to stdout except it is the output. */
  if (!strncmp68(outname,"stdout://",8)) {
    /* output to stdout; divert stdout message to stderr */
    sc68_debug_data.out = stderr;
  } else if (!strncmp68(outname,"stderr://",8)) {
    /* output to stderr; divert stderr message to stdout */
    sc68_debug_data.err = stdout;
  }

  Debug("sc68: input  '%s'\n",inname);
  Debug("sc68: output '%s'\n",outname);

  /* Parse --track= */
  if (!strcmp(tracks,"def")) {
    track = 0;
  } else if (!strcmp(tracks,"all")) {
    track = -1;
  } else {
    track = strtol(tracks,0,10);
  }

  /* Parse --loop= */
  if (!strcmp(loops,"def")) {
    loop = 0;
  } else if (!strcmp(loops,"inf")) {
    loop = -1;
  } else {
    loop = strtol(loops,0,0);
  }

  /* Parse --rate= */
  if (!strcmp(rates,"def")) {
    rate = 0;
  } else {
    rate = strtol(rates,0,0);
  }

  /* Create emulator instance */
  memset(&create68,0,sizeof(create68));
  create68.sampling_rate = rate;
  sc68 = sc68_create(&create68);
  if (!sc68) {
    goto error;
  }

  out = sc68_stream_create(outname, 2);
  if (!out) {
    goto error;
  }
  if (istream68_open(out)) {
    goto error;
  }

  /* Verify sc68 file. */
#if 0  /* Mess with stdin  */
  if (sc68_verify_file(inname) < 0) {
    goto error;
  }
#endif

  if (sc68_load_url(sc68, inname)) {
    goto error;
  }

  /* Loop */
  Debug("sc68: Enter Playloop\n");
  if (PlayLoop(out, track, loop) < 0) {
    Debug("sc68: Exit Playloop with error\n");
    goto error;
  }
  Debug("sc68: Exit Playloop normally\n");
  err = 0;

error:
  if (err) {
    spool_error_message(sc68);
  }
exit:
  istream68_destroy(out);
  free(namebuf);
  sc68_destroy(sc68);
  sc68_shutdown();

  return -!!err;
}
