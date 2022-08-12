/* $Id: fakepop.c,v 1.3 2004/12/01 17:13:02 pzn Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "msg.h"
#include "header.h"
#include "id.h"
#include "retrieve.h"
#include "common.h"

/* process a POP3 command. return 0 if command is "quit" */
int process_cmd (char * cmd) {
  int ret = 1;
  int known = 0;

  if ((strncasecmp ("dele", cmd, 4) == 0) ||
      (strncasecmp ("rset", cmd, 4) == 0) ||
      (strncasecmp ("user", cmd, 4) == 0) ||
      (strncasecmp ("pass", cmd, 4) == 0) ||
      (strncasecmp ("apop", cmd, 4) == 0) ||
      (strncasecmp ("noop", cmd, 4) == 0)){
    known = 1;
    printf("+OK\r\n");
  }
  
  if (strcasecmp ("stat", cmd) == 0) {
    known = 1;
    printf("+OK %d %d\r\n", msg_gettotalmsgs(), msg_gettotalsize());
  }
  
  if (strncasecmp ("list", cmd, 4) == 0) {
    known = 1;
    if (strlen(cmd) > 5) {
      /* single message list */
      int i;
      i = atoi(cmd+5);
      if (msg_exist(i)) {
	printf("+OK %d %d\r\n", i, msg_getsize(i));
      } else {
	printf("-ERR no such message\r\n");
      }
    } else {
      /* all messages list */
      int i, j;
      j = msg_gettotalmsgs();
      printf("+OK\r\n");
      for (i = 1; i <= j; i++) {
	printf("%d %d\n", i, msg_getsize(i));
      }
      printf(".\r\n");
    }
  }

  if (strncasecmp ("uidl", cmd, 4) == 0) {
    char s[71];
    int i;
    known = 1;
    if (strlen(cmd) > 5) {
      /* single message uidl */
      i = atoi(cmd+5);
      id_get_uidl (i, s);
      printf("+OK %d %s\r\n",i,s);
    } else {
      /* all messages uidl */
      int j;
      j = msg_gettotalmsgs();
      printf("+OK\r\n");
      for (i = 1; i <= j; i++) {
	id_get_uidl (i, s);
	printf("%d %s\r\n",i,s);
      }
      printf(".\r\n");
    }
  }
  
  if (strncasecmp ("retr", cmd, 4) == 0) {
    int i;
    known = 1;
    i = atoi(cmd+5);
    if (msg_exist(i)) {
      printf("+OK\r\n");
      retrieve_full (i, stdout);
      printf(".\r\n");
    } else {
      printf("-ERR no such message\r\n");
    }
  }

  if (strncasecmp ("top", cmd, 3) == 0) {
    int i = 0, lines = 0;
    known = 1;
    sscanf(cmd+4, "%d %d", &i, &lines);
    if (msg_exist(i)) {
      printf("+OK\r\n");
      retrieve_partial (i, stdout, lines);
      printf(".\r\n");
    } else {
      printf("-ERR no such message\r\n");
    }
  }
  
  if (strcasecmp ("quit", cmd) == 0) {
    known = 1;
    printf("+OK\r\n");
    ret = 0;
  }
  
  if (! known) {
    printf("-ERR Unknown command: \"%s\".\r\n", cmd);
  }
  
  return ret;
}

int main (void) {
  char cmd[CMD_MAX_SIZE+1];
  int i, j, running = 1;

  msg_init();

  printf ("+OK fakepop V%s %s <1234.5678901234@fakepop.invalid>\r\n",
	  PROGRAM_VERSION, URL);
  fflush(stdout);

  while ((running == 1) && (fgets(cmd, CMD_MAX_SIZE, stdin) != NULL)) {
    cmd[CMD_MAX_SIZE-1] = 0;
    j = strlen(cmd);
    for (i=0; i<j; i++) {
      if (cmd[i] < ' ') {
	cmd[i] = 0;
	break;
      }
    }
    running = process_cmd(cmd);
    fflush(stdout);
  }

  return 0;
}
