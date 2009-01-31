/*
 *    sc68 - atari st and amiga music emulator - command line player
 *
 *	      Copyright (C) 2001-2009 Ben(jamin) Gerard
 *	     <benjihan -4t- users.sourceforge -d0t- net>
 *
 * This  program is  free  software: you  can  redistribute it  and/or
 * modify  it under the  terms of  the GNU  General Public  License as
 * published by the Free Software  Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 *
 * You should have  received a copy of the  GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* $Id$ */

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* need this before sc68.h to have all features defined. */
#include <sc68/istream68.h>
#include <sc68/debugmsg68.h>
#include <sc68/init68.h>

/* sc68 includes */
#include <sc68/sc68.h>

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

#if defined(DEBUG) || defined(DEBUG_SC68)
static int sc68_feature = debugmsg68_NEVER;
#else
static int sc68_feature = debugmsg68_CURRENT;
#endif

static void Debug(const char * fmt, ...)
{
  va_list list;

  va_start(list,fmt);
  vdebugmsg68(sc68_feature,fmt,list);
  va_end(list);
}

static FILE * msgfile = 0;

static int opt_verb = 0;
static int opt_help = 0;
static int opt_vers = 0;
static int opt_list = 0;

/* Display standard message */
void Message(const char * fmt, ...)
{
  va_list list;

  if (opt_verb >= 0 && msgfile) {
    va_start(list, fmt);
    vfprintf(msgfile, fmt, list);
    va_end(list);
  }
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

static void print_option(void * data, 
			 const char * option,
			 const char * envvar,
			 const char * desc)
{
  fprintf(data, "  %-19s %s ($%s)\n", option, desc, envvar);
}	 

static void print_feature(void * data, 
			  const int bit,
			  const char * name,
			  const char * desc)
{
  const char * fmt = "%02d | %-10s | %-40s | %-3s\n";
  const int mask = (debugmsg68_mask >> bit) & 1;
  fprintf(data,fmt, bit, name, desc, mask?"ON":"OFF");
} 

static int print_features(void)
{
  printf("debug features: current mask is %08X\n",debugmsg68_mask);
  debugmsg68_feature_help(stdout,print_feature);
  return 0;
}


/** Display usage message. */
static int print_usage(void)
{
  puts
    (
     "Usage: sc68 [OPTION ...] <URI>\n"
     "\n"
     "  An /!\\ Atari ST and C= Amiga music player.\n"
     "\n"
     "Options:\n"
     "  -h --help           Display this message and exit\n"
     "  -v --version        Display sc68 version x.y.z and licence and exit\n"
     "  -v --verbose        Print more info\n"
     "  -q --quiet          Do not display music info\n"
     "  -t --track=#[,#]    Choose track to play [0=all tracks]\n"
     "                      and number of loop [0=infinite]\n"
     "  -o --output=<URI>   Set output\n" 
     "  -c --stdout         Set stdout for output (--output=stdout://)\n"
     "  -n --null           Set null for output (--output=null://)\n"
     );

  file68_option_help(stdout,print_option);

  puts
    (
     "\n"
     "URI:\n"
     "  - or \n"
     "  stdin://           : Read file fron standard input.\n"
     "  path or\n"
     "  file://path or \n"
     "  local://path       : Local sc68 music file.\n"
     "  http://path or \n"
     "  ftp://path or\n"
     "  ...                : Load nusic file via given protocol (see curl)\n"
     "  sc68://author/hw/title[/#track]\n"
     "  Access sc68 music database. The music file is searched first in local\n"
     "  music path and if not found in remote music path.\n"
     "\n"
     "Copyright (C) 1998-2009 Benjamin Gerard\n"
     "Visit <" PACKAGE_URL ">\n"
     "Report bugs to <" PACKAGE_BUGREPORT ">"
     );
  return 1;
}



/** Display to output debug statcked error messages.
 */
static void spool_error_message(sc68_t * sc68)
{
  const char * s;

  if (s = sc68_error_get(sc68), s) {
    fprintf(stderr, "Stacked Error Message:\n");
    do {
      fprintf(stderr, "%s\n", s);
    } while (sc68 && (s = sc68_error_get(sc68), !s));
  }
}

static void DisplayInfo(int track)
{
  sc68_music_info_t info;
  if (!sc68_music_info(sc68,&info,track,0)) {
    Message("Track      : %d/%d\n",info.track, info.tracks);
    Message("Title      : %s\n",info.title);
    Message("Author     : %s\n",info.author);
    Message("Composer   : %s\n",info.composer);
    Message("Ripper     : %s\n",info.ripper);
    Message("Converter  : %s\n",info.converter);
    Message("Replay     : %s\n",info.replay);
    Message("Hardware   : %s\n",info.hwname);
    Message("Start time : %u:%02u\n",
	    info.start_ms/60000u, (info.start_ms/1000u)%60u);
    Message("Duration   : %s\n", info.time);
  }
}

/* track:  0:all -1:default */
static int PlayLoop(istream68_t * out, int track, int loop)
{
  static char buffer[512 * 4];
  int all = 0;
  int code;

  Debug("PlayLoop:\n"
	" track  : %d\n"
	" loop   : %d\n"
	" output : %s\n",
	track,loop,istream68_filename(out));

  if (track == -1) {
    Debug("PlayLoop: default track resquested\n");
    track = 0;
  } else if (track == 0) {
    Debug("PlayLoop: all tracks requested\n");
    track = 1;
    all = 1;
  } else {
    Debug("PlayLoop: track #%d resquested\n",track);
  }
  Debug(" all    : %s\n",all?"yes":"no");

  /* Set track. */
  sc68_play(sc68, track, loop);
  
  /* DisplayInfo(-1); */

  /* Update return code (load the track) */
  code = sc68_process(sc68, 0, 0);
  if (!(code & SC68_END) && (code & SC68_CHANGE)) {
    DisplayInfo(-1);
  }

  while ( ! (code & SC68_END) ) {
    code = sc68_process(sc68, buffer, sizeof(buffer) >> 2);
    if (code == SC68_MIX_ERROR) {
      break;
    }
    if (code & SC68_LOOP) {
      Debug("Loop: #%d\n", sc68_play(sc68, -1, 1));
    }

    if (code & SC68_CHANGE) {
      if (!all) {
	break;
      }
      DisplayInfo(-1);
    }

    /* Send audio PCM to stdout. */
    if (1) {
      if (istream68_write(out, buffer, sizeof(buffer)) != sizeof(buffer)) {
	return -1;
      }
    }
  }

  return -(code == SC68_MIX_ERROR);
}

static int IsIntParam(const char *parm,
		      const char *what,
		      int * res, int * res2)
{
  int cnt = 0;
  if (strstr(parm, what) == parm) {
    int len = strlen(what);
    if (isdigit(parm[len])) {
      char *e;
      *res = strtol(parm+len, &e, 0);
      cnt = 1;
      if (res2 && *e==',' && isdigit(e[1])) {
	*res2 = strtol(e+1,&e, 0);
	cnt = 2;
      }
    }
  }
  return cnt;
}

static char urlbuf[1024];
int main(int argc, char *argv[]) 
{
  const char * outname = 0;
  const char * inname  = 0;
  const char * tracks  = "def";
  int i,j;
  int track = -1;
  int loop  = -1;
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
    {"output",     1, 0, 'o'},
    {"track",      1, 0, 't'},
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
  memset(&init68, 0, sizeof(init68));
  init68.argc = argc;
  init68.argv = argv;
#ifdef DEBUG
  init68.debug        = (sc68_debug_t) vfprintf;
  init68.debug_cookie = stderr;
#endif
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
    case  -1: break;		    /* Scan finish */
    case 'h': opt_help = 1; break;  /* --help */
    case 'V': opt_vers = 1; break;  /* --version */
    case 'D': opt_list = 1; break;  /* --debug-list */
    case 'v': ++opt_verb; break;    /* --verbose */
    case 'q': --opt_verb; break;    /* --quiet */
    case 'n':
      outname = "null://"; break;   /* --null */
    case 'c':
      outname = "stdout://"; break; /* --stdout */
    case 'o':			  
      outname = optarg; break;	    /* --output= */
    case 't':			  
      tracks = optarg; break;	    /* --track= */
    case '?':			    /* Unknown or missing parameter */
      goto error;
    default:
      fprintf(stderr,"%s: unexpected getopt return value (%d)\n", argv[0], val);
      goto error;
    }
    if (val == -1) break;
  };
