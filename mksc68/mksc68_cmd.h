/* $Id$ */

#ifndef _MKSC68_CMD_H_
#define _MKSC68_CMD_H_

#include "mksc68_def.h"

typedef struct cmd_s cmd_t;

struct cmd_s {
  int   (* run)(cmd_t*,int,char**);   /* execute the command        */
  char   * com;              /* command name                        */
  char   * alt;              /* alternate command name ( shortcut ) */
  char   * use;              /* usage strng                         */
  char   * des;              /* command short description           */
  char   * hlp;              /* command long help                   */
  void   * prv;              /* command private data                */
  cmd_t  * nxt;              /* next command in list                */
};

EXTERN68 cmd_t * cmd_lst(void);
EXTERN68 int     cmd_add(cmd_t *);
EXTERN68 int     cmd_run(int argc, char ** argv);
EXTERN68 char  * cmd_cur(void);

#endif /* ifndef _MKSC68_CMD_H_ */
