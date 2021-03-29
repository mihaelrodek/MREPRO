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

#define MAXLEN 512
#define PORT 5555


int main(int argc, char *argv[]){
	
	char *server_ip;
	char *reg = "REG\n";
	char *hello = "HELLO\n";
	char buffer[MAXLEN];
	char msg[MAXLEN];
	char *payload="payload";

	//char server_port[MAXLEN];
	
	int mysock,n,g;
	int port;
	struct sockaddr_in seraddr, udpser;
	
	
	//port 5555
	//- prvo posalje UDP sadrzaja "REG"
	//- nakon toga slusa komandu pa parove ip:port
	
	if (argc!=3){
		err(3,"Usage: ./bot server_ip server_port");
	}
	
	server_ip= argv[1];
	port=atoi(argv[2]);
	
	
	mysock = socket(PF_INET, SOCK_DGRAM, 0);
	
	memset(&seraddr, 0, sizeof(seraddr));
	memset(&udpser, 0, sizeof(udpser));
	
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(port);
	inet_pton(AF_INET, server_ip, &seraddr.sin_addr);
	

	socklen_t len = sizeof(struct sockaddr);

	sendto(mysock, (const char *)reg, sizeof(reg), 0, (struct sockaddr *)&seraddr, len);
	
	//printf("poslano\n");
	
	while(1){
		
		//printf("primam\n");
		n = recvfrom(mysock, (char *)buffer, MAXLEN, 0, (struct sockaddr *) &seraddr, &len);
		
		if(n==-1){
			perror("Error, cannot recieve anything\n");
			exit(1);
		}

		//printf("primljeno\n");
		
		buffer[n] = '\0';
		
		printf("%d\n",strlen(buffer));
		
		
		/**
		 *!!!! 
		 * Nije mi bilo jasno na koji tocno nacin da uzimam ip i adresu za svaki pojedini port, odnosno kako se oni salju
		 * Osim toga mislim da bi trebalo raditi ako se to slozi jer mi je za hardkodirane primjere vratilo ocekivani rezultat.
		 * Mozete li mi to objasniti pa da popravim do srijede za labos? Hvala.
		 * 
		 */
		
		if(buffer[0]=='0'){
		
			
			char subbuff[10];
			memcpy( subbuff, &buffer[1], 9 );
			subbuff[9] = '\0';
			
		
			udpser.sin_family = AF_INET;
			udpser.sin_port = htons(PORT);
			inet_pton(AF_INET, subbuff, &udpser.sin_addr);
			
			sendto(mysock, (const char *)hello, MAXLEN, 0, (struct sockaddr *)&udpser, len);
			printf("tu!\n");
			g = recvfrom(mysock, (char *)msg, MAXLEN, 0, (struct sockaddr *) &udpser, &len);
			
			if(g==-1){
				perror("Payload not recieved\n");
				exit(1);
			}
			
			msg[n]='\0';
			payload=msg;
			
			printf("Payload:%s\n", payload);
			
			
		}else if(buffer[0]=='1'){
			
			char subbuff[10];
			memcpy( subbuff, &buffer[1], 9 );
			subbuff[9] = '\0';
				
			udpser.sin_family = AF_INET;
			udpser.sin_port = htons(PORT);
			inet_pton(AF_INET, subbuff, &udpser.sin_addr);
			
			sendto(mysock, (const char *)hello, MAXLEN, 0, (struct sockaddr *)&udpser, len);
			printf("tu!\n");
			g = recvfrom(mysock, (char *)msg, MAXLEN, 0, (struct sockaddr *) &udpser, &len);
			
			if(g==-1){
				perror("Payload not recieved\n");
				exit(1);
			}
			
			msg[n]='\0';
			payload=msg;
			
			printf("Payload:%s\n", payload);
	
			
		}
		
		
	}
	
	close(mysock);
	
}




	
