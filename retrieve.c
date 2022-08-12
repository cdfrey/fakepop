/* $Id: retrieve.c,v 1.2 2004/11/30 21:26:37 pzn Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "msg.h"
#include "header.h"
#include "retrieve.h"
#include "common.h"

/* retrieve a message, convert end of lines to CR+LF.
   convert dots at the begining of a line to two dots */
void retrieve_partial (int msg, FILE * out, int bodylines) {
  FILE * fh;
  char * filename;
  int j, linecount = 0, bodyflag = 0;
  char * header;
  char * msgline;

  asprintf(&filename, "%s%s", MSGDIR, msg_getfilename(msg));
  fh = fopen(filename,"r");
  free(filename);
  if (fh == NULL) return;

  msgline = malloc (MSG_MAX_LINE_SIZE + 1);
  if (msgline == NULL)
    return;

  header = header_create (msg, 0);
  fprintf (out, "%s", header);
  free (header);

  while (fgets(msgline,MSG_MAX_LINE_SIZE,fh) != NULL) {
    msgline[MSG_MAX_LINE_SIZE] = 0;
    j = strlen(msgline);
    while (j && (msgline[j-1] == '\r' || msgline[j-1] == '\n')) {
      j--;
      msgline[j] = 0;
    }
    if (msgline[0] == '.') {
      fputc('.',out);
    }
    fprintf(out,"%s\r\n",msgline);
    if (bodyflag) {
      linecount++;
      if (linecount == bodylines) {
	/* end of partial message content */
	break;
      }
    } else {
      if (msgline[0] == 0) {
	bodyflag = 1;
      }
    }
  }
  fclose (fh);
  free (msgline);
}

