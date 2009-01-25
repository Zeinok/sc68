/**
 * @ingroup  file68_lib
 * @file     file68/file68_api.h
 * @author   Benjamin Gerard
 * @date     2007-02-25
 * @brief    Symbol exportation.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_FILE68_API_H_
#define _FILE68_FILE68_API_H_

/* Building DSO or library */
#if defined(FILE68_EXPORT)

# if defined(DLL_EXPORT) && defined(HAVE_DECLSPEC) && HAVE_DECLSPEC
#   define FILE68_API __declspec(dllexport)
# elif defined(HAVE_VISIBILITY) && HAVE_VISIBILITY
#  define FILE68_API __attribute__ ((visibility("default")))
# endif

/* Using */
#else /* elif ! defined(FILE68_EXPORT) */

# if defined(FILE68_DLL) && defined(HAVE_DECLSPEC) && HAVE_DECLSPEC
#  define FILE68_API __declspec(dllimport)
# elif defined(HAVE_VISIBILITY)
#  define FILE68_API /* __attribute__ ((visibility("default"))) */
# endif

#endif

/** @defgroup  file68_file68_api  sc68 files
 *  @ingroup   file68_lib
 *
 *     How symbols are exported.
 *
 *  @{
 */

#ifndef FILE68_API
/** file68 exported symbol.
 *
 *  Define special atribut for exported symbol.
 *
 *  - empty: static or classic .so library
 *  - __attribute__ (visibility("default"))): gcc support visibility.
 *  - __declspec(dllexport): creating a win32 DLL.
 *  - __declspec(dllimport): using a win32 DLL.
 */
#define FILE68_API
#endif

/**
 *  @}
 */

#endif /* ifndef _FILE68_FILE68_API_H_ */
