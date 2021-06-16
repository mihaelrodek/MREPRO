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
#include "wrapper.h"
#include <stdbool.h>

int main (int argc, char *argv[]){
	
	
	int option, seconds=0, delay=0;
	bool tcp_flag = false, udp_flag = false, r_flag = false, delay_flag=false;
	char *string = "string";
	int pos=1;
	
	
	while ((option = getopt(argc, argv, "r:d:tu")) != -1 ) {
		switch (option) {
			case 'r':			
				pos+=2;
				r_flaf = true;
				seconds = atoi(optarg);
				break;
			case 'd':
				pos+=2;
				delay_flag = true;
				delay=atoi(optarg);
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
				printf("USAGE: ./sender [-r sec] [-d delay] [-t |-u] \"string\" ip_adresa port ...\n");
				return 2;
		}
	}
	
	string = argc[pos];
	
	for(int i=pos;i<
	
	
	
	if(tcp_flag == true && udp_flag==falase){
	
		//TCP
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = 0;
		hints.ai_flags |= AI_PASSIVE;

		Getaddrinfo(NULL, tcp_port, &hints, &res);
		sockfd = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

		Bind(sockfd, res->ai_addr, res->ai_addrlen);
			
		Listen(sockfd, BACKLOG);
		
	}else if(tcp_flag == false && udp_flag==true){
		//UDP
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;

		Getaddrinfo(NULL, "5555", &hints, &res);

		mysock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		bind(mysock,res->ai_addr, res->ai_addrlen);
		
	}else{
		//TCP I UDP
		
	}
	
		
	
}












