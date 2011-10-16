/*
 *                       sc68 - ICE! depacker
 *             Copyright (C) 1998-2011 Benjamin Gerard
 *             <http://sourceforge.net/users/benjihan>
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
#ifndef PACKAGE_NAME
# define PACKAGE_NAME "unice68"
#endif
#ifndef PACKAGE_VERSION
# define PACKAGE_VERSION __DATE__
#endif

/* #define ICEVERSION  240 */
#include "unice68.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

static int verbose = 0;

/* Message levels */
enum {
  D = 2,                        /* Message level Debug   */
  V = 1,                        /* Message level verbose */
  N = 0,                        /* Message level normal  */
  Q = -1                        /* Message level quiet   */
};

static void message(const int level, const char *format, ...)
{
  va_list list;
  if (verbose >= level) {
    int save_errno = errno;
    va_start(list, format);
    vfprintf(stderr,format,list);
    fflush(stderr);
    va_end(list);
    errno = save_errno;
  }
}

static int print_usage(void)
{
  int ice_version = unice68_ice_version();
  printf
    (
      "Usage: " PACKAGE_NAME " [OPTION] [--] [[<input>] <output>]\n"
      "\n"
      "ICE! depacker %d.%02d\n"
      "\n"
      " `-' as input/output uses respectively stdin/stdout.\n"
      "\n"
      "Options:\n"
      "\n"
      "  -v --verbose   Be more verbose (multiple use possible)\n"
      "  -q --quiet     Be less verbose (multiple use possible)\n"
      "  -h --help      Print this message and exit\n"
      "  -V --version   Print version and copyright and exit\n"
      "\n"
      "Copyright (C) 2003-2009 Benjamin Gerard\n"
      "\n"
      "Visit <" PACKAGE_URL ">\n"
      "Report bugs to <" PACKAGE_BUGREPORT ">\n",
      ice_version/100, ice_version%100
      );
  return 1;
}

static int print_version(void)
{
  puts(unice68_versionstr());
  puts
    (
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

int main(int argc, char *argv[])
{
  int csize, dsize, i, n;
  void * buffer = 0, * depack = 0;
  int err = 255;
  FILE * in = 0, * out = 0;
  const char * fin=0, *fout=0;
  char header[12];

  argv[0] = "unice68";
  for (i=n=1; i<argc; ++i) {
    int c;
    char * arg = argv[i];

    if (arg[0] != '-' || !(c = arg[1])) { /* not an option */
      argv[n++] = arg; continue;
    }
    arg += 2;
    if (c == '-') {
      if (!*arg) {              /* `--' breaks parsing */
        ++i; break;
      }
      /* Long options */
      if (!strcmp(arg,"help")) {
        c = 'h';
      } else if (!strcmp(arg,"version")) {
        c ='V';
      } else if (!strcmp(arg,"verbose")) {
        c = 'v';
      } else if (!strcmp(arg,"quiet")) {
        c = 'q';
      } else {
        fprintf(stderr,"unice68: Invalid option `--%s'.\n", arg);
        return 255;
      }
      arg += strlen(arg);
    }

    /* parse short options */
    do {
      switch (c) {
      case 'h': return print_usage();
      case 'V': return print_version();
      case 'v': ++verbose; break;
      case 'q': --verbose; break;
      default:
        fprintf(stderr,"unice68: Invalid option `-%c'.\n", c);
        return 255;
      }
      c = *arg++;
    } while (c);

  }
  message(D,"Debug messages activated\n");
  while (i<argc) {
    argv[n++] = argv[i++];
  }
  argc = n;
  message(D,"%d arguments\n",argc);

  if (argc > 1) {
    fin = argv[1];
  }
  if (argc > 2) {
    fout = argv[2];
  }
  if (argc > 3) {
    fprintf(stderr,"unice68: Unexcepted argument `%s'\n", argv[3]);
    return 255;
  }

  if (!fin || !strcmp(fin,"-")) {
    fin = "<stdin>";
    in = stdin;
  } else {
    in = fopen(fin,"rb");
  }
  message(V,"input file `%s'\n",fin);
  if (!in) {
    perror(argv[0]);
    goto error;
  }

  err = (int)fread (header, 1, sizeof(header), in);
  if (err == -1) {
    perror(argv[0]);
    goto error;
  }

  if (err < sizeof(header)) {
    fprintf(stderr, "unice68: Not enought byte to be a valid ice file.\n");
    goto error;
  }
  csize = 0;
  dsize = unice68_depacked_size(header, &csize);
  if (dsize == -1) {
    fprintf(stderr, "unice68: Not a valid ice file : missing magic.\n");
    goto error;
  }
  if (csize < 0 || dsize < 0) {
    fprintf(stderr, "weird sizes: %d/%d.\n", csize,dsize);
    goto error;
  }

  message(N,"ICE! compressed:%d uncompressed:%d ratio:%d%%\n",
          csize,dsize,(csize+50)*100/dsize);

  buffer = malloc(csize+sizeof(header));
  if (!buffer) {
    perror(argv[0]);
    goto error;
  }

  memcpy(buffer, header, sizeof(header));
  message(D,"reading %d bytes from `%s' ... ", csize, fin);
  err = (int) fread((char *)buffer + sizeof(header) , 1, csize, in);
  message(D,"%s\n", err != csize ? "failed :(" : "success :)");

  if (err != csize) {
    if (err == -1) {
      perror(argv[0]);
      goto error;
    } else {
      fprintf(stderr, "unice68: Read error get %d bytes on %d requested\n",
              err, csize);
      err = 2;
      goto error;
    }
  }

  depack = malloc(dsize);
  if (!depack) {
    perror(argv[0]);
    err = 255;
    goto error;
  }
  message(D,"%d bytes output buffer allocated, running depacker ... ", dsize);
  err = unice68_depacker(depack, buffer);
  message(D,"%s\n",err ? "failed :(" : "success :)");

  if (!err) {
    if (!fout || !strcmp(fout,"-")) {
      fout = "<stdout>";
      out = stdout;
    } else {
      out = fopen(fout,"wb");
    }
    message(V,"output file is `%s'\n",fout);

    if (!out) {
      perror(argv[0]);
      goto error;
    }
    message(D,"writing %d bytes into output file `%s' ... ", dsize, fout);
    err = (int) fwrite(depack, 1, dsize, out);
    message(D,"%s\n", err != dsize ? "failed :(" : "success :)");
    if (err != dsize) {
      err = -1;
      perror(argv[0]);
      goto error;
    }
    err = 0;
  }

  error:
  if (in && in != stdin) {
    fclose(in);
  }
  if (out) {
    fflush(out);
    if (out != stdout) fclose(out);
  }
  free(buffer);
  free(depack);

  err &= 255;
  message(D,"exit with code %d\n", err);

  return err & 255;
}
