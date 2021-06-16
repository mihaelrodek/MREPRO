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


#define BACKLOG 20
#define MAXLEN 512

struct IP_PORT{
	char IP[INET_ADDRSTRLEN];
    char PORT[22];
};

int main (int argc, char *argv[]){
	
	
	const int on = 1;
	
	int option, seconds=0, delay=0;
	bool tcp_flag = false, udp_flag = false, r_flag = false, delay_flag=false;
	char *string = "string";
	int pos=1,j=0, portnum=0;
	int sockfd, mysock, recieved;
	char *IP_address, *buff;
	int k=0;
	
	struct addrinfo hints, *res;
    struct sockaddr cli, tcp_addr;
    struct sockaddr_in clientaddr;
    struct sockaddr cliaddr;
	socklen_t clilen;
	
	struct IP_PORT pairs[20];
	
	struct timeval {
		long tv_sec;
		long tv_usec;
	};
	
	while ((option = getopt(argc, argv, "r:d:tu")) != -1 ) {
		switch (option) {
			case 'r':			
				pos+=2;
				r_flag = true;
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
	
	
	struct timeval cekaj_sec = {seconds,0};
	struct timeval cekaj_delay = {delay,0};

	
		//FLAGS
	printf("%s, %s, %s, %s\n", r_flag ? "true" : "false", delay_flag ? "true" : "false",tcp_flag ? "true" : "false", udp_flag ? "true" : "false");
	
	printf("pos:%d, argc:%d\n", pos, argc);
	
	string = argv[pos];
	
	for(int i=pos+2;i<argc;i++){
		pairs[j].IP[0] = argv[pos+1];
		pairs[j].PORT[0] = argv[i];
		j++;
	}
	
	IP_address = argv[pos+1];

	portnum=j+1;
	printf("postnum:%d\n", portnum);
	
	if(tcp_flag == true && udp_flag==false){
	
		//TCP
		
		for(k=0;k<portnum;k++){
			
			memset(&hints, 0, sizeof(hints));
			memset(&res, 0, sizeof(res));
			
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = 0;
			hints.ai_flags |= AI_PASSIVE;

			Getaddrinfo(IP_address, pairs[k].PORT[0], &hints, &res);
			
			sockfd = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
			

			Bind(sockfd, res->ai_addr, res->ai_addrlen);
				
			Listen(sockfd, BACKLOG);
			
			setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &cekaj_sec, sizeof(struct timeval));
			if(seconds != 0)
				if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &cekaj_sec, sizeof(struct timeval))>=0){
					
					recieved = Recvfrom(sockfd, buff, MAXLEN, 0, &cliaddr, &clilen);
					
					if(recieved!=-1)
						printf("TCP %d open\n", pairs[k].PORT[0]);
					
				}
				
			sleep(delay);
		
		}
		
	}else if(tcp_flag == false && udp_flag==true){
		
		//UDP
		
		for(k=0;k<portnum;k++){
			memset(&hints, 0, sizeof hints);
			memset(&res, 0, sizeof(res));
				
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags = AI_PASSIVE;

			Getaddrinfo(IP_address, pairs[k].PORT[0], &hints, &res);
			mysock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			
			Bind(mysock,res->ai_addr, res->ai_addrlen);
			
			recieved = Sendto(mysock, string, strlen(string), 0, &cliaddr, clilen);
			
			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
			
			if(seconds != 0)
				if (setsockopt(mysock, SOL_SOCKET, SO_RCVTIMEO, &cekaj_sec, sizeof(struct timeval))>=0){
					
					recieved = Recvfrom(mysock, buff, MAXLEN, 0, &cliaddr, &clilen);
					
					if(recieved!=-1)
						printf("UDP %d open\n", pairs[k].PORT[0]);
					
				}
			sleep(delay);
		}
		
	}else{
		//TCP I UDP
		
		for(k=0;k<portnum;k++){
			
			memset(&hints, 0, sizeof hints);
			memset(&res, 0, sizeof(res));
				
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags = AI_PASSIVE;

			Getaddrinfo(IP_address, pairs[k].PORT[0], &hints, &res);
			mysock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			
			Bind(mysock,res->ai_addr, res->ai_addrlen);
			
			recieved = Sendto(mysock, string, strlen(string), 0, &cliaddr, clilen);
			
			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
			
			if(seconds != 0)
				if (setsockopt(mysock, SOL_SOCKET, SO_RCVTIMEO, &cekaj_sec, sizeof(struct timeval))>=0){
					
					recieved = Recvfrom(mysock, buff, MAXLEN, 0, &cliaddr, &clilen);
					
					if(recieved!=-1)
						printf("UDP %d open\n", pairs[k].PORT[0]);
					
				}
				
			setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));

			memset(&hints, 0, sizeof(hints));
			memset(&res, 0, sizeof(res));
			
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = 0;
			hints.ai_flags |= AI_PASSIVE;

			Getaddrinfo(IP_address, pairs[k].PORT[0], &hints, &res);
			
			sockfd = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
			
			Bind(sockfd, res->ai_addr, res->ai_addrlen);
				
			Listen(sockfd, BACKLOG);
			
			setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &cekaj_sec, sizeof(struct timeval));
			if(seconds != 0)
				if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &cekaj_sec, sizeof(struct timeval))>=0){
					
					recieved = Recvfrom(sockfd, buff, MAXLEN, 0, &cliaddr, &clilen);
					
					if(recieved!=-1)
						printf("TCP %d open\n", pairs[k].PORT[0]);
					
				}
				
			sleep(delay);
			
		}
	}
	
		
	
}












