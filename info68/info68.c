/*
 *		  info68 - get sc68 file information
 *	       Copyright (C) 2001-2009 Benjamin Gerard
 *	     <benjihan -4t- users.sourceforge -d0t- net>
 *
 * This  program is  free software:  you can  redistribute  it and/or
 * modify it  under the  terms of the  GNU General Public  License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program  is distributed in the  hope that it  will be useful,
 * but  WITHOUT ANY WARRANTY;  without even  the implied  warranty of
 * MERCHANTABILITY or FITNESS FOR  A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a  copy of the GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 *
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* file68 includes */
#include <sc68/error68.h>
#include <sc68/alloc68.h>
#include <sc68/file68.h>
#include <sc68/string68.h>
#include <sc68/init68.h>
#include <sc68/debugmsg68.h>
#include <sc68/url68.h>
#include <sc68/option68.h>

/* Standard includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>

#define BUILD_DATE  __DATE__

static int error(const char *format, ...)
{
  va_list list;
  va_start(list, format);
  error68_va(0, format, list);
  va_end(list);
  return -1;
}

static int file_error(const char *filename)
{
  error ("info68: bad or missing sc68 input file \"%s\"\n", filename);
  return 2;
}

static int tracklist_error(const char *command)
{
  error("Bad track-list \"%s\"\n", command);
  return 4;
}

static void print_option(void * data, 
			 const char * option,
			 const char * envvar,
			 const char * desc)
{
  fprintf(data, 
	  "  %s or `$%s'\n"
	  "     %s \n",
	  option, envvar, desc);
}	 


static int display_help(void)
{
  puts
    ("Usage: info68 [OPTION] [--] <URI> track-list format ...\n"
     "\n"
     "Get and format information on sc68 files.\n");

  puts
    ("Options:\n"
     "\n"
     "  --                  break option parsing\n"
     "  -h --help           display this message and exit\n"
     "  -V --version        display version and exit\n"
     "  -o --output=<URI>   change output to file (- is stdout)\n");
   
  option68_help(stdout,print_option);
   
  puts
    ("\n"
     "Track-list:\n"
     "\n"
     "  syntax:\n"
     "\n"
     "    -DIGIT[-DIGIT][,DIGIT[-DIGIT]...]\n"
     "\n"
     "    track-list executes the next format string for all listed tracks\n"
     "    one after another  in the given order.  First track is number 1.\n"
     "    0 is replaced by the last track number (number of track).\n"
     "\n"
     "  format:\n"
     "\n"
     "    format is a string with special commands sequence. Each of these\n"
     "    commands  start with  a percent '%' char  followed by  a command\n"
     "    identifer.\n");
  puts
    ("  disk-commands:\n"
     "\n"
     "    `%#'         number of tracks\n"
     "    `%?'         default track\n"
     "    `%N'         disk name\n"
     "    `%A'         default track author name\n"
     "    `%C'         default track composer name\n"
     "    `%P'         default track ripper name\n"
     "    `%V'         default track converter name\n"
     "    `%T'         disk time in sec\n"
     "    `%Y'         formated disk time. Format \"TT MM:SS\"\n"
     "    `%H'         all tracks ORed hardware flags (see %h)\n");
  puts
    ("  track-commands:\n"
     "\n"
     "    `%&'         track number\n"
     "    `%n'         track name\n"
     "    `%a'         author name\n"
     "    `%c'         composer name\n"
     "    `%p'         ripper name\n"
     "    `%v'         converter name\n"
     "    `%r'         replay name\n"
     "    `%t'         time in sec\n"
     "    `%y'         formated time. Format \"TT MM:SS\"\n"
     "    `%f'         replay frequency\n"
     "    `%@'         load address (in hexdecimal)\n"
     "    `%h'         hardware flags [YSA] uppercase means activated\n"
     "                   Y:YM-2149,  S:STE  A:Amiga\n");
  puts
    ("  misc-commands:\n"
     "\n"
     "    `%%'         display %\n"
     "    `%L'         display a newline character\n");

  puts
    ("URI:\n"
     "\n"
     "  $ - or\n"
     "  $ stdin://           read file fron standard input.\n"
     "  $ path or\n"
     "  $ file://path or\n"
     "  $ local://path       local file.\n"
     "  $ null://            null file.\n"
     "  $ http://path or\n"
     "  $ ftp://path or\n"
     "  $ ...                load file via given protocol (see curl)\n"
     "  $ sc68://author/hw/title[/track:loop\n"
     "  Access sc68 music database. The music file is first searched in\n"
     "  local music path and if not found in remote music path.\n");

  puts
    ("Examples:\n"
     "\n"
     "  info68 'rsc68://music/Jochen Hippel (Mad Max)/0/Wings Of Death/' '-1,5,4-6' '> %&/%# %N - %a - %n%L'\n"
     "\n"
     "  > 1/10 Wings Of Death - Jochen Hippel (Mad Max) - Level #1\n"
     "  > 5/10 Wings Of Death - Jochen Hippel (Mad Max) - Level #5\n"
     "  > 4/10 Wings Of Death - Jochen Hippel (Mad Max) - Level #4\n"
     "  > 5/10 Wings Of Death - Jochen Hippel (Mad Max) - Level #5\n"
     "  > 6/10 Wings Of Death - Jochen Hippel (Mad Max) - Level #6\n");

  puts
    ("Visit <" PACKAGE_URL ">\n"
     "Report bugs to <" PACKAGE_BUGREPORT ">");
   
  return 1;
}

/* Display version number. */
static int display_version(void)
{
  puts
    (PACKAGE_STRING "\n"
     "\n"
     "Copyright (C) 2001-2009 Benjamin Gerard.\n"
     "License GPLv3+ or later <http://gnu.org/licenses/gpl.html>\n"
     "This is free software: you are free to change and redistribute it.\n"
     "There is NO WARRANTY, to the extent permitted by law.\n"
     "\n"
     "Written by Benjamin Gerard"
     );
  return 0;
}


