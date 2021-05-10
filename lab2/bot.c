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

#define MAXLEN 1024

struct IP_port {
	char IP[INET_ADDRSTRLEN];
	char PORT[22];
};
	
struct MSG {
	char command;
	struct IP_port ip_pairs[20];
};

int main(int argc, char *argv[]){
	
	int mySock, myTCPSock, msgLen,i;
    const int on = 1;
	
	struct addrinfo hints, hints2, *res, *msgres;
	struct sockaddr serveraddr, addr;
	socklen_t serveraddrlen, addrlen;


	int CCport, recieved;
	char IP[INET_ADDRSTRLEN];

    char msg[MAXLEN] = {0};

	char *reg = "REG\n", *hello = "HELLO\n", *hostname, *port;
    char command;
    
	struct MSG message;
	
	if (argc!=3){
		err(3,"Usage: ./bot ip port");
	}
	
	hostname= argv[1];
	port=argv[2];
	
	
	addrlen = sizeof(addr);
	
	memset(&hints, 0, sizeof(hints));
	//memset(&msghints, 0, sizeof(msghints));


	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags |=  AI_CANONNAME;
	
	Getaddrinfo(hostname, port, &hints, &res);
	
	mySock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
    Setsockopt(mySock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
		
	*(&addr) = *(res->ai_addr);
	
	CCport = ntohs(((struct sockaddr_in *)(&addr))->sin_port);
	inet_ntop(AF_INET,&(((struct sockaddr_in *)(&addr))->sin_addr), &IP[0],INET_ADDRSTRLEN);
	
	//printf("%s:%d\n", &IP[0], CCport);

    recieved = Sendto(mySock, reg, strlen(reg), 0, (&addr), addrlen);

	while(1) {
		
		for (i = 0; i < 20; i++) {
			memset(&message.ip_pairs[i].IP[0], 0, sizeof(message.ip_pairs[i].IP));
			memset(&message.ip_pairs[i].PORT[0], 0, sizeof(message.ip_pairs[i].PORT));
		}
		
		serveraddrlen = sizeof(serveraddr);
		recieved = Recvfrom(mySock, &message, MAXLEN, 0, &serveraddr, &serveraddrlen);

		command = message.command;
		
		if(command== '0'){
			//exit(1);
			return 0;
			
		}else if(command=='1'){
			
			memset(&msg[0], 0, sizeof(msg));

			Getaddrinfo(&message.ip_pairs[0].IP[0],&message.ip_pairs[0].PORT[0],&hints2,&msgres);
			
			myTCPSock = Socket(msgres->ai_family,msgres->ai_socktype,msgres->ai_protocol);
			Connect(myTCPSock, msgres->ai_addr, msgres->ai_addrlen);
			
			recieved = send(myTCPSock, hello, strlen(hello), 0);


			msgLen = recv(myTCPSock, &msg[0], MAXLEN, 0);
			//printf("Recieved: '%s'\n", msg);
			
			close(myTCPSock);

		}else if(command=='2'){
			
			Getaddrinfo(&message.ip_pairs[0].IP[0],&message.ip_pairs[0].PORT[0],&hints,&res);
		
			recieved = Sendto(mySock, hello, strlen(hello), 0, res->ai_addr, res->ai_addrlen);

			memset(&msg[0], 0, sizeof(msg));
			
			msgLen = recvfrom(mySock, &msg, MAXLEN, 0, &serveraddr, &serveraddrlen);
			//printf("Recieved: '%s'\n", recMsg);
		}else if(command=='3'){
			//Jel mi mozete objasniti na koji nacin napisati za 3 i 4 da ispravim do labosa?
		}else if(command=='4'){
			//printf("Stopped\n");
			close(mySock);
			close(myTCPSock);
		
		}
		
		
	}
	
	
	
}
