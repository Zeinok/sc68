/**
 * @ingroup  file68_lib
 * @file     file68/registry68.h
 * @author   Benjamin Gerard
 * @date     2003-08-11
 * @brief    Registry header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_REGISTRY68_H_
#define _FILE68_REGISTRY68_H_

#ifndef FILE68_API
#include "file68_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  file68_registry68_devel  Registry
 *  @ingroup   file68_lib
 *
 *    Provides registry access functions.
 *
 *  @{
 */

/** Enumerate registry key types. */
enum registry68_key_e {
  REGISTRY68_INK = -1,
  REGISTRY68_CRK = 0,
  REGISTRY68_CUK,
  REGISTRY68_LMK,
  REGISTRY68_USK,
  REGISTRY68_PDK,
  REGISTRY68_CCK,
  REGISTRY68_DDK,

  REGISTRY68_LST  /* Last element */
};

/** Registry key type (override Microsoft HKEY type) */
typedef void * registry68_key_t;

/* Last error message. */
/* extern char registry68_errorstr[]; */

FILE68_API
/** Get key handler for a registry root type.
 *
 *  @param  rootkey  One of the M$ registry key-type/root-key/entry-point.
 *  @return key handler
 *  @retval  0  error (key invalid)
 */
registry68_key_t registry68_rootkey(enum registry68_key_e rootkey);

FILE68_API
/** Open a named hierarchic key.
 *
 *  @param hkey     Opened key handle or one of reserved registry key handles.
 *  @param kname    Hierarchic key name. Slash '/' caractere is interpreted
 *                  as sub-key separator.
 *
 *  @return Registry key handle
 *  @retval registry68InvalidKey Error
 */
registry68_key_t registry68_open(registry68_key_t hkey, const char *kname);

FILE68_API
/** Get value of a named hierarchic string key.
 *
 *  @param hkey     Opened key handle or one of reserved registry key handles.
 *  @param kname    Hierarchic key name. Slash '/' caractere is interpreted
 *                  as sub-key separator.
 *  @param kdata    Returned string storage location
 *  @param kdatasz  Maximum size of kdata buffer.
 *
 *  @return ErrorNo
 *  @retval 0  Success
 *  @retval <0 Error
 */
int registry68_gets(registry68_key_t hkey,
		    const char *kname,
		    char *kdata,
		    int kdatasz);

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _FILE68_REGISTRY68_H_ */