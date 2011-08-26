#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <assert.h>
#include <string.h>

#include "mksc68_cmd.h"
#include "mksc68_msg.h"

static cmd_t * head;

cmd_t * cmd_lst(void) {
  return head;
}

int cmd_add(cmd_t * cmd)
{
  assert(cmd);
  assert(cmd->com);
  assert(cmd->des);

  if (!cmd) {
    return -1;
  }
  if (cmd->nxt) {
    msgerr("command '%s' already in used\n", cmd->com);
    return -1;
  }
  if (!cmd->hlp) cmd->hlp = cmd->des;
  cmd->nxt = head;
  head = cmd; 
  msgdbg("command '%s' added\n", cmd->com);
  return 0;
}

static char * running_cmd;

char * cmd_cur(void)
{
  return running_cmd ? running_cmd : PACKAGE_NAME;
}

int cmd_run(int argc, char ** argv)
{
  cmd_t * cmd;
  int err = 0;

  if (argc >= 1) {
/*     msgdbg("run: looking for command [%s]\n", argv[0]); */
    for (cmd=head;
         cmd && strcmp(argv[0],cmd->com) &&
           (!cmd->alt || strcmp(argv[0],cmd->alt));
         cmd=cmd->nxt)
      ;
/*       msgdbg("run: not '%s' nxt:%p\n", cmd->com, cmd->nxt); */
    
    if (!cmd) {
      running_cmd = 0;
      msgerr("%s: command not found\n", argv[0]);
      err = -1;
    } else {
      running_cmd = cmd->com;
      msgdbg("run: command '%s' (%d arg)\n", cmd->com, argc);
      err = cmd->run(cmd, argc, argv);
      msgdbg("run: %s: returns %d\n", cmd->com, err);
      running_cmd = 0;
    }
  }

  return err;
}
