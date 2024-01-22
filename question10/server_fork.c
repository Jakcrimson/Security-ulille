#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#define PORT 5000
#define MAX 100

char passwordFile[] = "passwrd";

int logged_in = 0;
char username[MAX];
char password[MAX];
int client_cnt = 0;


int grant_access(char *givenUsername, char *givenPassword){
	printf("grant access\n");
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t readFile;


    int granted = 0;

    fp = fopen(passwordFile, "r");
    while ((readFile = getline(&line, &len, fp)) != -1) {
        if(line[strlen(line)-1] == '\n') {
            line = strtok(line, "\n");
        }
        char *user = strtok(line, ":");
        char *tmpPassword = strtok(NULL, ":");
        if (strcmp(&user[0], givenUsername) == 0) {
            if (strcmp(&tmpPassword[0], givenPassword) == 0) {
                granted = 1;
                break;
            }
        }
    }
    fclose(fp);

    return granted;
}


// Function designed for chat between client and server. 
void func(int connfd) {
	printf("func server\n");
    char buff[MAX]; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, MAX); 

        if (logged_in == 0) {
            read(connfd, buff, sizeof(buff)); 
            strcpy(username, strtok(buff, "\n"));
            bzero(buff, MAX); 

            read(connfd, buff, sizeof(buff)); 
            strcpy(password, strtok(buff, "\n"));
            bzero(buff, MAX); 

            if (grant_access(username, password) == 1) {
                bzero(buff, MAX);
                logged_in = 1;
                write(connfd, "[+]You are connected!\n", 19);
            } else {
                bzero(buff, MAX);
                write(connfd, "[-]You are not connected\n", 22);
            }

        } else {

            // read the message from client and copy it in buffer 
            read(connfd, buff, sizeof(buff)); 
            char *command = strtok(buff, " ");
            char *path = strtok(NULL, " ");

            printf("[+]Commande: %s, path: %s\n", command, path);

            if (strcmp(command, "list") == 0) {
                printf("[+]Client %d: %s\n", client_cnt, buff);
                printf("[*]Le client %d veut lister des trucs\n", client_cnt);
                send(connfd, buff, strlen(buff), 0);
                bzero(buff, sizeof(buff));
            } else if (strcmp(command, "read") == 0) {
				printf("[+]Client %d: %s\n", client_cnt, buff);
				printf("[*]Le client %d veut lire un fichier\n", client_cnt);
				send(connfd, buff, strlen(buff), 0);
				bzero(buff, sizeof(buff));
            } else if (strcmp(command, "close\n") == 0) {
                break;  
            } else {
				printf("[+]Client %d: %s\n", client_cnt, buff);
				printf("[-]Requête inconnue\n");
				send(connfd, buff, strlen(buff), 0);
				bzero(buff, sizeof(buff));
            }
            bzero(buff, MAX);

        }
    } 
} 



int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	int client_cnt = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", PORT);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			printf("new_socket <0 %d\n", newSocket);
			exit(1);
		}
		printf("[+]Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		client_cnt++;
		if((childpid = fork()) == 0){
			printf("childpid == 0\n");
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("[-]Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					func(newSocket);
				}
			}
		}

	}

	close(newSocket);


	return 0;
}