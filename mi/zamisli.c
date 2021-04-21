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
#include <netinet/ip.h>        

#include "wrapper.h"

#define MAXLEN 512
#define DEFAULT_PORT 5555


struct INIT {
	char command;
	uint16_t max;
	uint16_t nula;
};

struct BROJ {
	char command[4];
	uint32_t clid;
	uint16_t nn;
	uint16_t xx;
};

struct RESP {
	char command[2];
	uint16_t nula;
	uint32_t clid;
	uint16_t nn;
	uint16_t port;
};

int main(int argc, char *argv[]){
	
	int option, timeout=5;
	char *port="1234";
	
	int mysock;
	pid_t pid;
	struct sockaddr cliaddr;
	socklen_t clilen;
	
	int received;
	struct addrinfo hints, *res;
	
	
	
	if (argc!=1 && argc!=3 && argc!=4){
		err(3,"Usage: ./zamisli [-t timeout] [port]");
	}
	
	while ((option = getopt(argc, argv, "t:p")) != -1){
		switch (option){
			case 't':
				timeout = atoi(optarg);
				break;
			case 'p':
				port = optarg;
				break;
			default:
				timeout=5;
				port="1234";
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
	
	pid=getpid();
	
	
	
	
	//printf("tu sam");
	
	//memset(&hints, 0, sizeof(hints));


	//printf("a sad tu");
	
	while(true){
		
		
		
		
	}
	
	
	
}
