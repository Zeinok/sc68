/*
 *                       file68 - vlc stream
 *            Copyright (C) 2001-2009 Ben(jamin) Gerard
 *           <benjihan -4t- users.sourceforge -d0t- net>
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

/* $Id: istream68_file.c 7 2009-01-19 13:49:37Z benjihan $ */

/* #ifdef HAVE_CONFIG_H */
/* # include "config.h" */
/* #endif */

#define _(str)  (str)
#define N_(str) (str)

#include <vlc_common.h>
#include <vlc_input.h>
#include <vlc_plugin.h>
#include <vlc_demux.h>
#include <vlc_codec.h>
#include <vlc_meta.h>

#include <sc68/istream68_def.h>
#include <sc68/alloc68.h>

#include <stdio.h>
#include <string.h>

/** istream vlc structure. */
typedef struct {
  istream68_t   istream; /**< istream function.   */
  stream_t    * f;       /**< vlc slaved stream.  */
} istream68_vlc_t;

static const char * isf_name(istream68_t * istream)
{
  istream68_vlc_t * isf = (istream68_vlc_t *)istream;
  return "vlc://slave"; /* $$$ probably vlc stream has a name somewhere  */
}

static int isf_open(istream68_t * istream)
{
  istream68_vlc_t * isf = (istream68_vlc_t *) istream;
  return 0;
}

static int isf_close(istream68_t * istream)
{
  istream68_vlc_t * isf = (istream68_vlc_t *) istream;
  return 0;
}

static int isf_read(istream68_t * istream, void * data, int n)
{
  istream68_vlc_t * isf = (istream68_vlc_t *)istream;
  return stream_Read(isf->f, data, n);
}

static int isf_write(istream68_t * istream, const void * data, int n)
{
  istream68_vlc_t * isf = (istream68_vlc_t *)istream;
  return -1;
}

static int isf_flush(istream68_t * istream)
{
  istream68_vlc_t * isf = (istream68_vlc_t *)istream;
  return -1;
}

static int isf_length(istream68_t * istream)
{
  istream68_vlc_t * isf = (istream68_vlc_t *)istream;
  return stream_Size(isf->f);
}

static int isf_tell(istream68_t * istream)
{
  istream68_vlc_t * isf = (istream68_vlc_t *)istream;

  return stream_Tell(isf->f);
}

static int isf_seek(istream68_t * istream, int offset)
{
  istream68_vlc_t * isf = (istream68_vlc_t *)istream;
  int cur = isf_tell(istream);

  return (cur == -1)
    ? -1
    : stream_Seek(isf->f, cur+offset)
    ;
}

static void isf_destroy(istream68_t * istream)
{
  free68(istream);
}

static const istream68_t istream68_vlc = {
  isf_name,
  isf_open,
  isf_close,
  isf_read,
  isf_write,
  isf_flush,
  isf_length,
  isf_tell,
  isf_seek,
  isf_seek,
  isf_destroy
};

istream68_t * istream68_vlc_create(stream_t * vlc)
{
  istream68_vlc_t * isf;

  if (!vlc) {
    return 0;
  }

  isf = alloc68(sizeof(istream68_vlc_t));
  if (!isf) {
    return 0;
  }

  /* Copy istream functions. */
  memcpy(&isf->istream, &istream68_vlc, sizeof(istream68_vlc));

  /* set vlc stream handle. */
  isf->f = vlc;

  return &isf->istream;
}
