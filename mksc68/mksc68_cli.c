/*
 *                      mksc68 - command line
 *
 *            Copyright (C) 1998-2009 Ben(jamin) Gerard
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
 * $Id: sc68.c 57 2009-01-31 18:24:54Z benjihan $
 */

#ifdef _HAVE_CONFIG_H_
# include <config.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include "mksc68_cli.h"
#include "mksc68_msg.h"

#ifdef HAVE_READLINE_READLINE_H
# include <readline/readline.h>
#else
# undef HAVE_READLINE_HISTORY_H
/* A very simple readline() replacement */
static char * readline (const char * prompt)
{
  char tmp[1024], *s;
  if (prompt) {
    fputs(prompt, stdout);
  }
  errno = 0;
  s = fgets(tmp, sizeof(tmp)-1, stdin);
  if (s) {
    s[sizeof(tmp)-1] = 0;
    s = strdup(s);
  } else if (errno) {
    msgerr("readline: %s\n", strerror(errno));
  }

  return s;
}
#endif

#ifdef HAVE_READLINE_HISTORY_H
# include <readline/history.h>
#else
/* ... an even more simple add_history() replacement */
static void add_history(const char *s) {}
#endif

static char * cli, * prompt;

static char * killspace(char *s)
{
  while (isspace(*s)) s++;
  return s;
}

/* Get word start & end. Could be inside quote.
 * @retval next word
 */
static
char * word(char * word, char ** wordstart)
{
  char * start = word;
  int c, esc = 0, len = 0;

  word = killspace(word);
  if (!*word) {
    start = 0;
  } else {
    for ( ;; ) {
      c = *word++;
      if (!c) {
        --word;
        break;
      } else if (esc == '\\') {
        /* backslashed */
        esc = 0;
        start[len++] = c;
      } else if (esc == '"') {
      /* quoted string */
      if (c != '"')
        start[len++] = c;
      else
        esc = 0;
      } else if (c == '\\' || c == '"') {
        esc = c;
      } else if (!isspace(c)) {
        start[len++] = c;
      } else {
        break;
      }
    }
  }

  *wordstart = start;
  if (start) start[len] = 0;
  return word;
}

static
int dispatch_word(char ** here, int max, char *str)
{
  int i;

  for (i=0 ;; ++i) {
    char * start;
    str = word(str, &start);
    if (!start) break;
    assert (i < 512);
/*     msgdbg("arg[%2d]=%s\n", i, start); */
    if (i < max) {
      here[i] = start;
    } else {
      msgwrn("skipping arg[%2d] (%s)\n", i, start);
    }
  }
/*   msgdbg("cli: %d args\n", i); */
  return i;
}

void cli_release()
{
  free(prompt); prompt = 0;
  free(cli);    cli = 0;
}

char * cli_prompt(const char * fmt, ...)
{
  char * newprompt = 0;

  free(prompt);
  if (fmt) {
    va_list list;
    va_start(list,fmt);
    if (vasprintf(&newprompt,fmt,list) < 0) {
      msgerr("set prompt: %s\n", strerror(errno));
    }
    va_end(list);
  }
  return prompt = newprompt;
}

int cli_read(char * argv[], int max)
{
  free(cli);
  cli = readline(prompt);
  if (!cli) {
    return -1;
  }
  
  /* Add to history before word parsing. */
  add_history(cli);

  return
    dispatch_word(argv, max, cli);
}
