/* Time-stamp: <2013-08-05 22:20:47 ben> */

/* Minimal version */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif


#include <vlc_common.h>
#include <vlc_input.h>
#include <vlc_plugin.h>
#include <vlc_demux.h>
#include <vlc_codec.h>
#include <vlc_meta.h>

/* #ifdef HAVE_UNISTD_H */
/* # include <unistd.h> */
/* #endif */

#include <sc68/file68_vfs_def.h>
#include <sc68/file68_vfs.h>
#include <sc68/sc68.h>
#include <sc68/file68.h>
#include <sc68/file68_msg.h>
#include <sc68/file68_tag.h>

vfs68_t * vfs68_vlc_create(stream_t * vlc);

#ifndef _
# define _(str)  (str)
#endif

#ifndef N_
#define N_(str) (str)
#endif


//#define vlc_meta_TrackTotal (vlc_meta_TrackID+1)

static void msg(const int bit, void *userdata, const char *fmt, va_list list);
static void meta_lut_sort(void);

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open  ( vlc_object_t * );
static void Close ( vlc_object_t * );

static void sc68_debug(sc68_t * sc68, const char * fmt, ...) { };

/* Init sc68 library (call once on load) */
static int vlc_init_sc68(void)
{
  sc68_init_t init68;
  char appname[] = "vlc";
  char * argv[] = { appname };
  meta_lut_sort();
  memset(&init68,0,sizeof(init68));
  init68.argc = sizeof(argv)/sizeof(*argv);
  init68.argv = argv;
#ifdef DEBUG
  init68.debug_set_mask = -1;
  init68.debug_clr_mask =  0;
  init68.msg_handler = msg;
#endif
  return sc68_init(&init68);
}

/* Shutdown sc68 library (call once on unload) */
static void vlc_shutdown_sc68(void)
{
  sc68_shutdown();
}

/* Declare VLC module */
vlc_module_begin()
{
  if (vlc_init_sc68()) {
    goto error;
  }
  set_shortname("sc68");
  set_description( N_("sc68 demuxer") );
  set_category( CAT_INPUT );
  set_subcategory( SUBCAT_INPUT_DEMUX );
  set_capability( "demux", 100);
  set_callbacks( Open, Close );
  add_shortcut( "sc68" );
#ifdef HAVE_FILEEXT
  fileext_Add( FILEEXT_POOL, "*.sc68;*.sndh;*.snd", FILEEXT_AUDIO );
#endif
}
vlc_module_end()

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int Demux  ( demux_t * );
static int Control( demux_t *, int, va_list );

struct demux_sys_t {
#ifdef DEBUG
  char              name[16];     /* sc68 instance name                      */
#endif
  sc68_t          * sc68;         /* sc68 emulator instance                  */
  int               pcm_per_loop; /* PCM per sound rendering loop            */
  int               sample_rate;  /* Sample rate in hz                       */
  int               allin1;       /* 0:track by track                        */
  es_out_id_t     * es;           /* Elementary Stream                       */
  date_t            pts;
  sc68_minfo_t      info;         /* Current track info                      */
  input_title_t   * titles[SC68_MAX_TRACK];
  sc68_cinfo_t      infos[SC68_MAX_TRACK];
};

static void flush_sc68_errors(demux_t * const p_demux)
{
  /* sc68_error_flush(0); */
  /* if (p_demux && p_demux->p_sys && p_demux->p_sys->sc68) */
  /*   sc68_error_flush(p_demux->p_sys->sc68); */
}

/* Shutdown sc68 demux module private */
static void sys_shutdown(demux_t * p_demux)
{
  if (!p_demux)
    return;
  else {
    demux_sys_t * p_sys = p_demux->p_sys;
    int i;

    p_demux->p_sys = 0;
    if (!p_sys)
      return;

    for (i=0; i<SC68_MAX_TRACK; ++i)
      if (p_sys->titles[i])
        vlc_input_title_Delete(p_sys->titles[i]);

    sc68_destroy(p_sys->sc68);
    free(p_sys);
  }
}


typedef struct meta_lut_s {
  vlc_meta_type_t vlc;
  char * sc68;
} meta_lut_t;

static int cmp_meta_lut(const void * pa, const void * pb)
{
  const meta_lut_t * a = pa;
  const meta_lut_t * b = pb;
  return strcmp(a->sc68, b->sc68);
}

