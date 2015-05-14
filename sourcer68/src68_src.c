/*
 * @file    src68_src.c
 * @brief   source producer.
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
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
# include "config.h"
#endif

#include "src68_src.h"
#include "src68_dis.h"
#include "src68_msg.h"
#include <desa68.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern const char hexa[];               /* in src68_dis.c */

typedef struct {
  const char * uri;                     /* input URI */
  fmt_t * fmt;                          /* formatter */
  exe_t * exe;                          /* executable */

  /* segment */
  struct {
    uint_t org;                         /* segment origin */
    uint_t end;                         /* segment end address */
    uint_t len;                         /* segment length */
    uint_t adr;                         /* segment current address */
  } seg;

  /* disassebler options */
  struct {
    uint_t show_opcode:1;         /* show opcodes */
    char * star_symbol;           /* symbol for current address (*) */
  } opt;

  /* disassemby temp buffer */
  struct {
    char * buf;                         /* disassebly buffer */
    uint_t max;                         /* sizeof of buf in byte */
  } str;

} src_t;

static char strbuf[1024];    /* static srting buffer for disassembly */

static
/*
 * Print all symbols from address adr+ioff.
 * ioff == 0 -> Label:
 * ioff != 0 -> Label: = *+
 */
int symb_exactly_at(src_t * src, uint_t adr, uint_t ioff)
{
  exe_t * const exe = src->exe;

  int i = 0;
  int mib = mbk_getmib(exe->mbk, adr+ioff);
  int isym = symbol_byaddr(exe->symbols, adr+ioff, i);
  if ( (isym >= 0) || (mib & (MIB_ADDR|MIB_ENTRY)) ) {
    /* At least one */
    do {
      sym_t * sym = symbol_get(exe->symbols, isym);
      fmt_eol(src->fmt,0);                   /* next line if needed */
      if (sym)
        fmt_putf(src->fmt, "%s:", sym->name);
      else
        fmt_putf(src->fmt, "L%06X:", adr+ioff);
      if (ioff) {
        fmt_putf(src->fmt, " = %s%+d", src->opt.star_symbol, ioff);
        fmt_eol(src->fmt,0);
      }
      isym = symbol_byaddr(exe->symbols, adr+ioff, ++i);
    } while (isym >= 0);
  }
  return 0;
}

static
/*
 * Print all symbols from address adr up to adr+len exclusive.
 */
int symb_at(src_t * src, uint_t adr, uint_t len)
{
  uint_t ioff;

  /* For each address in range */
  for (ioff = 0; ioff < len; ++ioff)
    if (symb_exactly_at(src, adr, ioff) < 0)
      return -1;
  return 0;
}

static
/*
 * Get data type from mib [0:untyped 1:byte 2:word 4:long
 */
int mib2type(int mib, int deftype)
{
  assert(deftype >= 0 && deftype < 4);

  /* Relocated are always long */
  if ( mib & MIB_RELOC)
    return 4;

  /* no data type: keep default (can be undef) */
  if ( !(mib & (MIB_LONG|MIB_WORD|MIB_BYTE)) )
    return deftype;

  /* have a matching default */
  if (deftype > 0 && (mib & (MIB_BYTE<<(deftype-1))))
    return deftype;

  /* else get the smaller type first */
  if (mib & MIB_BYTE)
    return 1;
  else if (mib & MIB_WORD)
    return 2;
  assert (mib & MIB_LONG);              /* Must be long */
  return 4;
}

static uint_t data_max(mbk_t * mbk, uint_t adr, uint_t len)
{
  uint_t off;

  for (off = 0 ; off < len; off+=2) {
    uint_t mib = mbk_getmib(mbk, adr+off);
    if ( ( mib & (MIB_SET|MIB_EXEC) ) != MIB_SET )
      break;             /* break if EXEC or not SET (out of range) */
  }
  return off;
}

static
/*
 * Format one line of data from address adr with a maximum of len.
 */
