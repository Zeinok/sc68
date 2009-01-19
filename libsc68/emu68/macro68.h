/**
 * @ingroup   emu68_core_devel
 * @file      emu68/macro68.h
 * @author    Benjamin Gerard
 * @date      1999/13/03
 * @brief     68K instruction emulation macro definitions.
 *
 * $Id$
 *
 */
 
/* Copyright (C) 1998-2007 Benjamin Gerard */

#ifndef _EMU68_MACRO68_H_
#define _EMU68_MACRO68_H_

#include "srdef68.h"
#include "excep68.h"

/** @addtogroup  emu68_core_devel
 *  @brief       68K instruction emulation macro definitions.
 *
 *   A important part of EMU68 instruction emulation is done using
 *   macro in order to :
 *   - generate decent optimized compiled code.
 *   - simplify compilation process and to avoid "C" code generation.
 *
 * @{
 */

/** @name  Cycle counter
 *  @{
 */
#ifndef EMU68CYCLE
# define ADDCYCLE(N)  /**< Dummy internal cycle counter. */
# define SETCYCLE(N)  /**< Dummy internal cycle counter. */
#else
# define ADDCYCLE(N) emu68->cycle += (N) /**< Intermal cycle counter. */
# define SETCYCLE(N) emu68->cycle  = (N) /**< internal cycle counter. */
#endif
/**@}*/

/** @name  Exception handling
 *  @{
 */

/** General exception or interruption. */
#define EXCEPTION(VECTOR,LVL) \
do { \
  pushl(REG68.pc); pushw(REG68.sr); \
  REG68.sr &= 0x70FF; \
  REG68.sr |= (0x2000+(((LVL)&7)<<SR_I_BIT)); \
  REG68.pc  = read_L(VECTOR); \
} while(0)

/** Illegal instruction. */
#define ILLEGAL \
do {\
  emu68_error_add(emu68,"Illegal pc:%06x",REG68.pc); \
  EXCEPTION(ILLEGAL_VECTOR,ILLEGAL_LVL); \
} while (0)

/** Bus error exception. */
#define BUSERROR(ADDR,MODE) \
do {\
  emu68_error_add(emu68,"bus error pc:%06x addr:%06x (%c)",\
  REG68.pc,ADDR,MODE?'W':'R');\
  EXCEPTION(BUSERROR_VECTOR,BUSERROR_LVL) \
} while(0)

/** Line A exception. */
#define LINEA EXCEPTION(LINEA_VECTOR,LINEA_LVL)

/** Line F exception. */
#define LINEF EXCEPTION(LINEF_VECTOR,LINEF_LVL)

/** TRAPV exception. */
#define TRAPV \
do { if (REG68.sr&SR_V) EXCEPTION(TRAPV_VECTOR,TRAPV_LVL); } while(0)

/** TRAP exception. */
#define TRAP(TRAP_N) EXCEPTION(TRAP_VECTOR(TRAP_N),TRAP_LVL)

/** CHK exception. */
#define CHK EXCEPTION(CHK_VECTOR,CHK_LVL)

/** CHKW exception. */
#define CHKW(CHK_A,CHK_B) \
do { if ((CHK_B)<0 || (CHK_B)>(CHK_A)) { CHK; } } while(0)

/**@}*/


/** @name  Program control instructions
 *  @{
 */

/** No Operation. */
#define NOP while(0)

/** Soft reset. */
#define RESET emu68_reset(emu68)

/** STOP
 *
 *  @warning : Partially handled : only move value to SR
 */
#define STOP REG68.sr = (u16)get_nextw()/* ; emu68->status = 1; */

/** Return from subroutine. */
#define RTS  REG68.pc = popl()

/** Return from exception. */
#define RTE  REG68.sr = popw(); RTS

/** Return from exception restore CCR only. */
#define RTR \
do {\
 REG68.sr = (REG68.sr&0xFF00) | (u8)popw();\
 RTS;\
} while(0)

/**@}*/


/** @name  Miscellaneous instructions
 *  @{
 */

/** @todo Binary coded decimal sign change. */
#define NBCDB(NBCD_S,NBCD_A) (NBCD_S)=(NBCD_A)

