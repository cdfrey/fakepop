/* $Id: retrieve.h,v 1.1.1.1 2004/11/30 19:34:52 pzn Exp $ */

#ifndef _retrieve_h
#define _retrieve_h

#include <stdio.h>

void    retrieve_partial (int msg, FILE * out, int bodylines);
#define retrieve_full(i,f) retrieve_partial(i,f,-1)

#endif
