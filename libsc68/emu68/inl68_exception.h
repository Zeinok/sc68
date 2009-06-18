/**
 * @ingroup   emu68_lib_inline
 * @file      emu68/inl68_exception.h
 * @brief     68k exception header.
 * @date      2009/05/08
 * @author    Benjamin Gerard
 *
 */

/* $Id$ */

#ifndef _EMU68_INL_EXCEPTION68_H_
#define _EMU68_INL_EXCEPTION68_H_

static inline
void inl_exception68(emu68_t * const emu68,
                     const addr68_t vector, const int level)
{
  pushl(REG68.pc);
  pushw(REG68.sr);
  REG68.sr &= 0x70FF;
  REG68.sr |= ( 0x2000 + ( (level&7) <<SR_I_BIT ) );
  REG68.pc  = read_L(vector);
}

static inline
void inl_buserror68(emu68_t * const emu68, int addr, int mode)
{
  inl_exception68(emu68, BUSERROR_VECTOR, BUSERROR_LEVEL);
}

static inline
void inl_linea68(emu68_t * const emu68)
{
  inl_exception68(emu68, LINEA_VECTOR, LINEA_LEVEL);
}

static inline
void inl_linef68(emu68_t * const emu68)
{
  inl_exception68(emu68, LINEF_VECTOR, LINEF_LEVEL);
}

#endif /* #ifndef _EMU68_INL_EXCEPTION68_H_ */