static const char * HWflags(const hwflags68_t f)
{
  static char flags[] = "YSA";
  flags[0] = f.bit.ym    ? 'Y' : 'y';
  flags[1] = f.bit.ste   ? 'S' : 's';
  flags[2] = f.bit.amiga ? 'A' : 'a';
  return flags;
}

static void Putc(istream68_t *out, const int c)
{
  istream68_putc(out, c);
}

static void Puts(istream68_t *out, const char *s)
{
  istream68_puts(out, s);
}

static void PutI(istream68_t *out, int v)
{
  char buffer[64];
  sprintf(buffer,"%d",v);
  Puts(out, buffer);
}

static void PutX(istream68_t *out, int v)
{
  char buffer[64];
  sprintf(buffer,"%x",v);
  Puts(out, buffer);
}

static int ReadTrackNumber(char **ps, int max)
{
  int n;
  if (!isdigit(**ps)) {
    return -1;
  }
  n = strtol(*ps, ps, 10);
  if (n<0 || n>max) {
    return -1;
  } else if (n==0) {
    n = max;
  }
  return n - 1;
}

/* Process track-list
 * return  0  on end
 * return <0  on error
 * return  1  on continue
 */
static int ReadTrackList(char **trackList, int max, int *from, int *to)
{
  int fromTrack, toTrack;
  char *t = *trackList;
  
  if (t) {
    /* Skip comma ',' */
    while(*t == ',') {
      ++t;
    }
  }

  /* Done with this list. */
  if (!t || !*t) {
    *trackList = 0;
    return 0;
  }

  *trackList = t;
  fromTrack = ReadTrackNumber(trackList, max);
  if (fromTrack < 0) {
    return -1;
  }

  switch(**trackList) {
  case ',': case 0:
    toTrack = fromTrack;
    break;
  case '-':
    (*trackList)++;
    toTrack = ReadTrackNumber(trackList, max);
    if (toTrack < 0) {
      return -2;
    }
    break;
  default:
    return -1;
  }

  *from = fromTrack;
  *to   = toTrack;

  return 1;
}

