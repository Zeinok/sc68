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

#include <sc68/file68.h>
#include <sc68/file68_tag.h>
#include <sc68/file68_str.h>
#include <sc68/file68_uri.h>
#include <sc68/file68_vfs.h>
#include <sc68/sc68.h>
#ifdef USE_UNICE68
#include <unice68.h>
#endif

/* #include <assert.h> */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
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
/* static int w_B(uint8_t * dst, int off, const uint8_t val) { */
/*   if (dst) dst[off] = val; */
/*   return off+1; */
/* } */

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
      off = w_D(dst,off,"99",2);        /* write '99' */
    }
    tmp[0] = '0' + v / 10u;             /* write '99' */
    tmp[1] = '0' + v % 10u;
    tmp[2] = 0;
    off = w_D(dst, off, tmp, 3);
  }
  return off;
}

static int16_t get_W(const uint8_t * buf){
  return (((int16_t)(int8_t)buf[0]) << 8) | buf[1];
}

typedef union {
  int off[4];
  struct sndh_boot {
    int init, kill, play, data;
  } boot;
} sndh_boot_t;

static const char what[4][5] = { "init", "kill", "play", "data" };

enum {
  INST_NOP,
  INST_RTS,
  INST_JMP,
  INST_ERR = -1
};

static int decode_68k(const uint8_t * buf, int * _off)
{
  int ret, w, off = *_off;

  if (off < 0 || off >= 10)
    return INST_ERR;                    /* out of range */

  w = get_W(buf+off);
  off += 2;

  switch (w) {
  case 0X4E71:                          /* NOP */
    ret = INST_NOP;
    break;
  case 0x4E00:                          /* Illegal fix */
    if (off != 6)
      return INST_ERR;
    /* continue on rts */
  case 0x4E75:                          /* RTS */
    ret = INST_RTS;
    off -= 2;
    break;
  case 0x6000:                          /* BRA */
  case 0x4efa:                          /* JMP(PC) */
    if (off > 10)
      return INST_ERR;                  /* out of range */
    off += get_W(buf+off);
    ret = INST_JMP;
    break;
  default:
    if ( (w & 0xff00) != 0x6000 )       /* BRA.S */
      return INST_ERR;
    off += -2 + (int8_t) w;
    ret = INST_JMP;
  }
  *_off = off;
  return ret;
}

static int decode_inst(const uint8_t * buf, sndh_boot_t * sb, int i, int msk)
{
  int off = sb->off[i];

  if (off != -1)
    return 0;
  off = i << 2;

  for (;;) {
    assert( !(off&1) );
    assert( off >= 0 );
    assert( off < 12 );

    if (msk & (1<<off)) {
      msgerr("mksndh: <%s> infinite loop -- +%d\n",
             what[i], off);
      return -1;
    }
    msk |= 1<<off;

    switch (decode_68k(buf,&off)) {

    case INST_RTS:
      sb->off[i] = 0;
      return 0;

    case INST_JMP:
      if ((off & 1) || off < 0 || off >= sb->boot.data) {
        msgerr("mksndh: <%s> invalid jump location -- +%d\n",
               what[i], off);
        return -1;
      }
      if (off < 12)
        break;
      sb->off[i] = off;
      return 0;

    case INST_NOP:
      if (off == 12) {
        msgerr("mksndh: <%s> ran out range -- +%d\n",
               what[i], off);
        return -1;
      }
      break;

    default:
      assert(!"should not happen");
      /* continue on error */
    case INST_ERR:
      msgerr("mksndh: unable to decode <%s> instruction at +%d\n",
             what[i], off);
      return -1;
    }
  }
  assert(!"should not reach that point");
  return -1;


}

