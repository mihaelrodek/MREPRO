#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>


#define MAXLEN 85600
#define PORT 1234

int Socket(int family, int type, int protocol){
	int n;

	if ((n=socket(family,type,protocol))==-1){
		printf("%s\n", strerror(errno));
		exit(4);
	} else {
		return n;
	}
}

int Connect(int sockfd, const struct sockaddr *server, socklen_t addrlen){
	int n;
	if((n=connect(sockfd, server, addrlen)==-1)){
		printf("%s\n", strerror(errno));
		exit(4);
	}else return n;
	
}

int Getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result) {
	
	if (getaddrinfo(hostname, service, hints, result)==-1){
		strerror(6);
		exit(5);
	} else {
		return 0;
	}
}

int main (int argc, char *argv[]) {

	int port=1234, option;
	int i;
	bool p_flag = false, c_flag=false;
	int mysocket;
	int msglength, err;
	char *error;
	char message[MAXLEN];
	struct sockaddr_in serveraddr;
	struct addrinfo hints, *result;
	socklen_t serverlen;
	struct servent *service;
	int pomak;
	
	memset(&serveraddr, 0, sizeof(serveraddr));	
	
	if (argc < 2 || argc >7)	{
		err(3,"USAGE: ./tcpklijent [-s server] [-p port] [-c] filename\n");
	}
		
	while ((option = getopt(argc, argv, "s:p:c")) != -1 ) {
		switch (option) {
			case 'p':				
				port = atoi(optarg);
				if (port > 65535) {
					printf("Invalid port format number given.\n");
					return 2;
				}
				if (!port) {
					service = getservbyname(optarg, "tcp");
					if (!service) {
						printf("Invalid port format name given.\n");
						return 2;
					}
					service->s_port = ntohs(service->s_port);	
					port = service->s_port;
				}	
				break;
			case 's':
				server=atoi(optarg);
				
			case 'c':
				c_flag = true;
				break;			
			default :
				printf("USAGE: ./tcpklijent [-s server] [-p port] [-c] filename\n");
				return 2;
		}
	}

	
	if (inet_pton(AF_INET, server, &serveraddr.sin_addr) != 1) {

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_flags |= AI_CANONNAME;		
		Getaddrinfo(server, NULL, &hints, &result);
		
		serveraddr.sin_addr = ((struct sockaddr_in *) result->ai_addr)->sin_addr;

	}	
	
	//inet_ntop(AF_INET, &(serveraddr.sin_addr).s_addr, message, MAXLEN);
	//printf("Spojen sa: %s\n", temp);
	
	
	mysocket = Socket(AF_INET, SOCK_STREAM, 0);
		
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serverlen = sizeof(serveraddr);
	
	Connect(mysocket, (struct sockaddr *) &serveraddr, sizeof serveraddr);
		
	for (i = 0; i < strlen(argv[argc-1]); i++) {
		message[4+i] = argv[argc-1][i];
	}
	
	if ( strchr(argv[argc-1], '/') ) {
		msglength = strlen(argv[argc-1]);	
		i = send(mysocket, message, msglength + 5, 0);
		return 0;
	}
	
	if (c_flag) {	
		datoteka = fopen(argv[argc-1], "ab");	
		fstat(fileno(datoteka), &info);					//info
		pomak = info.st_size;
	}else {	
		if ((datoteka = fopen(argv[argc-1], "rb")) != NULL) {
			fclose(datoteka);
		}		
		datoteka = fopen(argv[argc-1], "wb");
		pomak = 0;
	}
	
	msglength = strlen(argv[argc-1]);		
	i = send(mysocket, message, msglength + 5, 0);
	
	//printf("POSLANO.\n");

	close(mysocket);
	return 0;

}
