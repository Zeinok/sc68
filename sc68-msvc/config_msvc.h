#define _CRT_SECURE_NO_WARNINGS

#ifndef inline
# define inline __inline
#endif
#define snprintf _snprintf

#undef HAVE_DECLSPEC

#if defined (_DEBUG) && !defined(DEBUG)
# define DEBUG _DEBUG
#endif

#define HAVE_WINDOWS_H 1
#define HAVE_WINREG_H 1

#define HAVE_ASSERT_H 1
#define HAVE_LIMITS_H 1
#define HAVE_STDIO_H  1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1

#define HAVE_GETENV 1
#define HAVE_FREE 1
#define HAVE_MALLOC 1
