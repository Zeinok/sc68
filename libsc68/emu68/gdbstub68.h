/**
 * @ingroup   emu68_lib
 * @file      emu68/gdbstub68.h
 * @author    Benjamin Gerard
 * @date      2013/06/20
 * @brief     68K gdbstub header.
 */
/* Time-stamp: <2013-06-23 16:51:51 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#if !defined(_EMU68_GDBSTUB68_H_) && defined(USE_GDBSTUB68)

#define _EMU68_GDBSTUB68_H_

#include "emu68_api.h"
#include "struct68.h"

/**
 * @defgroup  emu68_lib_gdb68  68k gdb stub
 * @ingroup   emu68_lib
 * @brief     Provide minimal support for gdb remote debugging.
 *
 *
 *  This gdb stub allows to debug programs running under emu68 with a
 *  remote gdb (configured for m68k target). It implements quiet the
 *  minimum needed to do so. Basically the application will have to
 *  write a single function used to send/receive data to/from gdb.
 *
 *  The following gdb commands are supported:
 *
 *    - g             Get CPU registers
 *    - G             Set CPU registers
 *    - mAA..AA,LLLL  Read LLLL bytes at address AA..AA
 *    - MAA..AA,LLLL  Write LLLL bytes at address AA.AA
 *    - c             Resume at current address
 *    - cAA..AA       Continue at address AA..AA
 *    -s              Step one instruction
 *    -sAA..AA        Step one instruction from AA..AA
 *    -k              kill
 *    -?              Get the last signal
 *
 *  All commands and responses are sent with a packet which includes a
 *  checksum.
 *
 *  $<packet-info>#<checksum>.
 *
 *  packet-info: characters representing the command or response
 *  checksum: two hex digits computed as modulo 256 sum of packet-info
 *
 *  When a packet is received, it is first acknowledged with either
 *  '+' or '-'.  '+' indicates a successful transfer. '-' indicates a
 *  failed transfer.
 */

/**
 *  Pointer to a function to communicate with gdb.
 *  @see emu68_gdbstub_create()
 */
typedef int (*emu68_gdbstub_f)(emu68_t * const, int);

EMU68_API
/**
 * Add a gdb stub to emu68.
 *
 * @param  emu68  emulator instance
 * @param  fct    a function called by em68 gdb stub to communicate
 *                with gdb. The second parameter can be either a byte
 *                from 0 to 255 to be sent to gdb or -1 to get a byte
 *                from gdb. It returns -1 on error or the value of
 *                the byte (read or written),
 *
 * @return error-code
 * @retval -1  on error
 * @retval  0  on success
 *
 */
int emu68_gdbstub_create(emu68_t * const emu68, emu68_gdbstub_f fct);

EMU68_API
/**
 * Destroy current emu68 gdb stub.
 *
 * @param  emu68  emulator instance
 */
void emu68_gdbstub_destroy(emu68_t * const emu68);


EMU68_API
/**
 * Handle exception.
 * @param  emu68   emulator instance
 * @param  vector  exception vector
 * @return @see emu68_status_e
 */
int emu68_gdbstub_handle(emu68_t * const emu68, int vector);

/**
 * @}
 */

#endif /* #ifndef _EMU68_GDBSTUB68_H_ */
