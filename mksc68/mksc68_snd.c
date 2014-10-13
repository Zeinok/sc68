/*
 * @file    mksc68_snd.c
 * @brief   disk functions.
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2014 Benjamin Gerard
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

/* generated config include */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mksc68_snd.h"
#include "mksc68_msg.h"
#include "mksc68_str.h"

/* #include "mksc68_cmd.h" */
/* #include "mksc68_tag.h" */
/* #include "mksc68_str.h" */
/* #include "mksc68_snd.h" */

#include <sc68/file68.h>
#include <sc68/file68_tag.h>
#include <sc68/file68_str.h>
#include <sc68/file68_uri.h>
#include <sc68/file68_vfs.h>
#include <sc68/sc68.h>

/* #include <assert.h> */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/* #include <time.h> */
/* #include <errno.h> */
/* #include <unistd.h> */
#include <stdint.h>


/* Even alignment */
static int w_E(uint8_t * dst, int off) {
  if (off & 1) {
    if (dst)
      dst[off] = 0;
    ++off;
  }
  return off;
}

/* Write data */
static int w_D(uint8_t * dst, int off, const void * src, int len) {
  if (dst)
    memcpy(dst+off,src,len);
  return off + len;
}

/* Write zero string */
static int w_S(uint8_t * dst, int off, const char * src) {
  return w_D(dst, off, src, strlen(src)+1);
}

/* Write zero string without trailing zero */
static int w_Z(uint8_t * dst, int off, const char * src) {
  return w_D(dst, off, src, strlen(src));
}

/* write byte */
static int w_B(uint8_t * dst, int off, const uint8_t val) {
  if (dst) dst[off] = val;
  return off+1;
}

/* write 68k word */
static int w_W(uint8_t * dst, int off, const uint16_t val) {
  if (dst) {
    dst[off+0] = val>>8;
    dst[off+1] = val;
  }
  return off+2;
}

/* write 68k long */
static int w_L(uint8_t * dst, int off, const uint32_t val) {
  if (dst) {
    dst[off+0] = val>>24;
    dst[off+1] = val>>16;
    dst[off+2] = val>>8;
    dst[off+3] = val;
  }
  return off+4;
}

/* write 4 bytes in order */
static int w_4(uint8_t * dst, int off, const void * src) {
  return w_D(dst, off, src, 4);
}

/* Write tag and string value, only if defined */
static int w_TS(uint8_t * dst, int off, const char * t, const char *s)
{
  if (t && s && *s) {
    off = w_Z(dst,off,t);
    off = w_S(dst,off,s);
  }
  return off;
}

static int w_T99(uint8_t * dst, int off, const char * t, int v)
{
  if (t && v > 1) {
    char tmp[3];
    off = w_Z(dst,off,t);               /* write the tag */
    while (v >= 99) {
      v -= 99;
      off = w_D(dst,off,"99",2);
    }
    tmp[0] = '0' + v / 10u;
    tmp[1] = '0' + v % 10u;
    tmp[2] = 0;
    off = w_D(dst, off, tmp, 3);
  }
  return off;
}

static uint16_t get_W(const uint8_t * buf)
{
  return (((uint16_t)(int8_t)buf[0]) << 8) | buf[1];
}


static int get_offset(const uint8_t * buf, int org, int off)
{
  int ret = -1;
  unsigned int w0, w1;

  if (off >= 12) {
    msgerr("mksndh: failed to decode offset +%d/%d\n", org, off);
    return -1;
  }

  w0 = get_W(buf + off + 0);
  msgdbg("w0[%02d] = $%04x\n", off, w0);
  /* Skip nop */
  if (w0 == 0x4e71)
    return get_offset(buf,org,off+2);
  w1 = get_W(buf + off + 2);
  msgdbg("w1[%02d] = $%04x\n", off+2, w1);

  /* decode 68k */
  if (w0 == 0x6000 || w0 == 0x4efa)
    ret = off + 2 + w1;                /* bra and jmp(pc) */
  else if ( (w0 & 0xff00) == 0x6000 )
    ret = off + (int8_t) w0;           /* bra.s */
  else if ( w0 == 0x4e75 )
    ret = off;                          /* rts */
  else
    msgerr("mksndh: failed to decode offset +%d/%d [$%04x:$%04x]\n",
           org,off,w0,w1);
  msgdbg("+%x %04x:%04x: -> %06x\n", off, w0,w1,ret);
  return ret;
}

static int sndh_decode_boot(const uint8_t * buf, int len, int off[4])
{
  static const char what[4][5] = { "init", "stop", "play", "data" };
  int i;

  off[3] = len;
  for (i=0; i<3; ++i) {

    /* decode init instruction */
    off[i] = get_offset(buf, i*4, i*4);
    if (off[i] < 0)
      return -1;
    if (off[i] >= len) {
      msgerr("mksndh: <%s> offset out of range -- %d not in [0..%d]\n",
             what[i], off[i], len);
      return -1;
    }
    if (off[i] > i*4 && off[i] < off[3])
      off[3] = off[i];
    msgdbg("mksndh: <%s> offset is +$%06x\n", what[i], off[i]);
  }
  msgdbg("mksndh: <%s> offset is +$%06x\n", what[i], off[i]);
  return 0;
}

static const char * tag(disk68_t *d, int track, const char * key)
{
  const char * s = file68_tag_get(d,track,key); /* try track */
  if ((!s || !*s) && track)
    s = file68_tag_get(d,0,key);        /* default to disk */

  if (s && !*s)
    s = 0;
  if (s)
    msgdbg("tag '%s' => '%s'\n", key, s);
  else
    msgdbg("tag '%s' missing\n", key);

  return s;
}