static meta_lut_t meta_lut[] = {
  { vlc_meta_ArtworkURL,  TAG68_IMAGE },
  { vlc_meta_Copyright,   TAG68_COPYRIGHT},
  { vlc_meta_Date,        "date" },
  { vlc_meta_Description, TAG68_COMMENT},
  { vlc_meta_EncodedBy,   TAG68_CONVERTER},
  { vlc_meta_Publisher,   "publisher" },
  { vlc_meta_Rating,      "rating"},
  /* { vlc_meta_Setting,     TAG68_HARDWARE}, */
  { vlc_meta_URL,         TAG68_URI},
};


/* const char * meta_lut2[] = { */
/*   [vlc_meta_ArtworkURL] = TAG68_IMAGE, */
/* }; */


static void meta_lut_sort(void)
{
  qsort(meta_lut, sizeof(meta_lut)/sizeof(*meta_lut),
        sizeof(*meta_lut), cmp_meta_lut);
}

static
vlc_meta_t * TrackMeta(demux_t * const p_demux, vlc_meta_t * meta, int track)
{
  demux_sys_t * const p_sys = p_demux->p_sys;
  sc68_minfo_t *info, tmpinfo;

  if (track == -1 && p_sys->info.trk.track > 0) {
    info = &p_sys->info;
  } else {
    info = &tmpinfo;
    if (sc68_music_info(p_sys->sc68, info, track, 0))
      return 0;

  }
  track = info->trk.track;

  if (track > 0) {
    int i;
    sc68_tag_t tag;
    char trackstr[4] = "00";

    if (!meta)
      meta = vlc_meta_New();
    if (unlikely(!meta))
      return 0;

    vlc_meta_Set( meta, vlc_meta_Album, info->album);
    vlc_meta_Set( meta, vlc_meta_Title, info->title);
    vlc_meta_Set( meta, vlc_meta_Artist,info->artist);
    vlc_meta_Set( meta, vlc_meta_Genre, info->trk.tag[TAG68_ID_GENRE].val);

    trackstr[0] += track / 10;
    trackstr[1] += track % 10;
    vlc_meta_Set( meta, vlc_meta_TrackNumber, trackstr);

    trackstr[0] = '0' + info->tracks / 10;
    trackstr[1] = '0' + info->tracks % 10;
//    vlc_meta_Set( meta, vlc_meta_TrackTotal, trackstr);

    /* vlc_meta_Set( meta, vlc_meta_Setting, info->trk.hw); */
  /* { vlc_meta_Setting,     TAG68_HARDWARE}, */

    /* TrackID ?
       vlc_meta_Set( meta, vlc_meta_TrackID, trackstr); */
    /* vlc_meta_Set( meta, vlc_meta_Description, */
    /*               info->dsk.tag[TAG68_ID_GENRE].val); */

    for (i=TAG68_ID_CUSTOM;
         !sc68_tag_enum(p_sys->sc68, &tag, track, i, 0);
         ++i) {
      meta_lut_t * lut, ent;
      ent.sc68 = tag.key;
      lut = bsearch(&ent, meta_lut, sizeof(meta_lut)/sizeof(*meta_lut),
                    sizeof(*meta_lut), cmp_meta_lut);
      if (lut)
        vlc_meta_Set( meta, lut->vlc, tag.val);
    }
  }
  return meta;
}

/**
 * @retval -1 error
 * @retval  1 track info updated
 * @retval  0 same track
 */
static
int TrackInfo(demux_t * const p_demux)
{
  demux_sys_t * const p_sys = p_demux->p_sys;

  int track = sc68_play(p_sys->sc68, -1, 0);
  int tracks;
  vlc_meta_t * meta;

  if (track > 0 && track == p_sys->info.trk.track) {
    /* Same valid track */
    return 0;
  }

  if (sc68_music_info(p_sys->sc68, &p_sys->info, -1, 0))
    return -1;

  tracks = p_sys->info.tracks;

  sc68_debug(p_sys->sc68, "TrackInfo: track #%d/%d",
             p_sys->info.trk.track, p_sys->info.tracks);


  /* meta = TrackMeta(p_demux, p_sys->meta, -1); */
  /* if (p_sys->meta && meta != p_sys->meta) { */
  /*   vlc_meta_Delete(p_sys->meta); */
  /* } */
  /* p_sys->meta = meta; */


  return meta ? 1 : -1;
}


/*****************************************************************************
 * open: initializes ES structures
 *****************************************************************************/
