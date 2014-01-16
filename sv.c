#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>


#define PORT 20000 
#define BACKLOG 5
#define LENGTH 512 


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

unsigned long diff(struct timeval second, struct timeval first)
{
	struct timeval lapsed;
	unsigned long t;
	if (first.tv_usec > second.tv_usec) {
		second.tv_usec += 1000000;
		second.tv_sec--;
	}
	lapsed.tv_usec = second.tv_usec - first.tv_usec;
	lapsed.tv_sec = second.tv_sec - first.tv_sec;
	t = lapsed.tv_sec*1000000 + lapsed.tv_usec;
	printf("delay: %ld,%lds\n", lapsed.tv_sec, lapsed.tv_usec);
	return t;
}

int main ()
{
    /* Defining Variables */
    int sockfd; 
    int nsockfd; 
    int num;
    int sin_size; 
    struct sockaddr_in addr_local; /* client addr */
    struct sockaddr_in addr_remote; /* server addr */
	struct timeval start,end;
    char revbuf[LENGTH]; // Receiver buffer
	char sdbuf[LENGTH]; 
    
    /* Get the Socket file descriptor */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
        exit(1);
    }
    else 
        printf("[Server] Obtaining socket descriptor successfully.\n");

    /* Fill the client socket address struct */
    addr_local.sin_family = AF_INET; // Protocol Family
    addr_local.sin_port = htons(PORT); // Port number
    addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
    bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct

    /* Bind a special Port */
    if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
    {
        fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
        exit(1);
    }
    else 
        printf("[Server] Binded tcp port %d in addr 127.0.0.1 sucessfully.\n",PORT);

    /* Listen remote connect/calling */
    if(listen(sockfd,BACKLOG) == -1)
    {
        fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
        exit(1);
    }
    else
        printf ("[Server] Listening the port %d successfully.\n", PORT);

    int success = 0;
	int n;
    while(success == 0)
    {
        sin_size = sizeof(struct sockaddr_in);
        /* Wait a connection, and obtain a new socket file despriptor for single connection */
        if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
        {	
            fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
            exit(1);
        }
        else 
            printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));
    //read data from socket
	memset(revbuf, 0, LENGTH);
    n = read(nsockfd,revbuf,LENGTH);    //block den khi co du lieu tu client gui toi
    if (n < 0) error("ERROR reading from socket");
    gettimeofday(&start,NULL);
    printf("Message from client: %s\n",revbuf);
	//write data via socket
	memset(sdbuf, 0, LENGTH);
	//	
	int i=0;
	while(revbuf[i])
	{
		if(revbuf[i] >= 'a' && revbuf[i] <= 'z')
                {
                    revbuf[i]=toupper(revbuf[i]);
                }
                else if(revbuf[i] >= 'A' && revbuf[i] <= 'Z')
                {
                    revbuf[i]=tolower(revbuf[i]);
                }
		i++;
	}
	strcpy(sdbuf,revbuf);
	n=send(nsockfd, sdbuf, LENGTH, 0);	
 	if (n < 0) 
		error("khong doc duoc tu socket");
   	printf("pong: %s\n",sdbuf); 
	gettimeofday(&end,NULL);
	diff(end,start);
    }
}
