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
	int new_socket;
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
	}


	return 0;
}
