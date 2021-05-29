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

#define MAXLEN 100
#define BACKLOG 10
#define TCP_PORT 80

struct BOTS{
    char hbuf[NI_MAXHOST];
    char sbuf[NI_MAXHOST];
};

struct IP_PAIRS {
    char IP[INET_ADDRSTRLEN];
    char PORT[22];
};

struct CANDC {
    char command;
    struct IP_PAIRS addresses[20];
};


int main (int argc, char *argv[]){
   
	bool bot_existing;
    
	const int on = 1;
    int i, mysock, error, sinsize, n, sockfd, newfd, ch, pid, recieved, filesize, bot_counter = 0;
    socklen_t sin_size;
	
    struct addrinfo hints, *res;
    struct sockaddr cli, tcp_addr;
    struct sockaddr_in clientaddr;
    socklen_t clilen;
    
    char ubuf[MAXLEN] = "", tbuf[MAXLEN], hbuf[NI_MAXHOST], sbuf[NI_MAXSERV], bbuf[1], cmd[100];
	char *tcp_port = "80", *nepoznata = "NEPOZNATA\n", *response;
    const char delim[2] = " ";
	
	
    FILE *file, *file2;
    fd_set readfds;
    struct timeval tv;
	
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	
	struct BOTS bots[20];
    struct CANDC MSG;
    memset(&MSG, 0, sizeof MSG);
	
		
	if(argc > 2) {
        err(3,"Usage: ./CandC [tcp_port]\n");
        return -1;
    }
    
    for(i = optind; i < argc; i++){
        tcp_port = argv[i];
    }

    printf("CandC started\n");
    printf("\n");
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    Getaddrinfo(NULL, "5555", &hints, &res);

    mysock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(mysock,res->ai_addr, res->ai_addrlen);
    
    // TCP initialize
    
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
    

	while(1){

		if(mysock > sockfd){
			n = mysock + 1;
		} else {
			n = sockfd + 1;
		}
		        
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(mysock, &readfds);
        FD_SET(sockfd, &readfds);
		
        recieved = Select(n, &readfds, NULL, NULL, &tv);
        
		if (recieved == 0) {

			//printf("Timeout!\n");

		} else {
			
			if(FD_ISSET(0, &readfds)){
						
				memset(&cmd, 0, sizeof(cmd));
				fgets(cmd, 100, stdin);
				
				if(strncmp(cmd, "pt", 2) == 0){
					
					printf("--> PROG_TCP(pt)\n");
					
					struct CANDC smsg;
					strcpy(&smsg.command, "1");
					strcpy(smsg.addresses[0].IP, "10.0.0.20");
					strcpy(smsg.addresses[0].PORT, "1234");
					
					for(i = 0; i < bot_counter; i++){
						memset(&clientaddr, 0, sizeof(clientaddr));
						clientaddr.sin_family = AF_INET;
						clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
						clientaddr.sin_port = htons(atoi(bots[i].sbuf));

						Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
						
						printf("SENT PROG_TCP message (pt) clientaddr %s:%s.\n", hbuf, sbuf);
							
					}
					
				} else if(strncmp(cmd, "ptl", 3) == 0){
					
					printf("--> PROG_TCP(ptl)\n");
					
					struct CANDC smsg;
					strcpy(&smsg.command, "1");
					strcpy(smsg.addresses[0].IP, "127.0.0.1");
					strcpy(smsg.addresses[0].PORT, "1234");
					
					for(i = 0; i < bot_counter; i++){
						memset(&clientaddr, 0, sizeof(clientaddr));
						clientaddr.sin_family = AF_INET;
						clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
						clientaddr.sin_port = htons(atoi(bots[i].sbuf));

						Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
						
						printf("SENT PROG_TCP message (pt1) clientaddr %s:%s.\n", hbuf, sbuf);
					
					}
				
				} else if(strncmp(cmd, "pu", 2) == 0){
					
					printf("--> PROG_UDP(pu)\n");
				
					struct CANDC smsg;
					strcpy(&smsg.command, "2");
					strcpy(smsg.addresses[0].IP, "10.0.0.20");
					strcpy(smsg.addresses[0].PORT, "1234");
					
					for(i = 0; i < bot_counter; i++){
						memset(&clientaddr, 0, sizeof(clientaddr));
						clientaddr.sin_family = AF_INET;
						clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
						clientaddr.sin_port = htons(atoi(bots[i].sbuf));

						Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
						
						printf("SENT PROG_UDP message (pu) clientaddr %s:%s.\n", hbuf, sbuf);
							
					}
									
				} else if(strncmp(cmd, "pul", 3) == 0){
					
					
					printf("--> PROG_UDP(pul)\n");
				
					struct CANDC smsg;
					strcpy(&smsg.command, "2");
					strcpy(smsg.addresses[0].IP, "127.0.0.1");
					strcpy(smsg.addresses[0].PORT, "1234");
					
					for(i = 0; i < bot_counter; i++){
						memset(&clientaddr, 0, sizeof(clientaddr));
						clientaddr.sin_family = AF_INET;
						clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
						clientaddr.sin_port = htons(atoi(bots[i].sbuf));

						Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
						
						printf("SENT PROG_UDP message (pu1) clientaddr %s:%s.\n", hbuf, sbuf);
							
					}
				
				} else if(strncmp(cmd, "r", 1) == 0){
					
					printf("--> RUN(r)\n");
				
					struct CANDC smsg;
					strcpy(&smsg.command, "3");
					strcpy(smsg.addresses[0].IP, "127.0.0.1");
					strcpy(smsg.addresses[0].PORT, "vat");
					strcpy(smsg.addresses[1].IP, "localhost");
					strcpy(smsg.addresses[1].PORT, "6789");
					
					for(i = 0; i < bot_counter; i++){
						memset(&clientaddr, 0, sizeof(clientaddr));
						clientaddr.sin_family = AF_INET;
						clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
						clientaddr.sin_port = htons(atoi(bots[i].sbuf));

						Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
						
						printf("SENT RUN message (r) clientaddr %s:%s.\n", hbuf, sbuf);
							
					}
				
				} else if(strncmp(cmd, "r2", 2) == 0){
					
					printf("--> RUN(r2)\n");
				
					struct CANDC smsg;
					strcpy(&smsg.command, "3");
					strcpy(smsg.addresses[0].IP, "20.0.0.11");
					strcpy(smsg.addresses[0].PORT, "1111");
					strcpy(smsg.addresses[1].IP, "20.0.0.12");
					strcpy(smsg.addresses[1].PORT, "2222");
					strcpy(smsg.addresses[2].IP, "20.0.0.13");
					strcpy(smsg.addresses[2].PORT, "dec-notes");
					
					for(i = 0; i < bot_counter; i++){
						memset(&clientaddr, 0, sizeof(clientaddr));
						clientaddr.sin_family = AF_INET;
						clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
						clientaddr.sin_port = htons(atoi(bots[i].sbuf));

						Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
						
						printf("SENT RUN message (r2) clientaddr %s:%s.\n", hbuf, sbuf);
					}
				
				}  else if(strncmp(cmd, "s", 1) == 0){
					
					printf("--> STOP\n");
				
					struct CANDC smsg;
					strcpy(&smsg.command, "4");
		
					for(i = 0; i < bot_counter; i++){
						memset(&clientaddr, 0, sizeof(clientaddr));
						clientaddr.sin_family = AF_INET;
						clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
						clientaddr.sin_port = htons(atoi(bots[i].sbuf));

						Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
						
						printf("SENT STOP message clientaddr %s:%s.\n", hbuf, sbuf);
					}
				
				}  else if(strncmp(cmd, "l", 1) == 0){
					
					printf("--> LIST\n");
					
					if(bot_counter > 0){
						printf("lista botova:\n");
						
						for(i = 0; i < bot_counter; i++){
							printf("%s:%s\n", bots[i].hbuf, bots[i].sbuf);
							//fflush( stdout );
						}
					} else {
						printf("Nema reigstriranih klijenata.\n");
					}
				
				}else if(strncmp(cmd, "n", 1) == 0){
					
					printf("--> NEPOZNATA\n");
				
					for(i = 0; i < bot_counter; i++){
						memset(&clientaddr, 0, sizeof(clientaddr));
						clientaddr.sin_family = AF_INET;
						clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
						clientaddr.sin_port = htons(atoi(bots[i].sbuf));

						Sendto(mysock, nepoznata, strlen(nepoznata), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
						
						printf("SENT NEPOZNATA message clientaddr %s:%s.\n", hbuf, sbuf);
							
					}
				
				} else if(strncmp(cmd, "q", 1) == 0){
					
					printf("--> QUIT\n");
				
					struct CANDC smsg;
					strcpy(&smsg.command, "0");
				
					for(i = 0; i < bot_counter; i++){
						memset(&clientaddr, 0, sizeof(clientaddr));
						clientaddr.sin_family = AF_INET;
						clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
						clientaddr.sin_port = htons(atoi(bots[i].sbuf));

						Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
						
						printf("SENT QUIT message clientaddr %s:%s.\n", hbuf, sbuf);
					}
					
					return 0;
				
				} else if(strncmp(cmd, "h", 1) == 0){
					
					printf("__________________________________________________________________________\n");
					printf("pt\t bot klijentima salje poruku PROG_TCP (struct MSG:1 10.0.0.20 1234)\n");
					printf("ptl\t bot klijentima salje poruku PROG_TCP (struct MSG:1 127.0.0.1 1234)\n");
					printf("pu\t bot klijentima salje poruku PROG_UDP (struct MSG:2 10.0.0.20 1234)\n");
					printf("pul\t bot klijentima salje poruku PROG_UDP (struct MSG:2 127.0.0.1 1234)\n");
					printf("r\t bot klijentima salje poruku RUN s adresama lokalnog racunala\n");
					printf(" \t struct MSG:3 127.0.0.1 vat localhost 6789\n");
					printf("r2\t bot klijentima salje poruku RUN s adresama iz IMUNES-a:\n");
					printf("  \t struct MSG:3 20.0.0.11 1111 20.0.0.12 2222 20.0.0.13 dec-notes\n");
					printf("s\t bot klijentima salje poruku STOP (struct MSG:4)\n");
					printf("l\t lokalni ispis adresa bot klijenata\n");
					printf("n\t salje poruku NEPOZNATA\n");
					printf("q\t salje poruku QUIT i zavrsava s radom (struct MSG:0)\n");
					printf("h\t ispis naredbi\n");
					printf("__________________________________________________________________________\n");
				} else {
					printf("Nepoznata naredba.\n");
				}
				
			}
			
			if(FD_ISSET(mysock, &readfds)){
			
				memset(&ubuf, 0, sizeof(ubuf));
				clilen = sizeof(cli);
				Recvfrom(mysock, &ubuf, sizeof(ubuf), 0, &cli, &clilen);

				if(strncmp(ubuf, "REG\n", 4) == 0){
					
					sinsize = sizeof (struct sockaddr);
					Getnameinfo(&cli,sinsize,hbuf,	sizeof(hbuf), sbuf, sizeof(sbuf), NI_NOFQDN | NI_NUMERICHOST | NI_NUMERICSERV);
					
					bot_existing = false;
					
					for(i = 0; i < bot_counter; i++){
						if(strcmp(bots[i].hbuf, hbuf) == 0 && strcmp(bots[i].sbuf, sbuf) == 0){
							bot_existing = true;
							break;
						}
					}
					
					if(bot_existing == false){
						printf("REG < %s:%s\n", hbuf, sbuf);
						
						strcpy(bots[bot_counter].hbuf, hbuf);
						strcpy(bots[bot_counter].sbuf, sbuf);
					
						bot_counter++;
					}
					
				}
				
			}
			
			if(FD_ISSET(sockfd, &readfds)){
				
				printf("----> TCP!\n");
			
				sin_size = sizeof (struct sockaddr);
				newfd = Accept(sockfd, &tcp_addr, &sin_size);
					
								
				if((pid = fork()) == 0){
					
					close(sockfd);
					
					recieved = read(newfd, tbuf, sizeof(tbuf));
						
					tbuf[recieved] = '\0';
					
					printf("primljeno: %s\n", tbuf);
					
					char *token;
					char *method, *path, *version;
				
					token = strtok(tbuf, delim);
					asprintf(&method, "%s", token);

					token = strtok(NULL, delim);
					asprintf(&path, "%s", token);
					
					token = strtok(NULL, delim);
					asprintf(&version, "%s", token);
					
					token = strtok(token, "\r\n");
					asprintf(&version, "%s", token);
					
					printf("Method: %s\n", method);
					printf("Path: %s\n", path);
					printf("Version: %s\n", version);
					
					if(strcmp(method, "GET") == 0){
						
						response = NULL;
						
						char *filename;
						asprintf(&filename, ".%s", path);
						printf("Filename: %s\n", filename);
						
						if(strncmp(path, "/bot/", 5) == 0){
							// REST zahtjev
							
							if(strcmp(path + 5, "prog_tcp_localhost") == 0){
								printf("--> PROG_TCP(ptl)\n");

								struct CANDC smsg;
								strcpy(&smsg.command, "1");
								strcpy(smsg.addresses[0].IP, "127.0.0.1");
								strcpy(smsg.addresses[0].PORT, "1234");
								
								for(i = 0; i < bot_counter; i++){
									memset(&clientaddr, 0, sizeof(clientaddr));
									clientaddr.sin_family = AF_INET;
									clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
									clientaddr.sin_port = htons(atoi(bots[i].sbuf));

									Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
									
									printf("SENT PROG_TCP message (ptl) clientaddr %s:%s.\n", hbuf, sbuf);
										
								}
								
								asprintf(&response, "%s\r\n", "HTTP/1.1 200 OK");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html><h3>Pokrenut ptl.</h3></html>", response);
								Send(newfd, response, strlen(response), 0);
								
							} else if(strcmp(path + 5, "prog_tcp") == 0){
								printf("--> PROG_TCP(pt)\n");

								
								struct CANDC smsg;
								strcpy(&smsg.command, "1");
								strcpy(smsg.addresses[0].IP, "10.0.0.20");
								strcpy(smsg.addresses[0].PORT, "1234");
								
								for(i = 0; i < bot_counter; i++){
									memset(&clientaddr, 0, sizeof(clientaddr));
									clientaddr.sin_family = AF_INET;
									clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
									clientaddr.sin_port = htons(atoi(bots[i].sbuf));

									Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
									
									printf("SENT PROG_TCP message (pt) clientaddr %s:%s.\n", hbuf, sbuf);
										
								}
								
								asprintf(&response, "%s\r\n", "HTTP/1.1 200 OK");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html><h3>Pokrenut pt.</h3></html>", response);
								Send(newfd, response, strlen(response), 0);
								
							} else if(strcmp(path + 5, "prog_udp_localhost") == 0){
								printf("--> PROG_UDP(pul)\n");

								
								struct CANDC smsg;
								strcpy(&smsg.command, "2");
								strcpy(smsg.addresses[0].IP, "127.0.0.1");
								strcpy(smsg.addresses[0].PORT, "1234");
								
								for(i = 0; i < bot_counter; i++){
									memset(&clientaddr, 0, sizeof(clientaddr));
									clientaddr.sin_family = AF_INET;
									clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
									clientaddr.sin_port = htons(atoi(bots[i].sbuf));

									Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
									
									printf("SENT PROG_UDP message (pul) clientaddr %s:%s.\n", hbuf, sbuf);
										
								}
								
								asprintf(&response, "%s\r\n", "HTTP/1.1 200 OK");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html><h3>Pokrenut pul.</h3></html>", response);
								Send(newfd, response, strlen(response), 0);
		
							} else if(strcmp(path + 5, "prog_udp") == 0){
								printf("--> PROG_UDP(pu)\n");

								struct CANDC smsg;
								strcpy(&smsg.command, "2");
								strcpy(smsg.addresses[0].IP, "10.0.0.20");
								strcpy(smsg.addresses[0].PORT, "1234");
								
								for(i = 0; i < bot_counter; i++){
									memset(&clientaddr, 0, sizeof(clientaddr));
									clientaddr.sin_family = AF_INET;
									clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
									clientaddr.sin_port = htons(atoi(bots[i].sbuf));

									Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
									
									printf("SENT PROG_UDP message (pu) clientaddr %s:%s.\n", hbuf, sbuf);
										
								}
								
								asprintf(&response, "%s\r\n", "HTTP/1.1 200 OK");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html><h3>Pokrenut pu.</h3></html>", response);
								Send(newfd, response, strlen(response), 0);

								
							} else if(strcmp(path + 5, "run2") == 0){
								
								printf("--> RUN(r2)\n");

								struct CANDC smsg;
								strcpy(&smsg.command, "3");
								strcpy(smsg.addresses[0].IP, "20.0.0.11");
								strcpy(smsg.addresses[0].PORT, "1111");
								strcpy(smsg.addresses[1].IP, "20.0.0.12");
								strcpy(smsg.addresses[1].PORT, "2222");
								strcpy(smsg.addresses[2].IP, "20.0.0.13");
								strcpy(smsg.addresses[2].PORT, "dec-notes");
								
								for(i = 0; i < bot_counter; i++){
									memset(&clientaddr, 0, sizeof(clientaddr));
									clientaddr.sin_family = AF_INET;
									clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
									clientaddr.sin_port = htons(atoi(bots[i].sbuf));

									Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
									
									printf("SENT RUN (r2) message clientaddr %s:%s.\n", hbuf, sbuf);
										
								}
								
								asprintf(&response, "%s\r\n", "HTTP/1.1 200 OK");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html><h3>Pokrenut r2.</h3></html>", response);
								Send(newfd, response, strlen(response), 0);
								
								
							} else if(strcmp(path + 5, "run") == 0){
								printf("--> RUN(r)\n");
								
								struct CANDC smsg;
								strcpy(&smsg.command, "3");
								strcpy(smsg.addresses[0].IP, "127.0.0.1");
								strcpy(smsg.addresses[0].PORT, "vat");
								strcpy(smsg.addresses[1].IP, "localhost");
								strcpy(smsg.addresses[1].PORT, "6789");
								
								for(i = 0; i < bot_counter; i++){
									memset(&clientaddr, 0, sizeof(clientaddr));
									clientaddr.sin_family = AF_INET;
									clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
									clientaddr.sin_port = htons(atoi(bots[i].sbuf));

									Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
									
									printf("SENT RUN (r) message clientaddr %s:%s.\n", hbuf, sbuf);
										
								}
								
								asprintf(&response, "%s\r\n", "HTTP/1.1 200 OK");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html><h3>Pokrenut r.</h3></html>", response);
								Send(newfd, response, strlen(response), 0);
								
							} else if(strcmp(path + 5, "stop") == 0){
								printf("--> STIOP\n");

								struct CANDC smsg;
								strcpy(&smsg.command, "4");
					
								for(i = 0; i < bot_counter; i++){
									memset(&clientaddr, 0, sizeof(clientaddr));
									clientaddr.sin_family = AF_INET;
									clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
									clientaddr.sin_port = htons(atoi(bots[i].sbuf));

									Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
									
									printf("SENT STOP message clientaddr %s:%s.\n", hbuf, sbuf);										
								}
								
								asprintf(&response, "%s\r\n", "HTTP/1.1 200 OK");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html><h3>Pokrenut stop.</h3></html>", response);
								Send(newfd, response, strlen(response), 0);
								
								
							} else if(strcmp(path + 5, "list") == 0){
								
								printf("--> LIST\n");
					
								if(bot_counter > 0){
									printf("lista botova:\n");
									
									for(i = 0; i < bot_counter; i++){
										printf("%s:%s\n", bots[i].hbuf, bots[i].sbuf);
										//fflush( stdout );
									}
								} else {
									printf("Nema reigstriranih klijenata.\n");
								}
								
								asprintf(&response, "%s\r\n", "HTTP/1.1 200 OK");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html>", response);
								
								if(bot_counter > 0){
									asprintf(&response, "%s<h3>Registrirani bot klijenti:</h3><br />", response);
									
									asprintf(&response, "%s<ul>", response);
									for(i = 0; i < bot_counter; i++){
										asprintf(&response, "%s<li>IP: %s, port: %s<br /></li>", response, bots[i].hbuf, bots[i].sbuf);
									}
									asprintf(&response, "%s</ul>", response);
								} else {
									asprintf(&response, "%s<h3>Nema registriranih botova.</h3><br />", response);
								}
								
								asprintf(&response, "%s</html>", response);
								Send(newfd, response, strlen(response), 0);
								
							} else if(strcmp(path + 5, "quit") == 0){
								printf("--> QUIT\n");
								
								struct CANDC smsg;
								strcpy(&smsg.command, "0");
							
								for(i = 0; i < bot_counter; i++){
									memset(&clientaddr, 0, sizeof(clientaddr));
									clientaddr.sin_family = AF_INET;
									clientaddr.sin_addr.s_addr = inet_addr(bots[i].hbuf);
									clientaddr.sin_port = htons(atoi(bots[i].sbuf));

									Sendto(mysock, &smsg, sizeof(smsg), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
									
									printf("SENT QUIT message clientaddr %s:%s.\n", hbuf, sbuf);
										
								}
								
								asprintf(&response, "%s\r\n", "HTTP/1.1 200 OK");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html><h3>Pokrenut quit.</h3></html>", response);
								Send(newfd, response, strlen(response), 0);
								
								
							} else {
								
								printf("Nepoznat REST zahtjev.\n");
								
								asprintf(&response, "%s\r\n", "HTTP/1.1 200 OK");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html>Nepoznat REST zahtjev.</html>", response);
								
								Send(newfd, response, strlen(response), 0);
								
							}
							
							
						} else {
							
							// Nije REST, citaj datoteku
							
							file = fopen(filename, "rb"); // read mode

							if(access(filename, F_OK) == -1) {
								
								perror("Datoteka ne postoji");
								asprintf(&response, "%s\r\n", "HTTP/1.1 404 Not Found");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html><h1>404 Not Found</html>", response);
								Send(newfd, response, strlen(response), 0);
								
							} else if (file == NULL){
								
								perror("Datoteka se ne moze procitati: ");
								asprintf(&response, "%s\r\n", "HTTP/1.1 403 Forbidden");
								asprintf(&response, "%s\r\n", response);
								asprintf(&response, "%s<html><h1>403 Forbidden</html>", response);
								Send(newfd, response, strlen(response), 0);
								
							}else{
								
								fseek(file, 0, SEEK_END);
								filesize = ftell(file);
								printf("Datoteka ima %d bajtova.\n", filesize);
								fclose(file);
								
								asprintf(&response, "%s\r\n", "HTTP/1.0 200 OK");
								asprintf(&response, "%s\r\n", response);
								Send(newfd, response, strlen(response), 0);
								
								char conbuf[filesize];
								memset(&bbuf,0, sizeof(bbuf));
								memset(&conbuf, 0,filesize);
								
								file2 = fopen(filename, "r"); // read mode

								if(file2 != NULL){
									i = 0;
									while((ch = fread(bbuf,1,1,file2)) > 0){
										memcpy(conbuf + i, bbuf, ch);
										i += ch;	
									}
									Send(newfd, conbuf, i, 0);
									printf("Poslano %d bajtova.\n", i);
								} else {
									printf("FATAL FILE READING ERR\n");
								}
							}
						}
					} else {
						
						response = NULL;
						asprintf(&response, "%s\r\n", "HTTP/1.1 405 Method Not Allowed");
						Send(newfd, response, strlen(response), 0);
					
					}					
					close(newfd);
					exit(0);
				}
				
				close(newfd);
				}
			FD_CLR(0, &readfds);
			FD_CLR(mysock, &readfds);
			FD_CLR(sockfd, &readfds);
		}	
	}
	return 0;
}

