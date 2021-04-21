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

#include "wrapper.h"

#define MAXLEN 512
#define DEFAULT_PORT 5555

int main(int argc, char *argv[]){
	
	int option;
	
	if (argc!=1 && argc!=3 && argc!=5){
		err(3,"Usage: ./UDP_server [-l port] [-p payload]");
	}
	
	while ((option = getopt(argc, argv, "l:p:")) != -1){
		switch (option){
			case 'l':
				port_addr = optarg;
				break;
			case 'p':
				payload = optarg;
				break;
			default:
				port_addr="1234";
				payload="";
				break;
		}
	}
	
	//printf("tu sam");
	
	//memset(&hints, 0, sizeof(hints));


	//printf("a sad tu");
	
	
	
}
