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
#include <stdbool.h>

#include "wrapper.h"


int main (int argc, char *argv[]){
	
	
	int option, timeout;
	bool tcp_flag = false, udp_flag = false;
	
	while ((option = getopt(argc, argv, "i:tu")) != -1 ) {
		switch (option) {
			case 'i':				
				timeout = atoi(optarg);
				break;
			case 't':
				tcp_flag = true;
				break;	
			case 'u':
				udp_flag = true;
				break;			
			default :
				printf("USAGE: ./receiver [-i timeout] [-t |-u] \"string\" port ...\n");
				return 2;
		}
	}
	
	
	
}
