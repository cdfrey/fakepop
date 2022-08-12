/* $Id: id.c,v 1.1.1.1 2004/11/30 19:34:52 pzn Exp $ */

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "id.h"

static char id_char[] = { "0123456789"
			  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			  "abcdefghijklmnopqrstuvwxyz" };

void rnd_init (int init_value) {
  static int rnd_base = 0;
  if (rnd_base == 0) {
    rnd_base = (getpid() << 8) + time(NULL);
  }
  srand(rnd_base + init_value);
}

void id_get_msgid (int n, char * msgid) {
  int i;
  rnd_init(n);
  for (i=0; i<20; i++) {
    msgid[i] = id_char[(int) (62.0*rand()/(RAND_MAX+1.0))];
  }
  msgid[i] = 0;
}

void id_get_uidl (int n, char * uidl) {
  int i;
  rnd_init(n ^ 0xabcd);
  for (i=0; i<20; i++) {
    uidl[i] = id_char[(int) (62.0*rand()/(RAND_MAX+1.0))];
  }
  uidl[i] = 0;
}
