#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10

int main() {
	int server_fd;
	int new_client;
	int client_sockets[MAX_CLIENTS];
	struct sockaddr_in addrees;
	socklen_t addrlen = sizeof(addrees);
	char buffer[1024];
	fd_set readfds;

	for(int i = 0; i < MAX_CLIENTS; i++) {
		client_sockets[i] = 0;
	}
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(server_fd == 0) {
		perror("Socket Failed");
		exit(EXIT_FAILURE);
	}
	addrees.sin_family = AF_INET;
	addrees.sin_addr.s_addr = INADDR_ANY;
	addrees.sin_port = htons(PORT);

	bind(server_fd, (struct sockaddr *)&addrees, sizeof(addrees));
	listen(server_fd, 5);

	printf("Server started on port %d\n", PORT);

	while(1) {
		FD_ZERO(&readfds);
		FD_SET(server_fd, &readfds);
		int max_fd = server_fd;
		for(int i = 0; i < MAX_CLIENTS; i++) {
			int fd = client_sockets[i];
			if(fd > 0) {
				FD_SET(fd, &readfds);
			} else {
				break;
			}
			if(fd > max_fd) {
				max_fd = fd;
			}
		} 
		select(max_fd + 1, &readfds,  NULL, NULL, NULL);
		if(FD_ISSET(server_fd, &readfds)) {
			new_client = accept(server_fd, (struct sockaddr *)&addrees, &addrlen);
			printf("New connection id: %d, IP: %s\n", new_client, inet_ntoa(addrees.sin_addr));
			for(int i = 0; i < MAX_CLIENTS; i++) {
				if(client_sockets[i] ==  0) {
					client_sockets[i] = new_client;
					break;
				}
			}
		}
	}
	for(int i = 0; i < MAX_CLIENTS; i++) {
		int fd = client_sockets[i];
		if(FD_ISSET(fd, &readfds)) {
			int val = read(fd, buffer, 1024);
			if(val ==  0) {
				getpeername(fd, (struct sockaddr *)&addrees, &addrlen);
				printf("Client disconnected: IP: %s", inet_ntoa(addrees.sin_addr));
				close(fd);
				client_sockets[i] = 0;
			} else {
				buffer[val] = '\0';
				for(int j = 0; j < MAX_CLIENTS; j++) {
					if(client_sockets[j] != 0 && client_sockets[j] != fd) {
						send(client_sockets[j], buffer, strlen(buffer), 0);
					}
				}
			}
		}
	}

	return 0;
}
