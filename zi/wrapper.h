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

int Setsockopt(int sockfd, int level, int optname, void *opval, socklen_t *optlen){
	int n;

	if ((n=setsockopt(sockfd,level,optname, opval, optlen))==-1){
		fprintf(stderr, "setsockopt: %s", gai_strerror(n));
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

int Sendto(int sockfd, void *buff, size_t nbytes, int flags, const struct sockaddr* to, socklen_t addrlen){
	int n;
	if((n=sendto(sockfd, buff, nbytes, flags, to, addrlen))!=nbytes){
		fprintf(stderr, "sendto: %s", gai_strerror(n));
		exit(1);
	}else return n;
}

int Recvfrom(int sock_fd,  char *msg, size_t nbytes, int flags, struct sockaddr* from, socklen_t *fromaddrlen){
	int n ;
	if((n = recvfrom(sock_fd, msg, nbytes, flags, from, fromaddrlen)) <0){
		fprintf(stderr, "recvfrom: %s", gai_strerror(n));
		exit(1);
	}else return n;
}

int Select(int maxfd, fd_set *readset, fd_set *writeset,fd_set *excepset,const struct timeval *timeout){
	int n ;
	if((n = select(maxfd, readset, writeset, excepset, timeout)) <0){
		fprintf(stderr, "select: %s", gai_strerror(n));
		exit(1);
	}else return n;

}

int Send(int s, const void *msg, size_t len, int flags) {
	int n;
	if ((n=send(s, msg, len, flags))==-1) { 
		fprintf(stderr, "send: %s", gai_strerror(n));
	}else return n;
}

int Read(int fd, char *buf, int max){
	int n;
	if ((n=read(fd, buf, max)==-1)) { 
		fprintf(stderr, "read: %s", gai_strerror(n));
	}else return n;
}

int Recv(int s, void *buf, size_t len, int flags){
	int n;
	if ((n=recv(s, buf, len,flags))==-1) { 
		fprintf(stderr, "recv: %s", gai_strerror(n));
	}else return n;
}

in_port_t getport(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return (((struct sockaddr_in*)sa)->sin_port);
    }
    return (((struct sockaddr_in6*)sa)->sin6_port);
}
