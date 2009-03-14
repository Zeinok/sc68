/*
 *                  debug68 - expression evaluator
 *
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
 * $Id: sc68.c 57 2009-01-31 18:24:54Z benjihan $
 */

#include "debug68_eval.h"
#include "emu68/srdef68.h"

#include <ctype.h>

/* Operator pririority level */
#define ADD_LEVEL 0x80
#define SUB_LEVEL ADD_LEVEL
#define MUL_LEVEL 0xc0
#define DIV_LEVEL MUL_LEVEL
#define AND_LEVEL 0x40
#define ORR_LEVEL AND_LEVEL
#define EOR_LEVEL AND_LEVEL

/* Prototype operator caller */
#define OP(NAME,OP) static int NAME(int a, int b){ return a OP b; }

OP(add,+)
OP(sub,-)
OP(mul,*)
OP(div,/)
OP(and,&)
OP(orr,|)
OP(eor,^)

/* Operator look-up table ( 0 terminated )
 */
static struct
{
  char symb;
  int  level;
  int  (*fct)(int,int);
} optable[] =
{
  { '+', ADD_LEVEL, add },
  { '-', SUB_LEVEL, sub },
  { '*', MUL_LEVEL, mul },
  { '/', DIV_LEVEL, div },
  { '&', AND_LEVEL, and },
  { '|', ORR_LEVEL, orr },
  { '^', EOR_LEVEL, eor },
  { 0 , 0 , 0 }
};

/* Retrieve unsigned decimal value
 */
static int get_decimal(char **ps, char **error)
{
  int v=0;
  char *s=*ps;
  if(!isdigit(*s))
  {
    *error=s;
    return 0;
  }
  for(;isdigit(*s);s++)
    v = v*10 + *s-'0';
  *ps = s;
  return v;
}

/* Retrieve unsigned hexadecimal value
 */
static int get_hexa(char **ps, char **error)
{
  int v=0;
  char *s=*ps;
  if(!isxdigit(*s))
  {
    *error=s;
    return 0;
  }
  for(;isxdigit(*s);s++)
  {
    v<<=4;
    if(*s<='9')      v+=*s-'0';
    else if(*s<='F') v+=*s-'A'+10;
    else             v+=*s-'a'+10;
  }
  *ps = s;
  return v;
}

/* Retrieve 68K register value (d0-d7, a0-a7, pc, sp, sr ,usp, ccr)
 */
static int get_reg(emu68_t * emu68, char **ps, char **error)
{
  reg68_t * const reg68 = &emu68->reg;
  char *s=*ps;
  int  regbase = -1;

  switch(toupper(s[0]))
  {
  case 'D': regbase = 0; break; /* D0-D7 */
  case 'A': regbase = 8; break; /* A0-A7 */
  case 'P': /* PC */
    if(toupper(s[1])=='C')
    {
      *ps = s+2;
      return reg68->pc;
    }
    break;
  case 'S':
    switch(toupper(s[1]))
    {
    case 'R': /* SR */
      *ps = s+2;
      return reg68->sr;
    case 'P': /* SP */
      *ps = s+2;
      return reg68->a[7];
    }
    break;
  case 'U': /* USP */
    if(toupper(s[1])=='S' && toupper(s[2])=='P')
    {
      *ps = s+3;
      return reg68->usp;
    }
    break;
  case 'C': /* CCR */
    if(toupper(s[1])=='C' && toupper(s[2])=='R')
    {
      *ps = s+3;
      return GET_CCR(reg68->sr);
    }
    break;
  }
  if(regbase>=0 && s[1]>='0' && s[1]<='7')
  {
    *ps = s+2;
    return reg68->d[regbase+s[1]-'0'];
  }
  else *error = s;
  return 0;
}

/* Recursif eval
 * s is a pointer to current string parsing
 * error is a pointer to string location where error occurs : first call with NULL
 * oplvl is previous operator level : first call with 0
 */
static int r_eval(emu68_t * emu68, char **s, char **error, int oplvl)
{
/*   reg68_t * const reg68 = &emu68->reg; */
  static int level = 0; /* Bracket level */
  int v=0;
  int c;

  if(*error) return v;

  //printf( "entering:%s oplvl=%d\n",*s,oplvl);

  /* First we need an operand  !!! */
  c = **s;
  (*s)++;

  //printf( "first char:%c\n",c);

  switch(c)
  {
    /* Open Bracket */
  case '(':
    level++;
    v = r_eval(emu68, s, error, 0);
    break;

    /* Unaire */
  case '+':
    v =  r_eval(emu68, s, error, oplvl);
    break;
  case '-':
    v = -r_eval(emu68, s, error, oplvl);
    break;
  case '~':
    v = ~r_eval(emu68, s, error, oplvl);
    break;

    /* Indirect 68K memory */
  case 'b': case 'w': case 'l':
  {
    u32 addr = r_eval(emu68, s, error, oplvl);
    if(!emu68->mem) v=0;
    else {
      v = emu68->mem[ (addr++) & emu68->memmsk ];
      if(c!='b')
        v = (v<<8) | emu68->mem[ (addr++) & emu68->memmsk ];
      if(c=='l')
      {
        v = (v<<8) | emu68->mem[ (addr++) & emu68->memmsk ];
        v = (v<<8) | emu68->mem[ (addr++) & emu68->memmsk ];
      }
    }
    break;
  }

  /* Number */
  case '0':
    (*s)--;
  case 'x':
  case '$':
    v = get_hexa(s,error);
    break;

  case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
    (*s)--;
  case '\\':
    v = get_decimal(s,error);
    break;
    /* Default is 68K register , else error is setted by get_reg() */
  default:
    (*s)--;
    v = get_reg(emu68, s, error);
    break;
  }
  if(*error) goto Error;

  while(1)
  {
    /* Second, an operator would be a good thing !!! */
    c = **s;
    (*s)++;
    //printf("read:%c\n",c);

    /* End of string */
    if(!c || isspace(c))
    {
      if(level) goto Error; /* Prematured end : all bracket not close */
      (*s)--;
      return v;
    }
    else if(c==')')
    {
      if(level<=0) goto Error; /* To many closing bracket */
      level--;
      return v;
    }

    else
    {
      int i,w;
      for(i=0; optable[i].symb; i++)
      {
        /* Operator found !!! */
        if(optable[i].symb==c)
        {
          /* Operator level is less or equal */
          if(optable[i].level <= oplvl)
          {
            (*s)--; /* rewind operator */
            //printf("return: v=%d lvl=%d s=%s\n",v,oplvl,*s);
            return v;
          }

          /* */
          else
          {
            w = r_eval(emu68, s, error, optable[i].level);
            if(*error) goto Error;
            if(c=='/' && !w) goto Error; /* 0 Divide check */
            v = (optable[i].fct)(v,w);
            break;
          }
        }
      }

      /* Nothing we know : syntax error */
      if(!optable[i].symb) goto Error;

      //printf("loop: v=%d lvl=%d s=%s\n",v,oplvl,*s);
    }
  }

  Error:
  if(!*error) *error = (*s)-1;
  level = 0;
  return v;
}

/* Evaluate a string 0 or space terminated
 * s       : string to evaluate
 * err_loc : pointer to error location
 *           *err_loc is a position to syntax error or NULL
 */
int debug68_eval(emu68_t * emu68, char *s, char **err_loc)
{
  if(!s || !err_loc) return 0;
  *err_loc = 0;
  return r_eval(emu68, &s, err_loc, 0);
}
