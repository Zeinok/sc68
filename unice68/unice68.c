/*
 * @file    unice68.c
 * @brief   program to pack and depack ICE! packed files
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-07-23 07:19:04 ben>
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

#ifndef PACKAGE_NAME
# define PACKAGE_NAME "unice68"
#endif

#ifndef PACKAGE_VERSION
# define PACKAGE_VERSION __DATE__
#endif

#include "unice68.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>
#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif

static char * prg;
static int    verbose;
static FILE * msgout;

#ifndef HAVE_FREOPEN
static FILE *freopen(const char *path, const char *mode, FILE *stream)
{
  return stream;
}
#endif

#ifndef HAVE_BASENAME
# ifndef PATHSEP
#  ifdef NATIVE_WIN32
#   define PATHSEP '\\'
#  else
#   define PATHSEP '/'
#  endif
# endif
static char *basename(char *path)
{
  char * s = strrchr(path, PATHSEP);
  return !s ? path : s+1;
}
#endif

/* Message levels */
enum {
  D = 2,                        /* Message level Debug   */
  V = 1,                        /* Message level verbose */
  N = 0,                        /* Message level normal  */
  E = -1                        /* Message level error   */
};

static void message_va(const int level, const char *fmt, va_list list)
{
  if (verbose >= level) {
    int save_errno = errno;
    FILE * out =  (level == E) ? stderr : msgout;
    vfprintf(out,fmt,list);
    fflush(out);
    errno = save_errno;
  }
}

static void message(const int level, const char *fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  message_va(level,fmt,list);
  va_end(list);
}

static void error(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  message(E,"%s: ", prg);
  message_va(E,fmt,list);
  va_end(list);
}

static void syserror(const char * obj)
{
  message(E,"%s: %s -- %s\n", prg, obj, strerror(errno));
}

static int print_usage(void)
{
  int ice_d_version = unice68_unice_version();
  int ice_p_version = unice68_ice_version();

  printf
    (
      "Usage: %s [MODE] [OPTION...] [--] [[<input>] <output>]\n"
      "\n"
      "ICE! depacker %d.%02d\n"
      "       packer %d.%-2d\n"
      "\n"
      " `-' as input/output uses respectively stdin/stdout.\n"
      "\n"
      "Modes:\n"
      /* "  -c --cat       Output input file as is if not ICE! packed\n" */
      "  -t --test      Test if input is an ICE! packed file\n"
      "  -T --deep-test Test if input is a valid ICE! packed file\n"
      "  -d --depack    Depack mode\n"
      "  -p --pack      Pack mode\n"
      "  -P --pack-old  Force pack with deprecated 'Ice!' identifier\n"
      "  -s --stress    Pack and unpack <input> for testing\n"
      "\n"
      " If no mode is given the default is to pack an unpacked file\n"
      " and to unpack a packed one.\n"
      "\n"
      "Options:\n"
      "  -h --help      Print this message and exit\n"
      "  -V --version   Print version and copyright and exit\n"
      "\n"
      "  -v --verbose   Be more verbose (multiple use possible)\n"
      "  -q --quiet     Be less verbose (multiple use possible)\n"
      "\n"
      "Copyright (C) 2003-2013 Benjamin Gerard\n"
      "\n"
      "Visit <" PACKAGE_URL ">\n"
      "Report bugs to <" PACKAGE_BUGREPORT ">\n",
      prg,
      ice_d_version/100, ice_d_version%100,
      ice_p_version/100, ice_p_version%100
      );
  return 1;
}

static int print_version(void)
{
  puts(unice68_versionstr());
  puts
    (
      "\n"
      "Copyright (C) 1998-2013 Benjamin Gerard.\n"
      "License GPLv3+ or later <http://gnu.org/licenses/gpl.html>\n"
      "This is free software: you are free to change and redistribute it.\n"
      "There is NO WARRANTY, to the extent permitted by law.\n"
      "\n"
      "Written by Benjamin Gerard"
      );
  return 0;
}

static unsigned hash_buffer(const char * buf, int len)
{
  const unsigned char * k = (unsigned char *) buf;
  unsigned int h = 0;
  while (--len >= 0) {
    h += *k++;
    h += h << 10;
    h ^= h >> 6;
  }
  return h;
}

