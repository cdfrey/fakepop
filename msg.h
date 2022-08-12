/* $Id: msg.h,v 1.1.1.1 2004/11/30 19:34:52 pzn Exp $ */

#ifndef _msg_h
#define _msg_h

void   msg_init (void);
int    msg_exist (int msg);
int    msg_gettotalmsgs (void);
int    msg_getsize (int msg);
char * msg_getfilename (int msg);
int    msg_gettotalsize (void);

#endif
