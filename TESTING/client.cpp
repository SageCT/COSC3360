// #include "huffmanTree.h"
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[1024] = {0};
  char *hello = "Hello from client";

  // Create a socket file descriptor
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creation error");
    return -1;
  }

  // Get server address from hostname
  if ((server = gethostbyname(argv[1])) == NULL) {
    perror("no such host");
    return -1;
  }

  // Set the server address
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);

  // Connect to the server
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("connection failed");
    return -1;
  }

  // Send a message to the server while not "-1"
  bool socketOn = true;
  while (socketOn) {

    // Get user input
    char buffer[256] = {0};
    string input;
    getline(cin, input);
    strcpy(buffer, input.c_str());

    int n = write(sock, buffer, sizeof(buffer));
    std::cout << input + " message sent\n" << std::endl;
    if (input == "-1") {
      socketOn = false;
      cout << "Socket Closed, Program terminated" << endl;
    }
    bzero(buffer, 256);

    // Read the server's response
    valread = read(sock, buffer, sizeof(buffer));
    printf("%s\n", buffer);
  }
  close(sock);
  return 0;
}