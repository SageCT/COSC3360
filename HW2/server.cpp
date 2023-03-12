#include "huffmanTree.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno, clilen;
  char buffer[256];
  huffmanTree tree;
  // struct with the codes and their positions in a vector of ints
  // vector with the characters and their frequencies
  vector<shared_ptr<code>> codes;
  vector<shared_ptr<node>> freq;

  // sockaddr_in is a structure containing an internet address
  struct sockaddr_in serv_addr, cli_addr;
  int n;

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

  // Creating socket file descriptor
  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }

  // if the socket is not created/error print error and exit
  if (sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0) {
    error("ERROR opening socket");
    exit(EXIT_FAILURE);
  }

  // if socket is created/valid, then bind the socket to an address
  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  // Binds the server address to the socket
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  // Listens to the socket, then sets a new socket that is connected to the
  // server, the old one remains open to create additional connections

  listen(sockfd, 5);
  clilen = sizeof(cli_addr);
  newsockfd =
      accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

  if (newsockfd < 0)
    error("ERROR on accept");

  bzero(buffer, 256);
  n = read(newsockfd, buffer, 255);
  if (n < 0)
    error("ERROR reading from socket");
  printf("Here is the message: %s\n", buffer);
  n = write(newsockfd, "I got your message", 18);
  if (n < 0)
    error("ERROR writing to socket");
  return 0;
}
