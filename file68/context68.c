/*
 *                      fil68 - Instance Context
 *         Copyright (C) 2001-2007 Benjamin Gerard <ben@sashipa.com>
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
# include "config_file68.h"
#endif

#include "file68_api.h"
#include "context68.h"
#include "alloc68.h"

#include <string.h>
#include <stdio.h>


static const char default_id[] = "default";
context68_t context68_default;

int context68_init(void)
{
  return -!(context68_create(&context68_default, default_id));
}

void context68_shutdown(void)
{
}

context68_t * context68_create(context68_t * context, const char * id)
{
  int allocated = !context;
  
  if (allocated) {
    context = calloc68(sizeof(context68_t));
  }
  if (context) {
    const int max = sizeof(context->id)-1;
    if (id) {
      strncpy(context->id, id, max);
    } else {
      snprintf(context->id, max, "%p", (void*)context);
    }
    context->created   = -1;
    context->allocated = allocated;
  }
  return context;
}


void context68_destroy(context68_t * context)
{
  if (context && context->created && context->allocated) {
    free68(context);
  }
}
