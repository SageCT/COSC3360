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

#include "huffmanTree.h"

void error(char *msg) {
  perror(msg);
  exit(1);
}

void fireman(int) {
  while (waitpid(-1, NULL, WNOHANG) > 0) {
    // cout << "A child process ended" << endl;
    break;
  }
}

int main(int argc, char *argv[]) {
  int server_fd, portno;
  // struct with the codes and their positions in a vector of ints
  // vector with the characters and their frequencies
  vector<shared_ptr<code>> codes;
  vector<shared_ptr<node>> freq;
  // sockaddr_in is a structure containing an internet address
  struct sockaddr_in address;
  int n, opt = 1;

  // Set up the signal handler
  signal(SIGCHLD, fireman);

  // Checking for valid number of arguments
  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }

  // Create socket file descriptor and check for errors
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

  // Binds the socket to the address
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Take inputs from STDIN
  string in;
  int nodeNum = 0;
  while (getline(cin, in)) {
    string temp = "";
    temp += in.at(2);
    // Make node and push to vector
    shared_ptr<node> newNode(
        make_shared<node>(node(in.substr(0, 1), stoi(temp), nodeNum++)));
    freq.push_back(newNode);
  }

  // Create the tree
  huffmanTree tree(freq);
  tree.print(true);

  vector<string> messages;

  // Listens to the socket for incoming connections
  if (listen(server_fd, nodeNum) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  int i = 0;
  while (i < nodeNum) {
    // Accepts the incoming client connection
    char buffer[256] = {0};
    sockaddr_in client_address = {0};
    socklen_t client_address_size = sizeof(client_address);
    int client_socket =
        accept(server_fd, (sockaddr *)&client_address, &client_address_size);
    if (client_socket < 0)
      error("ERROR on accept");

    if (fork() == 0) {

      n = recv(client_socket, buffer, 256, 0);
      if (n < 0)
        error("ERROR reading from socket");

      string temp(buffer);
      cout << "Buffer: " << temp << endl;

      shared_ptr<node> tempRoot = tree.getRoot();

      // Finds the desired node in the tree
      for (auto c : temp)
        tempRoot = c == '0' ? tempRoot->left : tempRoot->right;

      std::cout << "Character to send back to client: " << tempRoot->data
                << std::endl;

      // Send the character back to the client
      bzero(buffer, 256);
      buffer[0] = tempRoot->data[0];
      n = send(client_socket, buffer, 1, 0);
      if (n < 0)
        error("ERROR writing to socket");

      // Close socket and exit child process
      close(client_socket);
      _exit(0);
    }
    wait(nullptr);
    i++;
  }

  close(server_fd);
  return 0;
}
