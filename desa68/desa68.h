/**
 * @ingroup   lib_desa68
 * @file      desa68.h
 * @brief     Motorola 68K disassembler header.
 * @author    Benjamin Gerard
 * @date      1999-03-17
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef DESA68_H
#define DESA68_H

#ifndef DESA68_EXTERN
# ifdef __cplusplus
#   define DESA68_EXTERN extern "C"
# else
#   define DESA68_EXTERN extern
# endif
#endif

#ifndef DESA68_API
/* Building */
# ifdef DESA68_EXPORT
#  if defined(DLL_EXPORT) && defined(HAVE_DECLSPEC)
#   define DESA68_API __declspec(dllexport)
#  elif defined(HAVE_VISIBILITY)
#   define DESA68_API DESA68_EXTERN __attribute__ ((visibility("default")))
#  else
#   define DESA68_API DESA68_EXTERN
#  endif
/* Using */
# else
#  if defined(DESA68_DLL)
#   define DESA68_API __declspec(dllimport)
#  else
#   define DESA68_API DESA68_EXTERN
#  endif
# endif
#endif

/**
 * @defgroup lib_desa68 desa68 library
 * @ingroup  sc68_dev_lib
 *
 * desa68 is a standalone library to disassemble 68000 machine code.
 *
 * Additionally desa68 classes instructions by category; along with it
 * determines possible program branch. Together with it helps to
 * source machine code.
 *
 * Optionnally the disassembler may disassemble with symbol instead of
 * absolute address or long immediat. A supplemental control is
 * available to choose the range of address that must be disassembled
 * as symbol.
 *
 * A complete example of how to use it may be found in the @ref
 * prg_sourcer68 sourcer68 directory. This library is also used by the
 * deprecated @ref prg_debug68 debug68 and @ref prg_cdbg68 cdbg68
 * programs.
 *
 * @{
 */

#ifndef DESA68_API
# error "DESA68_API should be defined"
#endif

/**
 * Disassembly option flags.
 * @anchor desa68_opt_flags
 *
 * Use bitwise OR operation with these values to set the the
 * desa68_parm_t::flags in order to configure the disassembler.
 *
 */
enum {
  /**
   * Disassemble with symbol.
   *
   * If the DESA68_SYMBOL_FLAG is set in the desa68_parm_t::flags and
   * the value of absolute long addressing mode or an immediat long is
   * in greater or equal to desa68_parm_t::immsym_min and less than
   * desa68_parm_t::immsym_max then the disassembler replaces the
   * value by a named symbol.  The named symbol constist on the value
   * transformed to an 6 hexadecimal digit number with a prefixed 'L'.
   */
  DESA68_SYMBOL_FLAG = (1<<0),

  /**
   * Disassemble with ASCII chars.
   *
   * If the DESA68_ASCII_FLAG is set in the desa68_parm_t::flags
   * immediat values are converted to a tring string if its byte are
   * in a special set including alpha-numeric, space and a few other
   * chars [-_ !.#].
   *
   * @warning The name of this flag is misleading.
   *
   */
  DESA68_ASCII_FLAG = (1<<1),

  /**
   * Disassemble with alpha-numerical chars.
   *
   * If the DESA68_ALNUM_FLAG is set in the desa68_parm_t::flags
   * immediat values are converted to a string if its bytes are in the
   * alpha-numeric range [0-9A-za-z].
   */
  DESA68_ALNUM_FLAG = (1<<2),

  /**
   * Disassemble with graphical chars [32-126].
   *
   * If the DESA68_GRAPH_FLAG is set in the desa68_parm_t::flags
   * immediat values are converted to a string if its bytes are in
   * the range 32 to 126.
   *
   */
  DESA68_GRAPH_FLAG = DESA68_ASCII_FLAG|DESA68_ALNUM_FLAG,