static int Open(vlc_object_t * p_this)
{
  static int id;

  demux_t     * p_demux = (demux_t*)p_this;

  const uint8_t *p_peek;

  int         i_peek = 0;
  int err = VLC_EGENERIC;
  sc68_create_t  create68;     /* Parameters for creating sc68 instance */
  vfs68_t * stream68 = 0;
  sc68_music_info_t info;
  int i, tracks;
  es_format_t fmt;

  sc68_debug(0,"Open() {");

  /* assert(!p_demux->sc68); */

  /* Check if we are dealing with a sc68 file */
  if (stream_Peek(p_demux->s, &p_peek, 16) < 16) {
    goto exit;
  }

  /* Fast check for our files ... */
  /* TODO: Gzip                   */
  if (!memcmp(p_peek, "SC68", 4)) {
    /* msg_Dbg(p_demux,"SC68 4cc detected"); */
  } else if (!memcmp(p_peek, "ICE!", 4)) {
    /* msg_Dbg(p_demux,"ICE! 4cc detected"); */
  } else if (!memcmp(p_peek+12, "SNDH", 4)) {
    /* msg_Dbg(p_demux,"SNDH 4cc detected"); */
  } else if (!memcmp(p_peek, "\037\213\010", 3)) {
    /* msg_Dbg(p_demux,"GZIP detected"); */
  } else {
    msg_Dbg(p_demux,"Not sc68 ? '%02x-%02x-%02x-%02x' ?",
            p_peek[0], p_peek[1], p_peek[2], p_peek[3]);
    goto exit;
  }

  p_demux->p_sys = (demux_sys_t*) calloc (1, sizeof(demux_sys_t));
  if (unlikely(!p_demux->p_sys))
    goto error;

  /* Create sc68 instance */
  memset(&create68,0,sizeof(create68));
#ifdef DEBUG
  memcpy(p_demux->p_sys->name,"vlc68#",6);
  p_demux->p_sys->name[6] = '0' +  id / 100 % 10;
  p_demux->p_sys->name[7] = '0' +  id / 10 % 10;
  p_demux->p_sys->name[8] = '0' +  id % 10;
  create68.name = p_demux->p_sys->name;
#endif
  p_demux->p_sys->sc68 = sc68_create(&create68);
  if (!p_demux->p_sys->sc68)
    goto error;

  /* Set sc68 cookie */
  sc68_cntl(p_demux->p_sys->sc68, SC68_SET_COOKIE, p_demux);
  /* *(sc68_cookie_ptr(p_demux->p_sys->sc68)) = p_demux; */

  /* Load and prepare sc68 file */
  stream68 = vfs68_vlc_create(p_demux->s);
  if (unlikely(!stream68))
    goto error;
  if (sc68_load(p_demux->p_sys->sc68, stream68))
    goto error;

  /* Play all track from first for now */
  if (sc68_play(p_demux->p_sys->sc68, 1, -1) == -1)
    goto error;
  if (sc68_process(p_demux->p_sys->sc68, 0, 0) == SC68_ERROR)
    goto error;
  p_demux->p_sys->sample_rate = sc68_cntl(p_demux->p_sys->sc68, SC68_GET_SPR);

  /* Set track info */
  if (!sc68_music_info(p_demux->p_sys->sc68, &info, 1, 0)) {
    int i, tracks;
    for (i=0, tracks=info.tracks; i<tracks; ++i) {
      sc68_music_info(p_demux->p_sys->sc68, &info, i+1, 0);
      /* Copy track info */
      p_demux->p_sys->infos[i]  = info.trk;
      /* Create vlc title */
      p_demux->p_sys->titles[i] = vlc_input_title_New();
      if (!p_demux->p_sys->titles[i])
        continue;
      p_demux->p_sys->titles[i]->i_length = info.trk.time_ms * 1000u;
      p_demux->p_sys->titles[i]->b_menu   = false;
      if (strcmp(info.title, info.album)) {
        p_demux->p_sys->titles[i]->psz_name
          = malloc( strlen(info.title) + strlen(info.album) + 4);
        if (!p_demux->p_sys->titles[i]->psz_name)
          continue;
        sprintf(p_demux->p_sys->titles[i]->psz_name,
                "%s - %s",
                info.album, info.title);
      } else {
        p_demux->p_sys->titles[i]->psz_name
          = malloc(strlen(info.album) + 16);
        if (!p_demux->p_sys->titles[i]->psz_name)
          continue;
        sprintf(p_demux->p_sys->titles[i]->psz_name,
                "%s - #%02d",
                info.album, i+1);
      }
    }
  }

  /* Setup Audio Elementary Stream. */
  p_demux->p_sys->pcm_per_loop = 512;
  es_format_Init (&fmt, AUDIO_ES, VLC_CODEC_S16N);
  fmt.audio.i_channels        = 2;
  fmt.audio.i_bitspersample   = 16;
  fmt.audio.i_rate            = p_demux->p_sys->sample_rate;
  fmt.audio.i_bytes_per_frame =
    fmt.audio.i_frame_length  =
    fmt.audio.i_blockalign    = 4;
  fmt.i_bitrate = fmt.audio.i_rate * fmt.audio.i_bytes_per_frame;
  p_demux->p_sys->es = es_out_Add(p_demux->out, &fmt);
  date_Init(&p_demux->p_sys->pts, fmt.audio.i_rate, 1);
  date_Set(&p_demux->p_sys->pts, 0);

  /* Callbacks */
  p_demux->pf_demux   = Demux;
  p_demux->pf_control = Control;


  err = VLC_SUCCESS;
error:
  /* Don't need our stream anymore */
  vfs68_destroy(stream68);

  /* flush sc68 engin error */
  flush_sc68_errors(p_demux);

  if (err != VLC_SUCCESS) {
    /* On error ... */
    /* $$$ TODO : check for allocated stuff like titles and infos */
    sys_shutdown(p_demux);
  }

exit:
  return err;
}

