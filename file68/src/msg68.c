/*
 *                      file68 - debug message
 *            Copyright (C) 2001-2009 Ben(jamin) Gerard
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

/* $Id$ */

/* $$$ Nothing in there is thread safe... Anyway nothing should be
   really harmful either.
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "file68_api.h"
#include "msg68.h"
#include "string68.h"

static msg68_t debug  = 0;                /* Debug function.            */
static void * debug_cookie = 0;           /* Debug function user param. */
static int current_feature = msg68_DEBUG; /* Default current feature.   */

/** Filter debug messages.
 *
 *  Bit have to be clear to mask/filter a debug feature. The default
 *  value depend on compilation. If debug feature was enabled (DEBUG
 *  defined) msg68_bitmsk value is set to ~0, allowing all
 *  messages. On the contrary its msg68_bitmsk is set to 0,
 *  filtering out all messages (except
 *  enum_msg68_bit::msg68_unmaskable).
 */
static
#if defined(DEBUGMSG_MASK)
unsigned int msg68_bitmsk = DEBUGMSG_MASK; /* User defined mask.         */
#elif defined(DEBUG)
unsigned int msg68_bitmsk = ~0;            /* Filter none.               */
#else
unsigned int msg68_bitmsk =                /* Filter almost all.         */
  (1<<msg68_CRITICAL)|(1<<msg68_ERROR)|(1<<msg68_WARNING);
#endif

#define MAX_FEATURES (((sizeof(int)<<3)))

struct struct_debug_bit {
  int bit;           /* equal to its array's index when entry is used. */
  const char * name; /* feature name.                                  */
  const char * desc; /* short descrition.                              */
} debug_bits[MAX_FEATURES] = {
  { msg68_CRITICAL, "critical", "critical error message" },
  { msg68_ERROR   , "error"   , "error message"          },
  { msg68_WARNING , "warning" , "warning message"        },
  { msg68_INFO    , "info"    , "informational message"  },
  { msg68_DEBUG   , "debug"   , "debug message"          },
  { msg68_TRACE   , "trace"   , "trace message"          }
};

/* Set handler. */
msg68_t msg68_set_handler(msg68_t handler)
{
  msg68_t old = debug;
  debug = handler;
  return old;
}

/* Set cookie. */
void * msg68_set_cookie(void * cookie)
{
  void * old = debug_cookie;
  debug_cookie = cookie;
  return old;
}

/* Print debug message (variable argument). */
void msg68_va(int bit, const char * fmt, va_list list)
{
  if (debug) {
    const int feature = (bit == msg68_CURRENT)
      ? current_feature
      : bit
      ;

    switch (feature) {
    case msg68_NEVER:
      break;
    default:
      if ( ! (msg68_bitmsk&(1<<feature)) )
        break;
      if (feature>msg68_TRACE &&
          ! (msg68_bitmsk&(1<<msg68_TRACE)))
        break;
    case msg68_ALWAYS:
      debug(feature, debug_cookie, fmt, list);
    }
  }
}

/* Print debug message. */
void msg68(const int bit, const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  msg68_va(bit, fmt, list);
  va_end(list);
}

/* Print debug message (debug level) */
void msg68_trace(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  msg68_va(msg68_TRACE,fmt,list);
  va_end(list);
}

/* Print debug message (debug level) */
void msg68_debug(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  msg68_va(msg68_DEBUG,fmt,list);
  va_end(list);
}

/* Print debug message (info level) */
void msg68_info(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  msg68_va(msg68_INFO,fmt,list);
  va_end(list);
}

/* Print debug message (warning level) */
void msg68_warning(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  msg68_va(msg68_WARNING,fmt,list);
  va_end(list);
}

/* Print debug message (error level) */
void msg68_error(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  msg68_va(msg68_ERROR,fmt,list);
  va_end(list);
}

/* Print debug message (critical level) */
void msg68_critical(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  msg68_va(msg68_CRITICAL,fmt,list);
  va_end(list);
}

/* Print debug message (current level) */
void msg68_current(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  msg68_va(msg68_CURRENT,fmt,list);
  va_end(list);
}

/* Print debug message (always) */
void msg68_always(const char * fmt, ...)
{
  va_list list; va_start(list,fmt);
  msg68_va(msg68_ALWAYS,fmt,list);
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

static inline int is_free_feature(const int i)
{
  return debug_bits[i].bit != i;
}

static inline int get_free_feature(void)
{
  int i;
  for (i=MAX_FEATURES; --i>=0 && !is_free_feature(i);)
    ;
  if (i>=0)
    debug_bits[i].bit = i;
  return i;
}


/* Get named features. */
int msg68_feature_bit(const char * name)
{
  return name ? get_feature(name) : -1;
}

/* Create/Modify a feature. */
int msg68_feature(const char * name, const char * desc, const int masked)
{
  int i = msg68_NEVER;

  if (name && desc) {
    i = get_feature(name);
    if (i < 0) {
      i = get_free_feature();
    }
    if (i >= 0) {
      debug_bits[i].name = name;
      debug_bits[i].desc = desc;
      if (masked) {
        msg68_bitmsk |= 1<<i;
      } else {
        msg68_bitmsk &= ~(1<<i);
      }
    }
  }

  return i;
}

/* Free/Destroy a debug feature. */
void msg68_feature_free(const int feature)
{
  if (is_valid_feature(feature) && feature>msg68_TRACE) {
    debug_bits[feature].bit = -1;
    msg68_bitmsk |= 1<<feature;
  }
}

/* Get/Set current feature. */
int msg68_feature_current(const int feature)
{
  int old = current_feature;

  /* Allow always or never or any other existing feature */
  if (feature == msg68_ALWAYS || feature == msg68_NEVER ||
      (is_valid_feature(feature) && !is_free_feature(feature))) {
    current_feature = feature;
  }
  return old;
}

/* Set all predefined features mask according to given level. */
int msg68_feature_level(const int feature)
{
  int ret = -(feature < msg68_CRITICAL || feature > msg68_TRACE);
  if (!ret) {
    unsigned int v = msg68_bitmsk & ~((1<<(msg68_TRACE+1))-1);
    v |= (1<<(feature+1))-1;
    msg68_bitmsk = v;
  }
  return ret;
}

/* Get info on feature */
int msg68_feature_info(const int feature, const char **pname,
                       const char **pdesc, int *pnext)
{
  int ret = -1, next = feature;
  if (is_valid_feature(feature)) {
    if (pname) *pname = debug_bits[feature].name;
    if (pdesc) *pdesc = debug_bits[feature].desc;
    ret = 1 & (msg68_bitmsk>>feature);
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

void msg68_feature_help(void * cookie, msg68_help_t fct)
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

unsigned int msg68_mask(unsigned int clr, unsigned int set)
{
  msg68_bitmsk &= ~clr;
  msg68_bitmsk |=  set;
  return msg68_bitmsk;
}