/** Register MSW/LSW exchange. */
#define EXG(A,B) do { (A)^=(B); (B)^=(A); (A)^=(B); } while(0)

/** Byte to word sign extension. */
#define EXTW(D)  (D) = ((D)&0xFFFF0000) | ((u16)(s32)(s8)(D))

/** Word to long sign extension. */
#define EXTL(D) (D) = (s32)(s16)(D)

/** Test and set (mutual exclusion). */
#define TAS(TAS_A) do { TSTB(TAS_A,TAS_A); (TAS_A) |= 0x80000000; } while(0)

/** Generic clear memory or register. */
#define CLR(CLR_S,CLR_A) \
do {\
  (CLR_A) = (CLR_A);\
  REG68.sr =(REG68.sr&~(SR_N|SR_V|SR_C)) | SR_Z;\
  CLR_S = 0;\
} while (0)

/** Byte memory or register clear. */
#define CLRB(A,B) CLR(A,B)

/** Word memory or register clear. */
#define CLRW(A,B) CLR(A,B)

/** Long memory or register clear. */
#define CLRL(A,B) CLR(A,B)

/** Link (frame pointer). */
#define LINK(R_LNK) \
do {\
  pushl(REG68.a[R_LNK]); \
  REG68.a[R_LNK] = REG68.a[7]; \
  REG68.a[7] += get_nextw(); \
} while(0)

/** UNLK (frame pointer). */
#define UNLK(R_LNK) \
do {\
  REG68.a[7]=REG68.a[R_LNK]; \
  REG68.a[R_LNK]=popl(); \
} while(0)

/** Register value swapping. */
#define SWAP(SWP_A) \
do { \
  (SWP_A) = ((u32)(SWP_A)>>16) | ((SWP_A)<<16); \
  REG68.sr = (REG68.sr&~(SR_V|SR_C|SR_Z|SR_N)) | \
             ((!(SWP_A))<<SR_Z_BIT) | \
             (((s32)(SWP_A)>>31)&SR_N); \
} while (0)

/**@}*/


/** @name  Bit instructions
 *  @{
 */

#if 0
/** Bit test and set. */
#define BTST(V,BIT) \
        REG68.sr = (REG68.sr&(~SR_Z)) | ((((V)&(1<<(BIT)))==0)<<SR_Z_BIT)

/** Bit set. */
#define BSET(V,BIT) BTST(V,BIT); (V) |= (1<<(BIT));

/** Bit clear. */
#define BCLR(V,BIT) BTST(V,BIT); (V) &= ~(1<<(BIT));

/** Bit change. */
#define BCHG(V,BIT) BTST(V,BIT); (V) ^= (1<<(BIT));
*/
#endif

/** Bit test and set. */
#define BTST(V,BIT) \
REG68.sr = (REG68.sr&(~SR_Z)) | (((((V)>>(BIT))&1)^1)<<SR_Z_BIT)

/** Bit set. */
#define BSET(V,BIT) \
if( (V)&(1<<(BIT)) ) { REG68.sr &= ~SR_Z; }\
else do { (V) |= 1<<(BIT); REG68.sr |= SR_Z; } while(0)

/** Bit clear. */
#define BCLR(V,BIT) \
if( (V)&(1<<(BIT)) ) { (V) &= ~(1<<(BIT)); REG68.sr &= ~SR_Z; }\
else REG68.sr |= SR_Z

/** Bit change. */
#define BCHG(V,BIT) \
if( (V)&(1<<(BIT)) ) { (V) &= ~(1<<(BIT)); REG68.sr &= ~SR_Z; }\
else do { (V) |= 1<<(BIT); REG68.sr |= SR_Z; } while(0)

/**@}*/


/** @name  Move & test instructions
 *  @{
 */

#define MOVE(MOV_A) REG68.sr \
= (REG68.sr&(0xFF00|SR_X))\
|(((MOV_A)==0)<<SR_Z_BIT)\
|(((int68_t)(MOV_A)>>(sizeof(int68_t)*8-1))&SR_N)

