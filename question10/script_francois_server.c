#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()

#include <dirent.h>
#include <sys/stat.h>

#define MAX 80 
#define PORT 4000
#define SA struct sockaddr 


char passwordFile[] = "passwrd";

int logged_in = 0;
char username[MAX];
char password[MAX];

int grant_access(char *givenUsername, char *givenPassword){
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
    free(line);
    return granted;
}


// Function designed for chat between client and server. 
void func(int connfd) {
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
                write(connfd, "You are connected!\n", 19);
            } else {
                bzero(buff, MAX);
                write(connfd, "You are not connected\n", 22);
            }

        } else {
            // read the message from client and copy it in buffer 
            read(connfd, buff, sizeof(buff)); 
            char *line = strtok(buff, "\n");
            char *command = strtok(line, " ");
            char *path = strtok(NULL, " ");

            if (strcmp(command, "list") == 0) {

                FILE *fp2;
                int status;
                char newPath[500];

                char terminalCommand[100] = "ls ";
                strcat(terminalCommand, path);
                bzero(buff, MAX);

                fp2 = popen(terminalCommand, "r");
                if (fp2 == NULL)
                    printf("Error in opening the terminal\n");


                while (fgets(newPath, 500, fp2) != NULL){
                    strcat(buff, newPath);
                    buff[strlen(buff)-1] = '\0';
                    strcat(buff, " ");
                }

                status = pclose(fp2);

                write(connfd, buff, sizeof(buff));
                bzero(buff, MAX);

            } else if (strcmp(command, "read") == 0) {
                printf("le client veut lire un fichier\n");
            } else if (strcmp(command, "close") == 0) {
                break;  
            } else {
                printf("requÃªte inconnue\n");
                write(connfd, "Huh", 3);
            }
        }
    } 
} 
   
// Driver function 
int main() 
{ 
    printf("SERVER\n");
    int sockfd, connfd, len; 
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
   
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
   
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
   
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server accept the client...\n"); 
   
    // Function for chatting between client and server 
    func(connfd); 
   
    // After chatting close the socket 
    close(sockfd); 
}