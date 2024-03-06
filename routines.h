#ifndef _ROUTINES_H
#define _ROUTINES_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include "parser.h"

#define HEADERS_MAX 4096

void *start_rtn(void *arg);


#endif
