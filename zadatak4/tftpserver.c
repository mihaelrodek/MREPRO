#include "wrapper.h"
#include <signal.h>
#include <netdb.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/fcntl.h>
#include <stdbool.h>

#define MAXLEN64 64
#define MAXLEN512 512
#define MSGLEN 516
#define STDIN 0

struct MESSAGE{
	short code;
	short number;
	char buff[MAXLEN512];
};

struct REQUEST{
	short code;
	char buff[MAXLEN512];
} ;

struct ACK{
	short code;
	short number;
};

pid_t Fork(void)
{
    pid_t	    pid;

    if ((pid = fork()) == -1)
	err(1, "fork error");
    return (pid);
}

int daemon_init(const char *pname, int facility)
{
    int		    i;
    pid_t	    pid;

    if ((pid = Fork()) < 0)	return (-1);
    else if (pid) _exit(0);

    

    if (setsid() < 0)
	return (-1);

    signal(SIGHUP, SIG_IGN);
    if ((pid = Fork()) < 0)
	return (-1);
    else if (pid)
	_exit(0);

    chdir("/");			

    
    for (i = 0; i < 64; i++)
		close(i);

    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    openlog(pname, LOG_PID, facility);

    return (0);			/* success */
}

int main(int argc, char *argv[]) {
	
	int option, code, recieved, tmplen, check;
	int ackNum=-1, lastack = 0, transmissionCounter = 0, remembered = 0, imaklijenta = 0;
	bool deamong_flag = false;
	char c;
	short rcv;
	long stopped = 0;
	
	char filename[MAXLEN64], sendmethod[MAXLEN64], port[20] = "69";
	FILE *file;


	int mySock, clientsock;
	struct sockaddr_in addr;
	struct sockaddr_in clientaddr;
	
	struct addrinfo hints, *res;
	socklen_t addrlen;
	
	struct REQUEST req;
	struct MESSAGE mess;
	struct ACK ack;
	
	if (argc!=3 && argc!=2){
		err(3,"Usage ./tftpserver [-d] port_name_or_number\n");
		exit(1);
	}
	
	while((option = getopt(argc, argv, "d")) != -1) {
		switch (option) {
			case 'd':	
				deamong_flag = true;
				break;			
			default:
				err(3,"Usage ./tftpserver [-d] port_name_or_number\n");
				exit(1);
				return 1;
		}
	}
		
	strcpy(port, argv[argc-1]);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	
	Getaddrinfo(NULL, port, &hints, &res);
	
	mySock = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	Bind(mySock, res->ai_addr, res->ai_addrlen);
	
	if(deamong_flag)
	{
		char myLogin[30] = "mr51847:MrePro tftpserver";
		daemon_init(myLogin, LOG_FTP);
	}
	
	while(1)
	{
		addrlen=sizeof(addr);
		
		if(imaklijenta == 0){
			recieved = Recvfrom(mySock, &req, sizeof(req), 0 , (struct sockaddr *) &addr, &addrlen);
			code = req.code;
		}
		else if(imaklijenta == 1){
			recieved = recvfrom(clientsock, &ack, sizeof(ack), 0 , (struct sockaddr *) &addr, &addrlen);
			
			if (recieved == -1) 
			{
				if (errno == EAGAIN){ 
					
					if(transmissionCounter >= 3){
						errx(1, "Nemoguce pristupiti klijentu.");
					}
					transmissionCounter++;
					Sendto(clientsock, &mess, 2+2+tmplen, 0, (struct sockaddr *) &clientaddr, addrlen);
					
					continue;
				} 
				else{
					err(1,"Greska");
				}
			}
			
			code = ack.code;
			if(ack.number == ackNum){
				char adresa[50];
				inet_ntop(AF_INET, &((struct sockaddr_in*) &addr)-> sin_addr,  adresa, INET_ADDRSTRLEN );
				if(deamong_flag == true) syslog(LOG_INFO, "%s->%s\n", adresa , filename);
				else printf("%s->%s\n", adresa , filename);
				return 0;
			}
        }
        
        if(ntohs(code) == 1){
        	strcpy(filename, req.buff);
        	strcpy(sendmethod, &(req.buff[strlen(filename) + 1]));
			
			for(int k = 0; k < strlen(sendmethod); k++){
        		if(sendmethod[k] > 96) sendmethod[k] -= 32;
			}
			
			//printf("%s, %s\n", filename, sendmethod);
        	
        	int slash = 0;
        	for(int k = 0; k < strlen(filename); k++){
        		if(filename[k] == '/') slash++;
			}
			
			if(slash == 0){
				char default_name[20] = "/tftpboot/";
				strcat(default_name, filename);
				strcpy(filename, default_name);
			}else if(((strstr(filename, "/tftpboot/") == NULL) && slash > 0 ) ||
				((strstr(filename, "/tftpboot/") != NULL) && slash >= 3 )){
				mess.code = htons(5);
				mess.number = 2;
				strcpy(mess.buff, "Greska");
				Sendto(mySock, &mess, 2+2+strlen(mess.buff) + 1, 0, (struct sockaddr *) &addr, addrlen);
				
				if(deamong_flag == true) syslog(LOG_INFO, "TFTP ERROR 2 Zabranjen pristup datoteci");
				else fprintf(stderr, "TFTP ERROR 2 Zabranjen pristup datoteci");
				continue;	
			}
        	
        	if(strncmp(sendmethod, "NETASCII", 8) == 0){
				file = fopen(filename,"r");
				
				if(file == NULL){
					mess.code = htons(5);
					mess.number = 1;
					strcpy(mess.buff, "Greska");
					Sendto(mySock, &mess, 2+2+strlen(mess.buff) + 1, 0, (struct sockaddr *) &addr, addrlen);
					
					if(deamong_flag == true) syslog(LOG_INFO, "TFTP ERROR 1 Datoteka ne postoji");
					else warnx("TFTP ERROR 1 Datoteka ne postoji");
					continue;
				}
				
				memset(&mess, 0, sizeof(mess));
				mess.code = htons(3);
				mess.number = htons(1);
				tmplen = 0;
				
				check = 1;
				while (tmplen < MAXLEN512 && check == 1)
				{
					check = fread( &c, 1, 1, file);
					if(check < 1) break;
					if(c == '\n') mess.buff[tmplen++] = '\r';
			        if(tmplen == MAXLEN512  && c == '\n'){
						remembered = 1;
						break;
					}	
					mess.buff[tmplen++] = c;
				}
			    stopped = ftell(file);
			    fclose(file);
			}
			else if(strncmp(sendmethod, "OCTET", 5) == 0)
        	{
				file = fopen(filename,"rb");
				
				if(file == NULL)
				{
					mess.code = htons(5);
					mess.number = 1;
					strcpy(mess.buff, "Greska");
					Sendto(mySock, &mess, 2+2+strlen(mess.buff) + 1, 0, (struct sockaddr *) &addr, addrlen);
					
					if(deamong_flag == true) syslog(LOG_INFO, "TFTP ERROR 1 Datoteka ne postoji");
					else warnx("TFTP ERROR 1 Datoteka ne postoji");
					continue;
				}
				
				memset(&mess, 0, sizeof(mess));
				mess.code = htons(3);
				mess.number = htons(1);
				tmplen = 0;
				check = 1;
				while (tmplen < MAXLEN512 && check == 1)
				{
					check = fread( &(mess.buff[tmplen]), 1, 1, file);
					if(check == 1) tmplen++;
				}
				fclose(file);
			}
			else
			{
				mess.code = htons(5);
				mess.number = 4;
				strcpy(mess.buff, "Greska");
				Sendto(mySock, &mess, 2+2+strlen(mess.buff) + 1, 0, (struct sockaddr *) &addr, addrlen);
				
				if(deamong_flag == true) syslog(LOG_INFO, "TFTP ERROR 4 Nacin prijenosa pogresan");
				else warnx("TFTP ERROR 4 Nacin prijenosa pogresan");
				continue;
			}
			    
			if (fork() == 0) 
			{
			  	clientsock = Socket(AF_INET, SOCK_DGRAM, 0);
			  	memcpy(&clientaddr, &addr, sizeof(addr));
			  	imaklijenta = 1;
			  	
			  	struct timeval cekaj_max = {3,0};
				setsockopt(clientsock, SOL_SOCKET, SO_RCVTIMEO, &cekaj_max, sizeof(struct timeval));
			  	
			  	if(tmplen < MAXLEN512)
				{
					ackNum = mess.number;
				}
				
			   	Sendto(clientsock, &mess, 2+2+tmplen, 0, (struct sockaddr *) &addr, addrlen);
				
				
			}
			    	
		}
		
		else if(ntohs(code) == 4)
        {
        	if(strncmp(sendmethod, "NETASCII", 8) == 0)
        	{
				rcv = ntohs(ack.number);
				if(lastack == rcv) continue;
				file = fopen(filename,"r");
				fseek(file, stopped , SEEK_SET);
				
				memset(&mess, 0, sizeof(mess));
				mess.code = htons(3);
				mess.number = htons(rcv + 1);
				tmplen = 0;
				
				if(remembered)
				{
					mess.buff[tmplen++] = '\n';
					remembered = 0;
				}
				
				check = 1;
				while (tmplen < MAXLEN512 && check == 1)
				{
					check = fread( &c, 1, 1, file);
					if(check < 1) break;
					if(c == '\n') mess.buff[tmplen++] = '\r';
			        if(tmplen == MAXLEN512 && c == '\n') 
			        {
						remembered = 1;
						break;
					}	
					mess.buff[tmplen++] = c;
				}
			    stopped = ftell(file);
			    fclose(file);
			}
			else if(strncmp(sendmethod, "OCTET", 5) == 0)
        	{
				rcv = ntohs(ack.number);
				if(lastack == rcv) continue;
				file = fopen(filename,"rb");
				fseek(file, (long) rcv * 512L , SEEK_SET);
				
				memset(&mess, 0, sizeof(mess));
				mess.code = htons(3);
				mess.number = htons(rcv + 1);
				tmplen = 0;
				check = 1;
				while (tmplen < MAXLEN512 && check == 1)
				{
					check = fread( &(mess.buff[tmplen]), 1, 1, file);
					if(check == 1) tmplen++;
				}
				fclose(file);
			}
			
			lastack = rcv;
			
			if(tmplen < MAXLEN512)
			{
				ackNum = mess.number;
			}
			
			Sendto(clientsock, &mess, 2+2+tmplen, 0, (struct sockaddr *) &addr, addrlen);
			
			
        }
        else
        {
        	mess.code = htons(5);
			mess.number = 0;
			strcpy(mess.buff, "Greska");
			Sendto(mySock, &mess, 2+2+strlen(mess.buff) + 1, 0, (struct sockaddr *) &addr, addrlen);
			
			if(deamong_flag == true) syslog(LOG_INFO, "TFTP ERROR 0 Kod ne postoji");
			else warnx("TFTP ERROR 0 Kod ne postoji");
			continue;
		}
		
	}
	
}
