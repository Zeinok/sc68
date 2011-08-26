/* $Id$ */

/* Time-stamp: <2009-10-11 19:58:30 ben>  */

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "mksc68_cmd.h"
#include "mksc68_dsk.h"
#include "mksc68_msg.h"
#include "mksc68_opt.h"
#include "mksc68_tag.h"
#include "mksc68_str.h"

#include <string.h>
#include <stdio.h>

static const opt_t longopts[] = {
  { "list",       0, 0, 'l' },
  { "view",       0, 0, 'v' },
  { "disk",       0, 0, 'd' },
  { "tracks",     1, 0, 't' },
  { "del",        0, 0, 'D' },
  { "purge",      0, 0, 'P' },
  { 0,0,0,0 }
};

static
int purge_all_tags(void)
{
  int t;
  if (!dsk_has_disk())
    return -1;
  for (t = dsk_get_tracks(); t >= 0; --t)
    dsk_tag_clr(t);
  return 0;
}

static
int list_all_tags(void)
{
  const char * var, * des, * bis;
  int i,j,l;

  for (l=0, i=1; j=tag_std(i,&var, &des), j >= 0; ++i) {
    int len = strlen(var);
    if ( len > l ) l = len;
  }
  for (i=1; j=tag_std(i,&var, &des), j >= 0; ++i) {
    if (!j) {
      printf("%-*s ... %s\n", l, var, des);
    } else if ( ! tag_std(j,&bis, 0) ) {
      printf("%-*s ... alias for %s\n", l, var, bis);
    }
  }
  return 0;
}

static
void view_tags(int trk, int argc, char ** argv)
{
  const char * var, * val;
  void * hdl
    = dsk_tag_lst(trk);
  while (hdl) {
    hdl = dsk_tag_nxt(hdl, &var, &val);
    if (!argc)
      printf("%02d:%-12s \"%s\"\n", trk, var, val);
    else {
      int i;
      for (i=0; i<argc; ++i) {
        if (!strcmp(argv[i],var)) {
          printf("%02d:%-12s \"%s\"\n", trk, var, val);
          break;
        }
      }
    }
  }
}

static
int run_tag(cmd_t * cmd, int argc, char ** argv)
{
  char shortopts[(sizeof(longopts)/sizeof(*longopts))*3];
  int ret = -1, i, disktarget = 0, action='A', a, b;
  const char * tracklist = 0, * tl;

  opt_create_short(shortopts, longopts);
  while (1) {
    int longindex;
    int val
      = getopt_long(argc, argv, shortopts, longopts, &longindex);

    switch (val) {
    case  -1: break;                    /* Scan finish */

    case 'P':                           /* --purge     */
    case 'l':                           /* --list      */
    case 'v':                           /* --view      */
    case 'D':                           /* --delete    */
      action = val;
      break;
      
    case 'd':
      disktarget = 1;
      break;

    case 't':                           /* --track=    */
      if (tracklist) {
        msgerr("multiple track-list not allowed.\n");
        goto error;
      } else {
        tracklist = optarg;
        while ( ret = str_tracklist(&tracklist, &a, &b), ret > 0 ) {
          msgdbg("track interval %d-%d\n",a,b);
        }
        tracklist = optarg;
        if (ret)
          goto error;
        ret = -1;
      }
      break;

    case '?':                       /* Unknown or missing parameter */
      goto error;

    default:
      msgerr("unexpected getopt return value (%d)\n", val);
      goto error;
    }
    if (val == -1) break;
  }
  i = optind;


  switch (action) {
  case 'P':                             /* purge */
    ret = purge_all_tags();
    break;
  case 'l':                             /* list */
    ret = list_all_tags();
    break;

  case 'v':                             /* view */
    if (disktarget)
      view_tags(0,argc-i,argv+i);
    tl = tracklist;
    while ( str_tracklist(&tl, &a, &b) > 0 ) {
      int t;
      for (t = a; t <= b; ++t)
        view_tags(t,argc-i,argv+i);
    }
    ret = 0;
    break;

  case 'D':                             /* delete */
    for ( ; i<argc; ++i) {
      int a,b;
      const char * var = argv[i], * tl;
      if (disktarget) {
        dsk_tag_del(0, var);
      }
      tl = tracklist;
      while ( str_tracklist(&tl, &a, &b) > 0 ) {
        int t;
        for (t = a; t <= b; ++t) {
          dsk_tag_del(t, var);
        }
      }
    }
    ret = 0;
    break;

  case 'A':                             /* add */
    if ((argc-i)&1) {
      msgerr("invalid number of argument.\n");
      goto error;
    }
    for (ret = 0 ; !ret && i < argc; i += 2) {
      const char * var = argv[i+0], * val=argv[i+1];
      if (disktarget && !dsk_tag_set(0,var,val))
        ret = -1;
      tl = tracklist;
      while (!ret && str_tracklist(&tl, &a, &b) > 0 ) {
        int t;
        for (t = a; !ret && t <= b; ++t)
          ret = -!dsk_tag_set(t,var,val);
      }
    }
    break;
    
  default:
    assert(!!"invalid action" );
    return -1;
  }
  
error:
  return ret;
}

cmd_t cmd_tag = {
  /* run */ run_tag,
  /* com */ "tag",
  /* alt */ 0,
  /* use */ "[OPTION ...] [--] NAME [VALUE]",
  /* des */ "get or set meta-tag information.",
  /* hlp */

  "The `tag' command gets or sets meta-tag information. Tags are\n"
  "associated either to the whole disk or to specific tracks.\n"
  "It is allowed to address both by using a `-d' and `-t' switches.\n"
  "\n"

  "OPTION\n"
  "   -l --list        list all well-known tags and exit.\n"
  "   -d --disk        select disk tag.\n"
  "   -t\n"
  "   --tracks=TRACKS  select tracks tag.\n"
  "   -v --view        view specified tags.\n"
  "   -D --del         delete specified tags.\n"
  "   -P --purge       delete all tags (disk and tracks).\n"
  "\n"
  "TRACKS := N[-N][,N[-N]]* N:=[0-9]+ (0 is the last track)\n"

};
