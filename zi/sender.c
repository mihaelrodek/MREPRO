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
#define N 20

struct IP_PORT{
    char PORT[22];
};

int main (int argc, char *argv[]){
	
	
	const int on = 1;
	
	int option, seconds=0, delay=0;
	bool tcp_flag = false, udp_flag = false, r_flag = false, delay_flag=false;
	char *string_send, *response;
	int pos=1, portnum=0;
	int index, k=0;
	int sockfd, mysock;
	char *IP_address;
	
	struct addrinfo hints, *res;
	
	struct IP_PORT pairs[N];
	
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

	index = optind;
    string_send = argv[index++];
    IP_address = argv[index++];

    memset(&pairs, 0, sizeof(pairs));

    for(int j = index; j < argc; j++){
        strcpy(pairs[k++].PORT, argv[j]);
    }

    portnum=k;


        if (tcp_flag == true && udp_flag == false) {

            //TCP

            for (k = 0; k < portnum; k++) {

                memset(&hints, 0, sizeof(hints));
                memset(&res, 0, sizeof(res));

                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_flags = AI_PASSIVE;

                printf("%s, %s\n",IP_address, pairs[k].PORT);
                Getaddrinfo(IP_address, pairs[k].PORT, &hints, &res);

                sockfd = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);

                if (seconds != 0)
                    Setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &cekaj_sec, sizeof(struct timeval));

                connect(sockfd, res->ai_addr, res->ai_addrlen);

                if(write(sockfd, string_send, strlen(string_send)) < 0)
                    errx(1, "%s", "Error writing\n");

                memset(&response, 0, sizeof(response));

                if(seconds != 0){
                    int r = read(sockfd, response, 128);
                    //printf("tu\n");
                    if(r == -1){
                        if (errno == EAGAIN) {
                            printf("Vremenska kontrola istekla.\n");
                        } else {
                            errx(1, "%s", "Error reading\n");
                        }
                    }else{
                        printf("TCP %s open\n",pairs[k].PORT);
                    }
                }
                close(sockfd);

            }

        } else if (tcp_flag == false && udp_flag == true) {

            //UDP

            for (k = 0; k < portnum; k++) {
                memset(&hints, 0, sizeof hints);
                memset(&res, 0, sizeof(res));

                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_DGRAM;
                hints.ai_flags = AI_PASSIVE;

                Getaddrinfo(IP_address, pairs[k].PORT, &hints, &res);
                mysock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);

                if (seconds != 0)
                    Setsockopt(mysock, SOL_SOCKET, SO_RCVTIMEO, &cekaj_sec, sizeof(struct timeval));

                Sendto(mysock, string_send, strlen(string_send), 0, res->ai_addr, res->ai_addrlen);

                memset(&response, 0, sizeof(response));

                if (seconds != 0){
                    int recieved = recvfrom(mysock, response, 128, 0, res->ai_addr, &res->ai_addrlen);
                    if(recieved == -1){
                        if (errno == EAGAIN) {
                            printf("Vremenska kontrola istekla.\n");
                        } else {
                            errx(1, "%s", "Error reading\n");
                        }
                    }else{
                        printf("UDP %s open\n",pairs[k].PORT);
                    }
                }

                close(mysock);
            }

        } else{
            //TCP I UDP

            for (k = 0; k < portnum; k++) {

                //UDP
                memset(&hints, 0, sizeof hints);
                memset(&res, 0, sizeof(res));

                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_DGRAM;
                hints.ai_flags = AI_PASSIVE;

                Getaddrinfo(IP_address, pairs[k].PORT, &hints, &res);
                mysock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);

                if (seconds != 0)
                    Setsockopt(mysock, SOL_SOCKET, SO_RCVTIMEO, &cekaj_sec, sizeof(struct timeval));

                Sendto(mysock, string_send, strlen(string_send), 0, res->ai_addr, res->ai_addrlen);

                memset(&response, 0, sizeof(response));

                if (seconds != 0){
                    int recieved = recvfrom(mysock, response, 128, 0, res->ai_addr, &res->ai_addrlen);
                    if(recieved == -1){
                        if (errno == EAGAIN) {
                            printf("Vremenska kontrola istekla.\n");
                        } else {
                            errx(1, "%s", "Error reading\n");
                        }
                    }else{
                        printf("UDP %s open\n",pairs[k].PORT);
                    }
                }

                //TCP
                memset(&hints, 0, sizeof(hints));
                memset(&res, 0, sizeof(res));

                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_flags |= AI_PASSIVE;

                Getaddrinfo(IP_address, pairs[k].PORT, &hints, &res);

                sockfd = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);

                if (seconds != 0)
                    Setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &cekaj_sec, sizeof(struct timeval));

                Connect(sockfd, res->ai_addr, res->ai_addrlen);

                if(write(sockfd, string_send, strlen(string_send)) < 0)
                    errx(1, "%s", "Error writing\n");

                memset(&response, 0, sizeof(response));

                if(seconds != 0){
                    int recieved = read(sockfd, response, 128);
                    if(recieved == -1){
                        if (errno == EAGAIN) {
                            printf("Vremenska kontrola istekla.\n");
                        } else {
                            errx(1, "%s", "Error reading\n");
                        }
                    }else{
                        printf("TCP %s open\n",pairs[k].PORT);
                    }
                }

                close(mysock);
                close(sockfd);
                sleep(delay);
            }
    }
}