/*****************************************************************************
 * Close: frees unused data
 *****************************************************************************/
static void Close( vlc_object_t * p_this )
{
  demux_t        *p_demux = (demux_t*)p_this;
  sys_shutdown(p_demux);
}

/*****************************************************************************
 * Demux: reads and demuxes data packets
 *****************************************************************************
 * Returns -1 in case of error, 0 in case of EOF, 1 otherwise
 *****************************************************************************/
static int Demux( demux_t *p_demux )
{
  int code, ms, pcm_per_loop;
  demux_sys_t *p_sys   = p_demux->p_sys;
  block_t     *p_block = 0;

  p_block = block_Alloc(p_sys->pcm_per_loop << 2);
  if (unlikely(!p_block)) {
    return 0;
  }

  pcm_per_loop = p_block->i_buffer >> 2;
  code = sc68_process(p_sys->sc68,  p_block->p_buffer, &pcm_per_loop);
  if (code == SC68_ERROR) {
    block_Release(p_block);
    return 0;
  }

  if (pcm_per_loop <= 0) {
    block_Release(p_block);
  } else {
    p_block->i_nb_samples = pcm_per_loop;
    p_block->i_dts        = VLC_TS_0 + date_Get (&p_sys->pts);
    p_block->i_pts        = p_block->i_dts;

    /* set PCR */
    es_out_Control( p_demux->out, ES_OUT_SET_PCR, p_block->i_pts );
    es_out_Send( p_demux->out, p_sys->es, p_block );
    date_Increment( &p_sys->pts, pcm_per_loop);
  }

  if (code & SC68_LOOP) {
    /* Nothing to do really */
  }

  if (code & SC68_CHANGE) {
    TrackInfo(p_demux);
    p_demux->info.i_title  = p_sys->info.trk.track-1;
    p_demux->info.i_update = INPUT_UPDATE_TITLE | INPUT_UPDATE_META;
  }

  /* sc68_debug(p_sys->sc68,"} Demux: => [%d]", (code & SC68_END) ? 0 : 1); */

  return (code & SC68_END)
    ? 0                                 /* on EOF */
    : 1                                 /* on continue */
    ;
}

/*****************************************************************************
 * Control:
 *****************************************************************************/
