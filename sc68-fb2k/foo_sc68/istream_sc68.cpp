/*
 * @file    istream_sc68.c
 * @brief   implements istream68 VFS for foobar2000
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 2013 Benjamin Gerard
 *
 * Time-stamp: <2013-05-31 22:43:37 ben>
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

#include "stdafx.h"
#include <sc68/istream68.h>
#include <sc68/istream68_def.h>

using namespace foobar2000_io;

/** istream file structure. */
struct istream68_fb2k {
	istream68_fb2k(service_ptr_t<file> p_file, const char * p_path, t_input_open_reason p_reason, abort_callback & p_abort);
	~istream68_fb2k();

	istream68_t         istream;
	service_ptr_t<file> file;
	abort_callback     &abort;
	t_input_open_reason reason;
	char               *path;
};

typedef struct istream68_fb2k istream68_fb2k_t;

static const char * isn_name(istream68_t * istream)
{
  istream68_fb2k_t * isn = (istream68_fb2k_t *)istream;
  return isn->path;
}

static int isn_open(istream68_t * istream)
{
	istream68_fb2k_t * isn = (istream68_fb2k_t *)istream;
	input_open_file_helper(isn->file, isn->path, isn->reason, isn->abort);
	return 0;
}

static int isn_close(istream68_t * istream)
{
	istream68_fb2k_t * isn = (istream68_fb2k_t *)istream;
	isn->file.release();
	return 0;
}

static int isn_read(istream68_t * istream, void * data, int n)
{
	istream68_fb2k_t * isn = (istream68_fb2k_t *)istream;
	return isn->file->read(data, (int)n, isn->abort);
}

static int isn_write(istream68_t * istream, const void * data, int n)
{
	istream68_fb2k_t * isn = (istream68_fb2k_t *)istream;
	isn->file->write(data, (int)n, isn->abort);
	return n;
}

static int isn_flush(istream68_t * istream)
{
	return 0;
}

static int isn_length(istream68_t * istream)
{
	istream68_fb2k_t * isn = (istream68_fb2k_t *)istream;
	t_filesize size = isn->file->get_size(isn->abort);
	return (size != filesize_invalid)
		? (int) size
		: -1
		;
}

static int isn_tell(istream68_t * istream)
{
	istream68_fb2k_t * isn = (istream68_fb2k_t *)istream;
	t_filesize pos = isn->file->get_position(isn->abort);
	return (pos != filesize_invalid)
		? (int) pos
		: -1
		;
}

static int isn_seek(istream68_t * istream, int offset)
{
	istream68_fb2k_t * isn = (istream68_fb2k_t *)istream;
	int pos = isn_tell(istream);

	if (pos == -1)
		return -1;
	isn->file->seek(pos + offset, isn->abort);
	return 0;
}

static void isn_destroy(istream68_t * istream)
{
	isn_close(istream);
	delete istream;
}

istream68_fb2k::istream68_fb2k(service_ptr_t<foobar2000_io::file> p_file, const char * p_path,
	t_input_open_reason p_reason, abort_callback & p_abort)
	: abort(p_abort), reason(p_reason), file(p_file)
{
	istream.name    = isn_name;
	istream.open    = isn_open;
	istream.close   = isn_close;
	istream.read    = isn_read;
	istream.write   = isn_write;
	istream.flush   = isn_flush;
	istream.length  = isn_length;
	istream.tell    = isn_tell;
	istream.destroy = isn_destroy;
	istream.seekb   = isn_seek;
	istream.seekf   = isn_seek;

	path = p_path ? _strdup(p_path) : 0;
}

istream68_fb2k::~istream68_fb2k()
{
	free(path);
}

istream68_t * istream68_fb2k_create(service_ptr_t<file> p_file, const char * p_path,
	t_input_open_reason p_reason, abort_callback & p_abort)
{
  istream68_fb2k_t *isn = new istream68_fb2k_t(p_file, p_path, p_reason, p_abort);
  if (!isn)
	  return 0;
  return &isn->istream;
}
