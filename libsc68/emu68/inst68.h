/**
 * @ingroup   emu68_core_devel
 * @file      emu68/inst68.h
 * @author    Benjamin Gerard
 * @date      1999/03/13
 * @brief     68k arithmetic and logical instruction header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _INST68_H_
#define _INST68_H_

#include "emu68_api.h"
#include "type68.h"

/** @addtogroup   emu68_core_devel
 *
 *   68k arithmetic and logical instruction emulation.
 *   Logical and arithmetical instructions are emulated with functions
 *   instead of macros to prevent from excessive code size generation that
 *   hurt processor cache. By the way these functions could easily be
 *   written in assembler and improve emulator execution time. All these
 *   functions work with 32 bit values. To perform other size instructions,
 *   operands must be left shifted in order to locate operands most
 *   signifiant bit at the 31st bit.
 *
 *  @{
 */

/** @name Arithmetic instruction functions
 *  @{
 */

EMU68_EXTERN
/** Addition @return a+b+c */
s32 add68(emu68_t * const emu68, s32 a, s32 b, s32 c);

EMU68_EXTERN
/** Subtraction @return a-b-c */
s32 sub68(emu68_t * const emu68, s32 a, s32 b, s32 c);

EMU68_EXTERN
/** Signed multiplication @return (a>>16)*(b>>16) */
s32 muls68(emu68_t * const emu68, s32 a, s32 b);

EMU68_EXTERN
/** Unsigned multiplication @return (a>>16)*(b>>16) */
s32 mulu68(emu68_t * const emu68, u32 a, u32 b);

EMU68_EXTERN
/** Signed divide @return MSW:a%(b>>16) LSW:a/(b>>16) */
s32 divs68(emu68_t * const emu68, s32 a, s32 b);

EMU68_EXTERN
/** Unsigned divide @return MSW:a%(b>>16) LSW:a/(b>>16) */
s32 divu68(emu68_t * const emu68, u32 a, u32 b);

/** @} */


/** @name Logical instruction functions
 *  @{
 */

EMU68_EXTERN
/** Bitwise AND @return a&b */
s32 and68(emu68_t * const emu68, u32 a, u32 b);

EMU68_EXTERN
/** Bitwise OR @return a|b */
s32 orr68(emu68_t * const emu68, u32 a, u32 b);

EMU68_EXTERN
/** Bitwise exclusif OR @return a^b */
s32 eor68(emu68_t * const emu68, u32 a, u32 b);

EMU68_EXTERN
/** First complement @return ~s */
s32 not68(emu68_t * const emu68, s32 s);

/** @} */

/**
 *  @}
 */

#endif /* #ifndef _INST68_H_ */
