/*
 * @file    istream68_file.c
 * @brief   implements istream68 VFS for FILE
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 2001-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-08-02 21:45:30 ben>
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
#include "file68_vfs_file.h"

/* define this if you don't want FILE support. */
#ifndef ISTREAM68_NO_FILE

#include "file68_vfs_def.h"
#include "file68_uri.h"
#include "file68_str.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static int file_ismine(const char *);
static vfs68_t * file_create(const char *, int, int, va_list);
static scheme68_t file_scheme = {
  0, "vfs-file", file_ismine, file_create
};

static int file_ismine(const char * uri)
{
  const int ok = SCHEME68_ISMINE | SCHEME68_READ | SCHEME68_WRITE;

  if (!strncmp68(uri,"file://",7) || !strncmp68(uri,"local://",8))
    return ok;

#ifdef NATIVE_WIN32
  if (isalpha((int)uri[0]) && uri[1] == ':')
    return ok;
#endif

  if (uri68_get_scheme(0, 0, uri) == 0)
    return ok;

  return 0;
}


/** vfs file structure. */
typedef struct {
  vfs68_t vfs; /**< vfs function. */
  FILE *f;             /**< FILE handle.      */

  /** Open modes. */
  int mode;

  /* MUST BE at the end of the structure because supplemental bytes will
   * be allocated to store filename.
   */
  char name[1];       /**< filename. */

} vfs68_file_t;

static const char * isf_name(vfs68_t * vfs)
{
  vfs68_file_t * isf = (vfs68_file_t *)vfs;

  return !isf->name[0]
    ? 0
    : isf->name
    ;
}

static int isf_open(vfs68_t * vfs)
{
  int imode;
  char mode[8];
  vfs68_file_t * isf = (vfs68_file_t *)vfs;

  if (!isf->name[0] || isf->f) {
    return -1;
  }

  imode = 0;
  if (VFS68_IS_OPEN_READ(isf->mode)) {
    mode[imode++] = 'r';
  }
  if (VFS68_IS_OPEN_WRITE(isf->mode)) {
    mode[imode] = !imode ? 'w' : '+';
    ++imode;
  }
  if (!imode) {
    return -1;
  }
  mode[imode++] = 'b';
  mode[imode] = 0;

  isf->f = fopen(isf->name, mode);
#ifdef _O_BINARY
  if (isf->f) {
    _setmode(_fileno(isf->f), _O_BINARY);
  }
#endif
  return isf->f ? 0 : -1;
}

static int isf_close(vfs68_t * vfs)
{
  vfs68_file_t * isf = (vfs68_file_t *)vfs;
  FILE * f = isf->f;

  isf->f = 0;
  return !f
    ? -1
    : fclose(f)
    ;
}

static int isf_read(vfs68_t * vfs, void * data, int n)
{
  vfs68_file_t * isf = (vfs68_file_t *)vfs;

  return !isf->f
    ? -1
    : fread(data, 1, n, isf->f)
    ;
}

static int isf_write(vfs68_t * vfs, const void * data, int n)
{
  vfs68_file_t * isf = (vfs68_file_t *)vfs;

  return !isf->f
    ? -1
    : fwrite(data, 1, n, isf->f)
    ;
}

static int isf_flush(vfs68_t * vfs)
{
  vfs68_file_t * isf = (vfs68_file_t *)vfs;

  return !isf->f
    ? -1
    : fflush(isf->f)
    ;
}


/* We could have store the length value at opening, but this way it handles
 * dynamic changes of file size.
 */
static int isf_length(vfs68_t * vfs)
{
  vfs68_file_t * isf = (vfs68_file_t *)vfs;
  int pos,len;

  if (!isf->f) {
    return -1;
  }
  /* save current position. */
  len = ftell(isf->f);
  if (len != -1) {
    pos = len;
    /* seek t end of file */
    len = fseek(isf->f, 0, SEEK_END);
    if (len != -1) {
      /* get end of file position (AKA file length) */
      len = ftell(isf->f);
      /* restore saved position. ( $$$ no error check here ) */
      fseek(isf->f, pos, SEEK_SET);
    }
  }
  return len;
}

static int isf_tell(vfs68_t * vfs)
{
  vfs68_file_t * isf = (vfs68_file_t *)vfs;

  return !isf->f
    ? -1
    : ftell(isf->f)
    ;
}

static int isf_seek(vfs68_t * vfs, int offset)
{
  vfs68_file_t * isf = (vfs68_file_t *)vfs;

  return !isf->f
    ? -1
    : fseek(isf->f, offset, SEEK_CUR)
    ;
}

static void isf_destroy(vfs68_t * vfs)
{
  free(vfs);
}

static const vfs68_t vfs68_file = {
  isf_name,
  isf_open, isf_close,
  isf_read, isf_write, isf_flush,
  isf_length, isf_tell, isf_seek, isf_seek,
  isf_destroy
};

vfs68_t * vfs68_file_create(const char * fname, int mode)
{
  vfs68_file_t *isf;
  int len;

  if (!fname || !fname[0]) {
    return 0;
  }

  /* Don't to add 1 for the trailing zero, it has already been coundted
   * in the definition of vfs68_file_t::fname.
   */
  len = strlen(fname);
  isf = malloc(sizeof(vfs68_file_t) + len);
  if (!isf) {
    return 0;
  }

  /* Copy vfs functions. */
  memcpy(&isf->vfs, &vfs68_file, sizeof(vfs68_file));
  /* Clean file handle. */
  isf->f    = 0;
  isf->mode = mode & (VFS68_OPEN_READ|VFS68_OPEN_WRITE);

  /* Copy filename. */
  /* $$$ May be later, we should add a check for relative path and add
   * CWD ... */
  strcpy(isf->name, fname);
  return &isf->vfs;
}

static vfs68_t * file_create(const char * uri, int mode,
                             int argc , va_list list)
{
  if (!strncmp68(uri,"file://",7))
    uri += 7;
  else if (!strncmp68(uri,"local://",8))
    uri += 8;
  return vfs68_file_create(uri, mode);
}


int vfs68_file_init(void)
{
  return uri68_register(&file_scheme);
}

#else /* #ifndef VFS68_NO_FILE */

/* vfs file must not be include in this package. Anyway the creation
 * still exist but it always returns error.
 */

#include "file68_msg.h"

vfs68_t * vfs68_file_create(const char * fname, int mode)
{
  msg68_error("file68: create -- *NOT SUPPORTED*");
  return 0;
}

int vfs68_file_init(void)
{
  return 0;
}

#endif
