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
#include <stdlib.h>
#include <time.h> 

#define MAXLEN 12
#define DEFAULT_PORT 5555


void sig_alrm(int signo)
{
    alarm(1);
    return;
}



struct INIT {
	char command[4];
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

struct TCP {
	uint32_t clid;
	char message[30];
};

void *Signal(int signo, void *func){				
    void *sigfunc;

    if ((sigfunc = signal(signo, func)) == SIG_ERR)
	warn("signal error");
    return (sigfunc);
}

int main(int argc, char *argv[]){
	
	int option, timeout=5,received;
	char port[10];
	
	int mysock, sockTcp;
	
	pid_t pid;
	uint16_t max, nula, pokusaj;
	uint32_t clidCounter=1;
	struct addrinfo hints, *res;
	struct sockaddr cliaddr;
	socklen_t clilen, tcpaddrlen;
		
	char brojChar[5];
	
	short pokusaji[10]={0};
	int *numberList[10];
	
	struct BROJ broj;
	struct RESP resp;
	struct INIT initial;
	struct TCP tcpS;
	
	char buff[MAXLEN];
	
	
	srand(time(0));
	
	while ((option = getopt(argc, argv, "t:")) != -1){
		switch (option){
			case 't':
				timeout = atoi(optarg);
				break;
			default:
				err(3,"Usage: ./zamisli [-t timeout] [port]\n");
				break;
		}
	}
	
	if (optind == argc) {
		strcpy(port, "1234");
	} else if (optind < argc && argc - optind == 1) {
		strcpy(port, argv[optind]);
	} else {
		err(3,"Usage: ./zamisli [-t timeout] [port]\n");
	}
	
	memset(&hints, 0, sizeof(hints));
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags =  AI_PASSIVE;
			
	Getaddrinfo(NULL, port, &hints, &res);
	
	mysock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	Bind(mysock, res->ai_addr, res->ai_addrlen);
	
	clilen = sizeof(cliaddr);
	
	while(true){
		
		received = Recvfrom(mysock, buff, MAXLEN, 0, &cliaddr, &clilen);
		strncpy(brojChar, buff, 4);
		brojChar[4] = '\0';
		
		if(strcmp("INIT",brojChar)==0){

			memset(&initial,0,sizeof(initial));
			memcpy(&initial,buff,sizeof(initial));
			int clid=clidCounter;
			clidCounter++;
			
			max = ntohs(initial.max);
			nula = initial.nula;
		
		
			int num = (rand() % (max - nula + 1)) + nula;
	
			numberList[clid]=&num;
	
			memset(&resp,0,sizeof(resp));
			
			strncpy(resp.command,"ID",2);
			resp.clid=htonl(clid);
    
			received = Sendto(mysock, &resp, MAXLEN, 0, &cliaddr, clilen);
		
		}else if(strcmp("BROJ",brojChar)==0){
			
			memset(&broj, 0, sizeof(broj));
			memcpy(&broj, buff, sizeof(broj));
			memset(&resp, 0, sizeof resp);
			
			pokusaj=ntohs(broj.xx);
			int clid=ntohl(broj.clid);
			int brojCompare = *numberList[clid];
		
			if (numberList[clid] == NULL || clid >= clidCounter ){
				//out of bound, do nothing
				continue;
			}

			if(pokusaj==brojCompare){
				
				strncpy(resp.command,"OK",2);
				
				resp.clid=broj.clid;
				pokusaji[clid]++;
				resp.nn=htons(pokusaji[clid]);
				struct sockaddr_in tcpaddr;

				memset(&tcpaddr, 0, sizeof (tcpaddr));
				sockTcp = Socket(PF_INET, SOCK_STREAM, 0);
				setsockopt(sockTcp, SOL_SOCKET, SO_REUSEADDR, 1, sizeof(int));
				
				tcpaddr.sin_family = AF_INET;
				tcpaddr.sin_port = 0;
				tcpaddr.sin_addr.s_addr = INADDR_ANY;

				tcpaddrlen = sizeof(tcpaddr); 
				
				Bind(sockTcp, (struct sockaddr*)&tcpaddr, tcpaddrlen);
				
				Listen(sockTcp, 1);
				
				getsockname(sockTcp, (struct sockaddr*)&tcpaddr, &tcpaddrlen);

						
				resp.port = tcpaddr.sin_port;
				if (fork()==0){
				
						Signal(SIGALRM, sig_alrm);
						siginterrupt(SIGALRM,1);
						alarm(timeout);
						struct sockaddr tcpclientaddr;
						received = Accept(sockTcp, &tcpclientaddr, &tcpaddrlen);
						
						if(received==-1){
							printf("Neuspjelo spajanje nakon sto je proslo %d sekunda.\n", timeout);
							close(sockTcp);
							return 0;
						}
						
						//uint32_t tcpId = htonl((uint32_t) broj.clid);
						
						memset(&tcpS, 0, sizeof(tcpS));
						tcpS.clid=broj.clid;
						strcpy(tcpS.message,":<–FLAG–MrePro–2020-2021-MI–>\n");
						writen(received, &tcpS, sizeof(tcpS));
						close(received);
						close(sockTcp);
						return 0;
				}
				received = Sendto(mysock, &resp, MAXLEN, 0, &cliaddr, clilen);
				close(sockTcp);
				
			}else if(pokusaj>brojCompare){
				strncpy(resp.command,"LO",2);
				resp.clid=broj.clid;
				pokusaji[clid]++;
				resp.nn=htons(pokusaji[clid]);
		
			received = Sendto(mysock, &resp, MAXLEN, 0, &cliaddr, clilen);
				
			}else if(pokusaj<brojCompare){
				strncpy(resp.command,"HI",2);
				resp.clid=broj.clid;
				pokusaji[clid]++;
				resp.nn=htons(pokusaji[clid]);
		
				received = Sendto(mysock, &resp, MAXLEN, 0, &cliaddr, clilen);
				
			}
			
			
		}
		if(clidCounter==10)break;
	}
	close(mysock);
	
}
