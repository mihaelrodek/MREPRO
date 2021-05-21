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
#include "wrapper.h"

#define MAXLEN 1024
#define PORT 1234
#define STDIN 0

int main(int argc, char *argv[]){
	
	int option,recieved,myTcpSock, myUdpSock,len;
	struct sockaddr fromServerAddr;
	socklen_t fromServerLen;
	struct addrinfo hints, *res;
	
	const int on = 1;
	char popis[MAXLEN] = "\0";
	char tcp_port_addr[MAXLEN]="1234", udp_port_addr[MAXLEN]="1234";
	char input[MAXLEN],message[MAXLEN] = {0};
	
	fd_set readfds;
    int fdmax;
    
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 500000;

	if (argc!=1 && argc!=3 && argc!=5 && argc!=7){
		err(3,"./server [-t tcp_port] [-u udp_port] [-p popis]");
	}

	while ((option = getopt(argc, argv, "t:u:p:")) != -1){
		switch (option){
			case 't':
				strcpy(tcp_port_addr, optarg);
				break;
			case 'u':
				strcpy(udp_port_addr, optarg);
				break;
			case 'p':
				strcpy(popis, optarg);
				break;
			default:
				err(3,"./server [-t tcp_port] [-u udp_port] [-p popis]");
				break;
		}
	}
	
	popis[strlen(popis)] = '\n';

	
	fromServerLen = sizeof(fromServerAddr);
	//TCP
	memset(&hints, 0, sizeof(hints));
	
    hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
	Getaddrinfo(NULL, tcp_port_addr, &hints, &res);
	

    myTcpSock = Socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    
	Setsockopt(myTcpSock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	Bind(myTcpSock, res->ai_addr, res->ai_addrlen);
	
	Listen(myTcpSock, 1);
	
	//UDP
	
	memset(&hints, 0, sizeof(hints));
	
    hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_DGRAM;
    
	Getaddrinfo(NULL, udp_port_addr, &hints, &res);
	
   
    myUdpSock = Socket(res->ai_family,	res->ai_socktype,res->ai_protocol);
    
	Bind(myUdpSock, res->ai_addr, res->ai_addrlen);
	
	if (myTcpSock > myUdpSock)
		fdmax = myTcpSock;
	else
		fdmax = myUdpSock;

	while(1){
		
		FD_ZERO(&readfds);
		FD_SET(STDIN, &readfds);
		FD_SET(myTcpSock, &readfds);
		FD_SET(myUdpSock, &readfds);
		
		Select(fdmax + 1, &readfds, NULL, NULL, &tv);
		

		if (FD_ISSET(myTcpSock, &readfds)) {
			//printf("Tcp\n");

			recieved = Accept(myTcpSock, &fromServerAddr, &fromServerLen);
				
			len = recv(recieved, &message[0], MAXLEN, 0);
			
			if (strcmp(message, "HELLO\n") == 0) {
				send(recieved, popis, strlen(popis), 0);
			}			
            close(recieved);
		}
		
		if (FD_ISSET(myUdpSock, &readfds)) {
			//printf("Udp\n");

			recieved = Recvfrom(myUdpSock, message, MAXLEN, 0, &fromServerAddr, &fromServerLen);
			if (strcmp(message, "HELLO\n") == 0) {
				Sendto(myUdpSock, popis, MAXLEN, 0, &fromServerAddr, fromServerLen);
			}			
		}
		
		if (FD_ISSET(STDIN, &readfds)) {
			fgets(input, MAXLEN, stdin);
			//printf("STDIN\n");

			if (strncmp("PRINT", input, 5) == 0) {
				printf("Prinitng payload: %s", popis);
			} else if (strncmp("SET", input, 3) == 0) {
				memset(&popis, 0, sizeof(popis));
				printf("Payload set to: %s", input);
				strcpy(popis, input + 4);
			} else if (strncmp("QUIT", input, 4) == 0) {
				return 0;
			}
		}
	}
}
	
	
	
