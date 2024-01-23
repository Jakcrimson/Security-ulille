#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 4000
#define SA struct sockaddr

int logged_in = 0;

void func(int sockfd)
{

    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        if (logged_in == 0) {
            printf("Enter your username: ");
            n = 0;
            while ((buff[n++] = getchar()) != '\n')
                ;
            write(sockfd, buff, sizeof(buff));
            bzero(buff, sizeof(buff)); 
            printf("Enter your password: ");
            n = 0;
            while ((buff[n++] = getchar()) != '\n')
                ;
            write(sockfd, buff, sizeof(buff));
            bzero(buff, sizeof(buff));     
        } else {
            printf("Enter the string : ");
            n = 0;
            while ((buff[n++] = getchar()) != '\n')
                ;
            write(sockfd, buff, sizeof(buff));

            if ((strncmp(buff, "close\n", 4)) == 0) {
                printf("Client Exit...\n");
                break;
            }
            bzero(buff, sizeof(buff));
        }
        read(sockfd, buff, sizeof(buff));
        if (strcmp(buff, "You are connected!\n") == 0) {
            logged_in = 1;
        }
        printf("From Server : %s\n", buff);

    }
}
 
int main()
{
    printf("CLIENT\n");
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
 
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
 
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
 
    // function for chat
    func(sockfd);
 
    // close the socket
    close(sockfd);
}
