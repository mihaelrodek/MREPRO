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

#define MAXLEN 512
#define PORT 1234

int main(int argc, char *argv[]){
	
	int port_addr=1234;
	int option;
	char *payload="", *buff;
	int mysock;
	struct sockaddr_in myaddr, cliaddr;
	socklen_t cllen;
	int received;

	if (argc!=1 && argc!=3 && argc!=5){
		err(3,"Usage: ./UDP_server [-l port] [-p payload]");
	}

	while ((option = getopt(argc, argv, "l:p:")) != -1){
		switch (option){
			case 'l':
				port_addr = atoi(optarg);
				break;
			case 'p':
				payload = optarg;
				break;
			default:
				port_addr=1234;
				payload="";
				break;
		}
	}
	
	if((mysock = socket(PF_INET, SOCK_DGRAM, 0))==-1){
		perror("Socket error");
		exit(1);
	}

	
	memset(&myaddr, 0, sizeof(myaddr));
	
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port_addr);
	myaddr.sin_addr.s_addr = INADDR_ANY;

	bind(mysock, (struct sockaddr *)&myaddr, sizeof(myaddr));


	while(1){

		received = recvfrom(mysock, &buff, MAXLEN,0,(struct sockaddr *)&cliaddr, &cllen);
		
		if(received==-1){
			perror("Error, couldn't receive data");
			exit(1);
		}
		
		//printf("Cekam hello\n");
		
		if(strcmp("HELLO\n", buff)==0){
			//printf("HELLO recieved\n");
			sendto(mysock, payload, MAXLEN,0,(struct sockaddr *)&cliaddr, cllen);
		}
	}
	
	close(mysock);
	
}


