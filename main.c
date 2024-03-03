#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <syslog.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include "routines.h"
#include "bindsock.h"
#include "address.h"
#include "parser.h"
#include "err.h"

#define BUFLEN 128
#define BUFSIZE 1024
#define QLEN 2

pthread_attr_t attr;
pthread_mutex_t omutex;
pthread_mutexattr_t oattr;

int main(int argc , string argv[])
{
  setvbuf(stdin,NULL,_IONBF,0);
  setvbuf(stdout,NULL,_IONBF,0);
  setvbuf(stderr,NULL,_IONBF,0);
  struct addrinfo *addrinfo;
  int serverfd,err;

  if((err=getaddr("127.0.0.1",argv[1],&addrinfo)) != 0)
    err_user((char *)gai_strerror(err));

  if((serverfd = bindsock(addrinfo)) <0)
    err_sys();

  if(listen(serverfd,QLEN) < 0)
    err_sys();

  if((err=pthread_attr_init(&attr)) != 0)
    err_user((char *) strerror(err));

  if((err=pthread_mutexattr_init(&oattr)) != 0)
    err_user((char*) strerror(err));
  
  if((err=pthread_mutexattr_settype(&oattr,PTHREAD_MUTEX_ERRORCHECK)) != 0)
    err_user((char *) strerror(err));

  if((err=pthread_mutex_init(&omutex,&oattr)) != 0)
    err_user((char *) strerror(err));

 if((err=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)) != 0)
    err_user((char *) strerror(err));

  while(true){
    int clientfd = accept(serverfd,NULL,NULL);
    pthread_t tid;
    if((err= pthread_create(&tid,&attr,start_rtn,&clientfd)) != 0){
      err_log((char*) strerror(err));
      exit(1);
    }
  }

}