int main(int argc, char *argv[])
{
  int err = 255;
  int mode = 0, oneop = 0, sens = 0;

  char * finp = 0, *fout = 0;
  char * ibuffer = 0, * obuffer = 0;
  FILE * inp = 0, * out = 0;
  int    ilen = -1, olen = -1;
  int   csize = 0, dsize = -1;

  int i, n, hread;
  char header[12];

  unsigned hash1 = 0;
  int verified = 0;

  msgout = stdout;
  prg = basename(argv[0]);
  if (!prg)
    prg = PACKAGE_NAME;
  argv[0] = prg;
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
        c = 'V';
      } else if (!strcmp(arg,"verbose")) {
        c = 'v';
      } else if (!strcmp(arg,"quiet")) {
        c = 'q';
        /* } else if (!strcmp(arg,"cat")) { */
        /*   c = 'c'; */
      } else if (!strcmp(arg,"test")) {
        c = 't';
      } else if (!strcmp(arg,"deep-test")) {
        c = 'T';
      } else if (!strcmp(arg,"depack")) {
        c = 'd';
      } else if (!strcmp(arg,"pack")) {
        c = 'p';
      } else if (!strcmp(arg,"pack-old")) {
        c = 'P';
      } else if (!strcmp(arg,"stress")) {
        c = 's';
      } else {
        error("invalid option `--%s'.\n", arg);
        return 255;
      }
      arg += strlen(arg);
    }

    /* short options */
    do {
      switch (c) {
      case 'h': return print_usage();
      case 'V': return print_version();
      case 'v': ++verbose; break;
      case 'q': --verbose; break;
      case 'd': case 't': case 'T':
        /* case 'c': */
        sens = 'd';
      case 'p': case 'P': case 's':

        if (!sens) sens = 'p';
        if (mode != 0) {
          error("only one mode can be specified.\n");
          return 255;
        }
        oneop = !!strchr("tTs", c);
        mode = c;
        break;
      default:
        error("invalid option `-%c'.\n", c);
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

  if (argc > 2 + !oneop) {
    error("too many argument -- `%s'\n", argv[2 + !oneop]);
    return 255;
  }
  if (argc > 1) {
    finp = (argv[1][0] == '-' && !argv[1][1])
      ? 0 : argv[1];
  }
  if (argc > 2) {
    fout = (argv[2][0] == '-' && !argv[2][2])
      ? 0 : argv[2];
  }

  /* Divert all messages to stderr if neccessary */
  if (!oneop && !fout) {
    msgout = stderr;
    message(D,"All messages diverted to stderr\n");
  }

  /***********************************************************************
   * Input
   **********************************************************************/
  ilen = -1;
  if (!finp) {
    finp = "<stdin>";
    inp = freopen(0,"rb",stdin);
  } else {
    inp = fopen(finp,"rb");
    if (inp) {
      if (fseek(inp, 0, SEEK_END) != -1) {
        long err = ftell(inp);
        if (fseek(inp, 0, SEEK_SET) != -1) {
          ilen = err;
        }
      }
      if (ilen == -1) {
        syserror(finp);
        goto error;
      }
    }
  }
  message(V,"input: %s (%d)\n", finp, ilen);
  if (!inp) {
    syserror(finp);
    goto error;
  }
  err = (int) fread(header, 1, sizeof(header), inp);
  if (err == -1) {
    syserror(finp);
    goto error;
  }
  hread = err;

  if (hread < sizeof(header)) {
    if (sens == 'd') {
      error("input is too small, not ice packed.\n");
      goto error;
    }
    mode = sens = 'p';
    message(D,"Assume mode `%c'\n", mode);
  }

  if (sens != 'p') {
    csize = 0;
    dsize = unice68_depacked_size(header, &csize);
    if (dsize == -1) {
      message(D,"Not ice\n");
      if (sens == 'd') {
        error("input is not ice packed.\n");
        goto error;
      }
      assert(!sens);
      sens = mode = 'p';
      message(D,"Assume mode `%c'\n", mode);
    } else if (!sens) {
      sens = mode = 'd';
      message(D,"Assume mode `%c'\n", mode);
    }

    if (sens == 'd') {
      if (ilen != -1 && ilen != csize+sizeof(header)) {
        error("file size (%d) and packed  size (%d) does not match.\n",
              ilen, csize+sizeof(header));
        goto error;
      }
      /* sanity check */
      if (csize <= 0 || dsize <= 0 ||
          csize > 0x1000000 || dsize > 0x1000000) {
        error("cowardly refuse to unpack this file (%d/%d).\n", csize, dsize);
        goto error;
      }
      ilen = csize+sizeof(header);

      if (mode == 't') {
        err = dsize == -1;
        goto error;
      }
    }
  }

  /* Create and fill input buffer */
  if (ilen != -1) {
    ibuffer = malloc(ilen);
    if (ibuffer) {
      int n;
      memcpy(ibuffer, header, hread);
      n = fread(ibuffer+hread, 1, ilen - hread, inp);
      if (n < 0)
        ilen = -1;
      else if (ilen < n+hread) {
        error("only got %d bytes out of %d expected\n", n, ilen);
        goto error;
      }
    }
  } else {
    /* Don't know about the input size ! now doing an awful
     * read/realloc thing.
     */
    int max = 4096, m, n;

    ilen = 0;
    ibuffer = malloc(max);
    if (ibuffer) {
      memcpy(ibuffer, header, hread);
      ilen = hread;
    }
    do {
      m = max - ilen;
      if (!m) {
        max <<= 1;
        ibuffer = realloc(ibuffer, max);
        if (!ibuffer) break;
        m = max - ilen;
      }
      n = fread(ibuffer+ilen, 1, m, inp);
      if (n == -1) {
        ilen = -1;
        break;
      }
      ilen += n;
    } while (n == m);
  }
  if (!ibuffer || ilen == -1) {
    syserror(finp);
    goto error;
  }
  message(D,"Have read all %d input bytes from `%s' ...\n", ilen, finp);

  olen = (sens == 'd') ? dsize : (ilen + (ilen>>1) + 1000);
  message(D, "Allocating %d bytes for the output buffer\n", olen);
  obuffer = malloc(olen);
  if (!obuffer) {
    syserror(fout);
    goto error;
  }

  /***********************************************************************
   * Process
   **********************************************************************/
  switch (sens) {
  case 'p':
    message(D, "Now packing %d bytes\n", ilen);
    err = unice68_packer(obuffer, olen, ibuffer, ilen);
    if (err == -1)
      break;
    if (err > olen) {
      error("CRITICAL ! ice packer buffer overflow (%d > %d)\n",
            err , olen);
      return 66;
    }
    csize = err-12;
    dsize = unice68_depacked_size(obuffer, &csize);
    if (dsize == -1 || dsize != ilen) {
      if (dsize != -1) dsize = ~dsize;
      error("size inconsistency (%d != %d)\n", dsize, ilen);
      goto error;
    }
    olen = csize;

    err = 0;
    if (mode != 's') {
      break;
    } else {
      int tlen;
      char * tbuffer;
      hash1 = hash_buffer(ibuffer,ilen);
      message(D,"input hash: %x\n", hash1);
      memset(ibuffer,0,ilen);
      tbuffer = ibuffer; tlen = ilen;
      ibuffer = obuffer; ilen = olen;
      obuffer = tbuffer; olen = tlen;
    }
  case 'd':
    message(D, "Now depacking %d bytes\n", ilen);
    err = unice68_depacker(obuffer, ibuffer);
    if (!err && mode =='s') {
      unsigned int hash2 = hash_buffer(obuffer, olen);
      message(D,"depack hash: %x\n", hash2);
      err = - (hash1 != hash2);
      verified = !err;
    }
    if (err)
      error("stress has failed\n");
    break;

  default:
    assert(!"!!!! internal error !!!! invalid sens");
  }


/***********************************************************************
 * Output
 **********************************************************************/
  if (!oneop && !err) {
    err = -1;
    if (!fout) {
      fout = "<stdout>";
      out = freopen(0,"wb", stdout);
    } else {
      out = fopen(fout,"wb");
    }
    message(V,"output: %s (%d)\n", fout, olen);
    if (!fout) {
      syserror(fout);
      goto error;
    }
    n = fwrite(obuffer,1,olen,out);
    message(D,"Have written %d bytes to %s\n", n, fout);
    if (n != olen) {
      syserror(fout);
      goto error;
    }
    err = 0;
  }

error:
  if (inp && inp != stdin) {
    fclose(inp);
  }
  if (out) {
    fflush(out);
    if (out != stdout) {
      fclose(out);
    }
  }
  free(ibuffer);
  free(obuffer);

  if (!err) {
    message(N,"ICE! compressed:%d uncompressed:%d ratio:%d%%%s\n",
            csize, dsize, (csize+50)*100/dsize,verified?" (verified)":"");
  }

  err &= 255;
  message(D,"exit with code %d\n", err);

  return err;
}
