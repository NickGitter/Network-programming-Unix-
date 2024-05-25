

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 8888

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr, "Error, no such host
            ");
            exit(0);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting");
        exit(1);
    }

    int num;
    read(sockfd, &num, sizeof(num));
    printf("Server number is: %d", num);

        int my_num;
    printf("Enter your number (1-9): ");
    scanf("%d", &my_num);
    write(sockfd, &my_num, sizeof(my_num));

    char msg[100];
    read(sockfd, msg, sizeof(msg));
    printf("%s", msg);

        close(sockfd);
        return 0;
}