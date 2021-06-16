#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/stat.h>

#define MAXLEN 100
#define BACKLOG 10

int main(int argc, char *argv[]) {
	
	struct addrinfo hints, *res;
    struct sockaddr cli;
    socklen_t clilen = sizeof(cli);
    struct sockaddr_in to;
    struct sockaddr their_addr;
    socklen_t sin_size;
    
    int sockfd, newfd, i, ch, FILESIZE;
    int error, numbytes, pid;
    char *tport = "80";
    const char delim[2] = " ";
    
    char tbuf[MAXLEN];
    char *response;
    char bbuf[1];
    
    FILE *fp, *fp2;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags |= AI_PASSIVE;

	if ((error = getaddrinfo(NULL, tport, &hints, &res)))
		errx(1, "%s", gai_strerror(error));
	
	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		err(1,"socket");
	}
	
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int));

	if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		err(1,"bind");
	}

	if (listen(sockfd, BACKLOG) == -1) {
		err(1,"listen");
	}
	
	while(1) {
		
		sin_size = sizeof (struct sockaddr);
		if ((newfd = accept(sockfd, &their_addr, &sin_size)) == -1) {
			err(1,"accept");
		}
		
		if((pid = fork()) == 0){
					
				close(sockfd);
				
				if((numbytes = read(newfd, tbuf, sizeof(tbuf))) == -1){
						perror("read");
				}
				tbuf[numbytes] = '\0';
					
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
				
				char *filename;
				asprintf(&filename, ".%s", path);
				
				fp = fopen(filename, "rb"); // read mode

				if(access(filename, F_OK) == -1) {
					
					perror("Datoteka ne postoji");
					asprintf(&response, "%s\r\n", "HTTP/1.1 404 Not Found");
					asprintf(&response, "%s\r\n", response);
					asprintf(&response, "%s<html><h1>404 Not Found</html>", response);
					if(send(newfd, response, strlen(response), 0) == -1){
							perror("send1");
					}
					
				} else if (fp == NULL){
					
					perror("Datoteka se ne moze procitati: ");
					asprintf(&response, "%s\r\n", "HTTP/1.1 403 Forbidden");
					asprintf(&response, "%s\r\n", response);
					asprintf(&response, "%s<html><h1>403 Forbidden</html>", response);
					if(send(newfd, response, strlen(response), 0) == -1){
							perror("send1");
					}
					
				} else {
					
					fseek(fp, 0, SEEK_END);
					FILESIZE = ftell(fp);
					printf("Datoteka ima %d bajtova.\n", FILESIZE);
					fclose(fp);
					
					asprintf(&response, "%s\r\n", "HTTP/1.0 200 OK");
					asprintf(&response, "%s\r\n", response);
					if(send(newfd, response, strlen(response), 0) == -1){
							perror("send1");
					}
					
					char conbuf[FILESIZE];
					memset(&bbuf, sizeof(bbuf), 0);
					memset(&conbuf, FILESIZE, 0);
					
					fp2 = fopen(filename, "r"); // read mode

					if(fp2 != NULL){
						i = 0;
						while((ch = fread(bbuf,1,1,fp2)) > 0){
							memcpy(conbuf + i, bbuf, ch);
							i += ch;	
						}
						if(send(newfd, conbuf, i, 0) == -1){
							perror("send1");
						}
						printf("Poslano %d bajtova.\n", i);
					} else {
						printf("FATAL FILE READING ERR\n");
					}
					
				}
				
				close(newfd);
				exit(0);
					
		}
		
		close(newfd);
		
	}
    


	return 0;
}

