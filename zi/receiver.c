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
#define N 20

int pronadi_max(int x, int y) {
    if(x > y) return x;
    else return y;
}


int main (int argc, char *argv[]){
	
	int on=1;
	int option, timeout, pos=0, recieved, index, maxfd,port;
	bool tcp_flag = false, udp_flag = false, timeout_flag=false;
	char *string;
	int portnum=0,j=0;
	int sockfd_tcp=0, sockfd_udp=0, sockfd=0;
	char ports[20];
	struct addrinfo hints, *res;
	fd_set readfds;
	int tcp_sockfd[N], udp_sockfd[N];

	
	while ((option = getopt(argc, argv, "i:tu")) != -1 ) {
		switch (option) {
			case 'i':	
				pos+=2;
				timeout_flag = true;
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

	index = optind;
	string = argv[index];
	index++;
	printf("string: %s\n", string);

	for(int i=index; i<argc;i++){
        strcpy(&ports[j++], argv[i]);
	}
	
	portnum=j;
	
	struct timeval tv = {timeout,0};

	
	FD_ZERO(&readfds);
	
	if(tcp_flag == true && udp_flag==false){
	
		//TCP
		
		for(int k=0;k<portnum;k++){
			
			memset(&hints, 0, sizeof(hints));
			memset(&res, 0, sizeof(res));

            sockfd = Socket(AF_INET, SOCK_STREAM, 0);
            tcp_sockfd[k] = sockfd;

			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;

			Getaddrinfo(NULL, &ports[k], &hints, &res);

			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

			Bind(sockfd, res->ai_addr, res->ai_addrlen);
				
			Listen(sockfd, BACKLOG);


			//recieved = Recvfrom(mysock, buff, MAXLEN, 0, &cliaddr, &clilen);
			//printf("\t Local: \t Remote:\n");

			//printf("proto  %s:%s\t %s:%s", buff,buff,buff,buff);


		}
		
	}else if(tcp_flag == false && udp_flag==true){
		
		//UDP
		
		for(int k=0;k<portnum;k++){
			memset(&hints, 0, sizeof hints);
			memset(&res, 0, sizeof(res));

            sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
            udp_sockfd[k] = sockfd;

            hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags = AI_PASSIVE;

			Getaddrinfo(NULL, &ports[k], &hints, &res);

			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
			
			Bind(sockfd,res->ai_addr, res->ai_addrlen);
			
			//recieved = Recvfrom(sockfd, buff, MAXLEN, 0, &cliaddr, &clilen);
			//printf("\t Local: \t Remote:\n");

			//printf("proto  %s:%s\t %s:%s", buff,buff,buff,buff);

			//FD_SET(sockfd, &readfds);
		}
		
	}else{
		//TCP I UDP

        for(int k=0;k<portnum;k++) {
            //TCP
            memset(&hints, 0, sizeof(hints));
            memset(&res, 0, sizeof(res));

            sockfd = Socket(AF_INET, SOCK_STREAM, 0);
            tcp_sockfd[k] = sockfd;

            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;

            Getaddrinfo(NULL, &ports[k], &hints, &res);

            setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

            Bind(sockfd, res->ai_addr, res->ai_addrlen);

            Listen(sockfd, BACKLOG);

            //UDP
            memset(&hints, 0, sizeof hints);
            memset(&res, 0, sizeof(res));

            sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
            udp_sockfd[k] = sockfd;

            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_DGRAM;
            hints.ai_flags = AI_PASSIVE;

            Getaddrinfo(NULL, &ports[k], &hints, &res);

            setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

            Bind(sockfd,res->ai_addr, res->ai_addrlen);

        }

	}

	maxfd=-1;

    while(1){

        for(int k=0;k<portnum;k++) {

            if (udp_flag == 1 || (udp_flag == 0 && tcp_flag == 0)) {
                FD_SET(udp_sockfd[k], &readfds);
                maxfd = pronadi_max(maxfd, udp_sockfd[k]);
            }

            if (tcp_flag == 1 || (udp_flag== 0 && tcp_flag == 0)) {
                FD_SET(tcp_sockfd[k], &readfds);
                maxfd = pronadi_max(maxfd, tcp_sockfd[k]);
            }
        }

        if(timeout_flag == true)
            recieved = Select(maxfd+1, &readfds, NULL, NULL, &tv);
        else
            recieved = Select(maxfd+1, &readfds, NULL, NULL, NULL);

        for(int l=0; l < portnum;l++){

            if(FD_ISSET(udp_sockfd[l], &readfds) && (udp_flag == 1 || (udp_flag==0 && tcp_flag==0))){
                sockfd_udp = udp_sockfd[l];
                port = ports[l];
                break;
            }

            if(FD_ISSET(tcp_sockfd[l], &readfds) && (tcp_flag == 1 || (udp_flag==0 && tcp_flag==0))){
                sockfd_tcp = tcp_sockfd[l];
                port = ports[l];
                break;
            }
        }


    }
}
