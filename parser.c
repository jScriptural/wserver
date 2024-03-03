#include <stdlib.h>
#include <fcntl.h>
#include <libgen.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include "parser.h"



static char *replace(int chr,int sub,char buf[],size_t nbytes,bool fwd,bool all)
{
  if(fwd){
    for(int i=0; i < nbytes; ++i){
      if(buf[i] == chr){
	buf[i] = sub;
	if(!all)break;
      }
    }
  }else {
    int i = strlen(buf)==nbytes?(nbytes-1):nbytes;
    for(; i > 0; --i){
      if(buf[i] == chr){
	buf[i] = sub;
	if(!all)break;
      }	
    }
  }
  return buf;
}


int reqparser(int sockfd, struct request *req)
{

  int status;
  memset(req,0,sizeof(struct request));
  char buf[REQBUFSZ];
  char *reqfile = "reqdata.dat";
  ssize_t recvbytes;
  int fd[2],fdout;

  if((recvbytes = recv(sockfd,buf,REQBUFSZ,0)) <= 0)
    return -1;

  if((fdout = open(reqfile,O_WRONLY|O_CREAT|O_TRUNC)) < 0){
    perror("open");
    return -1;;

  }
  write(STDOUT_FILENO," CLIENT: ",9);

  if(write(fdout,buf,recvbytes) < recvbytes)
    perror("write");


  if(write(STDOUT_FILENO,buf,recvbytes) < recvbytes)
    perror("write");



  if(pipe(fd) < 0){
    perror("pipe");
    return -1;
  }

  pid_t pid;
  if((pid = fork()) < 0){
    perror("fork");
    return -1;
  }


  switch(pid){
    case 0:
      close(fd[0]);
      if(fd[1] != STDOUT_FILENO){
	if(dup2(fd[1],STDOUT_FILENO) != STDOUT_FILENO){
	  perror("dup2");
	  exit(1);
	}
      }
      if(execl("/bin/node","node","parser.js",(char *) 0) !=0){
	perror("execl");
	exit(1);
      }
      break;
    default:
      close(fd[1]);

      if(waitpid(pid,&status,0) < 0){
	perror("waitpid");
	return -1;
      }
      if(!WIFEXITED(status))
	return -1;
      if(WEXITSTATUS(status) != 0)
	return -1;
      char line[BUFSIZ];
      int lineno=1;
      FILE *fp = fdopen(fd[0],"r");
      while(fgets(line,BUFSIZ,fp) != NULL){
	if(strchr(line,'\n') != NULL)
	  replace('\n','\0',line,strlen(line),FORWARD,FIRST_OCC);

	switch(lineno){
	  case 1:
	    if(!strncmp(line,"GET",strlen("GET"))){
	      req->method = HTTP_GET;
	    }else if(!strncmp(line,"POST",strlen("POST"))){
	      req->method = HTTP_POST;
	    }else if(!strncmp(line,"PUT",strlen("PUT"))){
	      req->method = HTTP_PUT;
	    }else if(!strncmp(line,"DELETE",strlen("DELETE"))){
	      req->method = HTTP_DELETE;
	    }else {
	      req->method = HTTP_UNKNOWN;
	    }
	    ++lineno;
	    break;
	  case 2:
	    if(strlen(line) == 0)
	      req->url.route = NULL;
	    else
	      req->url.route = strdup(line);
	    ++lineno;
	    break;
	  case 3:
	    if(strlen(line) == 0)
	      req->url.search = NULL;
	    else
	      req->url.search = strdup(line);
	    ++lineno;
	    break;
	  case 4:
	    if(strlen(line) == 0)
	      req->host = NULL;
	    else
	      req->host = strdup(line);
	    req->url.host = req->host;
	    ++lineno;
	    break;
	  case 5:
	    if(strlen(line) == 0)
	      req->hostname = NULL;
	    else
	      req->hostname = strdup(line);
	    ++lineno;
	      break;
	  case 6:
	    if(strlen(line) == 0)
	      req->port = 0;
	    else 
	      req->port = (in_port_t) atoi(line);
	    ++lineno;
	    break;
	  case 7:
	    if(strlen(line) == 0)
	      req->accept = NULL;
	    else
	      req->accept = strdup(line);
	    ++lineno;
	    break;
	  case 8:
	    if(strlen(line) == 0)
	      req->user_agent = NULL;
	    else
	      req->user_agent = strdup(line);
	    ++lineno;
	    break;
	}
      }
 }

/*
 * PARSE ORDER
 * method
 * route
 * search
 * host
 * hostname
 * port
 * accept
 * user_agent
 */

  close(fdout);
  remove(reqfile);

  return 0;
}