int one_data(fmt_t * fmt, exe_t * exe, uint_t adr, uint_t len)
{
#if 0
  mbk_t * const mbk = exe->mbk;
  int dtype = 0, mib, isym;

  assert (adr >= mbk->org && adr+len <= mbk->org+mbk->len);

  if (symb_at(fmt, exe, adr, 1) < 0)
    return -1;

  /* Scan to determine data-type (dtype) */
  if (adr & 1)
    dtype = 1;                /* on odd address request byte format */
  else {
    uint_t ioff;
    for (ioff = 0; ioff < len; ++ioff) {
      int type;

      mib = mbk_getmib(mbk, adr+ioff);
      type = mib2type(mib, dtype);


    if (!dtype)
      ;

    if (type != dtype && type && dtype)
      dtype = type;
    }
    mib = mbk_getmib(exe->mbk, ++adr);
  }
  return ilen;

#endif
  return len;
}

static
int src_data(fmt_t * fmt, exe_t * exe, uint_t adr, uint_t len)
{
  mbk_t * mbk = exe->mbk;
  uint_t off, end;

  /* should be tested upstream */
  assert (adr >= mbk->org && adr+len <= mbk->org+mbk->len);

  for (off=adr-mbk->org, end=off+len; off < end; ) {
    uint_t adr0, ioff, ilen, mib;
    int ityp;

    adr0 = ioff = mbk->org + off;       /* instrution address */
    ilen = len - off;                   /* remaining byte(s) */
    ityp = DESA68_DCW;                  /* default instruction type */

    /* For each address in the instruction range */
    for (ioff = 0; ioff < ilen; ++ioff ) {
      uint_t adr = adr0 + ioff;
      int isym, atleast1;

      mib = mbk_getmib(mbk, adr);
      if (mib & MIB_ENTRY)
      atleast1 = mib & (MIB_ADDR|MIB_ENTRY);
    }

    off += ilen;
  }
  return -1;
}

static
int src_bss(fmt_t * fmt, exe_t * exe, uint_t adr, uint_t len)
{
  return -1;
}

static
int inst_fmt(fmt_t * fmt, char * str)
{
  int err;
  char * space = strchr(str,' ');
  err = fmt_tab(fmt);
  if (space) {
    err |= fmt_cat(fmt, str, space-str);
    err |= fmt_tab(fmt);
  } else {
    space = str-1;
  }
  err |= fmt_puts(fmt,space+1);
  return err;
}

static
/*
 * Source from address adr to adr+len exclusive as code.
 */
int src_code(src_t * src, const uint_t adr, const uint_t len)
{
  char str[256];
  uint_t off, end;

  fmt_t * const fmt = src->fmt;
  exe_t * const exe = src->exe;
  mbk_t * const mbk = src->exe->mbk;

  /* should be tested upstream */
  assert(adr >= mbk->org && adr+len <= mbk->org+mbk->len);

  for (off = adr-mbk->org, end = off+len; off < end; ) {
    uint_t adr0, ioff, ilen, mib, ityp;

    adr0 = ioff = mbk->org + off;       /* instruction address */
    ilen = len - off;                   /* remaining byte(s) */
    ityp = DESA68_DCW;                  /* default instruction type */
    if (ilen >= 2 && !(adr0 & 1)) {
      ityp = dis_disa(exe, &ioff, str, sizeof(str));
      ioff -= mbk->org;
      if (ioff > end)        /* instruction decoding has overflowed */
        ityp = DESA68_DCW;
      else if (ityp != DESA68_DCW)
        ilen = ioff - off;
    }

    /* $$$ XXX For now just one data at a time */
    if (ityp == DESA68_DCW) {
      ilen = 2 - (adr0&1);
      if (off+ilen > end)
        ilen = end - off;
    }

    /* Labels in instruction range */
    if (symb_at(src, adr0, ilen) < 0)
      return -1;

    fmt_tab(fmt);
    if (ityp == DESA68_DCW) {
      if (ilen >= 2) {
        fmt_puts(fmt, "dc.w");
        fmt_tab(fmt);
        fmt_putf(fmt, "$%02x%02x\n",mbk->mem[off],mbk->mem[off+1]);
        ilen = 2;
      } else {
        fmt_puts(fmt, "dc.b");
        fmt_putf(fmt, "$%02x\n", mbk->mem[off]);
        assert(ilen == 1);
      }
    } else {
      inst_fmt(fmt, str);
    }

    fmt_eol(fmt,0);

    /* else it would loop forever ! */
    assert (ilen > 0 || off == len);

    off += ilen;
  }

  return -1;
}