  /**
   * Force symbol disassemble.
   *
   * The DESA68_FORCESYMB_FLAG is a set of 5 bits [8-12]. If the N-th
   * bit is set it forces a symbolic dissassembly for a long starting
   * at the Nth word.  Since 68000 instructions are not more than 10
   * bytes long 5 bit is just enough.
   */
  DESA68_FORCESYMB_FLAG = (1<<8),
};

/**
 * Instruction types
 * @anchor desa68_inst_types
 *
 * These values are used by the desa68_t::itype to help determine what
 * kind of instruction was disassenbled.
 *
 */
enum {
  /** Invalid instruction (disassembled as data). */
  DESA68_DCW = 0,

  /** Generic instruction. */
  DESA68_INS,

  /** Branch always instruction (bra/jmp). */
  DESA68_BRA,

  /** Subroutine (bsr/jsr) or Conditionnal branch instruction (bcc/dbcc). */
  DESA68_BSR,

  /** Return from subroutine/Interruption instruction (rts/rte). */
  DESA68_RTS,

  /** Software interrupt instruction (trap/chk). */
  DESA68_INT,

  /** Nop instruction. */
  DESA68_NOP
};

/**
 * Error flags.
 * @anchor desa68_error_flags
 */
enum {
  /** Output writer failed. */
  DESA68_ERR_OUT  = 1,

  /** Attempt to read word or long at odd address. */
  DESA68_ERR_ODD  = 2,

  /** Memory access failed. */
  DESA68_ERR_MEM  = 4,

  /** Effectve address set. */
  DESA68_ERR_EA   = 8,
};

/**
 * Operand types/
 * @anchor desa68_op_types
 */
enum {
  DESA68_OP_UND,           /**< Operand is not defined. */
  DESA68_OP_IMP,           /**< Operand is implicite. */
  DESA68_OP_REG,           /**< Operand is a register. */
  DESA68_OP_MVM,           /**< Operand is register list (movem). */
  DESA68_OP_IMM,           /**< Operand is an immediat value. */
  DESA68_OP_ABS,           /**< Operand is an absolute address. */
  DESA68_OP_MEM,           /**< Operand is memory indexed by registers. */
};

enum {
  DESA68_SZ_BYTE,
  DESA68_SZ_WORD,
  DESA68_SZ_LONG,
  DESA68_SZ_QUAD,
  DESA68_SZ_IMPL = 254,
  DESA68_SZ_UDEF = 255
};

/**
 * 68k registers identifier (8bit).
 *
 * @warning changing the order might require modifications in the code.
 */
enum {
  DESA68_REG_DN  = 0,                 /**< First data register. */
  DESA68_REG_AN  = 8,                 /**< First address register. */
  DESA68_REG_SP  = 15,                /**< Stack pointer.  */
  DESA68_REG_USP,                     /**< User stack pointer. */
  DESA68_REG_CCR,                     /**< Code condition register. */
  DESA68_REG_SR,                      /**< Status register. */
  DESA68_REG_PC,                      /**< Program counter. */

  DESA68_REG_LAST,                    /**< Not real registers. */
  DESA68_REG_SREF, /**< used to signal that desa68_t::srcref is set. */
  DESA68_REG_DREF  /**< used to signal that desa68_t::dstref is set. */
};

enum {
  DESA68_INVALID_ADDR = -1
};

/**
 * type for the 68K disassemble pass parameters structure.
 */
typedef struct desa68_parm_s desa68_t;

/**
 * 68K disassemble pass parameters.
 *
 * The desa68_parm_t data structure contains the information necessary to
 * disassemble 68K instructions.
 *
 * There are 3 categories of fields in this structure.
 * - Input parameters; Must be set before calling the desa68() function.
 * - Output parameters; Information on disassembled instruction
 *                          filled by desa68() function.
 * - Miscellaneous internal fields.
 *
 * @note The desa68_parm_t::pc field is both input and output since it is use
 *       to set the address of the instruction to decode and returns with
 *       the value of the next one.
 */
struct desa68_parm_s
{

  /**
   * @name Input parameters.
   *
   * These parameters must be set before calling the desa68() functions.
   *
   * @{
   */

