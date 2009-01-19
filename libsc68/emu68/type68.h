/**
 * @ingroup   emu68_type68_devel
 * @file      emu68/type68.h
 * @brief     Type definitions header.
 * @date      1999/03/13
 * @author    Benjamin Gerard
 *
 * $Id$
 *
 */

/* Copyright (C) 1998-2007 Benjamin Gerard */

#ifndef _TYPE68_H_
#define _TYPE68_H_

/* #include <config_type68.h> */

#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  emu68_type68_devel  Type definitions
 *  @ingroup   emu68_devel
 *
 *   Definition of types used by EMU68 and SC68 related projects.
 *   Some of them are probably not neccessary and should be remove to
 *   improve execution time on 64 or more bit platforms.
 *   Setting wrong types may probably produce annoying effects on EMU68.
 *   It is a limitation to EMU68 portability.
 *
 *  @{
 */

/** @name Fixed size integer types.
 *  @{
 */

typedef uint8_t u8;        /**< Must be an unsigned 8 bit integer. */
typedef int8_t  s8;        /**< Must be an   signed 8 bit integer. */

typedef uint16_t u16;      /**< Must be an unsigned 16 bit integer. */
typedef int16_t  s16;      /**< Must be an   signed 16 bit integer. */

typedef uint32_t u32;      /**< Must be an unsigned 32 bit integer. */
typedef int32_t  s32;      /**< Must be an   signed 32 bit integer. */

typedef uint64_t u64;      /**< Must be at an unsigned 64 bit integer. */
typedef int64_t  s64;      /**< Must be an   signed 64 bit integer. */

/**@}*/


/** Commonly used int (should be fastest at least 32 bit type). */
typedef  int_least32_t  int68_t;
typedef        int68_t sint68_t;
typedef uint_least32_t uint68_t;

/** Used by cycle counters. */
typedef uint68_t cycle68_t;

/** Used for addressing 68k memory. */
typedef uint68_t addr68_t;

/** IO chip instance type. */
typedef struct io68_s io68_t;

/** 68k Emulateur instance type. */
typedef struct emu68_s emu68_t;

/** 68k memory acces function. */
typedef void (*memfunc68_t)(emu68_t * const);

/**
 *@}
 */

#define BYTE_MASK ~0xFF
#define WORD_MASK ~0xFFFF
#define LONG_MASK 0

#define BYTE_SHIFT ((sizeof(int68_t)-1)<<3)
#define WORD_SHIFT ((sizeof(int68_t)-2)<<3)
#define LONG_SHIFT ((sizeof(int68_t)-4)<<3)

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _TYPE68_H_ */