int main(int na, char **a)
{
  int i;
  /*   hwflags68_t diskHW; */
  disk68_t *d = 0;
  int curTrack, toTrack;
  char *trackList;
  istream68_t *out = 0;
  const char * inname  = 0;
  const char * outname = "stdout://";

  a[0] = "info68";
  alloc68_set(malloc);
  free68_set(free);
  na = file68_init(a,na);
  if (na < 0) {
    error("info68: file68 initialisation failed.\n");
    return -1;
  }

  if (na < 2) {
    error("info68: missing argument. Try --help.\n");
    return 1;
  }
     
  /* Scan options ...   */
  for (i=1; i<na; ++i) {
    if (!strcmp(a[i],"--")) {
      ++i;
      break;
    } else if (!strcmp(a[i],"--help") || !strcmp(a[i],"-h")) {
      return display_help();
    } else if (!strcmp(a[i],"--version") || !strcmp(a[i],"-V")) {
      return display_version();
    } else if ((!strcmp(a[i], "-o") || !strcmp(a[i],"--output"))) {
      if (++i >= na) {
	error("info69: option `%s' missing argument.\n", a[i-1]);
	return 2;
      }
      outname = a[i];
    } else if (a[i] == strstr(a[i],"--output=")) {
      outname = a[i]+9;
    } else {
      break;
    }
  }

  if (i >= na) {
    error ("info68: missing input file.\n");
    return 2;
  }
  inname = a[i];

  out = url68_stream_create(outname, 2);
  if (istream68_open(out)) {
    error ("info68: error opening output (%s).\n",
	   out ? istream68_filename(out) : outname);
    return 2;
  }
  
  /* Load input file */
  d = file68_load_url(inname);
  if (!d) {
    return file_error(inname);
  }

  /* Setup variable */
  trackList = 0;
  toTrack = curTrack = d->default_six;

  /* Main loop */
  for (++i; i<na; ++i) {
    if (!trackList && a[i][0] == '-' && isdigit(a[i][1])) {
      int res;
      trackList = a[i]+1;
      res = ReadTrackList(&trackList, d->nb_six, &curTrack , &toTrack);
      if (res < 0) {
	return tracklist_error(trackList);
      } else if (!res) {
	/* This can't be coz we check that almost one digit was there above */
	error("info68: %s(%d) : Internal bug error;"
	      " program should not reach this point\n", __FILE__, __LINE__);
	return 0x666;
      }
      continue;
    }
       
    while (curTrack <= toTrack) {
      int esc = 0, c;
      char * s;
      music68_t *m = d->mus + curTrack;

      /* Parse format string for this track */
      for (s=a[i], esc=0; (c = *s, c); ++s) {
	if (!esc) {
	  /* Not escaped */
	  if(esc = (c=='%'), !esc) {
	    Putc(out,c);
	  }
	} else {
	  /* Escaped */
	  esc = 0;
	  switch (c) {

	    /* SPECIAL commands */
	  case 'L':
	    Putc(out,'\n');
	    break;
	  case '%':
	    Putc(out,'%');
	    break;
	    
	    /* DISK commands */
	  case '#':
	    PutI(out,d->nb_six);
	    break;
	  case '?':
	    PutI(out,d->default_six+1);
	    break;
	  case 'N':
	    Puts(out,d->name);
	    break;
	  case 'A':
	    Puts(out,d->mus[d->default_six].author);
	    break;
	  case 'C':
	    Puts(out,d->mus[d->default_six].composer);
	    break;
	  case 'P':
	    Puts(out,d->mus[d->default_six].ripper);
	    break;
	  case 'V':
	    Puts(out,d->mus[d->default_six].converter);
	    break;
	  case 'T':
	    PutI(out,d->time_ms/1000u);
	    break;
	  case 'Y':
	    Puts(out,strtime68(0, d->nb_six, d->time_ms/1000u));
	    break;
	  case 'H':
	    Puts(out,HWflags(d->hwflags));
	    break;

	    /* TRACK Commands */
	  case '&':
	    PutI(out,curTrack+1);
	    break;
	  case 'n':
	    Puts(out,m->name);
	    break;
	  case 'a':
	    Puts(out,m->author);
	    break;
	  case 'c':
	    Puts(out,m->composer);
	    break;
	  case 'p':
	    Puts(out,m->ripper);
	    break;
	  case 'v':
	    Puts(out,m->converter);
	    break;
	  case 'r':
	    Puts(out,m->replay ? m->replay : "built-in");
	    break;
	  case 't':
	    PutI(out,m->time_ms/1000u);
	    break;
	  case 'y':
	    Puts(out,strtime68(0, curTrack+1, m->time_ms/1000u));
	    break;
	  case 'h':
	    Puts(out, HWflags(m->hwflags));
	    break;
	  case 'f':
	    PutI(out,m->frq);
	    break;
	  case '@':
	    PutX(out,m->a0);
	    break;

	  default:
	    Putc(out,'%');
	    Putc(out,c);
	  } /* switch */
	} /* if else */
      } /* for */

	/* Get next track. */
      if (++curTrack > toTrack) {
	int res = ReadTrackList(&trackList, d->nb_six, &curTrack , &toTrack);
	if (res < 0) {
	  return tracklist_error(trackList);
	} else if (!res) {
	  toTrack = curTrack = d->default_six;
	  break;
	}
      }
    } /* while track */
  } /* while arg */
    
  istream68_destroy(out);
  free68(d);
  
  return 0;
}
