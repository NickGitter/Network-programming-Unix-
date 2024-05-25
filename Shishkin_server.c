#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "EventHandler.h"

#define PORT 8080

int main() {
int server_fd, new_socket;
struct sockaddr_in address;
int addrlen = sizeof(address);
char buffer[1024] = {0};
//buffer[0] = random_num();

int random_number = 4; //= random_num(); // Генерация случайного числа
sprintf(buffer, "%d", random_number);

if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
perror("socket failed");
exit(EXIT_FAILURE);
}

address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);

bind(server_fd, (struct sockaddr *)&address, sizeof(address));
listen(server_fd, 3);
new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

read(new_socket, buffer, 1024);
//send(new_socket, buffer, 1, 0);

if(atoi(buffer) == random_number){
printf("yes this is the number %s\n", buffer );
}
else{
printf("no this is the number %s\n", buffer );
}
return 0;
}
