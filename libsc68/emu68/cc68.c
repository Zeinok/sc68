/**
 * @ingroup   emu68_devel
 * @file      cc68.c
 * @author    Benjamin Gerard
 * @date      1999/03/13
 * @brief     Code condition function table.
 * @version   $Id$
 */

/* Copyright (C) 1998-2007 Benjamin Gerard */
 
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
