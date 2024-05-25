#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    int player_number;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    while(1) {
        recv(sock, buffer, BUFFER_SIZE, 0);

        if (strstr(buffer, "Waiting for another player...") != NULL) {
            player_number = 1;
            printf("You are player %d. Waiting for another player...\n", player_number);
        } else if (strstr(buffer, "Game is starting!") != NULL) {
            player_number = 2;
            printf("You are player %d. Game is starting!\n", player_number);
        } else if (strstr(buffer, "Enter your number (1-9): ") != NULL) {
            int number;
            printf("%s", buffer);
            scanf("%d", &number);
            send(sock, buffer, strlen(buffer), 0);
            sprintf(buffer, "%d", number);
            send(sock, buffer, strlen(buffer), 0);
        } else {
            printf("%s\n", buffer);

            if (strstr(buffer, "Congratulations! You won!") != NULL || strstr(buffer, "You lost. Better luck next time!") != NULL || strstr(buffer, "It's a draw!") != NULL) {
                break;
            }
        }

        memset(buffer, 0, BUFFER_SIZE);
    }

    close(sock);
    return 0;
}