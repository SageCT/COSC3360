#include "huffmanTree.h"
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void error(char *msg) {
  perror(msg);
  exit(1);
}

void *socketDecodeThread(void *args) {
  // Change the void pointer to a socketThreadData pointer
  socketThreadData *data = (socketThreadData *)args;
  // Get the socket file descriptor
  int sockfd = serv_addr->sockfd;
}

int main(int argc, char *argv[]) {
  int sockfd, portno, n;

  struct sockaddr_in serv_addr;
  struct hostent *server;

  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
    error("ERROR opening socket");
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  // Creating the buffer, input string, and vectors for
  // the threads (pthread_t and threadData) and inputs
  char buffer[256];
  std::string in = "";
  int size = 0;
  vector<string> inputs;
  vector<pthread_t> threads;
  vector<threadData> args;
  int size = 0;
  while (getline(std::cin, in))
    inputs.push_back(in), size++;

  for (int i = 0; i < size; i++) {
  }

  strcpy(buffer, to_string(size).c_str());

  for (int i = 0; i < size; i++) {
    // Get the code from the inputs vector and send it to the server
    strcpy(buffer,
           inputs[i].substr(0, find(inputs[i].begin(), inputs[i].end(), ' ')));
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
      error("ERROR writing to socket");
    bzero(buffer, 256);
  }

  n = write(sockfd, &size, sizeof(int));
  if (n < 0)
    error("ERROR writing to socket");

  strcpy(buffer, buffer + '\0');
  int value = 11;
  n = write(sockfd, &value, sizeof(int));
  if (n < 0)
    error("ERROR reading from socket");

  return 0;
}