static int decode_boot(const uint8_t * buf, int len, sndh_boot_t * sb)
{
  int i;

  /* init */
  for (i=0; i<3; ++i)
    sb->off[i] = -1;
  sb->off[i] = len;                     /* use this for range */

  /* decode 3 boot instructions */
  for (i=0; i<3; ++i)
    if (decode_inst(buf,sb,i,0) < 0)
      return -1;
    else
      msgdbg("mksndh: <%s> offset is +$%06x\n", what[i], sb->off[i]);

  /* locate data */
  for (i=0; i<3; ++i) {
    int off = sb->off[i];

    if (!off)
      continue;                         /* RTS */

    if (off < 16) {
      msgerr("sndh: <%s> offset +%d is invalid\n", what[i], off);
      return -1;
    }

    if (off < sb->boot.data)
      sb->boot.data = off;
  }

  /* ensure data is in range */
  if (sb->boot.data < 16 || sb->boot.data >= len) {
    msgerr("sndh: unable to locate sndh data location -- %d\n", sb->boot.data);
    return -1;
  }

  /* looking for 'HDNS' tag */
  for (i=sb->boot.data-4; i>=16; --i)
    if (!memcmp(buf+i,"HDNS",4)) {
      msgdbg("sndh: 'HDNS' marker found at +%d\n",i+4);
      sb->boot.data = i+4;
      break;
    }

  if (i<16)
    msgwrn("sndh: no 'HDNS' marker; output might be corrupt\n");

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


int sndh_save_buf(const char * uri, const void * buf, int len , int gz)
{
  int ret = -1;
  if (!gz) {
    vfs68_t * vfs = uri68_vfs(uri, SCHEME68_WRITE, 0);
    if (vfs) {
      ret = vfs68_open(vfs);
      if (!ret) {
        ret = -(vfs68_write(vfs,buf,len) != len);
        vfs68_close(vfs);
        if (ret)
          msgerr("sndh: failed to save -- %s\n", uri);
        else
          msginf("sndh: saved (%d bytes) -- %s\n", len, uri);
      }
      vfs68_destroy(vfs);
    }
  } else {
    int max = len + (len>>1);
    char * dst = malloc(max);
    if (!dst) {
      msgerr("sndh: %s\n", strerror(errno));
      ret = -1;
    } else {
#ifdef USE_UNICE68
      ret = unice68_packer(dst, max, buf, len);
      if (ret > 0) {
        dst[1]='C'; dst[2]='E';         /* fix ICE header */
        ret = sndh_save_buf(uri,dst, ret, 0);
      } else
        ret = -1;
#else
      msgerr("sndh: ICE! packer not supported\n");
#endif
    }
  }
  return ret;
}

static
int save_sndh(const char * uri, disk68_t * d, int vers, int gz)
{
  int ret = -1;

  const uint8_t * sbuf = (const uint8_t *)d->mus[0].data;
  int             slen = d->mus[0].datasz;
  uint8_t       * dbuf = 0;
  int             dlen = 0;
  int             hlen = 0;

  sndh_boot_t sb;

  ret = decode_boot(sbuf, slen, &sb);
  if (ret)
    goto exit;
  ret = -1;

  /* Process in 2 pass:
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

    /* Sub names */
    for (i=0; i<d->nb_mus; ++i)
      if (strcmp(d->tags.tag.title.val, d->mus[i].tags.tag.title.val))
        break;
    if (i != d->nb_mus) {
      int tbl, bas;
      bas = off = w_E(dbuf,off);        /* Even / base string offset */
      tbl = w_D(dbuf,off,"!#SN",4);     /* Table after the tag */
      off = tbl+2*d->nb_mus;            /* Skip the table */
      for (i=0; i<d->nb_mus; ++i) {
        w_W(dbuf,tbl+i*2,off-bas);           /* Fill offset to string */
        off = w_S(dbuf,off,d->mus[i].tags.tag.title.val);
      }
    }

    /* Footer */
    off  = w_4(dbuf, off, "HDNS");      /* sndh footer   */
    off  = w_E(dbuf, off);              /* ensure even   */
    hlen = off;                         /* header length */
    dlen = off + slen - sb.boot.data;
    msgdbg("sndh header end at +$%x (%d bytes)\n", off, dlen);

    if (dlen < 22) {
      msgerr("%d bytes is too small for being valid sndh data\n", dlen);
      break;
    }

    /* Rewrite boot */
    if (dbuf) {
      int * bt = sb.off;
      for (i=0; i<3; ++i) {
        if (bt[i] < hlen)
          w_L(dbuf, i*4, 0x4e754e75);
        else {
          w_W(dbuf, i*4+0, 0x6000);
          w_W(dbuf, i*4+2, (bt[i] - bt[3] + hlen) - (i*4+2));
        }
      }
      memcpy(dbuf+hlen,sbuf+sb.boot.data,slen-sb.boot.data);
      ret = sndh_save_buf(uri, dbuf, dlen, gz);
      break;
    }

    /* Finally alloc buffer and ready for pass 2 */
    dbuf = malloc(dlen);
    if (!dbuf) {
      msgerr("sndh: %s\n", strerror(errno));
      break;
    }
  }

exit:
  free(dbuf);
  return ret;
}


static
int sndh_save_sc68(const char * uri, disk68_t * d, int vers, int gz)
{
  int ret = -1;
  msgerr("sndh: save '%s' version:%d compress:%d"
         " -- sc68 -> sndh unsupported\n", uri, vers, gz);
  return ret;
}

int sndh_save(const char * uri, void * _dsk, int vers, int gz)
{
  int ret;
  disk68_t * d = (disk68_t *)_dsk;

  if (!strcmp68(d->tags.array[TAG68_ID_FORMAT].val,"sc68"))
    ret = sndh_save_sc68(uri, d, vers, gz);
  else
    ret = save_sndh(uri, d, vers, gz);

  return ret;
}
