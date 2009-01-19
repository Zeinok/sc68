/*
 *                init68 - initialization functions
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "file68_api.h"
#include "init68.h"

#include "debugmsg68.h"
#include "error68.h"
#include "alloc68.h"
#include "registry68.h"
#include "istream68_curl.h"
#include "rsc68.h"
#include "string68.h"

#include <stdlib.h>
#include <string.h>

static volatile int init;

/* Convert option name to envvar name */
static char * opt2env(char * tmp, const int max, const char * name)
{
  int i,c;
  for (i=0; i<max && (c=*name++); ) {
    tmp[i++] = (c=='-')
      ? '_'
      : (c>'9') ? c+'A'-'a' : c
      ;
  }
  tmp[i] = 0;
  return tmp;
}

static char * mygetenv(const char *name, int prefix)
{
#ifndef HAVE_GETENV
  return 0;
#else
  char tmp[256] = "SC68_";
  prefix = prefix ? 5 : 0;
  opt2env(tmp+prefix,sizeof(tmp)-1-prefix,name);
  return getenv(tmp);
#endif
}

/* returns 0    : no match
 *         arg  : exact match, no '='
 *         >arg : points to value after the '='
 */
static const char * get_str_arg(const char *arg, const char *name)
{
  const char * ret = 0;
  if (strstr(arg,name) == arg) {
    const int l = strlen(name);
    switch (arg[l]) {
    case 0:   ret = arg; break;
    case '=': ret = arg+l+1; break;
    }
  }
  return ret;
}

/* Get path from registry, converts '\' to '/' and adds missing trailing '/'.
 *
 * @return pointer to the end of string
 * @retval 0 error
 */
static char * get_reg_path(registry68_key_t key, char * kname,
			   char * buffer, int buflen)
{
  int i = registry68_gets(key,kname,buffer,buflen);
  buffer[buflen-1] = 0;
  if (i < 0) {
    buffer[0] = 0;
    return 0;
  } else {
    char *e;
    for (e=buffer; *e; ++e) {
      if (*e == '\\') *e = '/';
    }
    if (e > buffer && e[-1] != '/') {
      *e++ = '/';
      *e = 0;
    }
    return e;
  }
}

/* list of options / envvars */
static struct optvar68_s {
  int            has_arg; /* 0:no 1:str 2:numeric; 1st complement => setted */
  const char   * name;	  /* key name */
  const char   * desc;	  /* Short description */
  union {
    const char * str;
    int          num;
  }              val; 	  /* Melted value */

} vars[] = {
  { 0, "no-debug", "disable all debug output"      },
  { 2, "debug"   , "set debug features"            },
  { 1, "data"    , "shared (system) resource path" },
  { 1, "home"    , "private (user) resource path"  },
  { 1, "music"   , "music database path"           },
  { 1, "rmusic"  , "online music base URI"         }
};
#define NVARS (sizeof(vars)/sizeof(*vars))
static struct optvar68_s * evar = vars+NVARS;

static inline int var_type (const struct optvar68_s * var)
{
  return var->has_arg >= 0
    ? var->has_arg
    : 1 - var->has_arg
    ;
}

static inline int var_set_bool(struct optvar68_s * var, int val)
{
  var->has_arg = -1 - 0;
  return var->val.num = -!!val;
}

static inline void var_set_str(struct optvar68_s * var, const char * val)
{
  var->has_arg = -1 - 1;
  var->val.str = val;
}

static inline int var_set_int(struct optvar68_s * var, int val)
{
  var->has_arg = -1 - 2;
  return var->val.num = val;
}

static int var_set_strtol(struct optvar68_s * var, const char * val)
{
  int res, ok;

  if (!val) {
    ok = 0;
  } else if (!strcmp68(val,"yes")  || 
	     !strcmp68(val,"true") ||
	     !strcmp68(val,"on")) {
    ok  = 1;
    res = -1;
  } else if (!strcmp68(val,"no")    || 
	     !strcmp68(val,"false") ||
	     !strcmp68(val,"off")) {
    ok  = 1;
    res = 0;
  } else {
    ok = val[*val=='-'];
    if (ok >= '0' && ok <= '9') {
      res = strtol(val,0,0);
    } else {
      ok = 0;
      /* $$$ TODO */
    }
  }

  if (!ok) {
    res = var->val.num;
  }
  if (var_type(var) == 0) {
    res = var_set_bool(var, res);
  } else {
    res = var_set_int(var, res);
  }
  return res;
}

