/*
 *                      debug68 - command line
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "debug68_cli.h"

#ifdef HAVE_READLINE_READLINE_H
# include <readline/readline.h>
#else
# undef HAVE_READLINE_HISTORY_H
/* A very simple readline() replacement */
static char *readline (const char *prompt)
{
  char tmp[1024], *s;
  if (prompt) {
    fputs(prompt, stdout);
  }
  s = fgets(tmp, sizeof(tmp)-1, stdin);
  if (s) {
    s[sizeof(tmp)-1] = 0;
    s = strdup(s);
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

#define MAX_COMARG 15

static char * killspace(char *s)
{
  while (*s && !isgraph(*s))
    s++;
  return s;
}

/* Get word start & end. Could be inside quote.
 * Returns wordend or 0 is no word.
 */
static char * word(char * word, char ** wordstart)
{
  word = killspace(word);
  if (!word[0]) {
    wordstart = 0;
    return 0;
  } else if (word[0] == '"') {
    *wordstart = ++word;
    for (; *word && *word != '"'; word++);
  } else {
    *wordstart = word;
    for (; *word && !isspace(*word); word++);
  }
  if (*word) {
    *word++ = 0;
  }
  return word;
}

static int dispatch_word(char ** here, int max_args, char *str)
{
  int i;
  for (i = 0; i < max_args && str; i++) {
    str = word(str, here + i);
  }
  return i - 1;
}

/** Free command line. */
void debug68_cli_free(debug68_cli_t * cli)
{
  if (cli) {
    free(cli->cli);
    memset(cli,0,sizeof(*cli));
  }
}

int debug68_cli_read(const char * prompt, debug68_cli_t * cli)
{
  const int max_coms = sizeof(cli->coms)/sizeof(*cli->coms)-1;
  char * e;

  if (!cli) {
    return 0;
  }

  debug68_cli_free(cli);

  cli->cli = readline(prompt);
  if (!cli->cli) {
    return 0;
  }

  /* Remove trailing space */
  for (e = cli->cli+strlen(cli->cli); --e >= cli->cli && !isgraph(*e); )
    ;
  e[1] = 0;

  e = killspace(cli->cli);
  /* No command line */
  if (!*e) {
    debug68_cli_free(cli);
    return 0;
  }

  /* Add to history before word parsing. */
  add_history(e);

  /* Process */
  cli->argc =
    dispatch_word(cli->coms, max_coms, e);

  if (cli->argc <= 0) {
    debug68_cli_free(cli);
    return 0;
  }

  return cli->argc;
}
