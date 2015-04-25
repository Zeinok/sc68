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

static
/*
 * Print all symbols from address adr+ioff.
 */
int symb_exactly_at(fmt_t * fmt, exe_t * exe, uint_t adr, uint_t ioff)
{
  int i = 0;
  int mib = mbk_getmib(exe->mbk, adr+ioff);
  int isym = symbol_byaddr(exe->symbols, adr+ioff, i);
  if ( (isym >= 0) || (mib & (MIB_ADDR|MIB_ENTRY)) ) {
    /* At least one */
    do {
      sym_t * sym = symbol_get(exe->symbols, isym);

      /* dmsg("symbol #%d at $%x+%d [%s]\n", i, adr,ioff,sym?sym->name:"auto"); */

      fmt_eol(fmt,0);                   /* next line if needed */
      if (sym)
        fmt_putf(fmt, "%s:", sym->name);
      else
        fmt_putf(fmt, "L%06X:", adr+ioff);
      if (ioff) {
        fmt_putf(fmt, " = *%+d", ioff);
        fmt_eol(fmt,0);
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
int symb_at(fmt_t * fmt, exe_t * exe, uint_t adr, uint_t len)
{
  uint_t ioff;

  /* dmsg("symbol from $%x to $%x\n", adr, adr+len); */

  /* for each address in the instruction range */
  for (ioff = 0; ioff < len; ++ioff)
    if (symb_exactly_at(fmt, exe, adr, ioff) < 0)
      return -1;
  return 0;
}

static
/*
 * get data type from mib [0:untyped 1:byte 2:word 4:long
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
  char str[256];
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
/*
 * Source from address adr to adr+len exclusive as code.
 */
int src_code(fmt_t * fmt, exe_t * exe, const uint_t adr, const uint_t len)
{
  char str[256];
  mbk_t * const mbk = exe->mbk;
  uint_t off, end;

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
    if (symb_at(fmt, exe, adr0, ilen) < 0)
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
      char * space;
      space = strchr(str,' ');
      if (space) {
        fmt_cat(fmt, str, space-str);
        fmt_tab(fmt);
      } else {
        space = str-1;
      }
      fmt_puts(fmt,space+1);
    }
    fmt_eol(fmt,0);

    /* else it would loop forever ! */
    assert (ilen > 0 || off == len);

    off += ilen;
  }

  return -1;
}

int src_exec(fmt_t * fmt, exe_t * exe/* , const char * uri */)
{
  if (!fmt || !exe)
    return -1;
  src_code(fmt, exe, exe->mbk->org, exe->mbk->len);
  src_data(fmt, exe, exe->mbk->org, exe->mbk->len);
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
