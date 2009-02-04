/**
 * @ingroup   io68_paula_devel
 * @file      io68/paulaemul.h
 * @author    Benjamin Gerard
 * @date      1998/07/18
 * @brief     Paula emulator header.
 *
 * $Id$
 *
 */

/* Copyright (C) 1998-2007 Benjamin Gerard */

#ifndef _IO68_PAULA_EMUL_H_
#define _IO68_PAULA_EMUL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "emu68/struct68.h"

/** @defgroup  io68_paula_devel  Amiga Paula emulator.
 *  @ingroup   io68_devel
 *  @brief     Paula emulator (Amiga soundchip).
 *
 * @par About Amiga hardware registers
 *
 * Amiga hardware registers could have a different address for read
 * and write access. It is the case for DMA control register (DMACON).
 *
 * Registers could be (B)yte or (W)ord wide.
 *
 * @par SET/CLR bit
 *
 * Some hardware registers work with a SET/CLEAR bit. When writing these
 * registers the value is not copied as is. The operation depends on the
 * value of S the most signifiant bit (#15).
 * - If S is SET then all others setted bits are setted (bitwise OR).
 * - If S is CLEAR then all others setted bits are cleared (bitwise NAND).
 *
 * @{ */

/*
 *
 * @par Register map
 *
 * @code {
 *
 * // NAME SZ  ADDR    
 *
 * VHPOSR  (B) DFF006 [xxxxxxxx] Vertical/Horizontal electron bean position.
 *
 * INTREQR (W) DFF01E [-x---DCBA--------] Interrupt request read.
 * INTREQW (W) DFF09C [-x---DCBA--------] Interrupt request write.
 *
 * INTENAR (W) DFF01C [-M---DCBA--------] Interrupt enable read (M=Master).
 * INTENAW (W) DFF09A [-M---DCBA--------] Interrupt enable write (M=Master).
 *
 * DMACONW (W) DFF096 [S00000E00000DBCA] DMA control register write (E=Enable).
 * DMACONR (W) DFF002 [000000E00000DBCA] DMA control register read (E=Enable).
 *
 * VOICEA      DFF0A0
 * VOICEB      DFF0B0
 * VOICEC      DFF0C0
 * VOICED      DFF0D0
 *
 * VOICEX
 * +0 (L) START  [00000000 00000xxx xxxxxxxx xxxxxxx0] start address (even).
 * +4 (W) LENGHT [xxxxxxxx xxxxxxxx] Length in word (0=010000).
 * +6 (W) PERIOD [0000xxxx xxxxxxxx] Period (in paula cycle).
 * +8 (B) VOLUME [0xxxxxxx] Volume [0-64] (presume value > 64 => 64).
 * }
 */


/** Amiga Vertical/Horizontal electron bean position */
#define PAULA_VHPOSR    0x06

/** @name  Amiga interruption registers
 *  @{
 *
 *  All hardware registers involved with interruption handling use the
 *  same bit organisation : 
 *   - bit 7   Audio channel A
 *   - bit 8   Audio channel B
 *   - bit 9   Audio channel C
 *   - bit 10  Audio channel D
 *   - bit 14  Master interrupt
 *
 */

#define PAULA_INTREQR   0x1E  /**< Interruption request read      */
#define PAULA_INTREQRH  0x1E  /**< Interruption request read MSB  */
#define PAULA_INTREQRL  0x1F  /**< Interruption request read LSB  */

#define PAULA_INTREQ    0x9C  /**< Interruption request write     */
#define PAULA_INTREQH   0x9C  /**< Interruption request write MSB */
#define PAULA_INTREQL   0x9D  /**< Interruption request write LSB */

#define PAULA_INTENAR   0x1C  /**< Interruption enable read       */
#define PAULA_INTENARH  0x1C  /**< Interruption enable read MSB   */
#define PAULA_INTENARL  0x1D  /**< Interruption enable read LSB   */

#define PAULA_INTENA    0x9A  /**< Interruption enable write      */
#define PAULA_INTENAH   0x9A  /**< Interruption enable write MSB  */
#define PAULA_INTENAL   0x9B  /**< Interruption enable write LSB  */

