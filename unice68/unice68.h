/**
 * @ingroup  unice68_lib
 * @file     unice68.h
 * @author   Benjamin Gerard
 * @date     2003/08/06
 * @brief    ICE! depacker header
 *
 * $Id$
 */

/*
 *                       sc68 - ICE! depacker
 *             Copyright (C) 1998-2009 Benjamin Gerard
 *           <benjihan -4t- users.sourceforge -d0t- net>
 *
 * This  program is  free  software: you  can  redistribute it  and/or
 * modify  it under the  terms of  the GNU  General Public  License as
 * published by the Free Software  Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 *
 * You should have  received a copy of the  GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _UNICE68_H_
#define _UNICE68_H_

/* Building DSO or library */
#if defined(UNICE68_EXPORT)

# if defined(DLL_EXPORT) && defined(HAVE_DECLSPEC)
#   define UNICE68_API __declspec(dllexport)
# elif defined(HAVE_VISIBILITY)
#  define UNICE68_API extern __attribute__ ((visibility("default")))
# endif

/* Using */
#else

# if defined(UNICE68_DLL) && defined(HAVE_DECLSPEC)
#  define UNICE68_API __declspec(dllimport)
# elif defined(HAVE_VISIBILITY)
#  define UNICE68_API extern
# endif

#endif


#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  unice68_lib  unice68 library
 *  @ingroup   api68
 *  @brief     ICE! 2.4 depacker.
 *
 *    unice68 library provides functions to depack ICE! packed buffer.
 *    The depacker used is a rought conversion of the original ICE 2.4
 *    routine written in 68000 assembler. ICE packer was (and may be
 *    is) widely used on the Atari ST the Atari ST.
 *
 *  @note unice68 library is re-entrant (thread-safe).
 *
 *  @note It is pretty much impossible to implement a progressive
 *  version of the depacker. At least the entire packed file must be
 *  loaded before depacking it because data order is reversed.
 *
 *  @{
 */

#ifndef UNICE68_API
/** unice68 exported symbol.
 *
 *  Define special atribut for exported symbol.
 *
 *  - extern: static or classic .so library
 *  - __attribute__ (visibility("default"))): gcc with visibility support.
 *  - __declspec(dllexport): creating a win32 DLL.
 *  - __declspec(dllimport): using a win32 DLL.
 */
# define UNICE68_API extern
#endif

  /* */

UNICE68_API
/** Get unice68 version.
 *
 *  @return version number
 */
int unice68_version(void);

UNICE68_API
/** Get unice68 version.
 *
 *  @return version string
 */
const char * unice68_versionstr(void);

UNICE68_API
/** Get ICE depacker version.
 *
 *  @return version number (MAJOR*100+MINOR)
 */
int unice68_ice_version(void);

UNICE68_API
/** Test ICE and get compressed and uncompresed size.
 *
 *    The unice68_get_depacked_size() function returns the uncompressed
 *    size of a ICE compressed buffer. If p_size is not 0 it is fill with
 *    the size of the compressed data found in header (useful for stream
 *    operation). 
 *    If the value pointed by p_csize is not 0 the function assumes that it is
 *    an expected compressed size and compares it to header one. If it differs
 *    the function returns the bitwise NOT value of uncompressed data. This
 *    should be a minus value but not -1.
 *
 *  @param  buffer   buffer with at least 12 bytes of data (ice header).
 *  @param  p_csize  Optionnal pointer to store compressed size.
 *                   May be use to verify an expected compressed size.
 *                   See function documentation for more details.
 *
 *  @return uncompressed size
 *  @retval >0   Uncompressed size
 *  @retval -1   Error; not a valid ICE packed buffer
 *  @retval <0   Bitwise NOT of uncompressed size but verify failed.
 *
 */
int unice68_get_depacked_size(const void * buffer, int * p_csize);

UNICE68_API
/** Depack an ICE buffer to another.
 *
 *   The unice68_depacker() function depacks src input ICE compressed
 *   buffer into dst output buffer.  The dst buffer is assumed to be
 *   already allocated and large enough.
 *
 * @param  dst   output (destination) buffer (uncompressed data).
 * @param  src   input  (source)      buffer (compressed data).
 *
 * @return error code
 * @retval 0     succcess
 * @retval -1    failure
 *
 * @warning The original ICE depacker may allow to use the same buffer for
 *          compressed and uncompressed data. Anyway this has not been tested
 *          and you are encouraged to add guard bands.
 */
int unice68_depacker(void * dst, const void * src);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _UNICE68_H_ */