static int Control( demux_t *p_demux, int i_query, va_list args )
{
  demux_sys_t *p_sys  = p_demux->p_sys;

  switch (i_query) {

  case DEMUX_GET_META: {
    vlc_meta_t *p_meta = (vlc_meta_t *)va_arg(args, vlc_meta_t*);
    if (TrackInfo(p_demux) >=0 &&
        p_meta == TrackMeta(p_demux, p_meta, -1))
      return VLC_SUCCESS;
  } break;

  case DEMUX_HAS_UNSUPPORTED_META: {
    bool * pb_bool;
    pb_bool = (bool*)va_arg(args, bool*);
    *pb_bool = true;
    return VLC_SUCCESS;
  } break;

  case DEMUX_GET_LENGTH: {
    int ms = sc68_cntl(p_sys->sc68, p_sys->allin1
                       ? SC68_GET_DSKLEN : SC68_GET_LEN);

    int64_t * pi64 = (int64_t *) va_arg(args, int64_t *);
    
    *pi64 = (int64_t) ms * 1000;
    return VLC_SUCCESS;
  } break;

  case DEMUX_GET_TIME: {
    int64_t * pi64 = (int64_t *) va_arg(args, int64_t *);
    int ms = sc68_cntl(p_sys->sc68, p_sys->allin1 ? SC68_GET_PLAYPOS : SC68_GET_POS);
    if (ms == -1) break;
    *pi64 = (int64_t)ms * 1000;
    return VLC_SUCCESS;
  } break;

  case DEMUX_GET_POSITION: {
    double lf, * plf = (double *) va_arg(args, double *);
    int ms = sc68_cntl(p_sys->sc68, p_sys->allin1 ? SC68_GET_PLAYPOS : SC68_GET_POS);
    if (ms == -1 || !p_sys->info.trk.time_ms) break;
    /* ms -= p_sys->info.start_ms; */
    *plf = lf = (double)ms / (double)p_sys->info.trk.time_ms;
    return VLC_SUCCESS;
  } break;

  /* case DEMUX_SET_POSITION: { */
  /*   double lf = (double)va_arg(args, double); */
  /*   int ms; */
  /*   ms = p_sys->info.start_ms + (int)(p_sys->info.trk.time_ms * lf); */
  /*   if (ms < p_sys->info.start_ms) */
  /*     ms = p_sys->info.start_ms; */
  /*   else if (ms > p_sys->info.start_ms+p_sys->info.trk.time_ms) */
  /*     ms = p_sys->info.start_ms+p_sys->info.trk.time_ms; */
  /*   if (sc68_seek(p_sys->sc68, ms, 0) == -1) */
  /*     break; */
  /*   return VLC_SUCCESS; */
  /* } break; */

  /* case DEMUX_SET_TIME: */
  /*   break; */

  case DEMUX_GET_TITLE_INFO: {
    input_title_t *** ppp_title;
    int * pi_int, i;

    TrackInfo(p_demux);
    sc68_debug(p_sys->sc68, "GET_TITLE_INFO: %d", p_sys->info.tracks);

    if (p_sys->info.tracks <= 1)
      break;
    ppp_title = (input_title_t***) va_arg (args, input_title_t***);
    pi_int    = (int*)va_arg( args, int* );

    *pi_int = p_sys->info.tracks;
    *ppp_title = (input_title_t**) malloc(sizeof(input_title_t**) * *pi_int);
    for (i=0; i<*pi_int; i++) {
      (*ppp_title)[i] = vlc_input_title_Duplicate(p_sys->titles[i]);
    }
    return VLC_SUCCESS;
  } break;

  case DEMUX_SET_TITLE: {
    int track = (int) va_arg(args, int);
    if (track < 0 || track >= p_sys->info.tracks)
      break;
    if (-1 == sc68_play(p_sys->sc68,track+1,-1))
      break;
    p_demux->info.i_title  = track;
    p_demux->info.i_update = INPUT_UPDATE_TITLE;
    return VLC_SUCCESS;
  } break;
  }

  return VLC_EGENERIC;
}

static void msg(const int bit, void *userdata, const char *fmt, va_list list)
{
#if 0
  demux_t * p_demux = (demux_t *) *sc68_cookie_ptr(userdata);

  if (p_demux) {
    vlc_object_t * vlc_object = VLC_OBJECT(p_demux);
    int level;
    switch (bit) {
    case  msg68_CRITICAL: case  msg68_ERROR:
      level = VLC_MSG_ERR; break;
    case msg68_WARNING:
      level = VLC_MSG_WARN; break;
    case msg68_INFO: case msg68_NOTICE:
      level = VLC_MSG_INFO; break;
    default:
      level = VLC_MSG_DBG;
    }
    vlc_vaLog(vlc_object, level, MODULE_STRING, fmt , list);
  }
#elif defined (WIN32)
  char tmp[512];
  vsnprintf(tmp,512,fmt,list);
  OutputDebugString(tmp);
#endif
}
