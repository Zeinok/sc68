/**
 * @ingroup   io68_mw_devel
 * @file      io68/mwemul.h
 * @author    Benjamin Gerard
 * @date      1999/03/20
 * @brief     STE sound emulator header.
 *
 * $Id$
 */

/* Copyright (C) 1998-2007 Benjamin Gerard */

#ifndef _MWEMUL_H_
#define _MWEMUL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "emu68/type68.h"

/** @defgroup  io68_mw_devel  STE sound emulator
 *  @ingroup   io68_devel
 *  @brief     STE (MicroWire/LMC/DMA) emulator.
 *  
 *  @{
 */

/** @name  Micro-Wire registers
 *  @{
 */

#define MW_ACTI 0x01        /**< Microwire enabled */

#define MW_BASH 0x03        /**< Microwire sample start address, bit 16-23 */
#define MW_BASM (MW_BASH+2) /**< Microwire sample start address, bit 8-15  */
#define MW_BASL (MW_BASH+4) /**< Microwire sample start address, bit 0-7   */

#define MW_CTH 0x09         /**< Microwire sample counter, bit 16-23 */
#define MW_CTM (MW_CTH+2)   /**< Microwire sample counter, bit 8-15  */
#define MW_CTL (MW_CTH+4)   /**< Microwire sample counter, bit 0-7   */

#define MW_ENDH 0x0f        /**< Microwire sample end address, bit 16-23 */
#define MW_ENDM (MW_ENDH+2) /**< Microwire sample end address, bit 8-15  */
#define MW_ENDL (MW_ENDH+4) /**< Microwire sample end address, bit 0-7   */

#define MW_MODE 0x21        /**< Microwire playing mode     */

#define MW_DATA 0x22        /**< Microwire data register    */
#define MW_CTRL 0x24        /**< Microwire control register */

/**@}*/


/** @name  Micro-Wire internal data
 *  @{
 */

/** available emulation  modes. */
enum mw_emul_e {
  MW_EMUL_DEFAULT = 0, /**< Use default mode.                           */
  MW_EMUL_SIMPLE,      /**< Simple emulation without interpolation.     */
  MW_EMUL_LINEAR       /**< Simple emulation with linear interpolation. */
};

/** init parameters. */
typedef struct
{
  int68_t  emul;  /**< Default interpolation mode. @see mw_emul_e. */
  uint68_t hz;    /**< Default sampling rate in Hz.                */
  int    * argc;  /**< Argument count (before and after).          */
  char  ** argv;  /**< Arguments.                                  */
} mw_parms_t;

/** setup data. */
typedef struct {
  mw_parms_t parms; /**< Mw_ params.                       */
  u8 * mem;         /**< 68K memory buffer.                  */
  int log2mem;      /**< 68K memory buffer size (2^log2mem). */
} mw_setup_t;

typedef struct {
  u8 map[0x40];     /**< Micro-Wire register array.                  */
  addr68_t ct;      /**< DMA current location (8 bit fixed point).   */
  addr68_t end;     /**< DMA end point location (8 bit fixed point). */

  /** LMC */
  struct
  {
    u8 master; /**< Master volume.             */
    u8 left;   /**< Left volume.               */
    u8 right;  /**< Right volume.              */
    u8 lr;     /**< Left/Right average volume. */
    u8 high;   /**< High pass filter.          */
    u8 low;    /**< Low pass filter.           */
    u8 mixer;  /**< Mixer mode.                */
    u8 align;  /**< reserved for struct align. */
  } lmc;
  const int68_t * db_conv; /**< Current decibel table (depend on lmc::mixer) */

  uint68_t hz;      /**< Output frequency.                        */
  int ct_fix;       /**< fixed point: sizeof(addr68_t)*8-log2mem. */
  const u8 * mem;   /**< 68000 memory buffer.                     */
  int log2mem;      /**< Size of 68K memory (2^log2mem).          */

} mw_t;


/**@}*/


/** @name  Library initialization functions
 *  @{
 */

/** Micro-Wire emulator initialization.
 *
 *    The mw_init() function must be call before all other MW
 *    functions.  It mainly setups internal tables.
 *
 *    @return error-code (always success)
 *    @retval 0  Success
 *
 *  @see mw_shutdown()
 */
int mw_init(mw_parms_t * const parms);

/** Micro-Wire emulator shutdown.
 *
 *  @see mw_init()
 */
void mw_shutdown(void);

/**@}*/