#else
  fprintf(stderr,"sc68: getopt() need to be implemented\n");
  return -1;
#endif
  i = optind;

/* #if defined(HAVE_GETOPT) || defined(HAVE_GETOPT_LONG) */
/* # endif /\* ifdef HAVE_GETOPT_LONG *\/ */
/* #endif /\* ifdef HAVE_GETOPT *\/ */


  /* Parse tracks */
  if (!strcmp(tracks,"def")) {
    track = -1;
  } else if (!strcmp(tracks,"all")) {
    track = 0;
  } else {
    track = strtol(tracks,0,0);
  }

  /* Scan help and info options */
/*   for (i=1; i<na; ++i) { */
/*     if (!strcmp(a[i],"--")) { */
/*       break; */
/*     } else if (!strcmp(a[i],"--help")) { */
/*       help = 1; */
/*       break; */
/*     } else if (!strcmp(a[i],"--version")) { */
/*       return Version(); */
/*     } else if (!strcmp(a[i],"--quiet")) { */
/*       quiet = 1; */
/*     } else if (!strcmp(a[i],"--debug-features")) { */
/*       debug_list = 1; */
/*     } */

/*   } */

  if (opt_help) {
    return print_usage();
  }

  if (opt_vers) {
    return print_version();
  }

  if (opt_list) {
    return print_features();
  }


  memset(&create68,0,sizeof(create68));
