/*
 * @file    mksc68_gdb.c
 * @brief   remote gdb functions.
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-07-12 22:21:42 ben>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* generated config include */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mksc68_gdb.h"
#include "mksc68_dsk.h"
#include "mksc68_msg.h"
#include "mksc68_cmd.h"
#include "mksc68_tag.h"
#include "mksc68_str.h"

#include <sc68/file68.h>
#include <sc68/alloc68.h>
#include <sc68/istream68.h>
#include <sc68/sc68.h>
#include <emu68/emu68.h>
#include <emu68/excep68.h>
#include <io68/io68.h>


#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>


#define STATUS(S,C,M) \
  if (1) { gdb->run = RUN_##S; gdb->code = CODE_##C; gdb->msg = M; } else
#define SIGNAL(S,C,M) \
if (1) { gdb->run = RUN_##S; gdb->code = -C; gdb->msg = M; } else

/* $$$ FIXME */ extern char *stpcpy(char *dest, const char *src);

enum {
  default_port = 6800,
  default_addr = 0x7f000001
};

static uint8_t thex[16] = {
  '0','1','2','3','4','5','6','7',
  '8','9','a','b','c','d','e','f'
};

typedef struct serverinfo_s serverinfo_t;

struct serverinfo_s {
  int       fd;                         /* server listening socket */
  uint32_t  addr;                       /* bound ipv4 address      */
  uint16_t  port;                       /* bound port              */
};

serverinfo_t serverinfo = { -1, default_addr, default_port };

struct gdb_s {
  int            fd;                    /* client/server socket        */
  serverinfo_t * server;                /* gdb server                  */
  emu68_t      * emu;                   /* emu68 instance              */
  int            code;                  /* exit or stop code           */
  int            step;                  /* number of step before break */
  int            run;                   /* run mode                    */
  int            vector;                /* last interrupt vector       */
  int            sigval;                /* last signal                 */
  const char   * msg;                   /* last message                */
  char           buf[1024];             /* buffer for send/recv        */
};

static inline uint8_t Bpeek(emu68_t * const emu68, addr68_t addr)
{
  return emu68_peek(emu68, addr);
}

static uint16_t Wpeek(emu68_t* const emu68, addr68_t  addr)
{
  return (uint16_t) ( ( (uint16_t) Bpeek(emu68, addr+0) << 8 ) |
                      ( (uint16_t) Bpeek(emu68, addr+1)      ) );
}

static uint32_t Lpeek(emu68_t* const emu68, addr68_t  addr)
{
  return (uint32_t) ( ( (uint32_t) Bpeek(emu68, addr+0) << 24 ) |
                      ( (uint32_t) Bpeek(emu68, addr+1) << 16 ) |
                      ( (uint32_t) Bpeek(emu68, addr+2) <<  8 ) |
                      ( (uint32_t) Bpeek(emu68, addr+3)       ) );
}

struct trap_s {
  const char * name;
  const char * parm;
};


static int decode_trap(char * s, int trapnum, emu68_t * emu)
{
  int fct, valid = 1;
  switch (trapnum) {
  case 1:
    fct = Wpeek(emu, emu->reg.a[7]+6);
    s += sprintf(s," gemdos(%02x)", fct);
    switch (fct) {
    case 0x09:
      s += sprintf(s, " cconws($%x)", Lpeek(emu, emu->reg.a[7]+8));
      break;
    case 0x20:
      s += sprintf(s, " super($%x)" , Lpeek(emu, emu->reg.a[7]+8));
      break;
    case 0x30: s += sprintf(s, " version()");
      break;
    case 0x48: s += sprintf(s, " malloc(%d)", Lpeek(emu, emu->reg.a[7]+8));
      break;
    case 0x49: s += sprintf(s, " free($%x)",  Lpeek(emu, emu->reg.a[7]+8));
      break;
    default:
      valid = 0;
    }
    break;
  case 13:
    sprintf(s," bios(%02x)", Wpeek(emu, emu->reg.a[7]+6));
    valid = 0;
    break;
  case 14:
    sprintf(s," xbios(%02x)", Wpeek(emu, emu->reg.a[7]+6));
    valid = 0;
  default:
    valid = 0;
    *s = 0;
  }
  return valid;
}
/* GEMDOS */
/* static const struct trap_S gemdos[] = { */
/*   /\* 00 *\/ { "Pterm",    "0" }, */
/*   /\* 01 *\/ { "Cconin",   "4" }, */
/*   /\* 02 *\/ { "Cconout",  "02" }, */
/*   /\* 03 *\/ { "Cauxin",   "2" }, */
/*   /\* 04 *\/ { "Cauxout",  "02" }, */
/*   /\* 05 *\/ { "Cprnout",  "02" }, */
/*   /\* 06 *\/ { "Crawio",   "42" }, */
/*   /\* 07 *\/ { "Crawcin",  "4" }, */
/*   /\* 08 *\/ { "Cnecin",   "4" }, */
/*   /\* 09 *\/ { "Cconws",   "04" }, */



/* Create and bind ipv4/tcp server socket.
 * @return 0
 * @retval -1 on error
 */
static int server_tcp(serverinfo_t * si, int force)
{
  int fd = -1;
  struct sockaddr_in ia;

  if (si->fd != -1) {
    if (!force) {
      msgerr("already have a server socket\n");
      goto error;
    } else {
      msgwrn("forced to close previous server socket\n");
      close(si->fd);
      si->fd = -1;
    }
  }

  /* Prepare for bind */
  ia.sin_family      = AF_INET;
  ia.sin_port        = htons(si->port);
  ia.sin_addr.s_addr = htonl(si->addr);

  /* Create listen socket. */
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    msgerr("socket -- %s\n", strerror(errno));
    goto error;
  }

  /* Bind socket to ip/port address */
  if (bind(fd,(struct sockaddr*)&ia,sizeof(ia))) {
    msgerr("bind -- %s\n", strerror(errno));
    goto error;
  }
  si->fd = fd;

  return 0;

error:
  if (fd != -1)
    close(fd);
  return -1;
}

/**
 *  @retval  0  on success (packet has been ack)
 *  @retval  1  on error   (packet has not been ack)
 *  @retval -1  on failure (communication failure)
 */
static int send_packet(gdb_t * gdb)
{
  int c, i, j, sum = 0;
  char * const buf = gdb->buf;

  buf[0] = '$';
  for (i=1, sum=0; !!(c = buf[i]); ++i)
    sum += c;
  j = i;
  buf[i++] = '#';
  buf[i++] = thex[ (sum>>4) & 15 ];
  buf[i++] = thex[  sum     & 15 ];
  buf[i] = 0;

  if (send(gdb->fd, buf, i, 0) != i) goto fail;
  buf[j] = 0;
  do {
    if (recv(gdb->fd, &c, 1, 0) != 1) goto fail;
    if (c == '+') {
      return 0;
    }
  } while (c != '-');
  return 1;
fail:
  STATUS(EXIT,ERROR,strerror(errno));
  return -1;
}

/**
*  @retval  0  on success (packet has been ack)
*  @retval -1  on failure (communication failure)
*/
static int send_or_fail(gdb_t * gdb)
{
  int ret;
  while (ret = send_packet(gdb), ret > 0)
    ;
  return ret;
}

/**
 *  @retval  0  on success (packet has been ack)
 *  @retval -1  on failure (communication failure)
 */
static int send_reply(gdb_t * gdb, int type, int code)
{
  if (code < 0)
    code = -code;
  gdb->buf[1] = type;
  gdb->buf[2] = thex [ (code >> 4) & 15 ];
  gdb->buf[3] = thex [  code       & 15 ];
  gdb->buf[4] = 0;
  return send_or_fail(gdb);
}

/**
 *  @retval  0  on success
 *  @retval  1  om failure
 */
static int rsp_event(gdb_t * const gdb)
{
  char c;
  char * const buf = gdb->buf;
  char * const max = gdb->buf + sizeof(gdb->buf) - 1;
  char * ptr;

  /* read a packet */
  do {
    if (recv(gdb->fd, &c, 1, 0) != 1) goto fail;
  } while (c != '$');

  ptr = buf;
  do {
    if (recv(gdb->fd, &c, 1, 0) != 1) goto fail;
    if (c == '#') break;
    *ptr++ = c;
  } while (ptr < max);

  *ptr = 0;
  if (c != '#')
    goto fail;

  /* skip check sum */
  if (recv(gdb->fd, &c, 1, 0) != 1) goto fail;
  if (recv(gdb->fd, &c, 1, 0) != 1) goto fail;

  /* trace("got: [%s]\n", buf); */

  /* ack */
  c = '+';
  if (send(gdb->fd, &c, 1, 0) != 1) goto fail;

  ptr = buf;
  switch(*ptr) {

    /* Minimal commands for a stub to implement are 'g' 'G' 'm' 'M'
     * 'c' 's'
     */

  case '?': {
    /***********************************************************************
     * ? (why target is halted)
     */
    /* msgdbg("COMMAND <WHY?>\n"); */
    ptr = stpcpy(buf+1,"S05");          /* $$$ TODO */
  } break;

  case 'p': case 'P': {
    /***********************************************************************
     * P n...=r... (Write register)
     * p n (Read register)
     */
    int n, v, l;
    int read = *ptr == 'p';
    errno = 0;
    n = strtoul(++ptr, &ptr, 16);
    if (errno)
      break;
    /* trace("Got reg-num: %d\n", n); */
    if (read) {
      /* msgdbg("COMMAND <read-register,%02d>\n", n); */
      /* Register mapping:
         00-07 D0-D7     32-bit
         08-15 A0-A7     32-bit
         16    SR/PS     16-bit
         17    PC        32-bit
         18-25 FP0-FP7   80-bit but 64-bit for gdb
         26    FPC       32-bit
         27    FPS       32-bit
         28    FPI       32-bit
      */

      v = 0;                          /* default value */
      l = 8;                          /* default length of 8 xdigit */
      if (n < 8)
        v = gdb->emu->reg.d[n];
      else if (n < 16)
        v = gdb->emu->reg.a[n-8];
      else if (n == 16) {
        v = gdb->emu->reg.sr;
        l = 4;
      } else if (n == 17) {
        v = gdb->emu->reg.pc;
      } else if (n < 26) {
        l = 16;                         /* 64-bit ? */
      }
      ptr = buf + 1 + sprintf(buf+1,"%0*x",l,v);
      break;
    } else if (*ptr == '=') {
      char * end=++ptr;

      v = strtoul(end, &end, 16);
      if (!errno) {
        /* int l = (end-ptr)/\* , m = (1<<(l<<2))-1 *\/; */
        msgdbg("COMMAND <write-register,%02d>\n",
             n,v);
        ptr = stpcpy(buf+1,"OK");
        if (n < 8)
          gdb->emu->reg.d[n] = v;
        else if (n < 16)
          gdb->emu->reg.a[n-8] = v;
        else if (n == 16)
          gdb->emu->reg.sr = v & 0xFFFF;
        else if (n == 17)
          gdb->emu->reg.pc = v;
        else
          ptr = stpcpy(buf+1,"E05");
        break;
      }
    }
    ptr = stpcpy(buf+1,"E01");
  } break;

  case 'g': {
    /***********************************************************************
     * g (read General Registers)
     */
    int i,j;

    /* msgdbg("COMMAND <read-general-register>\n"); */
    ptr = buf+1;
    for (i=0;i<8;++i)
      for (j=0; j<8; ++j)
        *ptr++ = thex[ ( gdb->emu->reg.d[i] >> ((7-j)<<2)) & 15];
    for (i=0;i<8;++i)
      for (j=0; j<8; ++j)
        *ptr++ = thex[ ( gdb->emu->reg.a[i] >> ((7-j)<<2)) & 15];
    for (j=0; j<8; ++j)
      *ptr++ = thex[ ( gdb->emu->reg.sr >> ((7-j)<<2)) & 15];
    for (j=0; j<8; ++j)
      *ptr++ = thex[ ( gdb->emu->reg.pc >> ((7-j)<<2)) & 15];
    *ptr = 0;
  } break;

  case 'm': {
    /***********************************************************************
     * m addr,length (read memory)
     */
    int adr, len;
    errno = 0;
    adr = strtoul(++ptr, &ptr, 16);
    if (!errno && *ptr == ',') {
      len = strtoul(++ptr, &ptr, 16);
      if (!errno && !*ptr) {
        u8 * mem;
        int i,n = (max - buf - 4) >> 1;
        /* msgdbg("COMMAND <memory-read,0x%06x,%d>\n", adr,len); */
        if (len > n) {
          len = n;
        }
        mem = emu68_memptr(gdb->emu, adr, len);
        if (!mem) {
          gdb->msg = emu68_error_get(gdb->emu);
          msgnot("gdb tried to access invalid memory [$%x..$%x]\n",
                 adr,adr+len-1);
        } else {
          for (i=0, ptr = buf+1; i<len; ++i) {
            *ptr++ = thex[ mem[i] >> 4 ];
            *ptr++ = thex[ mem[i] & 15 ];
          }
          *ptr = 0;
          break;
        }
      }
    }
    ptr = stpcpy(buf+1,"E01");
  } break;

  case 'k': {
    /***********************************************************************
     * k (kill)
     */
    STATUS(EXIT,KILL,"properly killed");
    return 0;
  } break;

  case 'D': {
    /***********************************************************************
     *  D[:pid] (detach)
     */
    char * end;
    int pid = 0;

    end = strchr(ptr+1,':');
    if (end) {
      *end = 0;
      errno = 0;
      pid = strtoul(end+1, &ptr, 16);
      if (errno) pid = -1;
    }
    msgdbg("COMMAND <detach,%d>\n", pid);
    ptr = stpcpy(buf+1, "OK");
    STATUS(SKIP,DETACH,"detached");
  } break;

  case 'H': {
    /***********************************************************************
     * H op thread-id (Thread Operation)
     */
    int    op = *++ptr;
    char * tid = op ? ++ptr : 0;

    switch (op) {
    case 'g':
      msgdbg("COMMAND <thread-op-general> <%s>\n", tid);
      ptr = stpcpy(buf+1,"OK");
      break;

    case 'c':
      msgdbg("COMMAND <thread-op-continue> <%s>\n", tid);
      ptr = stpcpy(buf+1,"OK");
      break;
    default:
      ptr = stpcpy(buf+1,"E01");
    }
  } break;

  case 'Q': case 'q': {
    /***********************************************************************
     * q Query (General Query)
     * Q Query (General Set)
     */
    int query = *ptr++ == 'q';
    char * name;
    char * end;

    /* looking for query name separator */
    name = ptr;
    end = strchr(ptr,':');
    if (end) {
      *end = 0;
      ptr = end+1;
    } else {
      ptr = name + strlen(name);
    }

    /* First looking for legacy queries qC, qP and qL */
    if (query && !strcmp(name, "C")) {
      /* query current thread */
      msgdbg("COMMAND <query-thread-id>\n");
      sprintf(ptr = buf+1, "QC%08x", 5568);
    } else if (query &&  !strcmp(name, "P")) {
      /* qP mode thread-id ( */
      /* Deprecated by qThreadExtraInfo */
      ptr = 0;
    } else if (query && !strcmp(name, "L")) {
      /* qL startflag threadcount nextthread */
      /* Deprecated by qfThreadInfo */
      ptr = 0;
    } else if (query && !strcmp(name,"Offsets")) {
      /* query section relocations */
      msgdbg("COMMAND <query-offsets>\n");
      ptr = buf + 1 + sprintf(buf+1, "Text=%x;Data=%x;Bss=0",0,0);
    } else if (!query && !strcmp(name,"NonStop")) {
      switch (*ptr) {
      case '0':                       /* all-stop */
        msgdbg("COMMAND <set-all-stop>\n");
        ptr = stpcpy(buf+1,"OK");
        break;
      case '1':                       /* non-stop */
        msgdbg("COMMAND <set-non-stop>\n");
        ptr = stpcpy(buf+1,"E01");
        break;
      default:
        ptr = 0;
      }
    } else if (query && !strcmp(name,"Supported")) {
      if (*ptr) {
        char * feat = ptr, *end, *rep;
        msgdbg("SUPPORTED[%s]\n",ptr);
        ptr = buf+1;
        msgdbg("COMMAND <query-suported,%s>\n",feat);
        while (*feat) {
          rep = 0;
          end = strchr(feat, ';');
          if (end) *end = 0;
          if (!strncmp(feat, "multiprocess", 12))
            rep="multiprocess-";
          else if (!strncmp(feat, "xmlRegisters", 12))
            rep="xmlRegisters-";
          else if (!strncmp(feat, "qRelocInsn", 10))
            rep="qRelocInsn-";
          if (rep) {
            if (ptr != buf+1)
              *ptr++ = ';';
            ptr = stpcpy( ptr, rep);
          }
          feat = end ? end+1 : feat+strlen(feat);
        }
      }
    } else if (query && !strcmp(name,"Attached")) {
      ptr = stpcpy(buf+1, "0");       /* new process */
      ptr = stpcpy(buf+1, "1");       /* attached to an existing process */
    } else if (query && !strcmp(name,"TStatus")) {
      /* Tracepoints status ( )*/
      msgdbg("COMMAND <query-tracepoint-status>\n");
      ptr = stpcpy(buf+1, "T0;tnotrun:0");
      ptr = 0;                        /* force unsupported */
    } else {
      ptr = 0; /* stpcpy(buf+1,"E55"); */
    }
  } break;

  case 'z': case 'Z': {
    /***********************************************************************
     * z type,addr,kind (remove breakpoint)
     * Z type,addr,kind (insert breakpoint)
     */

    static const char * types[5] = {
      "software-breakpoint",
      "hardware-breakpoint",
      "write-watchpoint",
      "read-watchpoint",
      "access-watchpoint" };


    int remove = *ptr++ == 'z';
    int type   = *ptr++;
    int adr    = gdb->emu->reg.pc;    /* $$$ default to pc ? */
    int kind   = -1;                  /* undef */

    if (type < '0' || type > '4')     /* Those are only know types */
      break;

    if (*ptr == ',') {
      errno = 0;
      adr = strtoul(++ptr, &ptr, 16);
      if (errno) {
        ptr = stpcpy(buf+1,"E01");
        break;
      }
    }

    if (*ptr == ',') {
      errno = 0;
      kind = strtoul(++ptr, &ptr, 16);
      if (errno) {
        ptr = stpcpy(buf+1,"E02");
        break;
      }
    }

    msgdbg("COMMAND <%s-%s,%x,%d%s>\n",
         remove?"remove":"insert", types[type-'0'],
         adr, kind, ptr);

    switch (type) {
    case '0':
      /* software breakpoint. using hardware right now */

    case '1':
      /* hardware breakpoint. Unsupported by target according to gdb
       * which is really annoying if you ask me. gdb should not
       * assume such things when conencted to any kind of remote
       * target.
       */
      if (remove) {
        emu68_bp_del(gdb->emu,
                     emu68_bp_find(gdb->emu,adr));
      } else if (emu68_bp_set(gdb->emu, -1, adr, 1, 1) < 0) {
        ptr = stpcpy(buf+1,"E03");
        break;
      }
      break;
    default:
      ptr = 0;                        /* unsupported */
      break;
    }
  } break;

  case 's': case 'c': case 'C': {
    /***********************************************************************
     * c [addr] (continue)
     * s [addr] (step)
     */
    int cmd  = *ptr;
    int step = cmd == 's';
    int sig  = -1;

    if (!*++ptr) {
      /* msgdbg("COMMAND <%s>\n", step ? "step" : "continue"); */
    } else {
      int adr;
      errno = 0;
      if (cmd == 'C') {
        sig = strtoul(++ptr, &ptr, 16);
        if (errno) {
          ptr = stpcpy(buf+1,"E02");
          break;
        }
        if (*ptr != ';') *ptr = 0;
      }
      adr = strtoul(++ptr, &ptr, 16);
      if (errno) {
        ptr = stpcpy(buf+1,"E01");
        break;
      }
      msgdbg("COMMAND <%s-at,%02d,%06x>\n", step ? "step" : "continue", sig, adr);
      gdb->emu->reg.pc = adr;
    }

    if (step) {
      gdb->step = 1;                    /* 1 */
    } else {
      gdb->step = 0;                    /* 0 is unlimited */
    }
    gdb->run  = RUN_CONT;

    switch (sig) {
    case 1: case 2: case 3: case 6:
      SIGNAL(EXIT,sig,"signal");
      sprintf(ptr = buf+1,"X%02u", sig % 100u);
      break;
    default:
      return 0;                         /* skip sending reply */
    }
  } break;

  default:
    ptr = 0;
  }

  /* any command not supported by the stub, an empty response
   * (‘$#00’) should be returned if (!ptr) { ptr=buf+1; *ptr = 0; }
   */
  if (!ptr) {
    ptr=buf+1;
    *ptr = 0;
  }

  return send_or_fail(gdb);

fail:
  STATUS(EXIT,ERROR,strerror(errno));
  return -1;
}

int gdb_event(gdb_t * gdb, int vector, void * emu)
{
  int  pc, sr;
  char irqname[32], fctname[32];

  assert (gdb->run == RUN_CONT);

  /* Escape this as fast as possible */
  if (vector == HWTRACE_VECTOR) {
    if (!gdb->step || --gdb->step)
      return gdb->run;
    gdb->sigval = SIGVAL_TRAP;
    SIGNAL(STOP,gdb->sigval,"trace");
  }

  gdb->emu = emu;
  emu68_exception_name(vector,irqname);
  fctname[0] = 0;

  if (vector < 0x100) {
    /***********************************************************************
     * 68k exceptions
     */
    int sigval = SIGVAL_0;

    sr = Wpeek(gdb->emu, gdb->emu->reg.a[7]);
    pc = Lpeek(gdb->emu, gdb->emu->reg.a[7]+2);

    switch (vector) {
    case BUSERR_VECTOR: case ADRERR_VECTOR:
      sigval = SIGVAL_BUS; break;
    case ILLEGAL_VECTOR:
      /* sc68 trap emulator will set regiter d1 to 0xDEAD000* in case
       * a trap function is not implemented */
      sigval = (gdb->emu->reg.d[1] >= 0xDEAD0000 && gdb->emu->reg.d[1] <= 0xDEAD000F)
        ? SIGVAL_SYS
        : SIGVAL_ILL
        ;
      break;
    case DIVIDE_VECTOR: case TRAPV_VECTOR:
      sigval = SIGVAL_FPE; break;
    case LINEA_VECTOR: case LINEF_VECTOR: case CHK_VECTOR:
      sigval = SIGVAL_EMT; break;
    case TRACE_VECTOR:
      sigval = SIGVAL_TRAP; break;
    case PRIVV_VECTOR:
      sigval = SIGVAL_SEGV; break;

      /* Timer interruptions: not always right as it depends on MFP VR
       * register bits 4-7
       */
    case 0X134 >> 2: /* Timer-A */
      strcpy(fctname," timer-A"); break;
    case 0X120 >> 2: /* Timer-B */
      strcpy(fctname," timer-B"); break;
    case 0X114 >> 2: /* Timer-C */
      strcpy(fctname," timer-C"); break;
    case 0X110 >> 2: /* Timer-D */
      strcpy(fctname," timer-D"); break;

    case SPURIOUS_VECTOR:
    default:
      if  (vector >= TRAP_VECTOR(0) && vector <= TRAP_VECTOR(15)) {
        sigval = decode_trap(fctname, vector-TRAP_VECTOR(0), gdb->emu)
        ? SIGVAL_TRAP
        : SIGVAL_SYS
        ;
      } else
        sigval = SIGVAL_TRAP;
    }
    gdb->sigval = sigval;
    if (sigval != SIGVAL_0) {
      SIGNAL(STOP,sigval,"68k exception");
    }
  } else {
    /***********************************************************************
     * Private exceptions
     */
    sr = gdb->emu->reg.sr;
    pc = gdb->emu->reg.pc;

    if (vector < HWTRACE_VECTOR) {
      /* Got a breakpoint ! */
      gdb->sigval = SIGVAL_TRAP;
      SIGNAL(STOP,gdb->sigval,"breakpoint");
    } else if (vector == HWHALT_VECTOR) {
      gdb->sigval = SIGVAL_ABRT;
      STATUS(EXIT, HALT, "halted");
    } else if (vector == HWSTOP_VECTOR) {
      sprintf (irqname,"stop-#%04x",sr);
      if ((sr & 0x0700) == 0x0700) {
        gdb->sigval = SIGVAL_ABRT;
        STATUS(EXIT, STOP, "stopped");
      } else {
        gdb->sigval = SIGVAL_STOP;
        SIGNAL(STOP, gdb->sigval, "stopped");
      }
    } else if (vector != HWTRACE_VECTOR) {
      /* Unhamdled special exception */
      gdb->sigval = SIGVAL_0;
      SIGNAL(STOP,gdb->sigval,"private exception");
    }
  }

  if (gdb->run != RUN_CONT)
    msgnot("68k exception in <%s>\n"
           "  vector : %02x (%d)\n"
           "  type   : %s%s\n"
           "  from pc:%08x sr:%04x\n"
           "       pc:%08x sr:%04x\n"
           "       d0:%08x d1:%08x d2:%08x d3:%08x\n"
           "       d4:%08x d5:%08x d6:%08x d7:%08x\n"
           "       a0:%08x a1:%08x a2:%08x a3:%08x\n"
           "       a4:%08x a5:%08x a6:%08x a7:%08x\n",
           gdb->emu->name,
           vector, gdb->sigval,
           irqname, fctname,
           pc, sr,
           gdb->emu->reg.pc, gdb->emu->reg.sr,
           gdb->emu->reg.d[0], gdb->emu->reg.d[1],
           gdb->emu->reg.d[2], gdb->emu->reg.d[3],
           gdb->emu->reg.d[4], gdb->emu->reg.d[5],
           gdb->emu->reg.d[6], gdb->emu->reg.d[7],
           gdb->emu->reg.a[0], gdb->emu->reg.a[1],
           gdb->emu->reg.a[2], gdb->emu->reg.a[3],
           gdb->emu->reg.a[4], gdb->emu->reg.a[5],
           gdb->emu->reg.a[6], gdb->emu->reg.a[7]);

  for (;;) {
    switch (gdb->run) {

    case RUN_STOP:
      if (send_reply(gdb, 'S', gdb->code) < 0)
        goto exit;
      STATUS(IDLE,IDLE,"waiting for gdb commands");
      msgnot("waiting for gdb commands\n");
    case RUN_IDLE:
      if (rsp_event(gdb) < 0)
        goto exit;
      break;

    case RUN_SKIP:
      if (gdb->code == CODE_DETACH)
        goto exit;
    case RUN_EXIT:
      send_reply(gdb, 'X', gdb->code);
    case RUN_CONT:
      goto exit;

    default:
      assert(!"unexpected run status");
    }
  }

exit:
  return gdb->run;
}

void gdb_destroy(gdb_t * gdb)
{
  if (gdb) {

    if (gdb->fd != -1)
      close(gdb->fd);

    if (gdb->server->fd != -1) {
      close(gdb->server->fd);
      gdb->server->fd = -1;
    }

    free(gdb);
  }
}


gdb_t * gdb_create(void)
{
  gdb_t * gdb = calloc(sizeof(*gdb),1);

  gdb_get_conf(0,0);                    /* trick: load default */

  if (gdb) {
    gdb->fd     = -1;
    gdb->server = &serverinfo;
    if (server_tcp(gdb->server, 1) < 0)
      goto error;

    STATUS(CONT,IDLE,"started");
    gdb->step = 1;

    /* Make it passive. */
    if (listen(gdb->server->fd,1)) {
      msgerr("listen -- %s\n", strerror(errno));
      goto error;
    }

    /* Wait for a client to connect */
    msginf("waiting for gdb to connect:\n"
           "       *** Try gdb command 'target remote %d.%d.%d.%d:%d'\n",
           (gdb->server->addr >> 24) & 255,
           (gdb->server->addr >> 16) & 255,
           (gdb->server->addr >>  8) & 255,
           (gdb->server->addr >>  0) & 255,
           gdb->server->port);

    gdb->fd = accept(gdb->server->fd , 0, 0);

    /* Don't need that socket anymore */
    close(gdb->server->fd);
    gdb->server->fd = -1;

    if (gdb->fd == -1)
      goto error;
  }
  return gdb;

error:
  gdb_destroy(gdb);
  return 0;
}


int gdb_conf(char * uri)
{
  int ret = 0;
  uint16_t port = default_port;
  uint32_t addr = default_addr;
  /* struct in_addr ia; */

  if (uri) {
    char * s = strchr(uri,':');

    if (s) {
      unsigned long p;
      *s = 0;
      p = strtoul(s+1,0,10);
      if (!p || p > 0xffff) {
        p = 0;
        msgerr("%s -- invalid port number\n",s+1);
        ret = -2;
      }
      port = p;
    }

    /* convert host part */
    if (!*uri || !strcmp(uri,"*")) {
      addr = ntohl(INADDR_ANY);
    } else {
      struct hostent *he = gethostbyname(uri);
      if (he) {
        addr = ntohl(((struct in_addr *)he->h_addr)->s_addr);
      } else {
        msgerr("%s -- '%s'\n", uri, hstrerror(h_errno));
        ret = -1;
      }
    }
  }

  if (!ret) {
    serverinfo.addr = addr;
    serverinfo.port = port;
  }
  /* ia.s_addr = htonl(serverinfo.addr); */

  return ret;
}

void gdb_get_conf(unsigned char * addr, int * port)
{
  if (!serverinfo.port || serverinfo.addr == INADDR_NONE)
    gdb_conf(0);

  if (port)
    *port = serverinfo.port;

  if (addr) {
    addr[0] = serverinfo.addr >> 24;
    addr[1] = serverinfo.addr >> 16;
    addr[2] = serverinfo.addr >>  8;
    addr[3] = serverinfo.addr;
  }
}

int gdb_error(gdb_t * gdb, const char ** errp)
{
  int ret = -1;

  if (gdb) {
    ret = gdb->code;
    if (errp)
      *errp = gdb->msg;
  }
  return ret;
}
