/*
 * @file    sc68.c
 * @brief   audacious plugin
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2011 Benjamin Gerard
 *
 * Time-stamp: <2014-06-20 14:35:13 ben>
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

#include <audacious/plugin.h>
#include <libaudcore/audstrings.h>
#include <sc68/sc68.h>
#include <sc68/file68_tag.h>

#include <glib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GMutex * ctrl_mutex = NULL;
static GCond  * ctrl_cond  = NULL;
static gboolean pause_flag;
static gint64   seek_value = -1;
static gboolean sc68a_error;
static gboolean sc68a_playing;
static gboolean sc68a_eof;

#ifdef DEBUG
static void dmsg(const int bit, sc68_t * sc68, const char *fmt, va_list list) {
  fprintf(stderr, "-- %02d -- %p :: ", bit, (void*)sc68);
  vfprintf(stderr, fmt, list);
  if (fmt[strlen(fmt)-1] != '\n')
    fprintf(stderr, "\n");
}
#endif

static bool_t plg_init(void) {
  sc68_init_t init68;
  memset(&init68, 0, sizeof(init68));
#ifdef DEBUG
  init68.msg_handler = dmsg;
#endif
  sc68_init(&init68);
  ctrl_mutex = g_mutex_new();
  ctrl_cond  = g_cond_new();
  return TRUE;
}

static void plg_cleanup(void) {
  g_mutex_free(ctrl_mutex);
  g_cond_free(ctrl_cond);
  sc68_shutdown();
}

static void * read_file(VFSFile * file, int * psize) {
  void * buf = 0;
  gint64 fsize;
  int isize;

  fsize = vfs_fsize (file);
  if (fsize <= 0 || fsize > (1<<20))
    goto done;
  isize  = (int) fsize;
  *psize = isize;

  buf = malloc(isize);
  if (!buf)
    goto done;
  fsize = vfs_fread(buf, 1, isize, file);
  if (isize != (int)fsize) {
    free(buf);
    buf = 0;
  }
done:
  return buf;
}

/* Not quiet exact but shoud be ok for all sc68 files :) */
static int year_of_str(const char * val)
{
  int c, y = 0;
  while (c = *val++, (c >= '0' && c <= '9')) {
    y = y * 10 + c - '0';
  }
  if (y < 100)
    y += (y >= 70) ? 1900 : 2000;
  else if (y < 1970)
    y = 0;
  return y;
}