/**@}*/


/** @name  Amiga DMA registers.
 *  @{
 *
 *   Amiga DMA control register bits :
 *   - bit 0  Audio DMA channel A
 *   - bit 1  Audio DMA channel B
 *   - bit 2  Audio DMA channel C
 *   - bit 3  Audio DMA channel D
 *   - bit 9  General DMA
 */

#define PAULA_DMACONR   0x02  /**< DMA control read      */
#define PAULA_DMACONRH  0x02  /**< DMA control read MSB  */
#define PAULA_DMACONRL  0x03  /**< DMA control read LSB  */

#define PAULA_DMACON    0x96  /**< DMA control write     */
#define PAULA_DMACONH   0x96  /**< DMA control write MSB */
#define PAULA_DMACONL   0x97  /**< DMA control write LSB */

/**@}*/


/** @name The Audio & Disk Control Registers.
 *  @{
 *
 * Bits:
 * - 07   USE3PN    Use audio channel 3 to modulate nothing.
 * - 06   USE2P3    Use audio channel 2 to modulate period of channel 3.
 * - 05   USE1P2    Use audio channel 1 to modulate period of channel 2.
 * - 04   USE0P1    Use audio channel 0 to modulate period of channel 1.
 * - 03   USE3VN    Use audio channel 3 to modulate nothing.
 * - 02   USE2V3    Use audio channel 2 to modulate volume of channel 3.
 * - 01   USE1V2    Use audio channel 1 to modulate volume of channel 2.
 * - 00   USE0V1    Use audio channel 0 to modulate volume of channel 1.
 *
 * @note   If both period/volume are modulated on the same channel,
 * the period and volume will be alternated. First word xxxxxxxx V6-V0,
 * Second word P15-P0 (etc).
 */

#define PAULA_ADKCON    0x9E  /**< Audio, disk, control write. */
#define PAULA_ADKCONR   0x10  /**< Audio, disk, control read.  */
#define PAULA_ADKCONRH  0x10
#define PAULA_ADKCONRL  0x11

/**@}*/

/** @name  Amiga Paula registers.
 *  @{
 */
#define PAULA_VOICE(I) ((0xA+(I))<<4) /**< Paula channel I register base. */
#define PAULA_VOICEA   0xA0           /**< Paula channel A register base. */
#define PAULA_VOICEB   0xB0           /**< Paula channel B register base. */
#define PAULA_VOICEC   0xC0           /**< Paula channel C register base. */
#define PAULA_VOICED   0xD0           /**< Paula channel D register base. */
/**@}*/


/** @name  Amiga Paula Clock's frequency.
 *  @{
 */
#define PAULA_PAL_FRQ  3546897u      /**< Paula PAL frequency.  */
#define PAULA_NTSC_FRQ 3579545u      /**< Paula NTSC frequency. */
  /**@}*/

/** @name  Internal Paula emulation data.
 *  @{
 */

/** Paula voice data structure. */
typedef struct
{
  addr68_t adr;   /**< current sample counter (<<paula_t::ct_fix) */
  addr68_t start; /**< loop address                               */
  addr68_t end;   /**< end address (<<paula_t::ct_fix)            */
} paulav_t;

/** Paula emulator data structure. */
typedef struct
{
  u8 map[256];        /**< Paula regiters mapping                 */
  paulav_t voice[4];  /**< Paula voices (4 channels) */
  int68_t dmacon;     /**< Shadow DMACON. */
  int68_t intena;     /**< Shadow INTENA. */
  int68_t intreq;     /**< Shadow INTREQ. */
  int68_t adkcon;     /**< Shadow ADKCON. */
  int68_t vhpos;      /**< Shadow VHPOSR.  */

  int68_t emul;       /**< emulator type. @see paula_emul_e. */
  int68_t clock;      /**< clock type. @see paula_clock_e.   */
  uint68_t hz;        /**< Sampling rate in hz,              */
  uint68_t dividand;  /**< (clock<<PAULA_CT_FIX)/hz.         */

  const u8 * mem;     /**< 68K memory buffer.                  */
  int log2mem;        /**< 68K memory buffer size (2^log2mem). */
  int ct_fix;         /**< fixed precision used by counters.   */

} paula_t;

