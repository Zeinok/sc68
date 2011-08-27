/*
 *                     sc68 - audacious plugin
 *            Copyright (C) 2010-2011 Ben(jamin) Gerard
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
 * $Id $
 *
 */

#include <audacious/plugin.h>
#include <sc68/sc68.h>

static GMutex * ctrl_mutex = NULL;
static GCond  * ctrl_cond  = NULL;
static gboolean pause_flag;
static gint64   seek_value = -1;

#ifdef DEBUG
static void dmsg(const int bit, sc68_t * sc68, const char *fmt, va_list list) {
  fprintf(stderr, "-- %02d -- %p :: ", bit, sc68);
  vfprintf(stderr, fmt, list);
  if (fmt[strlen(fmt)-1] != '\n')
    fprintf(stderr, "\n");
}
#endif

static void plg_init(void) {
  sc68_init_t init68;
  memset(&init68, 0, sizeof(init68));
#ifdef DEBUG
  init68.msg_handler = dmsg;
#endif
  sc68_init(&init68);
  ctrl_mutex = g_mutex_new();
  ctrl_cond  = g_cond_new();
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

static int get_track_number(int * track, char * s) {
  int c, v = 0;
  while (c = *++s, c >= '0' && c <= '9')
    v = v * 10 + ( c - '0' );
  *track = v;
  return !c;
}

static Tuple * tuple_from_track(const gchar * filename,
                                sc68_t * sc68, sc68_disk_t disk, int track)
{
  Tuple * tu = filename
    ? tuple_new_from_filename(filename)
    : tuple_new();

  /* if (track == 0) */
  /*   track = -1; */

  fprintf(stderr, "tuple: '%s' sc68:%p disk:%p track:%d\n",
          filename,sc68,disk,track);

  if (tu) {
    sc68_music_info_t di, ti;
    tuple_associate_string(tu, FIELD_CODEC,    NULL, "sc68");
    tuple_associate_string(tu, FIELD_QUALITY,  NULL, "sequenced");
    tuple_associate_string(tu, FIELD_GENRE,    NULL, "soundchip");
    tuple_associate_string(tu, FIELD_MIMETYPE, NULL, "audio/x-sc68");
    if ( !sc68_music_info(sc68, &di,     0, disk))
      tuple_associate_string(tu, FIELD_ALBUM,    NULL, di.title);
    if (!sc68_music_info(sc68, &ti, track, disk)) {
      tuple_associate_string(tu, FIELD_ARTIST,   NULL, ti.author);
      tuple_associate_string(tu, FIELD_COMPOSER, NULL, ti.composer);
      tuple_associate_string(tu, FIELD_TITLE,    NULL, ti.title);
      if (track > 0) {
        tuple_associate_int   (tu, FIELD_TRACK_NUMBER, NULL, track);
        tuple_associate_int   (tu, FIELD_SUBSONG_ID,   NULL, ti.track);
      }
      tuple_associate_int   (tu, FIELD_LENGTH,       NULL, ti.time_ms);
      tuple_associate_string(tu, -1, "ripper",    ti.ripper);
      tuple_associate_string(tu, -1, "converter", ti.converter);
      tuple_associate_string(tu, -1, "replay",    ti.replay);
      tuple_associate_string(tu, -1, "hardware",  ti.hwname);
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
  int code = SC68_MIX_ERROR;
  sc68_create_t create68;
  sc68_t * sc68 = 0;
  void   * mem = 0;
  int len, rate;
  int track;
  int myfile = 0;
  gchar * newfile = 0;
  Tuple *tu = 0;
  gshort paused = 0;

  /* Error on exit. */
  g_mutex_lock(ctrl_mutex); {
    playback->data    = NULL;
    playback->error   = TRUE;
    playback->playing = FALSE;
    playback->eof     = FALSE;
  } g_mutex_unlock(ctrl_mutex);

  /* Copy URI to be patched. */
  newfile = strdup(filename);
  if (!newfile)
    goto out;

  /* Deal with multiple track files */
  if (!file) {
    gchar * q = strrchr(newfile, '?');
    if (q && get_track_number(&track, q)) {
      *q = 0;
      file = vfs_fopen (newfile, "rb");
      myfile = !!file;
    } else
      track = 1;
  }
  if (!file)
    goto out;

  /* Currently load the file in memory. Would be better to support
   * audacious VFS with stream68.
   */
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
  sc68_play(sc68, track > 0 ? track : 1, -1);
  rate = sc68_sampling_rate(sc68, SC68_SPR_QUERY);
  /* Safety net */
  if (rate <= 8000)
    rate = sc68_sampling_rate(sc68, 8000);
  else if (rate > 48000)
    rate = sc68_sampling_rate(sc68, 48000);

  /* Open and setup audio playback. */
  if (! playback->output->open_audio(FMT_S16_NE, rate, 2))
    goto out;

  g_mutex_lock(ctrl_mutex); {
    /* Set rate and channels (other info are deprecated */
    playback->set_params(playback, NULL, 0, 0, rate, 2);

    playback->data = sc68;
    code = sc68_process(sc68, 0, 0);
    tu = tuple_from_track(newfile, sc68, 0, -1);
    if (tu) {
      playback->set_tuple(playback, tu);
      /* playback->set_gain_from_playlist(playback); */
      tu = NULL;
    }

    pause_flag = pause;
    seek_value = (start_time > 0) ? start_time : -1;
    playback->playing = TRUE;
    playback->eof     = FALSE;
    playback->error   = FALSE;
    playback->set_pb_ready(playback);
  } g_mutex_unlock(ctrl_mutex);

  /* Update return code (load the track) */
  while (!playback->error && playback->playing && !playback->eof) {
    int pos, seeking;

    if (stop_time >= 0 && playback->output->written_time() >= stop_time) {
      playback->eof = TRUE;
      continue;
    }

    g_mutex_lock(ctrl_mutex); {
      /* Handle seek request */
      if (seek_value >= 0) {
        sc68_seek(sc68, seek_value, 0);
        seek_value = -1;
        g_cond_signal(ctrl_cond);
      }

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
    code = sc68_process(sc68, buffer, sizeof(buffer) >> 2);
    if (code == SC68_MIX_ERROR) {
      playback->error = TRUE;
      continue;
    }
    if (code & SC68_END) {
      playback->eof = TRUE;
      continue;
    }
    if (code & SC68_CHANGE) {
      tu = tuple_from_track(newfile, sc68, 0, -1);
      if (tu) {
        playback->set_tuple(playback, tu);
        playback->set_params(playback, NULL, 0, 0, 44100, 2);
        tu = NULL;
      }
    }

    /* Is sc68 currently seeking ? */
    pos = sc68_seek(sc68, -1, &seeking);
    if (pos >= 0 && seeking) {
      while (playback->output->buffer_playing())
        g_usleep(5000);
      playback->output->flush(pos);
    }
    playback->output->write_audio(buffer, sizeof(buffer));
  }
  sc68_stop(sc68);

  /* Flush buffer */
  g_mutex_lock(ctrl_mutex); {
    playback->data = NULL;
    while (!playback->error && playback->playing && playback->output->buffer_playing())
      g_usleep(5000);
    playback->playing = FALSE;
    g_cond_signal(ctrl_cond);
  } g_mutex_unlock(ctrl_mutex);

close:
  playback->output->close_audio();

out:
  free(mem);
  free(newfile);
  sc68_destroy(sc68);
  return ! playback->error;
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
    playback->playing = FALSE;
  } g_mutex_unlock(ctrl_mutex);
}

static
/* Must pause or unpause a file currently being played.  This function will
 * be called from a different thread than play, but it will not be called
 * before the plugin calls set_pb_ready or after stop is called. */
/* Bug: paused should be a gboolean, not a gshort. */
/* Bug: There is no way to indicate success or failure. */
void plg_pause(InputPlayback * playback, gshort paused)
{
  g_mutex_lock(ctrl_mutex); {
    if (playback->playing) {
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
void plg_seek(InputPlayback * playback, gulong time)
{
  g_mutex_lock(ctrl_mutex); {
    if (playback->playing) {
      seek_value = time;
      g_cond_signal(ctrl_cond);
      g_cond_wait(ctrl_cond, ctrl_mutex);
    }
  } g_mutex_unlock(ctrl_mutex);
}

/* Must return a tuple containing metadata for this file, or NULL if no
 * metadata could be read.  If the file could not be opened, "file" will be
 * NULL.  Audacious takes over one reference to the tuple returned. */
Tuple * plg_probe(const gchar * filename, VFSFile * file)
{
  Tuple *tu = NULL;
  sc68_music_info_t di, ti;
  sc68_disk_t disk = 0;
  int size, track = 0;
  void * buf = 0;
  char * q;

  if (!file)
    goto done;
  if (buf = read_file(file, &size), !buf)
    goto done;
  if(disk = sc68_disk_load_mem(buf, size), !disk)
    goto done;
#if 1
  sc68_music_info(0, &di, 0, disk);
  tu = tuple_from_track(filename, 0, disk, 0);
  if (di.tracks > 1) {
    tu->nsubtunes = di.tracks;
    tuple_associate_int(tu, FIELD_SUBSONG_NUM,  NULL, di.tracks);
  }
#else
  tu = tuple_from_track(filename, 0, disk, 0);
#endif

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

static InputPlugin sc68_plugin = {

  /* plugin header */
  .description = desc,
  .init        = plg_init,
  .cleanup     = plg_cleanup,

  /* input plugin */
  .have_subtune         = 1,
  .vfs_extensions       = fmts,
  .priority             = 1,

  .is_our_file_from_vfs = plg_is_our,
  .probe_for_tuple      = plg_probe,

  .play                 = plg_play,
  .stop                 = plg_stop,
  .pause                = plg_pause,
  .mseek                = plg_seek,
};

InputPlugin * ip_list [] = { &sc68_plugin, NULL };

SIMPLE_INPUT_PLUGIN(name, ip_list)