static Tuple * tuple_from_track(const gchar * filename,
                                sc68_t * sc68, sc68_disk_t disk, int track)
{
  Tuple * tu = filename
    ? tuple_new_from_filename(filename)
    : tuple_new();

  if (tu) {
    sc68_music_info_t  info;
    sc68_cinfo_t     * cinfo;
    char track_str[32];

    tuple_set_str(tu, FIELD_CODEC,    NULL, "sc68");
    tuple_set_str(tu, FIELD_QUALITY,  NULL, "sequenced");
    tuple_set_str(tu, FIELD_MIMETYPE, NULL, sc68_mimetype());

    if (track <= 0)
      track = 1;

    if ( !sc68_music_info(sc68, &info, track, disk)) {

      if (strcmp(info.album,info.title)) {
        tuple_set_str(tu, FIELD_ALBUM,       NULL, info.album);
        tuple_set_str(tu, FIELD_TITLE,       NULL, info.title);
      } else {
        tuple_set_str(tu, FIELD_TITLE,       NULL, info.title);
      }
      tuple_set_str(tu, FIELD_ARTIST,      NULL, info.dsk.tag[TAG68_ID_ARTIST].val);
      tuple_set_str(tu, FIELD_SONG_ARTIST, NULL, info.trk.tag[TAG68_ID_ARTIST].val);
      tuple_set_str(tu, FIELD_GENRE,       NULL, info.trk.tag[TAG68_ID_GENRE].val);
      tuple_set_int(tu, FIELD_LENGTH, NULL,  info.trk.time_ms);

      tuple_set_int(tu, FIELD_SUBSONG_NUM,  NULL, info.tracks);
      tuple_set_int(tu, FIELD_SUBSONG_ID,   NULL, track);
      tuple_set_int(tu, FIELD_TRACK_NUMBER, NULL, track);

      for (cinfo = &info.dsk;
           cinfo;
           cinfo = (cinfo == &info.dsk) ? &info.trk : 0) {
        int i;
        for (i=TAG68_ID_CUSTOM; i<cinfo->tags; ++i) {
          sc68_tag_t * tag = cinfo->tag+i;
          const char * val = tag->val;
          int           id = -1;
          if (!strcmp(tag->key, TAG68_COMMENT))
            id = FIELD_COMMENT;
          else if (!strcmp(tag->key, TAG68_COPYRIGHT))
            id = FIELD_COPYRIGHT;
          else if (!strcmp(tag->key, TAG68_COMPOSER))
            id = FIELD_COMPOSER;
          else if (!strcmp(tag->key, TAG68_FORMAT))
            id = FIELD_CODEC;
          else if (!strcmp(tag->key, TAG68_YEAR)) {
            int year = year_of_str(val);
            if (year)
              tuple_set_int(tu, FIELD_YEAR, NULL, year);
          }

          if (id != -1)
            tuple_set_str(tu, id, NULL, val);
          else
            tuple_set_str(tu, -1, tag->key, val);
        }
      }
    } else {
      /* fprintf(stderr, "!!! could not retrieve sc68 music info for track %d\n", track); */
    }
  }

  return tu;
}

/* Must try to play this file.  "playback" is a structure containing output-
 * related functions which the plugin may make use of.  It also contains a
 * "data" pointer which the plugin may use to refer private data associated
 * with the playback state.  This pointer can then be used from pause,
 * mseek, and stop. If the file could not be opened, "file" will be NULL.
 * "start_time" is the position in milliseconds at which to start from, or
 * -1 to start from the beginning of the file.  "stop_time" is the position
 * in milliseconds at which to end playback, or -1 to play to the end of the
 * file.  "paused" specifies whether playback should immediately be paused.
 * Must return nonzero if some of the file was successfully played or zero
 * on failure. */