#ifdef DEBUG
  create68.debug_bit    = debugmsg68_CURRENT;
#endif

  sc68 = sc68_create(&create68);
  if (!sc68) {
    goto error;
  }

  /* */
/*   na = init68.argc; */
/*   for (i=1; i<na; ++i) { */
/*     if (!strcmp(a[i],"--")) { */
/*       break; */
/*     } else if (a[i] == strstr(a[i],"--output=")) { */
/*       outname = a[i]+9; */
/*       fprintf (stderr, "set output to %s\n",outname); */
/*     } else if (!strcmp(a[i],"--stdout")) { */
/*       outname = "stdout://"; */
/*     } else if (!strcmp(a[i],"--null")) { */
/*       outname = "null://"; */
/*     } else if (!IsIntParam(a[i],"--track=", &track, &loop)) { */
/*       if (inname) { */
/* 	fprintf(stderr, "Invalid parameters \"%s\"\n", a[i]); */
/* 	return 2; */
/*       } else { */
/* 	inname = a[i]; */
/*       } */
/*     } */
/*   } */

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

  if (!outname) {
    outname = urlbuf;		/*  */
    snprintf(urlbuf,sizeof(urlbuf)-1,"audio://default/rate=%d",
	     create68.sampling_rate);
    urlbuf[sizeof(urlbuf)-1] = 0;
  }

  /* Output message to stdout except it is the output. */
  msgfile = (strstr(outname,"stdout://") == outname) ? stderr : stdout;

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
  Debug("Enter Playloop\n");
  if (PlayLoop(out, track, loop) < 0) {
    Debug("Exit Playloop with error\n");
    goto error;
  }
  Debug("Exit Playloop normally\n");
  err = 0;

 error:
  if (err) {
    spool_error_message(sc68);
  }
  istream68_destroy(out);

  sc68_destroy(sc68);
  sc68_shutdown();

  return -!!err;
}
