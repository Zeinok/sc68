/*
 * @file    info68.c
 * @brief   program to retrieve information from sc68 files
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2011 Benjamin Gerard
 *
 * Time-stamp: <2011-10-13 17:31:11 ben>
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
# include <config.h>
#endif

/* file68 includes */
#include <sc68/error68.h>
#include <sc68/alloc68.h>
#include <sc68/file68.h>
#include <sc68/string68.h>
#include <sc68/msg68.h>
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
  error68_va(format, list);
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
  error("info68: bad track-list \"%s\"\n", command);
  return 4;
}

static void print_option(void * data,
                         const char * option,
                         const char * envvar,
                         const char * desc)
{
  fprintf(data,
          "  %s or `$%s'\n"
          "                      %c%s\n",
          option, envvar,
          (desc[0]>='a' && desc[0]<='z') ? desc[0]-'a'+'A' : desc[0],
          desc+1);
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
     "  --                  Break option parsing\n"
     "  -h --help           Display this message and exit\n"
     "  -V --version        Display version and exit\n"
     "  -o --output=<URI>   Change output to file (- is stdout)\n"
     "  -A --all            Display all information and tags\n");

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
     "    `%0'         display null char\n"
     "    `%L'         display a newline character\n");

  puts
    ("  tag-commands:\n"
     "\n"
     "    `%{tag}'     display named-tag.\n"
     "                 Use uppercase first letter to address disk tag\n"
     "                 and lowercase to address current track tag\n");

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
     "  info68 'sc68://Jochen Hippel (Mad Max)/0/Wings Of Death/' '-1,5,4-6' '> %&/%# %N - %a - %n%L'\n"
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

static void PutC(istream68_t *out, const int c)
{
  istream68_putc(out, c);
}

static void PutS(istream68_t *out, const char *s)
{
  if (s)
    istream68_puts(out, s);
}

static void PutI(istream68_t *out, int v)
{
  char buffer[64];
  sprintf(buffer,"%d",v);
  PutS(out, buffer);
}

