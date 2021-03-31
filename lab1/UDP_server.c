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

#define MAXLEN 512
#define PORT 1234

int Socket(int family, int type, int protocol){
	int n;

	if ((n=socket(family,type,protocol))==-1){
		strerror(88);
		exit(4);
	} else {
		return n;
	}
}

int Bind(int sockfd, const struct sockaddr *myaddr, int addrlen){
	
	if(bind(sockfd, myaddr, addrlen)==-1){
		strerror(6);
		exit(4);
	} else {
		return 0;		
	}		
}

int Getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result) {
	
	if (getaddrinfo(hostname, service, hints, result)==-1){
		strerror(6);
		exit(5);
	} else {
		return 0;
	}
}

bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}



int main(int argc, char *argv[]){
	
	char *port_addr="1234";
	int option;
	char *payload="";
	char buff[MAXLEN];
	
	int mysock;
	
	struct sockaddr cliaddr;
	socklen_t clilen;
	
	int received;
	struct addrinfo hints, *res;


	if (argc!=1 && argc!=3 && argc!=5){
		err(3,"Usage: ./UDP_server [-l port] [-p payload]");
	}

	while ((option = getopt(argc, argv, "l:p:")) != -1){
		switch (option){
			case 'l':
				port_addr = optarg;
				break;
			case 'p':
				payload = optarg;
				break;
			default:
				port_addr="1234";
				payload="";
				break;
		}
	}
	
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags =  AI_PASSIVE;
			
	Getaddrinfo(NULL, port_addr, &hints, &res);
	
	mysock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	Bind(mysock, res->ai_addr, res->ai_addrlen);
	
	clilen = sizeof(cliaddr);

	while(1){

		received = recvfrom(mysock, buff, MAXLEN, 0, &cliaddr, &clilen);
		
		if(received==-1){
			strerror(4);
		}
		
		buff[received+1]='\0';
		
	
		if(startsWith("HELLO",buff)){
			
			received = sendto(mysock, payload, MAXLEN, 0, &cliaddr, clilen);
			
			if(received==-1){
				strerror(70);
			}
				
		}else {
			
		}
		
	}
	
	close(mysock);
	
}