static
gboolean plg_play(InputPlayback * playback,
                  const gchar * filename, VFSFile * file,
                  gint start_time, gint stop_time, gboolean pause)
{
  char buffer[512 * 4];
  int code = SC68_ERROR;
  sc68_create_t create68;
  sc68_t * sc68 = 0;
  void   * mem = 0;
  int len, rate;
  int track;
  int myfile = 0;
  Tuple *tu = 0;
  gshort paused = 0;

  /* Error on exit. */
  g_mutex_lock(ctrl_mutex); {
    playback->set_data(playback, NULL);
    sc68a_error = TRUE;
    sc68a_playing = FALSE;
    sc68a_eof = FALSE;
  } g_mutex_unlock(ctrl_mutex);

  uri_parse (filename, NULL, NULL, NULL, & track);

  /* Currently load the file in memory. Would be better to support
   * audacious VFS with stream68.
   */
  len = 0;                              /* GCC warning ... */
  mem = read_file(file, &len);
  if (myfile) {
    vfs_fclose(file);
    file = NULL;
  }
  if (!mem)
    goto out;

  /* Create emulator instance. */
  memset(&create68,0,sizeof(create68));
  sc68 = sc68_create(&create68);
  if (!sc68)
    goto out;
  /* Load sc68 file. */
  if (sc68_load_mem(sc68, mem, len))
    goto out;

  /* Set first track unless specified otherwise */
  sc68_play(sc68, track > 0 ? track : 1, SC68_DEF_LOOP);
  rate = sc68_cntl(sc68, SC68_GET_SPR);
  /* Safety net */
  if (rate <= 8000)
    rate = sc68_cntl(sc68, SC68_SET_SPR, 8000);
  else if (rate > 48000)
    rate = sc68_cntl(sc68, SC68_SET_SPR, 48000);

  /* Open and setup audio playback. */
  if (! playback->output->open_audio(FMT_S16_NE, rate, 2))
    goto out;

  g_mutex_lock(ctrl_mutex); {
    /* Set rate and channels (other info are deprecated */
    playback->set_params(playback, -1, rate, 2);

    playback->set_data(playback, sc68);
    code = sc68_process(sc68, 0, 0);
    tu = tuple_from_track(filename, sc68, 0, track);
    if (tu) {
      playback->set_tuple(playback, tu);
      /* playback->set_gain_from_playlist(playback); */
      tu = NULL;
    }

    pause_flag = pause;
    seek_value = (start_time > 0) ? start_time : -1;

    sc68a_playing = TRUE;
    sc68a_eof = FALSE;
    sc68a_error = FALSE;
    playback->set_pb_ready(playback);
  } g_mutex_unlock(ctrl_mutex);

  /* Update return code (load the track) */
  while (!sc68a_error && sc68a_playing && !sc68a_eof) {
	int n;

    if (stop_time >= 0 && playback->output->written_time() >= stop_time) {
      sc68a_eof = TRUE;
      continue;
    }

    g_mutex_lock(ctrl_mutex); {
      /* Handle seek request */
      /* not supported
      if (seek_value >= 0) {
        sc68_seek(sc68, seek_value, 0);
        seek_value = -1;
        g_cond_signal(ctrl_cond);
      }
      */

      /* Handle pause/unpause request */
      if (pause_flag != paused) {
        playback->output->pause(pause_flag);
        paused = pause_flag;
        g_cond_signal(ctrl_cond);
      }

      if (paused) {
        g_cond_wait(ctrl_cond, ctrl_mutex);
        g_mutex_unlock(ctrl_mutex);
        continue;
      }
    } g_mutex_unlock(ctrl_mutex);

    /* Run sc68 engine. */

    n = sizeof(buffer) >> 2;
    code = sc68_process(sc68, buffer, &n);
    if (code == SC68_ERROR) {
      sc68a_error = TRUE;
      continue;
    }
    if (code & SC68_END) {
      sc68a_eof = TRUE;
      continue;
    }
    if (code & SC68_CHANGE) {
      sc68a_eof = TRUE;
      continue;
    }

    /* Is sc68 currently seeking ? */
    /*
    pos = sc68_seek(sc68, -1, &seeking);
    if (pos >= 0 && seeking) {
      while (playback->output->buffer_playing())
        g_usleep(5000);
      playback->output->flush(pos);
    }
    */
    playback->output->write_audio(buffer, n << 2);
  }
  sc68_stop(sc68);

  /* Flush buffer */
  g_mutex_lock(ctrl_mutex); {
    playback->set_data(playback, NULL);
    sc68a_playing = FALSE;
    g_cond_signal(ctrl_cond);
  } g_mutex_unlock(ctrl_mutex);

/* close: */
  playback->output->abort_write();

out:
  free(mem);
  sc68_destroy(sc68);
  return ! sc68_error;
}

static
/* Must signal a currently playing song to stop and cause play to return.
 * This function will be called from a different thread than play.  It will
 * only be called once. It should not join the thread from which play is
 * called.
 */
void plg_stop(InputPlayback * playback)
{
  g_mutex_lock(ctrl_mutex); {
    sc68a_playing = FALSE;
  } g_mutex_unlock(ctrl_mutex);
}

static
/* Must pause or unpause a file currently being played.  This function will
 * be called from a different thread than play, but it will not be called
 * before the plugin calls set_pb_ready or after stop is called. */
/* Bug: paused should be a gboolean, not a gshort. */
/* Bug: There is no way to indicate success or failure. */
void plg_pause(InputPlayback * playback, gboolean paused)
{
  g_mutex_lock(ctrl_mutex); {
    if (sc68a_playing) {
      pause_flag = paused;
      g_cond_signal(ctrl_cond);
      g_cond_wait(ctrl_cond, ctrl_mutex);
    }
  } g_mutex_unlock(ctrl_mutex);
}

