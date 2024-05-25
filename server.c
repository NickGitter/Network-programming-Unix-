#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888
#define MAX_CLIENTS 2

int main()
{
    int sockfd, newsockfd, client_sockets[MAX_CLIENTS];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error on binding");
        exit(1);
    }

    listen(sockfd, MAX_CLIENTS);

    clilen = sizeof(cli_addr);

    int num_clients = 0;
    int server_num;
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");
            exit(1);
        }

        client_sockets[num_clients] = newsockfd;
        num_clients++;

        if (num_clients == MAX_CLIENTS) {
            server_num = rand() % 9 + 1;

            for (int i = 0; i < MAX_CLIENTS; i++) {
                write(client_sockets[i], &server_num, sizeof(server_num);
            }

            int num1, num2;
            read(client_sockets[0], &num1, sizeof(num1));
            read(client_sockets[1], &num2, sizeof(num2));

            int diff1 = abs(server_num - num1);
            int diff2 = abs(server_num - num2);

            if (diff1 < diff2) {
                write(client_sockets[0], "You win!", strlen("You win!"));
                write(client_sockets[1], "You lose!", strlen("You lose!"));
            }
            else {
                write(client_sockets[1], "You win!", strlen("You win!"));
                write(client_sockets[0], "You lose!", strlen("You lose!"));
            }

            num_clients = 0;
        }
    }

    close(sockfd);

    return 0;
}