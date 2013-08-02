/*
 * @file    uri68.c
 * @brief   uri parser and dispatcher
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 2001-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-08-02 23:10:16 ben>
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
#include "file68_api.h"
#include "file68_uri.h"
#include "file68_rsc.h"
#include "file68_str.h"
#include "file68_msg.h"
#include "file68_vfs_null.h"
#include "file68_vfs_file.h"
#include "file68_vfs_fd.h"
#include "file68_vfs_curl.h"
#include "file68_vfs_ao.h"

#include <string.h>
#include <ctype.h>
#include <assert.h>


static scheme68_t * schemes;

/**
 * @retval -1  on error
 * @retval >0  length of scheme string
 */
static
int parse_scheme(const char * uri)
{
  int i = 0;

  /* First char must be alpha */
  if ( ! isalpha((int)uri[i]))
    return 0;

  /* Others must be alpha, digit, dot `.', plus `+' or hyphen `-' */
  for (i=1;
       isalnum((int)uri[i]) || uri[i] == '+' || uri[i] == '.' || uri[i] == '-';
       ++i);

  /* Must end by a colon `:' */
  return (uri[i] == ':') ? i+1 : 0;
}

int uri68_get_scheme(char * scheme, int max, const char *uri)
{
  int len = -1;

  if (uri) {
    len = parse_scheme(uri);
    if (scheme) {
      if (len == 0 )
        scheme[0] = 0;
      else if (len > 0) {
        if (len >= max)
          return -1;
        len = max-1;
        memcpy(scheme, uri, len);
        scheme[len] = 0;
      }
    }
  }
  return len;
}

int uri68_register(scheme68_t * scheme)
{
  assert(!scheme->next);
  scheme->next = schemes;
  schemes = scheme;
  msg68_debug("uri68: registered scheme -- %s\n", scheme->name);

  return 0;
}

/* int url68_local_protocol(const char * protocol) */
/* { */
/*   int i; */

/*   static const char * local_proto[] = { */
/*     "", "FILE","LOCAL","NULL" */
/*     /\* , "STDIN", "STDOUT" remove this (not seekable) *\/ */
/*   }; */
/*   const int n_proto = sizeof(local_proto)/sizeof(*local_proto); */

/*   i = 0; */
/*   if (protocol) { */
/*     for (; i<n_proto && strcmp68(protocol, local_proto[i]); ++i) */
/*       ; */
/*   } */
/*   return i < n_proto; */
/* } */

vfs68_t * uri68_create_vfs(const char * uri, int mode, int argc, ...)
{
  vfs68_t * vfs = 0;
  scheme68_t * scheme;

  for (scheme = schemes; scheme; scheme = scheme->next) {
    int res = scheme->ismine(uri);
    if (!res)
      continue;
    if ( (mode & res & 3) == ( mode & 3 ) )
      break;
  }

  if (scheme) {
    va_list list;
    va_start(list,argc);
    vfs = scheme->create(uri, mode, argc, list);
    va_end(list);
  }

  msg68_debug("url68: create url='%s' %c%c => [%s,'%s']\n",
              strnevernull68(uri),
              (mode&1) ? 'R' : '.',
              (mode&2) ? 'W' : '.',
              strok68(!vfs),
              vfs68_filename(vfs));

  return vfs;
}