static inline int var_isset(const struct optvar68_s * var)
{
  return var->has_arg<0;
}

static inline void var_reset(struct optvar68_s * var)
{
  if (var->has_arg < 0) var->has_arg = 1 - var->has_arg;
  var->val.num = 0;
  var->val.str = 0;
}

static struct optvar68_s * var_of(const char * key)
{
  struct optvar68_s * var;
  for (var=vars; var<evar; ++var)
    if (!strcmp(key, var->name))
      return var;
  return 0;
}



extern int file68_feature, rsc68_feature, ao68_feature;

#ifdef DEBUG 
# define DEFAULT_DEBUG68 1
#else
# define DEFAULT_DEBUG68 0
#endif

#ifndef DEBUG_FILE68
# define DEBUG_FILE68 DEFAULT_DEBUG68
#endif

#ifndef DEBUG_RSC68
# define DEBUG_RSC68 DEFAULT_DEBUG68
#endif

#ifndef DEBUG_AO68
# define DEBUG_AO68 DEFAULT_DEBUG68
#endif


#ifndef NDEBUG
static struct struct_init_feature {
  int    mask;
  int  * pvar; 
  char * name;
  char * desc;
} init_features [] = 
{
  { DEBUG_FILE68, &file68_feature , "loader" , "music file loader" },
  { DEBUG_RSC68,  &rsc68_feature  , "rsc"    , "resource access protocol" },
  { DEBUG_AO68,   &ao68_feature   , "audio"  , "Xiph AO audio stream" },
};
#endif /* #ifndef NDEBUG */


