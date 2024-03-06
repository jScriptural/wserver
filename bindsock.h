#ifndef _BINDSOCK_H
#define _BINDSOCK_H

#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>


/* bindsock - wrapper for bind.
 * returns a bounded socket descriptor otherwise 
 * -1.
 */

int bindsock(struct addrinfo *info);

#endif