  /** User private data (cookie). */
  void          *user;

  /** Function to read memory (or null) */
  int          (*memget)(desa68_t *, unsigned int addr);
  /** Base of 68K memory. */
  unsigned char *mem;
  /** Size of memory - 1 (mask). */
  unsigned int   memsize;
  /** 68k memory addressing space (normally 24 bits). */
  unsigned int   memmask;

  /**
   * Address (Offset in mem) of instruction to disassemble; Returns
   * with the address of the next instruction.
   */
  unsigned int   pc;
  int            flags;  /**< @ref desa68_opt_flags "Disassemble options" */

  /**
   * A function to test for graphical chars. If not set (null) an
   * appropriate function is selected according to desa68_t::flags
   * DESA68_GRAPH_FLAG family flags.
   */
  int (*ischar)(desa68_t *, int);

  /** Writer function (0:use default writer). */
  void          (*strput)(desa68_t *, int);
  /** String used by default writer (can be 0) */
  char          *str;
  /** Size of the buffer pointed by str (forced to 0 on null pointer). */
  int            strmax;

  /**
   * Minimum value to interpret long immediat or absolute long as symbol.
   * @see DESA68_SYMBOL_FLAG for more details
   * @see immsym_max
   */
  unsigned int   immsym_min;
  /**
   * Maximum value to interpret long immediat or absolute long as symbol.
   * @see DESA68_SYMBOL_FLAG for more details
   * @see immsym_min
   */
  unsigned int   immsym_max;

  /**
   * @}
   */


  /**
   * @name Output parameters.
   *
   * These parameters are setted by the desa68() functions.
   * @{
   */

  unsigned int  regs;   /**< used registers bit mask.                 */
  unsigned int  sref;   /**< src operand reference address.           */
  unsigned int  dref;   /**< dst operand reference address.           */
  unsigned char itype;  /**< Instruction type. @see desa68_inst_types */
  unsigned char opsz;   /**< operands size.                           */
  unsigned char error;  /**< Error flags. @see desa68_error_flags     */

  /**
   * Branch or interrupt vector address.
   *
   * If the dissassembled instruction was a branch a call or a
   * sotfware interrupt the desa68_parm_t::branch is set to the jump
   * address or the interrupt vector involved.
   *
   * @see status for more information on instruction type.
   */
  unsigned int _branch;


  /**
   * Number of char output.
   */
  unsigned int out;

  /**
   * @}
   */


  /**
   * @name Miscellaneous internal variables.
   * @internal
   * @{
   */

  /**
   * Intermediat opcode decoding.
   */
  unsigned int  _pc;       /**< pc origin at start of disassembly. */
  unsigned int  _ea;       /**< temporarly decoded effective address. */
  unsigned int  _chsz;     /**< holds '.[BWL]'  */
  signed   int  _w;        /**< Last decoded word (sign extended).  */

  unsigned char _reg0;     /**< bit 2:0 of opcode word. */
  unsigned char _mode3;    /**< bit 5:3 of opcode word. */
  unsigned char _opsz;     /**< bit 7:6 of opcode word. */
  unsigned char _mode6;    /**< bit 8:6 of opcode word. */
  unsigned char _reg9;     /**< bit 11:9 of opcode word. */
  unsigned char _line;     /**< bit 15:12 of opcode word. */
  unsigned char _adrm0;    /**< _mode3 / _reg0 union. */
  unsigned char _adrm6;    /**< _mode6 / _reg9 union. */

  /**
   * @}
   */

};

DESA68_API
/**
 * Disassemble a single 68000 instruction.
 *
 * @param  d  Pointer to disassemble pass parameter structure.
 */
void desa68(desa68_t *d);


DESA68_API
/**
 * Get version number.
 *
 * @return X*100+Y*10+Z.
 */
int desa68_version(void);


DESA68_API
/**
 * Get version string.
 *
 * @return version string.
 */
const char * desa68_versionstr(void);

/**
 * @}
 */

#endif
