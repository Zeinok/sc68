/*
 *              emu68 - Code condition function table
 *             Copyright (C) 2001-2009 Benjamin Gerard
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
 */

/* $Id$ */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "struct68.h"
#include "srdef68.h"
#include "cc68.h"

/* ,--------------------------.
 * | Condition code functions |
 * `--------------------------' */

static int is_fa(const int sr) { return 0;           }
static int is_ls(const int sr) { return !!IS_LS(sr); }
static int is_cs(const int sr) { return !!IS_CS(sr); }
static int is_eq(const int sr) { return !!IS_EQ(sr); }
static int is_vs(const int sr) { return !!IS_VS(sr); }
static int is_mi(const int sr) { return !!IS_MI(sr); }
static int is_lt(const int sr) { return !!IS_LT(sr); }
static int is_le(const int sr) { return !!IS_LE(sr); }

int (* const is_cc68[8])(const int) = {
  is_fa, is_ls, is_cs, is_eq,
  is_vs, is_mi, is_lt, is_le,
};
