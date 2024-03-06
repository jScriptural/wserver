#include "address.h"

int getaddr(const char *hostname,const char *servname,struct addrinfo **info)
{
  struct addrinfo hints;
  memset(&hints,0,sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_CANONNAME;
  
  return getaddrinfo(hostname,servname,&hints,info);
}