/**@}*/



/** @name  Initialization functions.
 *  @{
 */

/** available emulation modes. */
enum paula_emul_e {
  PAULA_EMUL_DEFAULT = 0, /**< Use default mode.                           */
  PAULA_EMUL_SIMPLE,      /**< Simple emulation without interpolation.     */
  PAULA_EMUL_LINEAR       /**< Simple emulation with linear interpolation. */
};

/** clock type. */
enum paula_clock_e {
  PAULA_CLOCK_DEFAULT = 0,
  PAULA_CLOCK_PAL,
  PAULA_CLOCK_NTSC
};

typedef struct
{
  int68_t  emul;  /**< Default emulation mode. @see paula_emul_e.     */
  uint68_t hz;    /**< Default sampling rate in Hz.                   */
  int68_t  clock; /**< Default clock type. @see paula_clock_e.        */
  int    * argc;  /**< Argument count (before and after).             */
  char  ** argv;  /**< Arguments.                                     */
} paula_parms_t;

/** setup data. */
typedef struct {
  paula_parms_t parms; /**< Paula params.                       */
  const u8 * mem;      /**< 68K memory buffer.                  */
  int log2mem;         /**< 68K memory buffer size (2^log2mem). */
} paula_setup_t;

/** Init paula library.
 *
 *    The paula_init() must be call prior to all other paula functions.
 *
 *  @param   parms  initialisation parameters
 *  @return  error-code (always success)
 *  @return  0  Success
 *  @return  -1 Failure
 *
 *  @see paula_shutdown()
 */
int paula_init(paula_parms_t * const parms);

/** Shutdown paula library.
 *
 *  @see paula_init()
 */
void paula_shutdown(void);


/** Paula hardware reset.
 *
 *    The paula_reset() reset function perform a Paula reset.
 *    It performs following operations :
 *    - all registers zeroed
 *    - all internal voices set to dummy 2 samples len address.
 *    - general DMA enabled
 *    - all audio DMA disabled
 *    - interrupt master enabled
 *    - all audio interrupt disbled
 *
 *    @return error-code (always success)
 *    @return  0  Success
 */
int paula_reset(paula_t * const paula);

/** Paula emulator instance setup.
 *
 *    @return error-code (always success)
 *    @return  0  Success
 *
 *  @see paula_reset()
 */
int paula_setup(paula_t * const paula, paula_setup_t * const setup);

/** Paula emulator instance cleanup.
 */
void paula_cleanup(paula_t * const paula);

/** Change paula emulator parameters.
 */
int paula_configure(paula_t * const paula, paula_parms_t * const parms);

/** Set/Get paula emulator sampling rate.
 */
uint68_t paula_sampling_rate(paula_t * const paula, uint68_t hz);

/** Set paula emulator type. */
int paula_set_emulation(paula_t * const paula, int emul);

/** Set/Get paula clock type.
 */
int paula_clock(paula_t * const paula, int clock);


/**@}*/


/** @name  Emulation functions
 *  @{
 */

/** Execute Paula emulation.
 *
 *    The paula_mix() function processes sample mixing with current internal
 *    parameters for n samples. Mixed samples are stored in a large enough
 *    (at least n) 32 bit pcm buffer pointed by b. mem68 is a pointer to
 *    the 68K memory buffer. The Paula emulator assume that this buffer is
 *    at least the size of the Amiga "chip" RAM. This implies at leat 512Kb
 *    and PCM data must be in the first 512Kb.
 *
 *   @param  paula   Paula emulator instance
 *   @param  splbuf  Destination 32-bit sample buffer
 *   @param  n       Number of sample to mix in splbuf buffer
 *
 */
void paula_mix(paula_t * const paula, s32 * splbuf, int n);

/**@}*/

/**
 *@}
 */


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _IO68_PAULA_EMUL_H_ */