/** @name  MicroWire emulator initialization functions
 *  @{
 */

int mw_setup(mw_t * const mw,
	     mw_setup_t * const setup);

void mw_cleanup(mw_t * const mw);

// *  @see mw_reset()
/*  *    It performs following operations. */
/*  *    - Init output level (volume) table. */
/*  *    - Hardware reset */
/*  *    - Set replay frequency to default (44100 Hz) */


/** Set/Get sampling rate.
 *
 *    The mw_sampling_rate() function set Micro-Wire emulator sampling
 *    rate. f is given in hz. If f is 0 the current replay is return.
 *
 *  @param  mw  microwire instance
 *  @param  f   sampling rate in hz (0 to get current value).
 *
 *  @return new sampling rate
 */
uint68_t mw_sampling_rate(mw_t * const mw, uint68_t f);

/** Micro-Wire hardware reset.
 *
 *    The mw_reset() reset function perform a Micro-Wire reset. It performs
 *    following operations :
 *    - all registers zeroed
 *    - all internal counter zeroed
 *    - LMC reset
 *      - mixer mode YM2149+Micro-Wire
 *      - master volume to -40db
 *      - left and right volumes to -20db
 *      - low-pass filter to 0
 *
 *    @return error-code (always success)
 *    @retval 0  Success
 */
int mw_reset(mw_t * const mw);


/**@}*/
/** @name  Emulation functions
 *  @{
 */

/** Execute Micro-Wire emulation.
 *
 *    The mw_mix() function processes sample mixing with current internal
 *    parameters for n samples. Mixed samples are stored in a large enough
 *    (at least n) 32 bit buffer pointed by b. This buffer have to be
 *    previously filled with the YM-2149 samples. Typically it is the YM-2149
 *    emulator output buffer. This allows Micro-Wire emulator to honnor the
 *    LMC mixer mode.iven LMC mode. This porocess include the mono to stereo
 *    expansion. The mem68 starting pointer locates the 68K memory buffer
 *    where samples are stored to allow DMA fetch emulation.
 *
 *    @param  b      Pointer to YM-2149 source sample directly used for
 *                   Micro-Wire output mixing.
 *    @param  mem68  Pointer to 68K memory buffer start address
 *    @param  n      Number of sample to mix in b buffer
 *
 *    @see YM_mix()  @see YM_get_buffer()
 */
void mw_mix(mw_t * const mw,
	    s32 *b, int n);

/**@}*/


/** @name  Micro-Wire LMC control functions
 *  @{
 */

/** Set LMC mixer type.
 *
 *   The mw_set_lmc_mixer() function choose the mixer mode :
 *   - 0  -12 Db
 *   - 1  YM+STE
 *   - 2  STE only
 *   - 3  reserved ???
 *
 *   @param n  New mixer mode (see above)
 */
void mw_set_lmc_mixer(mw_t * const mw, int n);

/** Set LMC master volume.
 *
 *  @param  n  New volume in range [0..40]=>[-80Db..0Db]
 *
 *  @see mw_set_lmc_left()
 *  @see mw_set_lmc_right()
 */
void mw_set_lmc_master(mw_t * const mw, int n);

/** Set LMC left channel volume.
 *
 *    Set LMC left channel volume in decibel.
 *
 *    @param  n  New volume in range [0..20]=>[-40Db..0Db]
 *
 *  @see mw_set_lmc_master()
 *  @see mw_set_lmc_right()
 */
void mw_set_lmc_left(mw_t * const mw, int n);

/** Set LMC right channel volume.
 *
 *    @param  n  New volume in range [0..20]=>[-40Db..0Db]
 *
 *  @see mw_set_lmc_master()
 *  @see mw_set_lmc_left()
 */
void mw_set_lmc_right(mw_t * const mw, int n);

/** Set high pass filter.
 *
 *  @param  n  New high pass filter [0..12]=>[-12Db..0Db]
 *
 *  @see mw_set_lmc_low()
 *
 *  @warning  Filters are not supported by MicroWire emulator.
 */
void mw_set_lmc_high(mw_t * const mw, int n);

/** Set low pass filter.
 *
 *  @param  n  New low pass filter [0..12]=>[-12Db..0Db]
 *
 *  @see mw_set_lmc_high()
 *
 *  @warning  Unsupported by MicroWire emulator.
 */
void mw_set_lmc_low(mw_t * const mw,int n);

/**@}*/

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MWEMUL_H_*/