/* Optional.  Must seek to the given position in milliseconds within a file
 * currently being played.  This function will be called from a different
 * thread than play, but it will not be called before the plugin calls
 * set_pb_ready or after stop is called. */
/* Bug: time should be a gint, not a gulong. */
/* Bug: There is no way to indicate success or failure. */
void plg_seek(InputPlayback * playback, gint time)
{
  g_mutex_lock(ctrl_mutex); {
    if (sc68a_playing) {
      seek_value = time;
      g_cond_signal(ctrl_cond);
      g_cond_wait(ctrl_cond, ctrl_mutex);
    }
  } g_mutex_unlock(ctrl_mutex);
}

static void fill_subtunes(Tuple *tuple) {
  int tunes = tuple_get_int (tuple, FIELD_SUBSONG_NUM, NULL), i;
  int subtunes[tunes];

  for (i = 0; i < tunes; ++i)
	subtunes[i] = i + 1;

  tuple_set_subtunes (tuple, tunes, subtunes);
}

/* Must return a tuple containing metadata for this file, or NULL if no
 * metadata could be read.  If the file could not be opened, "file" will be
 * NULL.  Audacious takes over one reference to the tuple returned. */
Tuple * plg_probe(const gchar * filename, VFSFile * file)
{
  Tuple *tu = NULL;
  sc68_disk_t disk = 0;
  int size/* , track = 0 */;
  void * buf = 0;
  int track;

  fprintf(stderr,
          "PROBE FN: %s\n",
          filename ? filename  : "(nil)");

  if (!file)
    goto done;
  if (buf = read_file(file, &size), !buf)
    goto done;
  if(disk = sc68_disk_load_mem(buf, size), !disk)
    goto done;

  uri_parse (filename, NULL, NULL, NULL, & track);
  tu = tuple_from_track(filename, 0, disk, track);
  fill_subtunes(tu);

  if (0 && tu) {
    int i;
    for (i=0; i<TUPLE_FIELDS; ++i) {
      switch (tuple_get_value_type (tu, i, 0)) {
        case TUPLE_STRING:
          fprintf(stderr,"\"%s\"\n", tuple_get_str(tu, i, 0));
          break;
        case TUPLE_INT:
          fprintf(stderr,"%d\n", tuple_get_int(tu, i, 0));
          break;
        default:
          fprintf(stderr,"\n");
          break;
      }
    }
  }

done:
  sc68_disk_free(disk);
  free(buf);
  return tu;
}

static
/* Must return nonzero if the plugin can handle this file.  If the file
 * could not be opened, "file" will be NULL.  (This is normal in the case of
 * special URI schemes like cdda:// that do not represent actual files.) */
/* Bug: The return value should be a gboolean, not a gint. */
gint plg_is_our(const gchar * filename, VFSFile * file)
{
  sc68_disk_t disk = 0;
  char * buf = 0;
  int size;
  if (!file)
    goto done;
  if (buf = read_file(file, &size), !buf)
    goto done;
  disk = sc68_disk_load_mem(buf, size);
done:
  free(buf);
  sc68_disk_free(disk);
  return disk != 0;
}

static const gchar * fmts[] = { "sc68", "sndh", "snd", NULL };
static gchar name[] = "sc68";
static gchar desc[] = "Atari and Amiga music player";

AUD_INPUT_PLUGIN (

  /* plugin header */
  .name = name,
  .about_text = desc,
  .init        = plg_init,
  .cleanup     = plg_cleanup,

  /* input plugin */
  .have_subtune         = 1,
  .extensions       = fmts,
  .priority             = 1,

  .is_our_file_from_vfs = plg_is_our,
  .probe_for_tuple      = plg_probe,

  .play                 = plg_play,
  .stop                 = plg_stop,
  .pause                = plg_pause
  //.mseek                = plg_seek,
)