int sndh_save_buf(const char * uri, const void * buf, int len , int gzip)
{
  int ret = -1;
  if (!gzip) {
    vfs68_t * vfs = uri68_vfs(uri, SCHEME68_WRITE, 0);
    if (vfs) {
      ret = vfs68_open(vfs);
      if (!ret) {
        ret = -(vfs68_write(vfs,buf,len) != len);
        vfs68_close(vfs);
      }
      vfs68_destroy(vfs);
    }
  } else {
    /* $$$ TODO */
    msgerr("ICE! compression not supported yet\n");
  }
  return ret;
}

static
int sndh_save_sndh(const char * uri, disk68_t * d, int version, int gzip)
{
  int ret = -1;

  const uint8_t * sbuf = (const uint8_t *)d->mus[0].data;
  int             slen = d->mus[0].datasz;
  uint8_t       * dbuf = 0;
  int             dlen = 0;
  int             hlen = 0;

  struct {
    int init,stop,play,data;
  } boot;

  ret = sndh_decode_boot(sbuf, slen, (int*)&boot);
  if (ret)
    goto exit;
  ret = -1;
  if (boot.data < 16) {
    msgerr("mksndh: unable to detect sndh music data location\n");
    goto exit;
  }

  /* process in 2 pass:
   * - pass #1 compute required space and allocate buffer
   * - pass #2 do the real stuff
   */
  for (;;) {
    char tmp[32];
    const char *s;
    int off , i;

    /* SNDH */
    off = w_4(dbuf, 12, "SNDH");        /* sndh header  */

    /* Title */
    s = tag(d,0,TAG68_ALBUM);
    off = w_TS(dbuf, off, "TITL", s);   /* album -> title */

    /* Author */
    s = tag(d,0,TAG68_ARTIST);
    off = w_TS(dbuf, off, "COMM", s);   /* artist -> composer */

    /* Ripper */
    s = tag(d, 1, TAG68_RIPPER);
    off = w_TS(dbuf, off, "RIPP", s);   /* ripper -> ripper */

    /* Converter */
    s = tag(d, 1, TAG68_CONVERTER);
    off = w_TS(dbuf, off, "CONV", s);   /* converter -> converter */

    /* Year */
    s = tag(d, 1, TAG68_YEAR);
    off = w_TS(dbuf, off, "YEAR", s);   /* converter -> converter */

    /* Number of song */
    off = w_T99(dbuf, off, "##", d->nb_mus);

    /* Default song */
    off = w_T99(dbuf, off, "!#", d->def_mus+1);

    /* Replay method and frequency */
    if (d->mus[0].frq != 50) {
      /* $$$ FIXME: Replay method and frequency */
      sprintf(tmp,"TC%i",d->mus[0].frq);
      off = w_S(dbuf, off, tmp);
    }


    /* Duration */
    /* FIXME: now just testing track #1 */
    if (d->mus[0].first_ms) {
      off = w_E(dbuf,off);
      off = w_D(dbuf,off,"TIME",4);
      for (i=0; i<d->nb_mus; ++i) {
        unsigned int sec = (d->mus[i].first_ms+999u)/1000u;
        off = w_W(dbuf, off, sec);
        msgdbg("track #%03d %s\n",
               i+1, str_timefmt(tmp, sizeof(tmp), sec * 1000u));
      }
    }

    /* Footer */
    off  = w_4(dbuf, off, "HDNS");      /* sndh footer   */
    off  = w_E(dbuf, off);              /* ensure even   */
    hlen = off;                         /* header length */
    dlen = off + slen - boot.data;
    msgdbg("sndh header end at +$%x (%d bytes)\n", off, dlen);

    if (dlen < 22) {
      msgerr("%d bytes is too small for being valid sndh data\n", dlen);
      break;
    }

    /* rewrite boot */
    if (dbuf) {
      int * bt = &boot.init;
      for (i=0; i<3; ++i) {
        if (bt[i] < hlen)
          w_L(dbuf, i*4, 0x4e754e75);
        else {
          w_W(dbuf, i*4+0, 0x6000);
          w_W(dbuf, i*4+2, (bt[i] - bt[3] + hlen) - (i*4+2));
        }
      }
      memcpy(dbuf+hlen,sbuf+boot.data,slen-boot.data);
      ret = sndh_save_buf(uri, dbuf, dlen, gzip);
      break;
    }

    /* finally alloc buffer and ready for pass 2 */
    dbuf = malloc(dlen);
    if (!dbuf) {
      msgerr("memory allocation failed\n");
      break;
    }
  }

exit:
  free(dbuf);
  return ret;
}


static
int sndh_save_sc68(const char * uri, disk68_t * d, int version, int gzip)
{
  int ret = -1;
  msgerr("sndh: save '%s' version:%d compress:%d"
         " -- sc68 -> sndh unsupported\n", uri, version, gzip);
  return ret;
}

int sndh_save(const char * uri, void * _dsk, int version, int gzip)
{
  int ret;
  disk68_t * d = (disk68_t *)_dsk;

  if (!strcmp68(d->tags.array[TAG68_ID_FORMAT].val,"sc68"))
    ret = sndh_save_sc68(uri, d, version, gzip);
  else
    ret = sndh_save_sndh(uri, d, version, gzip);

  return ret;
}