#define TST(TST_V) MOVE(TST_V)
#define TSTB(TST_S,TST_A) do { TST_S=TST_A; TST(TST_S); } while (0)
#define TSTW(TST_S,TST_A) do { TST_S=TST_A; TST(TST_S); } while (0)
#define TSTL(TST_S,TST_A) do { TST_S=TST_A; TST(TST_S); } while (0)

/**@}*/


/** @name  Multiply & Divide instructions
 *  @{
 */

/** Signed multiplication. */
#define MULSW(MUL_S, MUL_A, MUL_B) MUL_S = muls68(emu68, MUL_A, MUL_B)

/** Unsigned multiplication. */
#define MULUW(MUL_S, MUL_A, MUL_B) MUL_S = mulu68(emu68, MUL_A, MUL_B)

/** Signed divide. */
#define DIVSW(DIV_S, DIV_A, DIV_B) DIV_S = divs68(emu68, DIV_A, DIV_B)

/** Unsigned divide. */
#define DIVUW(DIV_S, DIV_A, DIV_B) DIV_S = divu68(emu68, DIV_A, DIV_B)

/**@}*/


/** @name  Logical instructions
 *  @{
 */

/** Generic bitwise AND. */
#define AND(AND_S, AND_A, AND_B) AND_S = and68(emu68, AND_A, AND_B)

/** Byte bitwise AND. */
#define ANDB(AND_S, AND_A, AND_B) AND(AND_S, AND_A, AND_B)

/** Word bitwise AND. */
#define ANDW(AND_S, AND_A, AND_B) AND(AND_S, AND_A, AND_B)

/** Long bitwise AND. */
#define ANDL(AND_S, AND_A, AND_B) AND(AND_S, AND_A, AND_B)


/** Generic bitwise OR. */
#define ORR(ORR_S, ORR_A, ORR_B) ORR_S = orr68(emu68, ORR_A, ORR_B)

/** Byte bitwise OR. */
#define ORB(ORR_S, ORR_A, ORR_B) ORR(ORR_S, ORR_A, ORR_B)

/** Word bitwise OR. */
#define ORW(ORR_S, ORR_A, ORR_B) ORR(ORR_S, ORR_A, ORR_B)

/** Long bitwise OR. */
#define ORL(ORR_S, ORR_A, ORR_B) ORR(ORR_S, ORR_A, ORR_B)


/** Generic bitwise EOR (exclusive OR). */
#define EOR(EOR_S, EOR_A, EOR_B) EOR_S = eor68(emu68, EOR_A, EOR_B)

/** Byte bitwise EOR (exclusif OR). */
#define EORB(EOR_S, EOR_A, EOR_B) EOR(EOR_S, EOR_A, EOR_B)

/** Word bitwise EOR (exclusif OR). */
#define EORW(EOR_S, EOR_A, EOR_B) EOR(EOR_S, EOR_A, EOR_B)

/** Long bitwise EOR (exclusif OR). */
#define EORL(EOR_S, EOR_A, EOR_B) EOR(EOR_S, EOR_A, EOR_B)


/** Generic first complement. */
#define NOT(NOT_S,NOT_A) NOT_S = not68(emu68, NOT_A)

/** Byte first complement. */
#define NOTB(A,B) NOT(A,B)

/** Word first complement. */
#define NOTW(A,B) NOT(A,B)

/** Long first complement. */
#define NOTL(A,B) NOT(A,B)

/** @name  Arithmetic instructions
 *  @{
 */

#define ADD(ADD_S,ADD_A,ADD_B,ADD_X) ADD_S=add68(emu68,ADD_A,ADD_B,ADD_X)
#define SUB(SUB_S,SUB_A,SUB_B,SUB_X) SUB_S=sub68(emu68,SUB_B,SUB_A,SUB_X)
#define CMP(SUB_A,SUB_B)                   sub68(emu68,SUB_B,SUB_A,0)

#define ADDB(ADD_S, ADD_A, ADD_B) ADD(ADD_S, ADD_A, ADD_B,0)
#define ADDW(ADD_S, ADD_A, ADD_B) ADD(ADD_S, ADD_A, ADD_B,0)
#define ADDL(ADD_S, ADD_A, ADD_B) ADD(ADD_S, ADD_A, ADD_B,0)
#define ADDXB(ADD_S, ADD_A, ADD_B) \
        ADD(ADD_S, ADD_A, ADD_B, (REG68.sr&SR_X)<<(24-SR_X_BIT))
