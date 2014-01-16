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
#define LENGTH 512 


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    /* Variable Definition */
    int sockfd; 
    int nsockfd;
    char revbuf[LENGTH]; 
    struct sockaddr_in remote_addr;
    /* Get the Socket file descriptor */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
        exit(1);
    }
    /* Fill the socket address struct */
    remote_addr.sin_family = AF_INET; 
    remote_addr.sin_port = htons(PORT); 
    inet_pton(AF_INET, "127.0.0.1", &remote_addr.sin_addr); 
    bzero(&(remote_addr.sin_zero), 8);
    /* Try to connect the remote */
    if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
        exit(1);
    }
    else 
        printf("[Client] Connected to server at port %d...ok!\n", PORT);
   	char sdbuf[LENGTH]; 
	//Write
	memset(sdbuf, 0, LENGTH); //Khoi tao buffer
	printf("Input: ");
    fgets(sdbuf,LENGTH,stdin); //Chua thong diep doc tu ban phim 
	int n;
    n = write(sockfd,sdbuf,strlen(sdbuf));
    if (n < 0) 
         error("ERROR writing to socket");
	//Read
	memset(revbuf, 0, LENGTH);
	printf("Output: ");
    n = read(sockfd,revbuf,LENGTH);
  	//n = recv(sockfd, revbuf, LENGTH, 0);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",revbuf);
	close(sockfd);
}
