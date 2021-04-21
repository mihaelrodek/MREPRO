#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>

int Socket(int family, int type, int protocol){
	int n;

	if ((n=socket(family,type,protocol))==-1){
		fprintf(stderr, "socket: %s", gai_strerror(n));
		exit(1);
	} else return n;
}

int Bind(int sockfd, const struct sockaddr *myaddr, int addrlen){
	int n;
	if((n=bind(sockfd, myaddr, addrlen))==-1){
		fprintf(stderr, "bind: %s", gai_strerror(n));
		exit(1);
	} else return n;		
		
}

int Getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result) {
	int n;
	if ((n=getaddrinfo(hostname, service, hints, result))==-1){
		fprintf(stderr, "getaddrinfo: %s", gai_strerror(n));
		exit(1);
	} else return n;
	
}


int Getnameinfo(const struct sockaddr *sockaddr, socklen_t addrlen, char *host, size_t hostlen, char *serv, size_t servlen, int flags) {
	int n;
	if ((n=getnameinfo(sockaddr, addrlen, host, hostlen, serv, servlen, flags))==-1){
		fprintf(stderr, "getnameinfo: %s", gai_strerror(n));
		exit(1);
	} else return n;
	
}

int Listen(int sockfd, int backlog){
	int n;
	if ((n=listen(sockfd, backlog)) == -1) {
		fprintf(stderr, "listen: %s", gai_strerror(n));
		exit(1);
	}else return n;
}

int Accept( int sockfd,struct sockaddr* cliaddr,socklen_t *addrlen){
	int n;
	if((n=accept(sockfd, cliaddr, addrlen))==-1){
		fprintf(stderr, "accept: %s", gai_strerror(n));
		exit(1);
	}else return n;

}

int Connect(int sockfd, const struct sockaddr *server, socklen_t addrlen){
	int n;
	if((n=connect(sockfd, server, addrlen)==-1)){
		fprintf(stderr, "connect: %s", gai_strerror(n));
		exit(1);
	}else return n;
}

ssize_t readn(int fd, void *vptr, size_t n){

    size_t nleft;
    ssize_t nread;
    char *ptr;
    ptr = vptr;
    nleft = n;

    while(nleft > 0) {
	if( (nread = read(fd, ptr, nleft)) < 0) {
	    if(errno == EINTR)
		nread = 0;
	    else
		return(-1);
	} else if(nread == 0) {
	    break;
	}
	nleft -= nread;
	ptr += nread;
    }
    return(n - nleft);
} 

ssize_t writen(int fd, const void *vptr, size_t n) {

    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    ptr = vptr;
    nleft = n;

    while(nleft > 0) {
	if( (nwritten = write(fd, ptr, nleft)) <= 0) {
	    if(nwritten < 0 && errno == EINTR)
		nwritten = 0;
	    else
		return(-1);
	}
	nleft -= nwritten;
	ptr += nwritten;
    }
    return (n);
}

/*
int Inet_pton(int af, const char *src, void *dst){
	int n;
	if((n== inet_pton(af, src, dst))==-1){
		fprintf(stderr, "inet_ntop: %s", gai_strerror(n));
	}else return n;
}


int Inet_ntop(int af, const void *src, char *dst, socklen_t size){
	int n;
	if((n== inet_ntop(af, src, dst, size))==-1){
		fprintf(stderr, "inet_pton: %s", gai_strerror(n));
	}else return n;
}
* */

