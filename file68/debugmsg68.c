/*
 *                      file68 - debug message
 *	      Copyright (C) 2001-2009 Ben(jamin) Gerard
 *	     <benjihan -4t- users.sourceforge -d0t- net>
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

/* $Id$ */

/* $$$ Nothing in there is thread safe... Anyway nothing should be
   really harmful either.
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "file68_api.h"
#include "debugmsg68.h"
#include "string68.h"

static debugmsg68_t debug  = 0;                /* Debug function.            */
static void * debug_cookie = 0               ; /* Debug function user param. */
static int current_feature = debugmsg68_DEBUG; /* Default current feature.   */

#if defined(DEBUGMSG_MASK)
unsigned int debugmsg68_mask = DEBUGMSG_MASK;  /* User defined mask.         */
#elif defined(DEBUG)
unsigned int debugmsg68_mask = ~0;             /* Filter none.               */
#else
unsigned int debugmsg68_mask =	               /* Filter almost all.         */
  (1<<debugmsg68_CRITICAL)|(1<<debugmsg68_ERROR)|(1<<debugmsg68_WARNING)
#endif


#define MAX_FEATURES (((sizeof(int)<<3)))

struct struct_debug_bit {
  int bit;           /* equal to its array's index when entry is used. */
  const char * name; /* feature name.                                  */
  const char * desc; /* short descrition.                              */
} debug_bits[MAX_FEATURES] = {
  { debugmsg68_CRITICAL, "critical", "critical error message" },
  { debugmsg68_ERROR   , "error"   , "error message"          },
  { debugmsg68_WARNING , "warning" , "warning message"        },
  { debugmsg68_INFO    , "info"    , "informational message"  },
  { debugmsg68_DEBUG   , "debug"   , "debug message"          }
};

/* Set handler. */
debugmsg68_t debugmsg68_set_handler(debugmsg68_t handler)
{
  debugmsg68_t old = debug;
  debug = handler;
  return old;
}

/* Set cookie. */
void * debugmsg68_set_cookie(void * cookie)
{
  void * old = debug_cookie;
  debug_cookie = cookie;
  return old;
}

/* Print debug message (variable argument). */
void vdebugmsg68(int bit, const char * fmt, va_list list)
{
  if (debug) {
    const int feature = (bit == debugmsg68_CURRENT)
      ? current_feature
      : bit;

    if (feature != debugmsg68_NEVER &&  
	(feature == debugmsg68_ALWAYS || (debugmsg68_mask&(1<<feature))))
      debug(debug_cookie, /* feature,  */fmt, list);
  }
}

/* Print debug message. */
void debugmsg68(const int bit, const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  vdebugmsg68(bit, fmt, list);
  va_end(list);
}

/* Print debug message (debug level) */
void debugmsg68_debug(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  vdebugmsg68(debugmsg68_DEBUG,fmt,list);
  va_end(list);
}

/* Print debug message (info level) */
void debugmsg68_info(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  vdebugmsg68(debugmsg68_INFO,fmt,list);
  va_end(list);
}

/* Print debug message (warning level) */
void debugmsg68_warning(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  vdebugmsg68(debugmsg68_WARNING,fmt,list);
  va_end(list);
}

/* Print debug message (error level) */
void debugmsg68_error(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  vdebugmsg68(debugmsg68_ERROR,fmt,list);
  va_end(list);
}

/* Print debug message (critical level) */
void debugmsg68_critical(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  vdebugmsg68(debugmsg68_CRITICAL,fmt,list);
  va_end(list);
}

/* Print debug message (current level) */
void debugmsg68_current(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  vdebugmsg68(debugmsg68_CURRENT,fmt,list);
  va_end(list);
}

/* Print debug message (always) */
void debugmsg68_always(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  vdebugmsg68(debugmsg68_ALWAYS,fmt,list);
  va_end(list);
}

static inline int is_valid_feature(const int i)
{
  return i>=0 && i<MAX_FEATURES;
}

static inline int get_feature(const char *name)
{
  int i;
  for (i=MAX_FEATURES; --i>=0 && strcmp68(name,debug_bits[i].name);)
    ;
  return i;
}

static /* inline */ int is_free_feature(const int i)
{
  return debug_bits[i].bit != i;
}

static /* inline */ int get_free_feature(void)
{
  int i;
  for (i=MAX_FEATURES; --i>=0 && !is_free_feature(i);)
    ;
  if (i>=0)
    debug_bits[i].bit = i;
  return i;
}


/* Get named features. */
int debugmsg68_feature_bit(const char * name)
{
  return name ? get_feature(name) : -1;
}

/* Create/Modify a feature. */
int debugmsg68_feature(const char * name, const char * desc, const int masked)
{
  int i = debugmsg68_NEVER;

  if (name && desc) {
    i = get_feature(name);
    if (i < 0) {
      i = get_free_feature();
    }
    if (i >= 0) {
      debug_bits[i].name = name;
      debug_bits[i].desc = desc;
      if (masked) {
	debugmsg68_mask |= 1<<i;
      } else {
	debugmsg68_mask &= ~(1<<i);
      }
    }
  }

  return i;
}

/* Free/Destroy a debug feature. */
  void debugmsg68_feature_free(const int feature)
{
  if (is_valid_feature(feature)) {
    debug_bits[feature].bit = -1;
    debugmsg68_mask |= 1<<feature;
  }
}

/* Get/Set current feature. */
int debugmsg68_feature_current(const int feature)
{
  int old = current_feature;

  /* Allow always or never or any other existing feature */
  if (feature == debugmsg68_ALWAYS || feature == debugmsg68_NEVER ||
      (is_valid_feature(feature) && !is_free_feature(feature))) {
      current_feature = feature;
  }
  return old;
}

/* Set all predefined features mask according to given level. */
int debugmsg68_feature_level(const int feature)
{
  int ret = -(feature < debugmsg68_CRITICAL || feature > debugmsg68_DEBUG);
  if (!ret) {
    unsigned int v = debugmsg68_mask & ~((1<<(debugmsg68_DEBUG+1))-1);
    v |= (1<<(feature+1))-1;
    debugmsg68_mask = v;
  }
  return ret;
}

/* Get info on feature */
int debugmsg68_feature_info(const int feature, const char **pname,
			    const char **pdesc, int *pnext)
{
  int ret = -1, next = feature;
  if (is_valid_feature(feature)) {
    if (pname) *pname = debug_bits[feature].name;
    if (pdesc) *pdesc = debug_bits[feature].desc;
    ret = 1 & (debugmsg68_mask>>feature);
  } else {
    next = -1;
  }
  if (pnext) {
    while (++next<MAX_FEATURES && is_free_feature(next))
      ;
    if (feature<MAX_FEATURES) {
      *pnext = next;
    }
  }
  return ret;
}

void debugmsg68_feature_help(void * cookie, debugmsg68_help_t fct)
{
  if (fct) {
    int i;
    for (i=0; i<MAX_FEATURES; ++i) {
      const int isfree = is_free_feature(i);
      if (!isfree)
	fct (cookie, i, debug_bits[i].name, debug_bits[i].desc);
    }
  }
}
