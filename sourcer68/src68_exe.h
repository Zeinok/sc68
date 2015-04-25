/**
 * @ingroup  sourcer68_prg
 * @file     src68_exe.h
 * @author   Benjamin Gerard
 * @date     2015-03-18
 * @brief    executable loader.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef SOURCER68_EXE_H
#define SOURCER68_EXE_H

#include "src68_def.h"
#include "src68_sym.h"
#include "src68_mbk.h"
#include "src68_sec.h"

enum {
  LOAD_AS_AUTO, LOAD_AS_BIN, LOAD_AS_TOS, LOAD_AS_SC68
};

struct parts_s;
typedef struct parts_s parts_t;

enum {
  EXE_ORIGIN = 0x10000,
  EXE_DEFAULT = -1
};

typedef struct {
  char    * uri;
  int       loadas;
  vec_t   * sections;
  vec_t   * symbols;
  mbk_t   * mbk;
  parts_t * parts;
} exe_t;

/**
 * Load an executable to disassemble.
 *
 * @param  uri   file path or IRI.
 * @param  org   address to load the file at.
 * @param  type  Loas as type.
 * @return pointer to loaded exeutable.
 * @retval 0  on error
 */
exe_t * exe_load(char * uri, uint_t org, int loadas);

/**
 * Free executable.
 *
 * @param  exe  executable to free.
 */
void exe_del(exe_t * exe);

#endif
