#define _CRT_SECURE_NO_WARNINGS

#ifndef inline
# define inline __inline
#endif
#define snprintf _snprintf

#undef HAVE_DECLSPEC

#if defined (_DEBUG) && !defined(DEBUG)
# define DEBUG
#endif
