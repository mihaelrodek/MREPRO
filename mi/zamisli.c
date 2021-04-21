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

struct ID {
	char command[2];
	uint16_t nula;
	char clid[4];
};

int main(int argc, char *argv[]){
	
	int option, timeout=5;
	char *port="1234";
	
	int mysock;
	pid_t pid;
	struct sockaddr cliaddr;
	socklen_t clilen;
	
	int received;
	uint16_t max, nula;
	struct addrinfo hints, *res;
	uint32_t clid;
	char *sendClid;
	
	uint16_t pokusaj,nn;
	int brojPokusaja=0;
	
	
    struct ID id;
	struct BROJ broj;
	struct RESP resp;
	
	
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
	
	//printf("%d, %s\n", timeout, port);
	
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags =  AI_PASSIVE;
			
	Getaddrinfo(NULL, port, &hints, &res);
	
	mysock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	Bind(mysock, res->ai_addr, res->ai_addrlen);
	
	//clilen = sizeof(cliaddr);
	//pid=getpid();
	
	struct INIT initial;
	
	
	printf("tu sam\n");
	
	received = Recvfrom(mysock, (char *)&initial, MAXLEN, 0, &cliaddr, &clilen);

	max = initial.max;
	nula = initial.nula;
	
	srand(time(0));
	
	int num = (rand() % (max - nula + 1)) + nula;
    printf("Generiran %d\n ", num);
    
	
    id.command="ID";
    id.clid=clid;
    
    clilen = sizeof(cliaddr);

	received = Sendto(mysock, (char *)&id, strlen(id), 0, &cliaddr, clilen);


	//printf("a sad tu");
	
	
	while(true){
		
		received=Recvfrom(mysock, (char*)&broj, MAXLEN, 0, &cliaddr, &clilen);
		
		pokusaj=broj.xx;
		nn=broj.nn;
		
		if(pokusaj==num){
			resp.comand="OK";
			resp.clid=clid;
			resp.nn=nn;
			resp.port=htons(port);
			
			
		}else if(pokusaj>num){
			resp.comand="HI";
			resp.clid=clid;
			resp.nn=nn;
			
			
		}else if(pokusaj<num){
			resp.comand="LO";
			resp.clid=clid;
			resp.nn=nn;
		
			
			
		}
		
		
		brojPokusaja++;
		if(brojPokusaja==10)break;
			
		
		
	}
	
	
	
}
