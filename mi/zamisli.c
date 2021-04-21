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
#include <signal.h>
#include "wrapper.h"

#define MAXLEN 512
#define DEFAULT_PORT 5555


void sig_alrm(int signo)
{
    alarm(1);
    return;
}



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
	uint32_t clid;
};

void *Signal(int signo, void *func){				
    void *sigfunc;

    if ((sigfunc = signal(signo, func)) == SIG_ERR)
	warn("signal error");
    return (sigfunc);
}

int main(int argc, char *argv[]){
	
	int option, timeout=5;
	char *port="1234";
	
	int mysock, sockTcp;
	pid_t pid;
	struct sockaddr cliaddr;
	socklen_t clilen;
	
	int received;
	uint16_t max, nula,tcpPort;
	struct addrinfo hints, hintsTcp, *res, *resTcp;
	uint32_t clidCounter=1;
	char *sendClid;
	
	uint16_t pokusaj,nn;
	int brojPokusaja=0;
	char *brojChar;
	
	
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
	
	Signal(SIGALRM, sig_alrm);
	siginterrupt(SIGALRM,1);
	alarm(timeout);

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
	
	received = Recvfrom(mysock, (char *)&initial, sizeof(initial), 0, &cliaddr, &clilen);
	
	
	if(strncmp("INIT",brojChar,4)!=0){
		continue;
	}

	max = initial.max;
	nula = initial.nula;
	
	srand(time(0));
	
	int num = (rand() % (max - nula + 1)) + nula;
    printf("Generiran %d\n ", num);
    
	
    strncpy(id.command,"ID",2);
    id.clid=clidCounter;
    
    clidCounter++;
    
    clilen = sizeof(cliaddr);

	received = Sendto(mysock, (char *)&id, sizeof(id), 0, &cliaddr, clilen);


	//printf("a sad tu");
	
	
	while(true){
		
		received=Recvfrom(mysock, (char*)&broj, sizeof(broj), 0, &cliaddr, &clilen);
		
		brojChar=broj.command;
		pokusaj=broj.xx;
		nn=broj.nn;
		
		if(strncmp("BROJ",brojChar,4)!=0){
			continue;
		}
		
		printf("tu\n");
		if(pokusaj==num){
			
			Getaddrinfo(NULL, 0, &hintsTcp, &resTcp);
			
			strncpy(resp.command,"OK",2);
			resp.clid=clidCounter;
			resp.nn=nn;
					
			
			sockTcp = Socket(resTcp->ai_family, resTcp->ai_socktype, resTcp->ai_protocol);
			Bind(sockTcp, resTcp->ai_addr, resTcp->ai_addrlen);
			Listen(sockTcp, 1);
			
			resp.port = htons(resTcp->ai_addr.sin_port);

			received =  Sendto(mysock, (char *)&resp, sizeof(resp), 0, &cliaddr, clilen);
		
			while(true){
				
				received = Accept(sockTcp,resTcp->ai_addr,resTcp->ai_addrlen);
				
				if ((pid = fork())==0){
					
				}
				
			}
			
			//fork();
			
			//accept();
			
			//alarm(timeout);
			
			
		}else if(pokusaj>num){
			strncpy(resp.command,"HI",2);
			resp.clid=clidCounter;
			resp.nn=nn;
			
			received = Sendto(mysock, (char *)&resp, sizeof(resp), 0, &cliaddr, clilen);
			
		}else if(pokusaj<num){
			strncpy(resp.command,"LO",2);
			resp.clid=clidCounter;
			resp.nn=nn;
	
			received = Sendto(mysock, (char *)&resp, sizeof(resp), 0, &cliaddr, clilen);
		}

		
		brojPokusaja++;
		clidCounter++;
		if(brojPokusaja==10)break;
			
		
		
	}
	
	
	
}
