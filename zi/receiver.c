#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdbool.h>

#include "wrapper.h"

#define BACKLOG 20
#define MAXLEN 512

int main (int argc, char *argv[]){
	
	int on=1;
	int option, timeout, pos=0, recieved;
	bool tcp_flag = false, udp_flag = false;
	char *string;
	int portnum=0,j=0;
	int mysock, sockfd;
	char *buff=NULL;
	char ports[20];
	struct addrinfo hints, *res;
    struct sockaddr cli, tcp_addr;
    struct sockaddr_in clientaddr;
    struct sockaddr cliaddr;
	socklen_t clilen;
	fd_set readfds;

	
	while ((option = getopt(argc, argv, "i:tu")) != -1 ) {
		switch (option) {
			case 'i':	
				pos+=2;			
				timeout = atoi(optarg);
				break;
			case 't':
				pos+=1;	
				tcp_flag = true;
				break;	
			case 'u':
				pos+=1;	
				udp_flag = true;
				break;			
			default :
				printf("USAGE: ./receiver [-i timeout] [-t |-u] \"string\" port ...\n");
				return 2;
		}
	}
	
	struct timeval {
		long tv_sec;
		long tv_usec;
	};
	
	string = argv[pos];
	
	for(int i=pos+1; i<argc;i++){
		ports[j] = argv[i];
		j++;
	}
	
	portnum=j;
	
	struct timeval tv = {timeout,0};

		
	
	FD_ZERO(&readfds);
	if(tcp_flag == true && udp_flag==false){
	
		//TCP
		
		for(int k=0;k<portnum;k++){
			
			memset(&hints, 0, sizeof(hints));
			memset(&res, 0, sizeof(res));
			
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = 0;
			hints.ai_flags |= AI_PASSIVE;

			Getaddrinfo(NULL, ports[k], &hints, &res);
			
			sockfd = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

			Bind(sockfd, res->ai_addr, res->ai_addrlen);
				
			Listen(sockfd, BACKLOG);
			
			
			recieved = Recvfrom(mysock, buff, MAXLEN, 0, &cliaddr, &clilen);
			FD_SET(mysock, &readfds);
		
		}
		
	}else if(tcp_flag == false && udp_flag==true){
		
		//UDP
		
		for(int k=0;k<portnum;k++){
			memset(&hints, 0, sizeof hints);
			memset(&res, 0, sizeof(res));
				
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags = AI_PASSIVE;

			Getaddrinfo(NULL, ports[k], &hints, &res);
			mysock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			setsockopt(mysock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
			
			Bind(mysock,res->ai_addr, res->ai_addrlen);
			
			recieved = Recvfrom(mysock, buff, MAXLEN, 0, &cliaddr, &clilen);
	
			FD_SET(mysock, &readfds);
		}
		
	}else{
		//TCP I UDP
		
		for(int k=0;k<portnum;k++){
								
			FD_SET(0, &readfds);
			
			recieved = Select(portnum, &readfds, NULL, NULL, &tv);
			
			if (recieved == 0) {

			//printf("Timeout!\n");

			} else {
			
			if(FD_ISSET(0, &readfds)){
				
			}else if(FD_ISSET(mysock, &readfds)){
				
			}else if(FD_ISSET(sockfd, &readfds)){
				
			}
		
		}
		
	}
}
}