static
int inst(src_t * src)
{
  mbk_t * const mbk = src->exe->mbk;
  int err = 0, itype;
  uint_t ilen, iadr = src->seg.adr;

  assert( ! (iadr & 1 ) );

  itype = dis_disa(src->exe, &src->seg.adr, src->str.buf, src->str.max);
  if (itype <= DESA68_DCW) {
    /* Error or data */
    src->seg.adr = iadr;                /* restore position */
    return 0;
  }

  ilen = src->seg.adr - iadr;
  assert(ilen >= 2 && ilen <= 10);
  assert(!(ilen & 1));

  /* Symbols */
  symb_at(src, iadr, ilen);

  /* Instruction */
  err |= inst_fmt(src->fmt, src->str.buf);

  /* Opcode bytes */
  if (ilen && src->opt.show_opcode) {
    uint_t i;
    char * s = src->str.buf;
    strcpy(s,"; "); s += 3;
    for (i=0; i<ilen; ++i) {
      const uint8_t byte = mbk->buf[iadr+i-mbk->org];
      *s++ = hexa[byte>>4];
      *s++ = hexa[byte&15];
      if ( (i & 1) ) {
        if ( i > 2 && i+3 <= ilen && mbk_getmib(mbk, iadr+i-1) & MIB_LONG )
          continue;
        *s++ = '-';
      }
    }
    s[-1] = 0;
    err |= fmt_tab(src->fmt);
    err |= fmt_cat(src->fmt, src->str.buf, s-src->str.buf);
  }

  /* next line */
  err |= fmt_eol(src->fmt,0);

  /* $$$ XXX ignore errors for now */

  return ilen;
}

static
/* auto detect code and data blocks */
int mixed(src_t * src, const uint_t adr, const uint_t len)
{
  mbk_t * const mbk = src->exe->mbk;

  assert(adr >= mbk->org);
  assert(adr+len <= mbk->org+mbk->len);

  src->seg.adr = adr;                   /* segment address.     */
  src->seg.len = len;                   /* segment length.      */
  src->seg.end = adr + len;             /* segment end address. */

  while (src->seg.adr < src->seg.end) {
    uint_t mib = mbk_getmib(mbk, src->seg.adr);
    assert(mib);

    if ( !(mib & MIB_EXEC) || inst(src) <= 0) {
      uint_t ilen = data_max(src->exe->mbk, src->seg.adr, src->seg.end-src->seg.adr);
      assert (ilen > 0);
      fmt_tab(src->fmt);
      fmt_putf(src->fmt, ";; TODO DATA %x to %x (%d)",
               src->seg.adr, src->seg.adr+ilen-1, ilen);
      fmt_eol(src->fmt,0);

      src->seg.adr += ilen;
    }
  }

  return 0;
}


int src_exec(fmt_t * fmt, exe_t * exe)
{
  src_t src;

  if (!fmt || !exe)
    return -1;

  /* setup sourcer info struct */
  memset(&src,0,sizeof(src));

  src.fmt = fmt;
  src.exe = exe;
  src.str.buf = strbuf;                 /* tmp string buffer */
  src.str.max = sizeof(strbuf);
  src.opt.star_symbol = "*";            /* star symbol */
  src.opt.show_opcode = 1;

  mixed(&src, exe->mbk->org, exe->mbk->len);

  /* src_code(fmt, exe, exe->mbk->org, exe->mbk->len); */
  /* src_data(fmt, exe, exe->mbk->org, exe->mbk->len); */
  /* src_bss (fmt, exe, exe->mbk->org, exe->mbk->len); */

/*
  section = section_get(exe->sections, section_idx = 0);
  for (off = 0; off < mbk->len; ) {
    adr = off + mbk->org;
    while (section && adr < section->addr) {
      if ( !(adr & 1) && (mbk->mib[off] & MIB_MIB_EXEC) ) {
        // disassemble
      } else {
        // data
      }
    }
  }
*/

  return -1;
}
