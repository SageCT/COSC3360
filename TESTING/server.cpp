#include <algorithm>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

int main(int argc, char *argv[]) {
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  socklen_t addrlen = sizeof(address);
  char buffer[1024] = {0};

  // Create a socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set socket options
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  // Set the server address
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(atoi(argv[1]));

  // Bind the socket to the address
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Listen for incoming connections
  if (listen(server_fd, SOMAXCONN) < 0) {
    perror("Failed to listen on server socket");
    exit(EXIT_FAILURE);
  }

  sockaddr_in client_address = {0};
  socklen_t client_address_size = sizeof(client_address);
  int client_socket = accept(server_fd, (struct sockaddr *)&client_address,
                             &client_address_size);
  if (client_socket < 0) {
    perror("Failed to accept incoming client socket");
    exit(EXIT_FAILURE);
  }

  while (true) {
    // Listen for incoming messages from client
    char buffer[256] = {0};

    // Read the client's message
    valread = recv(client_socket, buffer, 256, 0);
    if (buffer[0] == '-' && buffer[1] == '1')
      break;
    std::cout << "Message recieved: " << buffer << std::endl;
    bzero(buffer, 256);

    // Send a response to the client
    std::string temp = "Message Recieved by server!";
    strcpy(buffer, temp.c_str());
    send(client_socket, buffer, sizeof(buffer), 0);
  }

  close(client_socket);
  return 0;
}