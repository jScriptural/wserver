#ifndef _PARSER_H
#define _PARSER_H

#include <netinet/in.h> //for in_port_t

#define DIRNAME_MAX 2048
#define REQBUFSZ 4096
#define BACKWARD 0
#define FORWARD 1
#define FIRST_OCC 0
#define ALL_OCC 1
#define HTTP_UNKNOWN 0
#define HTTP_GET 1
#define HTTP_POST 2
#define HTTP_PUT 3
#define HTTP_DELETE 4


struct URL {
  char *search;
  char *route;
  char *host;
};


struct request {
  short method;
  char *host; //hostname:port number
  char *hostname; 
  in_port_t port; //port in host byte order
  char *user_agent;
  char *accept;
  struct URL url;
};


int reqparser(int sockfd,struct request *req);

#endif /* _PARSER_H */
