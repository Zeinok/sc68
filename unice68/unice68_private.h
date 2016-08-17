/**
 * @ingroup  lib_unice68
 * @file     unice68_private.h
 * @author   Benjamin Gerard
 * @date     2016/07/29
 * @brief    ICE! depacker private header
 */

/* Copyright (c) 1998-2016 Benjamin Gerard */

#ifdef UNICE68_H
# error unice68_private.h must be include before unice68.h
#endif

#ifdef UNICE68_PRIVATE_H
# error unice68_private.h must be include once
#endif
#define UNICE68_PRIVATE_H 1

#ifndef UNICE68_WIN32
# if defined(WIN32) || defined(_WIN32)
#  define UNICE68_WIN32 1
# endif
#endif

#ifndef UNICE68_API
# if defined(UNICE68_WIN32) && defined(unice68_lib_EXPORTS)
#  define UNICE68_API __declspec(dllexport)
# elif ! defined()


# if defined(unice68_lib_EXPORTS) || 

# if UNICE68_WIN32 && U


#endif

