#include <arpa/inet.h>
#include <time.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 2
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, new_socket1, new_socket2;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    int clients_connected = 0;
    int secret_number, client1_number, client2_number;
    int winner = -1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < -1)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {

        if (clients_connected < MAX_CLIENTS)
        {
            printf("Waiting for a clients..\n");

            if (clients_connected == 0)
            {
                if ((new_socket1 = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                printf("Client 1 connected with IP: %s and Port: %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                send(new_socket1, "Waiting for another player...", strlen("Waiting for another player..."), 0);
                clients_connected++;
            }
            else
            {
                if ((new_socket2 = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                printf("Client 2 connected with IP: %s and Port: %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                send(new_socket1, "Game is starting!", strlen("Game is starting!"), 0);
                send(new_socket2, "Game is starting!", strlen("Game is starting!"), 0);
                clients_connected++;
            }
        }
        if (clients_connected == MAX_CLIENTS)
        {
            srand(time(NULL));
            secret_number = rand() % 9 + 1;
            printf("Secret number: %d\n", secret_number);

            send(new_socket1, "Enter your number (1-9): ", strlen("Enter your number (1-9): "), 0);
            recv(new_socket1, buffer, BUFFER_SIZE, 0);
            client1_number = atoi(buffer);

            send(new_socket2, "Enter your number (1-9): ", strlen("Enter your number (1-9): "), 0);
            recv(new_socket2, buffer, BUFFER_SIZE, 0);
            client2_number = atoi(buffer);

            if (abs(secret_number - client1_number) < abs(secret_number - client2_number))
            {
                winner = 1;
            }
            else if (abs(secret_number - client1_number) > abs(secret_number - client2_number))
            {
                winner = 2;
            }
            else
            {
                winner = 0;
            }

            if (winner == 1)
            {
                send(new_socket1, "Congratulations! You won!", strlen("Congratulations! You won!"), 0);
                send(new_socket2, "You lost. Better luck next time!", strlen("You lost. Better luck next time!"), 0);
            }
            else if (winner == 2)
            {
                send(new_socket1, "You lost. Better luck next time!", strlen("You lost. Better luck next time!"), 0);
                send(new_socket2, "Congratulations! You won!", strlen("Congratulations! You won!"), 0);
            }
            else
            {
                send(new_socket1, "It's a draw!", strlen("It's a draw!"), 0);
                send(new_socket2, "It's a draw!", strlen("It's a draw!"), 0);
            }

            clients_connected = 0;
            close(new_socket1);
            close(new_socket2);
            printf("Clients disconnected. Waiting for new players...\n");
        }
    }

    return 0;
}