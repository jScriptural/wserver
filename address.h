#ifndef _ADDRESS_H
#define _ADDRESS_H

/* getaddr - wrapper for getaddrinfo.
 * map a hostname and a service name to
 * SOCK_STREAM type  on  AF_INET domain address.
 * return errcode  on failure that can be provided
 * to gai_strerror() for interpretation otherwise
 * return 0.
 */

int getaddr(const char *hostname,const char *servname,struct addrinfo **info);

#endif
