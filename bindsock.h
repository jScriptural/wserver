#ifndef _BINDSOCK_H
#define _BINDSOCK_H

/* bindsock - wrapper for bind.
 * returns a bounded socket descriptor otherwise 
 * -1.
 */

int bindsock(struct addrinfo *info);

#endif
