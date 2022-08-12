/* $Id: header.c,v 1.1.1.1 2004/11/30 19:34:52 pzn Exp $ */

#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "id.h"
#include "common.h"

#define HEADER_MAXSIZE 512
#define DATE_MAXSIZE 40

char * header_create (int uid, int refresh_date) {
  static char * date = NULL;
  char * self;
  char msg_id [32];
  char uidl [32];

  if (refresh_date) {
    if (date != NULL)
      free (date);
    date = NULL;
  }

  if (date == NULL) {
    struct tm * t;
    time_t tt;
    date = malloc(DATE_MAXSIZE);
    assert (date != NULL);
    time(&tt);
    t = localtime(&tt);
    strftime(date, DATE_MAXSIZE - 1, "%a, %d %b %Y %H:%M:%S %z", t);
  }

  id_get_msgid (uid, msg_id);
  id_get_uidl (uid, uidl);
  
  self = malloc(HEADER_MAXSIZE);
  assert (self != NULL);
  snprintf (self, HEADER_MAXSIZE - 1,
	    "Message-Id: <%s@fakepop.invalid>\r\n"
	    "Date: %s\r\n"
	    "X-UIDL: %s\r\n"
	    "X-fakepop-version: %s\r\n"
	    "X-fakepop-build: %s %s\r\n"
	    "X-fakepop-url: %s\r\n"
	    "X-Message-Flag: message delivered by fakepop - %s\r\n",
	    msg_id, date, uidl, PROGRAM_VERSION, __DATE__, __TIME__,
	    URL, URL);
  return self;
}

int header_size (void) {
  static int self = -1;
  if (self < 0) {
    char * s;
    s = header_create (1, 0);
    self = strlen (s);
    free (s);
  }
  return self;
}
