/* $Id$ */

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "mksc68_tag.h"
#include "mksc68_msg.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static struct tag_std {
  const char * var;                     /* tag name           */
  int alias;                            /* is alias           */
  const char * des;                     /* tag description    */
} stdtags[] = {
  { 0, 0},                              /*  */
  { "artist",    0, 0 },
  { "author",    1, "artist" },
  { "composer",  0, 0 },
  { "title",     0, 0 },
  { "track",     0, 0 },
  { "tracks",    0, 0 },
  { "ripper",    0, 0 },
  { "converter", 0, 0 },
  { "comment",   0, 0 },
};

static
const int max = sizeof(stdtags) / sizeof(*stdtags);

static void setup_stdtags(void)
{
  int i,j;

  for (i=1; i<max; ++i) {
    if (!stdtags[i].des)
      stdtags[i].des = "no description";
    if (!stdtags[i].alias)
      continue;
      for (j=1; j<max && strcmp(stdtags[i].des,stdtags[j].var); ++j)
        ;
      if (j == max) {
        msgerr("unable to setup %s alias for %s\n",
               stdtags[i].des, stdtags[i].var);
        j = 0;
      }
      stdtags[i].alias = j;
  }
}

int tag_std(int i, const char ** var, const char ** des)
{
  const int max = sizeof(stdtags)/sizeof(*stdtags);
  static int setup = 0;

  if (!setup) {
    setup = 1;
    setup_stdtags();
  }

  if (i<1 || i>=max)
    return -1;

  if (var) *var = stdtags[i].var;
  if (des) *des = stdtags[i].des;

  return stdtags[i].alias;
}

static
tag_t * tag_find(tag_t * head, const char * var)
{
  tag_t * tag;
  for (tag=head; tag && strcmp(tag->var,var); tag = tag->nxt)
    ;
  return tag;
}

static
int tag_valid_var(const char * var)
{
  int i,l;

  if (!var) return 0;
  if (!isalpha(var[0])) {
    msgerr("tag name must begin by an alpha char\n");
    return 0;
  }
  for (i = 1; i < l; ++i) {
    if (var[i] != '_' && !isalnum(var[i])) {
      msgerr("tag name char must be letter digit or underscore\n");
      return 0;
    }
  }
  return 1;
}

static int tag_valid_val(const char * val)
{
  /* TODO: test valid encoding */
  return val && val[0];
}

tag_t * tag_get(tag_t ** head, const char * var)
{
  tag_t * tag;
  assert(head);
  assert(var);
  tag = tag_find(*head, var);
  return tag;
}

static inline void detach(tag_t ** head, tag_t * tag)
{
  if ( ( *( !tag->prv ? head : &tag->prv->nxt ) = tag->nxt ) != 0 )
    tag->nxt->prv = tag->prv;
}

static inline void attach(tag_t ** head, tag_t * tag)
{
  tag->prv = 0;
  tag->nxt = *head;
  *head    = tag;
}

tag_t * tag_set(tag_t ** head, const char * var, const char * val)
{
  tag_t * tag;
  assert(head);
  assert(var);
  assert(strlen(var));

  /* $$$ tag-name validity check */
  tag = tag_find(*head,var);
  if (tag) {
    /* tag exists. */
    if (!val) {
      /* have to destroy it. */
      free(tag->var); free(tag->val);
      detach(head, tag);
      free(tag);
      tag = 0;
    } else if (strcmp(val,tag->val)) {
      /* only update if value is different. */
      char * newval = strdup(val);
      if (newval) {
        free(tag->val);
        tag->val = newval;
      } else {
        tag = 0;
      }
    }
  } else if (val && tag_valid_var(var) && tag_valid_val(val))  {
    /* tag does not exit and have to be created */
    tag = malloc(sizeof(*tag));
    if (tag) {
      tag->var = strdup(var);
      tag->val = strdup(val);
      if (tag->var && tag->val) {
        attach(head, tag);
      } else {
        free(tag->var); free(tag->val); free(tag);
        tag = 0;
      }
    }
  }

  return tag;
}

void tag_del(tag_t ** head, const char * tag)
{
  tag_set(head, tag, 0);
}

void tag_clr(tag_t ** head)
{
  tag_t * tag, * nxt;
  assert(head);
  for (tag=*head; tag; tag=nxt) {
    nxt = tag->nxt;
    free(tag->var);
    free(tag->val);
    free(tag);
  }
  *head = 0;
}
