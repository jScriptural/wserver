#include "bindsock.h"

int bindsock(struct addrinfo *info)
{
  int sockfd;
  char domain[INET_ADDRSTRLEN];
  struct addrinfo *aip = info;
  for(; aip != NULL; aip = aip->ai_next){
    if((sockfd = socket(aip->ai_family,aip->ai_socktype,0)) < 0)
      return -1;
    if(bind(sockfd,aip->ai_addr,aip->ai_addrlen) < 0){
      close(sockfd);
      continue;
    }

    struct sockaddr_in *in_addr = (struct sockaddr_in *) aip->ai_addr;

    printf("PROTOCOL: %s\n",aip->ai_protocol==IPPROTO_TCP?"TCP":"UNKNOWN");
    printf("ADDR: %u\n",in_addr->sin_addr.s_addr);
    printf("PORT: %u\n",ntohs(in_addr->sin_port));
    if(inet_ntop(in_addr->sin_family,&in_addr->sin_addr,domain,INET_ADDRSTRLEN) == NULL){
      perror("inet_ntop fails");
      printf("\n");
      return sockfd;
    }
    printf("DOMAIN: %s\n",domain);
    printf("\n");
    return sockfd;
  }
  return -1;
}