#define ADDXW(ADD_S, ADD_A, ADD_B) \
        ADD(ADD_S, ADD_A, ADD_B, (REG68.sr&SR_X)<<(16-SR_X_BIT))
#define ADDXL(ADD_S, ADD_A, ADD_B) \
        ADD(ADD_S, ADD_A, ADD_B, (REG68.sr&SR_X)>>SR_X_BIT )

#define ADDA(ADD_S, ADD_A, ADD_B) (ADD_S) = (ADD_A) + (ADD_B)
#define ADDAW(ADD_S, ADD_A, ADD_B) ADDA(ADD_S, ADD_A>>16, ADD_B)
#define ADDAL(ADD_S, ADD_A, ADD_B) ADDA(ADD_S, ADD_A, ADD_B)

#define SUBB(SUB_S, SUB_A, SUB_B) SUB(SUB_S, SUB_A, SUB_B,0)
#define SUBW(SUB_S, SUB_A, SUB_B) SUB(SUB_S, SUB_A, SUB_B,0)
#define SUBL(SUB_S, SUB_A, SUB_B) SUB(SUB_S, SUB_A, SUB_B,0)

#define SUBXB(SUB_S, SUB_A, SUB_B) \
        SUB(SUB_S, SUB_A, SUB_B, (REG68.sr&SR_X)<<(24-SR_X_BIT))
#define SUBXW(SUB_S, SUB_A, SUB_B) \
        SUB(SUB_S, SUB_A, SUB_B, (REG68.sr&SR_X)<<(16-SR_X_BIT))
#define SUBXL(SUB_S, SUB_A, SUB_B) \
        SUB(SUB_S, SUB_A, SUB_B, (REG68.sr&SR_X)>>SR_X_BIT)

#define SUBA(SUB_S, SUB_A, SUB_B) (SUB_S) = (SUB_B) - (SUB_A)
#define SUBAW(SUB_S, SUB_A, SUB_B) \
do {\
  int68_t ZOB = (SUB_A)>>16;\
  SUBA(SUB_S, ZOB, SUB_B);\
} while(0)
#define SUBAL(SUB_S, SUB_A, SUB_B) SUBA(SUB_S, SUB_A, SUB_B)

#define CMPB(CMP_A, CMP_B) CMP(CMP_A, CMP_B)
#define CMPW(CMP_A, CMP_B) CMP(CMP_A, CMP_B)
#define CMPL(CMP_A, CMP_B) CMP(CMP_A, CMP_B)
#define CMPA(CMP_A, CMP_B) CMP(CMP_A, CMP_B)
#define CMPAW(CMP_A, CMP_B) \
do {\
  int68_t ZOB = (CMP_A)>>16;\
  CMPA( ZOB, CMP_B);\
} while(0)
#define CMPAL(CMP_A, CMP_B) CMP(CMP_A, CMP_B)

#define NEGB(NEG_S,NEG_A) SUBB(NEG_S,NEG_A,0)
#define NEGW(NEG_S,NEG_A) SUBW(NEG_S,NEG_A,0)
#define NEGL(NEG_S,NEG_A) SUBL(NEG_S,NEG_A,0)

#define NEGXB(NEG_S,NEG_A) SUBXB(NEG_S,NEG_A,0)
#define NEGXW(NEG_S,NEG_A) SUBXW(NEG_S,NEG_A,0)
#define NEGXL(NEG_S,NEG_A) SUBXL(NEG_S,NEG_A,0)

/**@}*/


/** @name   Logical & Arithmetic bit shifting instructions
 *  @{
 */

/** generic right shift. */
#define LSR(LSR_A,LSR_D,LSR_MSK,LSR_C) \
do {\
  REG68.sr &= 0xFF00;\
  if((LSR_D)!=0) {\
    ADDCYCLE(2*(LSR_D));\
    (LSR_A) >>= (LSR_D)-1;\
    if((LSR_A)&(LSR_C)) REG68.sr |= SR_X | SR_C;\
    (LSR_A)>>=1;\
  }\
  (LSR_A) &= (LSR_MSK);\
  REG68.sr |= (((LSR_A)==0)<<SR_Z_BIT) | (((s32)(LSR_A)<0)<<SR_N_BIT);\
} while(0)

