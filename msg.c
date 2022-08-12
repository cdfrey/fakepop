/* $Id: msg.c,v 1.2 2004/11/30 21:26:37 pzn Exp $ */

#include <string.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#ifndef _DIRENT_HAVE_D_TYPE
# error "sorry, _DIRENT_HAVE_D_TYPE is not defined..."
#endif
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

#include "header.h"
#include "common.h"

static GSList * msg_filename = NULL;
static GSList * msg_size = NULL;
static int msg_counter = 0;
static int msg_totalsize = 0;

/* return 1 if filename is a valid message, else 0 */
static int msg_check_name (char * filename) {
  if (filename[0] == '.')
    /* is a hidden file... not allowed */
    return 0;
  if (filename[strlen(filename)-1] == '~')
    /* is a backup... not allowed */
    return 0;
  if (strncasecmp(filename, "readme", 6) == 0)
    return 0;
  return 1;
}

/* return the real size of a message
 *   included in size: fakepop added header size
 *                     "\n" -> "\r\n" end of line conversions */
static int msg_calc_size(char * msgfilename) {
  FILE * fh;
  char * filename;
  char * msgline;
  int j, size = 0;

  asprintf (&filename, "%s%s", MSGDIR, msgfilename);
  fh = fopen (filename,"r");
  free (filename);
  if (fh == NULL) return 0;

  msgline = malloc (MSG_MAX_LINE_SIZE + 1);
  if (msgline == NULL)
    return 0;

  while (fgets(msgline,MSG_MAX_LINE_SIZE,fh) != NULL) {
    msgline[MSG_MAX_LINE_SIZE] = 0;
    j = strlen(msgline);
    while (j && (msgline[j-1] == '\r' || msgline[j-1] == '\n')) {
      j--;
      msgline[j] = 0;
    }
    size += strlen(msgline) + 2; /* 2 is strlen("\r\n") */
  }
  fclose(fh);
  free (msgline);
  size += header_size();
  return size;
}

/* initialize gslists, calculate total messages and sizes */
void msg_init (void) {
  DIR *dp;
  struct dirent *ep;

  /* free lists if msg_init called again */
  if (msg_filename != NULL) {
    g_slist_free(msg_filename);
    msg_filename = NULL;
  }
  if (msg_size != NULL) {
    g_slist_free(msg_size);
    msg_size = NULL;
  }
  
  msg_counter = 0;
  msg_totalsize = 0;
  dp = opendir (MSGDIR);
  if (dp == NULL)
    return;
  while ((ep = readdir (dp))) {
    if ((ep->d_type == DT_REG) || (ep->d_type == DT_LNK)) {
      if (msg_check_name(ep->d_name)) {
	int size;
	msg_counter++;
	size = msg_calc_size(ep->d_name);
	msg_size = g_slist_append(msg_size, GINT_TO_POINTER (size));
	msg_filename = g_slist_append(msg_filename, strdup(ep->d_name));
	msg_totalsize += size;
      }
    }
  }
  closedir (dp);
}

/* return if a message exist */
int msg_exist (int msg) {
  if ((msg < 1) ||
      (msg > msg_counter)) {
    return 0;
  }
  return 1;
}

/* return the total number of messages */
int msg_gettotalmsgs (void) {
  return msg_counter;
}

/* return the size of a message */
int msg_getsize (int msg) {
  if (! msg_exist(msg))
    return 0;
  return (int) g_slist_nth_data (msg_size, msg - 1);
}

/* return the filename of a message */
char * msg_getfilename (int msg) {
  if (! msg_exist(msg))
    return NULL;
  return g_slist_nth_data(msg_filename, msg - 1);
}

/* return the sum of the sizes of all messages */
int msg_gettotalsize (void) {
  return msg_totalsize;
}

