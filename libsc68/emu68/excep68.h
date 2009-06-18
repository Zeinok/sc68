/**
 * @ingroup   emu68_lib
 * @file      emu68/excep68.h
 * @author    Benjamin Gerard
 * @date      1999/13/03
 * @brief     68k exception and interruption definition header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _EMU68_EXCEP68_H_
#define _EMU68_EXCEP68_H_

/** @addtogroup  emu68_lib
 *  @{
 */

/** @name  68k exceptions and interruptions
 *
 *   68K interruptions are defined by a vector and a level. The
 *   interrupt vector is a long word stored in memory at vector
 *   address. This long word is the location of the interrupt routine
 *   which is loaded in the PC register. The interrupt level is the
 *   value transfered to the IPL field of SR so that no lower leveled
 *   interruption may be triggered.
 *
 * @{
 */

#define RESET_VECTOR     0x00 /**< RESET vector address.        */
#define RESET_LEVEL      7    /**< RESET interruption level.    */

#define BUSERROR_VECTOR  0x08 /**< BUSERROR vector address.     */
#define BUSERROR_LEVEL   7    /**< BUSERROR interruption level. */

#define ADRERROR_VECTOR  0x0C /**< ADRERROR vector address.     */
#define ADRERROR_LEVEL   7    /**< ADRERROR interruption level. */

#define ILLEGAL_VECTOR   0x10 /**< ILLEGAL vector address.      */
#define ILLEGAL_LEVEL    7    /**< ILLEGAL interruption level.  */

#define DIVIDE_VECTOR    0x14 /**< DIVIDE vector address.       */
#define DIVIDE_LEVEL     7    /**< DIVIDE interruption level.   */

#define CHK_VECTOR       0x18 /**< CHK vector address.          */
#define CHK_LEVEL        7    /**< CHK interruption level.      */

#define TRAPV_VECTOR     0x1C /**< TRAPV vector address.        */
#define TRAPV_LEVEL      7    /**< TRAPV interruption level.    */

#define LINEA_VECTOR     0x28 /**< LINEA vector address.        */
#define LINEA_LEVEL      7    /**< LINEA interruption level.    */

#define LINEF_VECTOR     0x28 /**< LINEF vector address.        */
#define LINEF_LEVEL      7    /**< LINEF interruption level.    */

#define TRAP_VECTOR(N)   (0x80+(4*(N))) /**< TRAP #N vector address.     */
#define TRAP_LEVEL       7              /**< TRAP #N interruption level. */

/** @} */

/**
 *  @}
 */

#endif /* #ifndef _EMU68_EXCEP68_H_ */
