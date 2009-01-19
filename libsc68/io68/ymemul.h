/**
 * @ingroup   io68_ym_devel
 * @file      io68/ymemul.h
 * @author    Benjamin Gerard
 * @date      1998/06/24
 * @brief     YM-2149 emulator header.
 *
 * $Id$
 */

/* Copyright (C) 1998-2007 Benjamin Gerard */

#ifndef _IO68_YM_EMUL_H_
#define _IO68_YM_EMUL_H_

#include "emu68/emu68.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup   io68_ym_devel  YM-2149 emulator
 *  @ingroup    io68_devel
 *
 *  The YM-2149 (Atari-ST soundchip) emulator.
 *
 * @{
 */

/** @name YM-2149 registers.
 *  @{
 */
#define YM_BASEPERL  0  /**< YM-2149 LSB period base (canal A)      */
#define YM_BASEPERH  1  /**< YM-2149 MSB period base (canal A)      */
#define YM_BASEVOL   8  /**< YM-2149 volume base register (canal A) */

#define YM_PERL(N) (YM_BASEPERL+(N)*2) /**< Canal #N LSB period     */
#define YM_PERH(N) (YM_BASEPERH+(N)*2) /**< Canal #N MSB periodr    */
#define YM_VOL(N)  (YM_BASEVOL+(N))    /**< Canal #N volume         */

#define YM_NOISE     6  /**< Noise period              */
#define YM_MIXER     7  /**< Mixer control             */
#define YM_ENVL      11 /**< Volume envelop LSB period */
#define YM_ENVH      12 /**< Volume envelop MSB period */
#define YM_ENVTYPE   13 /**< Volume envelop wave form  */
#define YM_ENVSHAPE  13 /**< Alias for YM_ENVTYPE      */
/**@}*/

/** @name YM-2149 internal register access. 
 *  @{
 */

/** YM write access structure. */
struct ym_waccess_s
{
  struct ym_waccess_s * link; /**< Link to prev or next entry.   */
  cycle68_t ymcycle;          /**< CPU cycle this access occurs. */
  u8 reg;                     /**< YM register to write into.    */
  u8 val;                     /**< Value to write.               */ 
};

/** YM write access type. */
typedef struct ym_waccess_s ym_waccess_t;

/** Sorted list of YM write access. */
typedef struct
{
  char name[4];             /**< Name (for debug).           */
  ym_waccess_t * head;      /**< First access in list.       */              
  ym_waccess_t * tail;      /**< Last acces in list.         */
} ym_waccess_list_t;

/**@}*/

struct ym2149_reg_s {
  /* 0 */  u8 per_a_lo;
  /* 1 */  u8 per_a_hi;
  /* 2 */  u8 per_b_lo;
  /* 3 */  u8 per_b_hi;
  /* 4 */  u8 per_c_lo;
  /* 5 */  u8 per_c_hi;
  /* 6 */  u8 per_noise;
  /* 7 */  u8 ctl_mixer;
  /* 8 */  u8 vol_a;
  /* 9 */  u8 vol_b;
  /* A */  u8 vol_c;
  /* B */  u8 per_env_lo;
  /* C */  u8 per_env_hi;
  /* D */  u8 env_shape;
  /* E */  u8 io_a;
  /* F */  u8 io_b;
};

typedef union {
  struct ym2149_reg_s name;
  u8 index[16];
} ym_reg_t;

/* define to use table for envelop emulation. */
#define YM_ENV_TABLE 

/** YM-2149 internal data structure */
struct ym2149_s
{
  /* Internal YM register */
  u8 ctrl;                  /**< Control (working) register.             */
  ym_reg_t reg;             /**< YM registers.                           */
  ym_reg_t shadow;          /**< Shadow YM registers (for reading).      */

  /* Envelop generator */
#ifdef YM_ENV_TABLE
  int env_ct;               /**< Envelop period counter                  */
  int env_bit;              /**< Envelop level : 5 LSB are envelop level */
#else
  unsigned int env_ct;      /**< Envelop period counter */
  unsigned int env_bit;     /**< Envelop level : 5 LSB are envelop level */
  unsigned int env_cont;    /**< Continue mask [0 or 0x1f] */
  unsigned int env_alt;     /**< Alternate mask [0 or 0x1f] */  
  unsigned int env_bitstp;  /**< Envelop level step : [0 or 1] */
#endif

  /* Noise generator */
  unsigned int noise_gen;   /**< Noise generator 17-bit shift register   */
  unsigned int noise_ct;    /**< Noise generator period counter          */

  /* Tone generator */
  int voice_ctA;            /**< Canal A sound period counter            */
  int voice_ctB;            /**< Canal B sound period counter            */
  int voice_ctC;            /**< Canal C sound period counter            */
  
  unsigned int levels;      /**< Square level 0xCBA                      */

  unsigned int voice_mute;  /**< Mask muted voices.                      */

  unsigned int hz;          /**< Sampling rate.                          */

  int outlevel;             /**< Output max level (volume) [0..65536].   */

  /* Filter */
  int hipass_inp1;
  int hipass_out1;
  int lopass_out1;

  s16 * ymout5;             /**< DAC lookup table                        */

  s32 * outbuf;             /**< output buffer given to ym_run()         */
  s32 * outptr;             /**< generated sample pointer (into outbuf)  */
  s32 * noiptr;             /**< generated noise pointer                 */
  s32 * envptr;             /**< generated envelop pointer               */
  s32 * tonptr;             /**< generated tone pointer                  */

  /** Write access back storage. */
  ym_waccess_list_t env_regs; /**< envelop generator access list.        */
  ym_waccess_list_t noi_regs; /**< noise generator access list.          */
  ym_waccess_list_t ton_regs; /**< tone generator access list.           */

  int            waccess_max; /**< Maximum number of entry in waccess.   */
  ym_waccess_t * waccess_nxt; /**< Next available ym_waccess_t.          */
  ym_waccess_t * waccess;     /**< Static register entry list.           */

  uint68_t clock;             /**< Master clock frequency in Hz.         */

  /* $$$ TEMP: should be allocated... */
  ym_waccess_t static_waccess[2048];

};

/** YM-2149 emulator instance type */
typedef struct ym2149_s ym_t;

/** Available emulation modes. */
enum ym_emul_e {
  YM_EMUL_DEFAULT = 0, /**< Use default mode.                             */
  YM_EMUL_ORG,         /**< sc68 original emulation .                     */
  YM_EMUL_BLEP,        /**< Antti Lankila's Band Limited Step synthesis.  */
};

/** Sampling rate. */
enum ym_hz_e {
  YM_HZ_DEFAULT = 0    /**< Default sampling rate. */
};


/** YM master clock frequency. */
enum ym_clock_e {
  /**< Default frequency (YM_CLOCK_ATARIST). */
  YM_CLOCK_DEFAULT = 0,
  /** Atari-ST YM-2149 clock is about 2Mz. */
  YM_CLOCK_ATARIST = EMU68_ATARIST_CLOCK/4u,
} ;

/** YM-2149 setup structure.
 *
 *    This structure is passed to the ym_init() and ym_setup()
 *    functions. The caller have to fill it with desired values before
 *    calling these functions. Each field can be set to a nul value in
 *    order to use the default value. The function will set the actual
 *    values before returning.
 *
 */
typedef struct
{
  int68_t  emul;     /**< Emulator mode. @see ym_emul_e. */
  uint68_t hz;       /**< Sampling rate in Hz.           */
  uint68_t clock;    /**< YM clock frequency.            */ 
  uint68_t outlevel; /**< Output level [0..256].         */
} ym_parms_t;


/** @name  Initialization functions
 *  @{
 */

/** Create an Yamaha-2149 emulator instance.
 *
 *   @param  ym
 *   @param  params
 *
 *   @return  error-code
 *   @retval   0  Success
 *   @retval  -1  Failure
 *
 *   @see  ym_destroy()
 */
int ym_setup(ym_t * const ym, ym_parms_t * const parms);


/** Destroy an Yamaha-2149 emulator instance.
 *
 *   @param  ym  ym emulator instance to destroy
 */
void ym_cleanup(ym_t * const ym);

/** Perform an Yamaha-2149 hardware reset.
 *
 *    The ym_reset() reset function perform a YM-2149 reset. It
 *    performs following operations :
 *    - all register zeroed
 *    - mixer is set to 077 (no sound and no noise)
 *    - envelop shape is set to 0xA (/\)
 *    - control register is set to 0
 *    - internal periods counter are zeroed
 *
 *    @param  ym       YM-2149 emulator instance.
 *    @param  ymcycle  ym cycle the reset has occured.
 *
 *    @return error-code (always success)
 *    @retval  0  Success
 *    @retval -1  Failure
 */
int ym_reset(ym_t * const ym, const cycle68_t ymcycle);

/** Yamaha-2149 first one first initialization.
 *
 *    The ym_init() function must be call before any other ym_
 *    function.  If params is non nul then all non nul fields will be
 *    used as the new default value for this field at ym_setup()
 *    function call.
 *
 *  @param  params  Default parameters for ym_setup().
 *
 *  @return error-code (always success)
 *  @retval 0  Success
 *
 *  @see ym_reset()
 */
int ym_init(ym_parms_t * const parms);

/** Shutdown the ym-2149 library.
 *
 *    The ym_shutdown() function should be call after all created ym
 *    emulator instances have been clean up.
 *
 */
void ym_shutdown(void);

/**@}*/


/** @name  Emulation functions
 *  @{
 */

/** Execute Yamaha-2149 emulation.
 *
 *    The ym_run() function execute Yamaha-2149 emulation for a given
 *    number of cycle. The output buffer 
 *
 *    @warning The requested output buffer size may seem larger than
 *    neccessary but it is not. Internally the emulator may need some
 *    extra place (for oversampling...). Always call ym_buffersize()
 *    to allocate this buffer large enougth.
 * 
 *  @param  ym        YM-2149 emulator instance.
 *  @param  output    Output sample buffer.
 *  @param  ymcycles  Number of cycle to mix.
 *
 *  @return  Number of sample in output mix-buffer
 *  @retval  -1  Failure
 *
 *  @see ym_buffersize()
 */
int ym_run(ym_t * const ym, s32 * output, const cycle68_t ymcycles);


/** Get required output buffer size.
 *
 *    The ym_buffersize() function returns the minimum size in
 *    PCM(32bit) of the output buffer for generating ymcycles
 *    cycles. It should be use to ensure the buffer is large enough
 *    before calling ym_run() function.
 *
 *  @param  ym        YM-2149 emulator instance.
 *  @param  ymcycles  Number of ym-cycles to generate
 *
 *  @return Minimum size in PCM (32bit) of output buffer to generate
 *          ymcycles samples.
 *
 */
static inline
uint68_t ym_buffersize(const ym_t const * ym, const cycle68_t ymcycles)
{
  return ((ymcycles+7u) >> 3);
}


/** Change YM cycle counter base.
 *
 *   The ym_adjest() function allow to corrige the internal cycle
 *   counter to prevent overflow. Because the number of cycle could
 *   grow very quickly, it is neccessary to get it down from time to
 *   time.
 *
 *  @param  ym        YM-2149 emulator instance.
 *  @param  ymcycles  Number of cpu cycle to substract to current the
 *                    cycle counter.
 *
 */
void ym_adjust_cycle(ym_t * const ym, const cycle68_t ymcycles);

/**@}*/


/** @name  YM-2149 register access functions
 *  @{
 */

/** Write in YM-2149 register.
 *
 *   The ym_writereg() function performs a write access to an YM-2149
 *   internal register.  The YM-2149 emulator does not really write
 *   register but store changes in separate list depending of the
 *   register nature and dependencies. There are 3 lists of update
 *   (sound, noise and envelop). This method allow to perform a very
 *   efficient cycle precise emulation. For this reason the YM-2149
 *   registers should be read by ym_readreg() function.
 *
 *  @param  ym       YM-2149 emulator instance.
 *  @param  val      Value to write.
 *  @param  ymcycle  YM cycle this access has occurred.
 *
 *  @see ym_readreg();
 */
void ym_writereg(ym_t * const ym,
		 const int val, const cycle68_t ymcycle);

/** Read a YM-2119 register.
 *
 *   The ym_readreg() function must be call to read an YM-2149
 *   register. For the reasons explained in YM_writereg(), register
 *   must not be read directly.
 *
 *  @param  ym       YM-2149 emulator instance.
 *  @param  ymcycle  CPU cycle number this access has occurred.
 *
 *  @return  Register value at given cycle
 *
 *  @see ym_writereg();
 */
static inline
int ym_readreg(ym_t * const ym, const cycle68_t ymcycle)
{
  const int reg = ym->ctrl;
  return (reg>=0 && reg<16)
    ? ym->shadow.index[reg]
    : 0;
}


/** Get/Set active channels status.
 *
 *   The ym_active_channels() function allows to activate/desactivate
 *   separarely channels (voices) of the YM-2149.
 *
 *   For both on and off parameters:
 *   -bit#0: canal A (0:no change 1:change to on or off)
 *   -bit#1: canal B (0:no change 1:change to on or off)
 *   -bit#2: canal C (0:no change 1:change to on or off)
 *
 *   How it works:
 *
 *   The ym_active_channels() function performs off parameter before on
 *   parameter. The formula looks like ``result=(current&^off)|on''.
 *
 *   How to use:
 *
 *   -Get current value            : ym_active_channels(ym,0,0);
 *   -Mute all channels            : ym_active_channels(ym,7,0);
 *   -Active all channels          : ym_active_channels(ym,0,7);
 *   -Active some channel channels : ym_active_channels(ym,0,x);
 *
 *  @param  ym   YM-2149 emulator instance
 *  @param  off  Mute the channels for bit set to 1
 *  @param  on   Active the channels for bit set to 1
 *
 *  @return  new active-voices status     
 *  @retval  -1 Failure
 *  @retval   0 All voice muted
 *  @retval   1 Only channel A
 *  @retval   7 all channels active
 *
 */
int ym_active_channels(ym_t * const ym, const int off, const int on);


/** Get/Set sampling rate.
 *
 *  @param  ym   YM-2149 emulator instance
 *  @param  hz   0:Get, >0:Set
 *
 *  @return Actual sampling rate (may differs from requested).
 *  @retval 0    Failure
 */
uint68_t ym_sampling_rate(ym_t * const ym, const uint68_t hz);

/**@}*/

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _IO68_YM_EMUL_H_ */