/** Byte logical right shift. */
#define LSRB(LSR_A,LSR_B) LSR(LSR_A,LSR_B,0xFF000000,(1<<24))

/** Word logical right shift. */
#define LSRW(LSR_A,LSR_B) LSR(LSR_A,LSR_B,0xFFFF0000,(1<<16))

/** Long logical right shift. */
#define LSRL(LSR_A,LSR_B) LSR(LSR_A,LSR_B,0xFFFFFFFF,(1<<0))

/** Byte arithmetic right shift. */
#define ASRB(LSR_A,LSR_B) LSR(LSR_A,LSR_B,0xFF000000,(1<<24))

/** Word arithmetic right shift. */
#define ASRW(LSR_A,LSR_B) LSR(LSR_A,LSR_B,0xFFFF0000,(1<<16))

/** Long arithmetic right shift. */
#define ASRL(LSR_A,LSR_B) LSR(LSR_A,LSR_B,0xFFFFFFFF,(1<<0))

/** Generic left shift. */
#define LSL(LSL_A,LSL_D,LSL_MSK) \
do {\
  REG68.sr &= 0xFF00;\
  if((LSL_D)!=0) {\
    ADDCYCLE(2*(LSL_D));\
    (LSL_A) <<= (LSL_D)-1;\
    if((LSL_A)&0x80000000) REG68.sr |= SR_X | SR_C;\
    (LSL_A)<<=1;\
  }\
  (LSL_A) &= (LSL_MSK);\
  REG68.sr |= (((LSL_A)==0)<<SR_Z_BIT) | (((s32)(LSL_A)<0)<<SR_N_BIT);\
} while(0)

/** Byte logical left shift. */
#define LSLB(LSL_A,LSL_B) LSL(LSL_A,LSL_B,0xFF000000)

/** Word logical left shift. */
#define LSLW(LSL_A,LSL_B) LSL(LSL_A,LSL_B,0xFFFF0000)

/** Long logical left shift. */
#define LSLL(LSL_A,LSL_B) LSL(LSL_A,LSL_B,0xFFFFFFFF)

/** Byte arithmetic left shift. */
#define ASLB(LSL_A,LSL_B) LSL(LSL_A,LSL_B,0xFF000000)

/** Word arithmetic left shift. */
#define ASLW(LSL_A,LSL_B) LSL(LSL_A,LSL_B,0xFFFF0000)

/** Long arithmetic left shift. */
#define ASLL(LSL_A,LSL_B) LSL(LSL_A,LSL_B,0xFFFFFFFF)

/** Generic right rotation. */
#define ROR(ROR_A,ROR_D,ROR_MSK,ROR_SZ) \
do {\
  REG68.sr &= 0xFF00 | SR_X;\
  if((ROR_D)!=0) {\
    ADDCYCLE(2*(ROR_D));\
    ROR_D &= (ROR_SZ)-1;\
    if((ROR_A)&(1<<((ROR_D)-1+32-(ROR_SZ)))) REG68.sr |= SR_C;\
    (ROR_A) &= (ROR_MSK);\
    (ROR_A) = ((ROR_A)>>(ROR_D)) + ((ROR_A)<<((ROR_SZ)-(ROR_D)));\
  }\
  (ROR_A) &= (ROR_MSK);\
  REG68.sr |= (((ROR_A)==0)<<SR_Z_BIT) | (((s32)(ROR_A)<0)<<SR_N_BIT);\
} while(0)

/** Generic left rotation. */
#define ROL(ROR_A,ROR_D,ROR_MSK,ROR_SZ) \
do {\
  REG68.sr &= 0xFF00 | SR_X;\
  if((ROR_D)!=0) {\
    ADDCYCLE(2*(ROR_D));\
    ROR_D &= (ROR_SZ)-1;\
    if ((ROR_A)&(1<<(32-(ROR_D)))) REG68.sr |= SR_C;\
    (ROR_A) &= (ROR_MSK);\
    (ROR_A) = ((ROR_A)<<(ROR_D)) + ((ROR_A)>>((ROR_SZ)-(ROR_D)));\
  }\
  (ROR_A) &= (ROR_MSK);\
  REG68.sr |= (((ROR_A)==0)<<SR_Z_BIT) | (((s32)(ROR_A)<0)<<SR_N_BIT);\
} while(0)

