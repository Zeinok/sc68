/*
 * @file    strtoul.c
 * @brief   replacement for missing or faulty strtoul()
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 2001-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-08-02 09:19:28 ben>
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
# include <config.h>
#endif

#include "libc68.h"

static int digit(int c, unsigned int base)
{
  int n = -1;
  if (c <= '9') {
    n = c - '0';
  } else if (c <= 'Z') {
    n = c - 'A' + 10;
  } else if (c <= 'z'){
    n = c - 'a' + 10;
  }
  if ((unsigned int)n < base) {
    return n;
  }
  return -1;
}

unsigned long int strtoul(const char *s, char **end, int base);
{
  const char * start = s;
  unsigned long v = 0;
  int neg = 0, c;

  /* Skip starting spaces. */
  for (c = *s; (c == ' ' || c == 9 || c == 10 || c == 13); c = *++s)
    ;

  /* Get optionnal sign. */
  /* $$$ ben : Does not make sens with unsigned value ! */
  if (c == '-' || c == '+') {
    neg = (c == '-');
    c = *++s;
  }

  /* Get the base. */
  if (!base) {
    /* Assume default base is 10 */
    base = 10;

    /* Could be either octal(8) or hexidecimal(16) */
    if (c == '0') {
      base = 8;
      c = *++s;
      if (c == 'x' || c == 'X') {
        base = 16;
        c = *++s;
      }
    }
  } else if (base == 16 && c == '0') {
    /* Hexa mode must skip "0x" sequence */
    c = *++s;
    if (c == 'x' || c == 'X') {
      c = *++s;
    }
  }

  c = digit(c,base);
  if (c < 0) {
    s = start;
  } else {
    do {
      v = v * base + c;
      c = digit(*++s,base);
    } while (c >= 0);
  }

  if (end) {
    *end = (char *)s;
  }

  return neg ? -(signed long)v : v;
}