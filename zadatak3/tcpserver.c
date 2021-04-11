#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <syslog.h>
#include <signal.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdbool.h>


#define PORT 1234
#define MAXLEN 25600

int Socket(int family, int type, int protocol){
	int n;

	if ((n=socket(family,type,protocol))==-1){
		printf("%s\n", strerror(errno));
		exit(4);
	} else {
		return n;
	}
}

int Listen(int sockfd, int backlog){
	int n;
	if ((n=listen(sockfd, backlog)) == -1) {
		printf("%s\n", strerror(errno));
		return -3;
	}else return n;
}

int Accept( int sockfd,struct sockaddr* cliaddr,socklen_t *addrlen){
	int n;
	if((n=accept(sockfd, cliaddr, addrlen))==-1){
		printf("%s\n",  strerror(errno));
		return -4;
	}else return n;
	
}

int Bind(int sockfd, const struct sockaddr *myaddr, int addrlen){
	
	if(bind(sockfd, myaddr, addrlen)==-1){
		printf("%s\n", strerror(errno));
		exit(4);
	} else {
		return 0;		
	}
}

struct respond {
	char offset[4];
	char *filename;
};
	

int main(int argc, char *argv[]) {	
	
	int mysocket, newsock;
	int option, port = PORT;
	//bool p_flag = false;
	int offset, i, pomak=0;

	char messagebuff[MAXLEN];
	char *ime;
	
	FILE *datoteka;
	struct stat info;
	char *buffer;
	
	struct sockaddr_in myaddr, clientaddr;
	struct servent *service;
	socklen_t serverlen;
	
	
	if ((argc != 1) && (argc != 3))	{
		printf("USAGE: ./tcpserver [-p port]\n");
		exit(1);
	}
	
	while ((option = getopt(argc, argv, "p:")) != -1 ) {					
		switch (option) {
			case 'p':	
				port = atoi(optarg);
				if (port > 65535) {
					printf("Invalid port format number given.\n");
					return 2;
				}
				if (!port) {
					//getaddrinfo()
					service = getservbyname(optarg, "tcp");
					if (!service) {
						printf("Invalid port name given.\n");
						return 2;
					}
					service->s_port = ntohs(service->s_port);	
					port = service->s_port;
				}
				break;
				
			default : 	
				printf("USAGE: ./tcpserver [-p port]\n");
				return 2;
		}
	}
	
	mysocket = Socket(PF_INET, SOCK_STREAM, 0);
	
	
	memset(&myaddr, 0, sizeof(myaddr));	
	
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons (port);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	Bind(mysocket, (struct sockaddr *) &myaddr, sizeof(myaddr));
		
	
	Listen(mysocket, 1);
	
	struct respond message;
	
	while (1) {
		
		serverlen = sizeof clientaddr;

		newsock = Accept(mysocket, (struct sockaddr *)&clientaddr, &serverlen);
		
		int len=recv(newsock, (char *)&message, sizeof(message), 0);
		
		for ( i = 0; i < 4; i++) {
			offset = message.offset[i] & 0xFF;
			offset = offset << 8*i;
			pomak |= offset;
		}
			
		ime=message.filename;

		//if (strcmp(messagebuff, "\0")) {
			//printf("Kraj datoteke.\n");
			//close(newsock);
			//close(mysocket);
			//return 0;printf("%s\n", strerror(errno));
		//}
	
	
		if (strchr(ime, '/') ) {
			buffer = 0x03;
			printf("Dan zahtijev za datotekom izvan trenutnog direktorija.\n");
			close(newsock);
			close(mysocket);
			return -5;
		}else if ((datoteka = fopen(ime, "rb")) == NULL) {
			buffer = 0x01;
			write(newsock, buffer, strlen(buffer)+1);
			printf("%s\n", strerror(errno));
			close(newsock);
			close(mysocket);
			return -6;
		}
	
		fstat(fileno(datoteka), &info);
		buffer = malloc(info.st_size + 1);	
		fread(buffer, 1, info.st_size, datoteka);
	
		option = write(newsock, buffer, strlen(buffer)+1);


		free(buffer);
		close(newsock);
		fclose(datoteka);
	}
	
	
}
