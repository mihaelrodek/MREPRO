#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <arpa/inet.h>
#include "wrapper.h"

#define MAXLEN 256
#define S_PORT 1234
#define BACKLOG 10
#define MAXDATASIZE 1024

int main (int argc, char *argv[]){

    if(argc < 3 || argc > 7) {
        printf("Usage: ./UDP_server [-t tcp_port] [-u udp_port] [-p popis] %d\n", argc);
        return -1;
    }

    printf("Starting TCP/UDP server...\n");
    printf("\n");

    int mysock, sockfd, newfd, ch, n, rv, error, numbytes, msglen;
	char *tcp_port = "1234", *udp_port = "1234", *hello = "HELLO\n", *payload;
    char newlist[] = "", buffer[MAXLEN] = {}, str[INET_ADDRSTRLEN];
	
    struct sockaddr their_addr;
    struct sockaddr_in cli;
    struct addrinfo hints, *res;
    socklen_t sin_size, clilen;
    
    fd_set readfds;
    struct timeval tv;
	
	tv.tv_sec = 5;
	tv.tv_usec = 0;

    while((ch = getopt(argc, argv, "t:u:p:")) != -1){
        switch(ch) {
            case 't': tcp_port = optarg;
                      break;
		    case 'u': udp_port = optarg;
                      break;
            case 'p': payload = optarg;
                      break;
            default: printf("Usage: ./server [-t tcp_port] [-u udp_port] [-p payload]");
                     return -1;
        }
    }
    
    strcat(payload, "\n");
    printf("Payload: %s", payload);
    printf("TCP port: %s\n", tcp_port);
    printf("UDP port: %s\n", udp_port);
    
    printf("\n");
    
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    Getaddrinfo(NULL, udp_port, &hints, &res);
    
    mysock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	Bind(mysock, res->ai_addr, res->ai_addrlen);
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags |= AI_PASSIVE;
	
	Getaddrinfo(NULL, tcp_port, &hints, &res);
	
	sockfd = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int));

	Bind(sockfd, res->ai_addr, res->ai_addrlen);

	Listen(sockfd, BACKLOG);
    
    while(1) {
		
		memset(&buffer, 0, sizeof(buffer));
		
		n = sockfd + 1;
		        
        FD_SET(0, &readfds);
        FD_SET(mysock, &readfds);
		FD_SET(sockfd, &readfds);
		
        rv = Select(n, &readfds, NULL, NULL, &tv);
        
        if (rv == 0) {
			printf("Timeout occurred!\n");
		} else {
			
			if(FD_ISSET(mysock, &readfds)){
				
				clilen = sizeof(cli);
				Recvfrom(mysock, buffer, MAXLEN, 0, (struct sockaddr *)&cli, &clilen);
				
				if(strncmp(buffer, hello, strlen(hello)) == 0){	
					Sendto(mysock, payload, strlen(payload), 0, (struct sockaddr *)&cli, clilen);
				}

			}
			
			if(FD_ISSET(sockfd, &readfds)){
				
				sin_size = sizeof (struct sockaddr);
				newfd = Accept(sockfd, &their_addr, &sin_size);
				
				numbytes = Read(newfd, buffer, MAXDATASIZE);
				
				buffer[numbytes] = '\0';
				
				if(strncmp(buffer, hello, strlen(hello)) == 0){	
					
					Send(newfd, payload, strlen(payload), 0);
				}
				
			}
			
			if(FD_ISSET(0, &readfds)){
				
				fgets(buffer, 1024, stdin);
				
				if(strncmp(buffer, "PRINT", 5) == 0){	
					
					printf("Payload: %s\n", payload);
					
				} else if(strncmp(buffer, "SET", 3) == 0){	
					
					memset(&newlist, 0, sizeof(newlist));
					strcpy(newlist, buffer + 4);
					
					payload = newlist;
					
					printf("Novi payload: %s\n", payload);
					
				} else if(strncmp(buffer, "QUIT", 4) == 0){	
					
					return 0;
				}
			}
		}
		
		FD_CLR(0, &readfds);
		FD_CLR(mysock, &readfds);
		FD_CLR(sockfd, &readfds);
    }
    return 0;
}