static void PutX(istream68_t *out, int v)
{
  char buffer[64];
  sprintf(buffer,"%x",v);
  PutS(out, buffer);
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

int main(int argc, char ** argv)
{
  int  i, opt_all = 0, code = 127;
  /*   hwflags68_t diskHW; */
  disk68_t *d = 0;
  int curTrack, toTrack;
  char *trackList;
  istream68_t * out = 0;
  const char * inname  = 0;
  const char * outname = "stdout://";

  argv[0] = "info68";
  argc = file68_init(argc, argv);
  if (argc < 0) {
    error("info68: file68 initialisation failed.\n");
    goto exit;
  }

  if (argc < 2) {
    error("info68: missing argument. Try --help.\n");
    code = 1;
    goto finish;
  }

  /* Scan options ...  */
  for (i=1; i<argc; ++i) {
    if (!strcmp(argv[i],"--")) {
      ++i;
      break;
    } else if (!strcmp(argv[i],"--help") || !strcmp(argv[i],"-h")) {
      return display_help();
    } else if (!strcmp(argv[i],"--version") || !strcmp(argv[i],"-V")) {
      return display_version();
    } else if (!strcmp(argv[i],"--all") || !strcmp(argv[i],"-A")) {
      opt_all = 1;
    } else if ((!strcmp(argv[i], "-o") || !strcmp(argv[i],"--output"))) {
      if (++i >= argc) {
        error("info69: option `%s' missing argument.\n", argv[i-1]);
        code = 2;
        goto finish;
      }
      outname = argv[i];
    } else if (argv[i] == strstr(argv[i],"--output=")) {
      outname = argv[i]+9;
    } else {
      break;
    }
  }

  if (i >= argc) {
    error ("info68: missing input file.\n");
    code = 3;
    goto finish;
  }
  inname = argv[i];

  out = url68_stream_create(outname, 2);
  if (istream68_open(out)) {
    error ("info68: error opening output (%s).\n",
           out ? istream68_filename(out) : outname);
    code = 4;
    goto finish;
  }

  /* Load input file */
  d = file68_load_url(inname);
  if (!d) {
    code = 5;
    file_error(inname);
    goto finish;
  }

  if (opt_all) {
    int i, j;
    const char * key, * val;

    PutS(out,"file: ");     PutS(out,inname);     PutC(out,'\n');
    PutS(out,"tracks: ");   PutI(out,d->nb_mus);  PutC(out,'\n');
    PutS(out,"default: ");  PutI(out,d->def_mus); PutC(out,'\n');
    PutS(out,"time_ms: ");  PutI(out,d->time_ms); PutC(out,'\n');
    PutS(out,"hardware: "); PutS(out,HWflags(d->hwflags)); PutC(out,'\n');
    for (j=0; !file68_tag_enum(d, 0, j, &key, &val); ++j) {
      PutS(out,key); PutS(out,": "); PutS(out,val); PutC(out,'\n');
    }

    for (i=1; i<=d->nb_mus; ++i) {
      music68_t *m = d->mus+(i-1);
      PutS(out,"track: ");    PutI(out,i);          PutC(out,'\n');
      PutS(out,"remap: ");    PutI(out,m->track);   PutC(out,'\n');
      PutS(out,"loop: ");     PutI(out,m->loop);    PutC(out,'\n');
      PutS(out,"timems: ");   PutI(out,m->time_ms); PutC(out,'\n');
      PutS(out,"frames: ");   PutI(out,m->frames);  PutC(out,'\n');
      PutS(out,"rate: ");     PutI(out,m->frq);     PutC(out,'\n');
      PutS(out,"hardware: "); PutS(out,HWflags(m->hwflags)); PutC(out,'\n');
      for (j=0; !file68_tag_enum(d, i, j, &key, &val); ++j) {
        PutS(out,key); PutS(out,": "); PutS(out,val); PutC(out,'\n');
      }
    }
    code = 0;
    goto finish;
  }

  /* Setup variable */
  trackList = 0;
  toTrack = curTrack = d->def_mus;

  /* Main loop */
  for (++i; i<argc; ++i) {
    if (!trackList && argv[i][0] == '-' && isdigit(argv[i][1])) {
      int res;
      trackList = argv[i]+1;
      res = ReadTrackList(&trackList, d->nb_mus, &curTrack , &toTrack);
      if (res < 0) {
        return tracklist_error(trackList);
      } else if (!res) {
        /* This can't be coz we check that almost one digit was there above */
        error("info68: %s(%d) : Internal bug error;"
              " program should not reach this point\n", __FILE__, __LINE__);
        code = 66;
        goto finish;
      }
      continue;
    }

    while (curTrack <= toTrack) {
      int esc = 0, c;
      char * s;
      music68_t *m = d->mus + curTrack;

      /* Parse format string for this track */
      for (s=argv[i], esc=0; (c = *s, c); ++s) {
        if (!esc) {
          /* Not escaped */
          if(esc = (c=='%'), !esc) {
            PutC(out,c);
          }
        } else {
          /* Escaped */
          esc = 0;
          switch (c) {

            /* SPECIAL commands */
          case 'L':
            PutC(out,'\n');
            break;
          case '%':
            PutC(out,'%');
            break;
          case '0':
            PutC(out,'\0');
            break;

            /* DISK commands */
          case '#':
            PutI(out,d->nb_mus);
            break;
          case '?':
            PutI(out,d->def_mus+1);
            break;
          case 'N':
            PutS(out,d->tags.tag.title.val);
            break;
          case 'A':
            PutS(out,d->tags.tag.artist.val);
            break;
          case 'C':
            PutS(out,file68_tag_get(d,0,TAG68_COMPOSER));
            break;
          case 'P':
            PutS(out,file68_tag_get(d,0,TAG68_RIPPER));
            break;
          case 'V':
            PutS(out,file68_tag_get(d,0,TAG68_CONVERTER));
            break;
          case 'T':
            PutI(out,d->time_ms/1000u);
            break;
          case 'Y':
            PutS(out,strtime68(0, d->nb_mus, d->time_ms/1000u));
            break;
          case 'H':
            PutS(out,HWflags(d->hwflags));
            break;

            /* TRACK Commands */
          case '&':
            PutI(out,curTrack+1);
            break;
          case 'n':
            PutS(out,m->tags.tag.title.val);
            break;
          case 'a':
            PutS(out,m->tags.tag.artist.val);
            break;
          case 'c':
            PutS(out,file68_tag_get(d,curTrack+1,TAG68_COMPOSER));
            break;
          case 'p':
            PutS(out,file68_tag_get(d,curTrack+1,TAG68_RIPPER));
            break;
          case 'v':
            PutS(out,file68_tag_get(d,curTrack+1,TAG68_CONVERTER));
            break;
          case 'r':
            PutS(out, m->replay ? m->replay : "built-in");
            break;
          case 't':
            PutI(out,m->time_ms/1000u);
            break;
          case 'y':
            PutS(out,strtime68(0, curTrack+1, m->time_ms/1000u));
            break;
          case 'h':
            PutS(out, HWflags(m->hwflags));
            break;
          case 'f':
            PutI(out,m->frq);
            break;
          case '@':
            PutX(out,m->a0);
            break;
          case '{': {
            char * key = s+1, * end;
            const char * val;
            if (end = strchr(key,'}'), end) {
              *end = 0;
              val = file68_tag_get(d, isupper(*key) ? 0 : curTrack, key);
              if (val) PutS(out,val);
              *end = '}';
              s = end;
              break;
            }
          }
          default:
            PutC(out,'%');
            PutC(out,c);
          } /* switch */
        } /* if else */
      } /* for */

        /* Get next track. */
      if (++curTrack > toTrack) {
        int res = ReadTrackList(&trackList, d->nb_mus, &curTrack , &toTrack);
        if (res < 0) {
          return tracklist_error(trackList);
        } else if (!res) {
          toTrack = curTrack = d->def_mus;
          break;
        }
      }
    } /* while track */
  } /* while arg */

  code = 0;
finish:
  istream68_destroy(out);
  free68(d);
  file68_shutdown();

exit:
  return code;
}
