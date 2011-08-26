/* $Id$ */

#ifndef _MKSC68_TAG_H_
#define _MKSC68_TAG_H_

#include "mksc68_def.h"

typedef struct tag_s tag_t;

struct tag_s {
  tag_t  * nxt;              /**< next in list     */
  tag_t  * prv;              /**< previous in list */
  char   * var;              /**< tag name         */
  char   * val;              /**< tag value        */
};

EXTERN68 tag_t * tag_get(tag_t ** head, const char * var);
EXTERN68 tag_t * tag_set(tag_t ** head, const char * var, const char * val);
EXTERN68 void    tag_del(tag_t ** head, const char * var);
EXTERN68 void    tag_clr(tag_t ** head);
EXTERN68 int     tag_std(int i, const char ** var, const char ** des);

#endif /* ifndef _MKSC68_TAG_H_ */