int file68_init(char **argv, int argc)
{
  int i,n;
  char tmp[1024];
  struct optvar68_s * var;

  if (init) {
    const int i = init & 3;
    const char *message[4] = {
      "clean","initialized","shutdowning","initializing"
    };
    error68_add("file68_init() : [error] %s", message[i]);
    argc = -1;
    goto out_no_init;
  }
  init = 3;

  /* Init debug system. */
#ifndef NDEBUG
  for (i=0; i<sizeof(init_features)/sizeof(*init_features); ++i) {
    int bit = debugmsg68_feature(init_features[i].name,
				 init_features[i].desc,
				 init_features[i].mask);
    if (init_features[i].pvar) *init_features[i].pvar = bit;
  }
#endif

  /* Init resource */
  rsc68_init();

  /* Init curl */
  istream68_curl_init();

  /* Reset options */
  for (var=vars; var<evar; ++var)
    var_reset(var);
  
  /* Parse arguments */
  for (i=n=1; i<argc; ++i) {
    const char * s;
    int negate = 0, k;
    
    /* Check for `--' prefix */
    if (argv[i][0] != '-' || argv[i][1] != '-') {
      goto keep_it;		/* Not an option; keep it */
    }

    /* '--' breaks options parsing */
    if (!argv[i][2]) {
      argv[n++] = argv[i++];
      break;
    }

    /* Checking for sc68 prefixed options (--sc68- or --no-sc68-) */
    if (strstr(argv[i]+2,"no-sc68-") == argv[i]+2) {
      negate = 1;
      k = 2 + 8;
    } else if (strstr(argv[i]+2,"sc68-") == argv[i]+2) {
      negate = 0;
      k = 2 + 5;
    } else {
      goto keep_it;
    }
    
    for (var=vars, s=0; var<evar; ++var) {
      const int vartype = var_type(var);

      s = get_str_arg(argv[i]+k, var->name);
      if (!s) {
	continue;
      }

      if (s == argv[i]+k) {	/* found without '=' */
	if (vartype == 0) {
	  var_set_bool(var,1);	/* No arg required, set the option */
	  break;
	}
	s = (i >= argc)
	  ? 0			/* No more args */
	  : argv[++i]		/* Get next arg */
	  ;
      }

      if (vartype == 1) {
	/* This is a string option. Negate a string option just
	   ignore it but set as modified to ``freeze'' its value.
	*/
	var_set_str(var, !negate ? s : var->val.str);
      } else {
	var_set_strtol(var, s);
	if (negate)  var_set_int(var, ~var->val.num);
      }
      break;
    }

    if (var < evar)
      continue;

    /* Not our option; keep it */
  keep_it:
    argv[n++] = argv[i];
  
  }

  /* Keep remaining arguments */
  for (; i<argc; ++i) {
    argv[n++] = argv[i];
  }
  argc = n;
  
  /* Get enviromment variables */
  for (var=vars; var<evar; ++var) {
    const char * val;

    if (var_isset(var)) continue; /* already setted by options */
    val = mygetenv(var->name,1);
    if (val) {
      switch (var_type(var)) {
      case 1: var_set_str(var,val); break;
      case 0: if (!val) val = "yes";
      case 2: var_set_strtol(var,val); break;
      }
    }
  }

  /* Check for --sc68-no-debug */
  var = var_of("no-debug");
  if (var && var->val.num) {
    /* Remove all debug messages whatsoever. */
    debugmsg68_set_handler(0);
  }

  /* Check for --sc68-debug= */
  var = var_of("debug");
  if (var && var_isset(var)) {
    debugmsg68_mask = var->val.num;
  }

  /* Get share path from registry */
  var = var_of("data");
  if (var) {

    /* Get data path from registry */
    if (!var_isset(var)) {
      char * e;
      const char path[] = "Resources";
      e = get_reg_path(registry68_rootkey(REGISTRY68_LMK),
		       "SOFTWARE/sashipa/sc68/Install_Dir",
		       tmp, sizeof(tmp));
      if (e && (e+sizeof(path) < tmp+sizeof(tmp))) {
	memcpy(e, path, sizeof(path));
	var_set_str(var,tmp);
      }
    }

    /* Setup new data path */
    if (var_isset(var)) {
      rsc68_set_share(var->val.str);
    }

  }

  /* Get user path from registry (prior to HOME) */
  var=var_of("home");
  if (var) {

    /* Get user path from registry */
    if (!var_isset(var)) {
      char * e;
      const char path[] = "sc68";
      e = get_reg_path(registry68_rootkey(REGISTRY68_CUK),
		       "Volatile Environment/APPDATA",
		       tmp, sizeof(tmp));
      if (e && (e+sizeof(path) < tmp+sizeof(tmp))) {
      memcpy(e, path, sizeof(path));
      var_set_str(var,tmp);
      }
    }
    
    /* Get user path from HOME */
    if (!var_isset(var)) {
      const char path[] = "/.sc68";
      const char * env = mygetenv("HOME",0);
      if(env && strlen(env)+sizeof(path) < sizeof(tmp)) {
	strncpy(tmp,env,sizeof(tmp));
	strcat68(tmp,path,sizeof(tmp));
	/* $$$ We should test if this directory actually exists */
	var_set_str(var,tmp);
      }
    }

    /* Setup new user path */
    if (var_isset(var)) {
      rsc68_set_user(var->val.str);
    }

  }

  /* Setup new music path */
  var = var_of("music");
  if (var && var_isset(var)) {
    rsc68_set_music(var->val.str);
  }

  /* Setup new remote path */
  var = var_of("remote");
  if (var && var_isset(var)) {
    rsc68_set_remote_music(var->val.str);
  }

  init = 1;
 out_no_init:
  return argc;
}

void file68_shutdown(void)
{
  if (init == 1) {
    init = 2;
    istream68_curl_shutdown();
    rsc68_shutdown();
    init = 0;
  }
}

void file68_option_help(void * cookie, init68_help_t fct)
{
  if (fct) {
    char option[64] = "--sc68-", envvar[64];
    struct optvar68_s * var;
    for (var=vars; var<evar; ++var) {
      strncpy(option+7,var->name,sizeof(option)-8);
      opt2env(envvar, sizeof(envvar)-1,option+2);
      switch (var_type(var)) {
      case 0: break;
      case 1: strcat68(option,"=path",sizeof(option)); break;
      case 2: strcat68(option,"=#",sizeof(option)); break;
      }
      fct (cookie, option, envvar, var->desc);
    }
  }
}