#warning "fix mask and shift here"
#define RORB(ROR_A,ROR_B) ROR(ROR_A,ROR_B,0xFF000000,8)
#define RORW(ROR_A,ROR_B) ROR(ROR_A,ROR_B,0xFFFF0000,16)
#define RORL(ROR_A,ROR_B) ROR(ROR_A,ROR_B,0xFFFFFFFF,32)
#define ROLB(ROR_A,ROR_B) ROL(ROR_A,ROR_B,0xFF000000,8)
#define ROLW(ROR_A,ROR_B) ROL(ROR_A,ROR_B,0xFFFF0000,16)
#define ROLL(ROR_A,ROR_B) ROL(ROR_A,ROR_B,0xFFFFFFFF,32)

/** Generic right extend-bit rotation. */
#define ROXR(ROR_A,ROR_D,ROR_MSK,ROR_SZ) \
do {\
  uint68_t ROR_X = (REG68.sr>>SR_X_BIT)&1;\
  REG68.sr &= 0xFF00;\
  if((ROR_D)!=0) {\
    ADDCYCLE(2*(ROR_D));\
    ROR_D &= (ROR_SZ)-1;\
    if((ROR_A)&(1<<((ROR_D)-1+32-(ROR_SZ)))) REG68.sr |= SR_C | SR_X;\
    (ROR_A) &= (ROR_MSK);\
    (ROR_A) = ((ROR_A)>>(ROR_D)) + ((ROR_A)<<((ROR_SZ)-(ROR_D)+1));\
    (ROR_A) |= (ROR_X)<<(32-(ROR_D));\
  }\
  (ROR_A) &= (ROR_MSK);\
  REG68.sr |= (((ROR_A)==0)<<SR_Z_BIT) | (((s32)(ROR_A)<0)<<SR_N_BIT);\
} while(0)

/** Generic left extend-bit rotation. */
#define ROXL(ROR_A,ROR_D,ROR_MSK,ROR_SZ) \
do {\
  uint68_t ROR_X = (REG68.sr>>SR_X_BIT)&1;\
  REG68.sr &= 0xFF00;\
  if((ROR_D)!=0) {\
    ADDCYCLE(2*(ROR_D));\
    ROR_D &= (ROR_SZ)-1;\
    if((ROR_A)&(1<<(32-(ROR_D)))) REG68.sr |= SR_C | SR_X ;\
    (ROR_A) &= (ROR_MSK);\
    (ROR_A) = ((ROR_A)<<(ROR_D)) + ((ROR_A)>>((ROR_SZ)-(ROR_D)+1));\
    (ROR_A) |= (ROR_X)<<((ROR_D)-1+(32-(ROR_SZ)));\
  }\
  (ROR_A) &= (ROR_MSK);\
  REG68.sr |= (((ROR_A)==0)<<SR_Z_BIT) | (((s32)(ROR_A)<0)<<SR_N_BIT);\
} while(0)


#define ROXRB(ROR_A,ROR_B) ROXR(ROR_A,ROR_B,0xFF000000,8)
#define ROXRW(ROR_A,ROR_B) ROXR(ROR_A,ROR_B,0xFFFF0000,16)
#define ROXRL(ROR_A,ROR_B) ROXR(ROR_A,ROR_B,0xFFFFFFFF,32)
#define ROXLB(ROR_A,ROR_B) ROXL(ROR_A,ROR_B,0xFF000000,8)
#define ROXLW(ROR_A,ROR_B) ROXL(ROR_A,ROR_B,0xFFFF0000,16)
#define ROXLL(ROR_A,ROR_B) ROXL(ROR_A,ROR_B,0xFFFFFFFF,32)

/**@}*/

/**
 *@}
 */

#endif /* #ifndef _EMU68_MACRO68_H_ */
