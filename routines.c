#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include "parser.h"
#include "routines.h"

extern pthread_mutex_t omutex;


static  ssize_t sendres(int sockfd,const struct msghdr *msg)
{
  ssize_t nbytes;
  nbytes = sendmsg(sockfd,msg,0);

  return nbytes;
}

static void notfound_rtn(int sockfd)
{
  struct msghdr msg;
  char headers[]="HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n";
  memset(&msg,0,sizeof(msg));
  struct iovec iov[1];
  iov[0].iov_base = headers;
  iov[0].iov_len = strlen(headers);
  sendres(sockfd,&msg);
}


static int get_rtn(int sockfd,const struct request *req)
{
  struct msghdr msg;
  char resource[FILENAME_MAX] = "./src";
  char contnsz[1024] = {0};
  char headers[HEADERS_MAX] = {0};
  struct stat stats;
  int fd;

  if(!strcmp(req->url.route,"/")){
    strcat(resource,"/index.html");
    if(stat(resource,&stats) < 0){
      notfound_rtn(sockfd);
      return -1;
    }
    strcpy(headers,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ");
  }else {
    strcat(resource,req->url.route);
    char *dup = strdup(req->url.route);
    char *bname = basename(dup);
    free(dup);
    if(strchr(bname,'.') == NULL){
      strcat(resource,".html");
    }
    if(stat(resource,&stats) < 0){
      notfound_rtn(sockfd);
      return -1;
    }
    char *type = strchr(bname,'.');
    ++type;
    if(!strncmp(type,"css",strlen("css"))){
      strcpy(headers,"HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: ");
    }else if(!strncmp(type,"js",strlen("js"))){
      strcpy(headers,"HTTP/1.1 200 OK\r\nContent-Type: application/javascript\r\nContent-Length: ");
    }else if(!strncmp(type,"html",strlen("html"))){
      strcpy(headers,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ");
    }else if(!strncmp(type,"svg",strlen("svg"))){
      strcpy(headers,"HTTP/1.1 200 OK\r\nContent-Type: image/svg+xml\r\nContent-Length: ");
    }else {
      strcpy(headers,"HTTP/1.1 200 OK\r\nContent-Type: */*\r\nContent-Length: ");
    }
    
  }

  sprintf(contnsz,"%zu",stats.st_size);
  strcat(headers,contnsz);
  strcat(headers,"\r\n\r\n");
  char body[stats.st_size];
  if((fd = open(resource,O_RDONLY)) < 0)
    return -1;

  ssize_t n;
  if((n=read(fd,body,stats.st_size)) < 0)
    return -1;

  printf("%s read, size: %zu\n",resource,n);

  memset(&msg,0,sizeof(msg));
  struct iovec iov[2];
  iov[0].iov_base = headers;
  iov[0].iov_len = strlen(headers);
  iov[1].iov_base = body;
  iov[1].iov_len = (size_t) stats.st_size;
  msg.msg_iov = iov;
  msg.msg_iovlen = 2;

  if((n=sendres(sockfd,&msg)) < 0)
    return -1;
  return 0;
}

void *start_rtn(void *arg)
{
  int clientfd =  *(int *) arg;
  int retval;
  struct request req;
  pthread_mutex_lock(&omutex);
  if((retval=reqparser(clientfd,&req)) != 0){
    fprintf(stderr,"error:No data sent\n");
    close(clientfd);
    pthread_mutex_unlock(&omutex);
    pthread_exit((void*) 1);
  }
  pthread_mutex_unlock(&omutex);
  if(req.method == HTTP_GET)
    get_rtn(clientfd,&req);
  close(clientfd);
  pthread_exit(NULL);
}
