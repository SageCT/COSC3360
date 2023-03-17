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

#include "huffmanTree.h"

void error(char *msg) {
  perror(msg);
  exit(1);
}

void *socketDecodeThread(void *args) {
  // Change the void pointer to a socketThreadData pointer
  socketThreadData *data = (socketThreadData *)args;
  // Get the socket file descriptor
  int sockfd, n;
  char buffer[256];
  struct sockaddr_in serv_addr;
  struct hostent *server;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");
  server = gethostbyname(data->hostname);
  if (server == nullptr) {
    error("ERROR, no such host");
    _exit(-1);
  }
  bzero((char *)&serv_addr, sizeof(serv_addr));
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  cout << "Connected to server, data to send: " << data->codeVal->data << endl;

  // Send the code to the server
  strcpy(buffer, data->codeVal->data.c_str());

  cout << "Sending buffer: " << buffer << endl;
  n = write(sockfd, buffer, 256);
  if (n < 0)
    error("ERROR writing to socket (in socketDecodeThread)");
  bzero(buffer, 256);

  // Read the decoded char from the server
  n = read(sockfd, buffer, 256);
  if (n < 0)
    error("ERROR reading from socket (in socketDecodeThread)");

  for (int c : data->codeVal->pos)
    data->decMessage[c] = buffer[0];

  close(sockfd);
  return nullptr;
}

int main(int argc, char *argv[]) {
  int sockfd, portno, n;
  char *hostname;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }

  portno = atoi(argv[2]);
  hostname = argv[1];
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
  int size = 0, maxIndex = 0;
  vector<string> inputs;
  vector<pthread_t> threads;
  vector<socketThreadData *> args;
  while (getline(std::cin, in))
    inputs.push_back(in), size++;

  // Get max index position for the final message
  for (auto i : inputs) {
    istringstream ss(i);
    int temp;
    ss >> temp;
    while (ss >> temp)
      if (temp > maxIndex)
        maxIndex = temp;
  }

  vector<char> finalMessage(maxIndex + 1, '*');

  strcpy(buffer, to_string(size).c_str());

  for (int i = 0; i < size; i++) {
    // Get the code from the inputs vector and send it to the server

    // copies the code from the input vector to the buffer
    // Ex. 11 1 3 5 would have '11' copied to the buffer
    string huffToSend = inputs[i].substr(0, inputs[i].find(" "));

    // Create the socketThreadData struct then push it to the args vector

    code *codeVal = new code(huffToSend, vector<int>{});
    vector<char> emptyMessageVec(maxIndex + 1, '*');

    int temp = 0;
    istringstream ss(inputs[i]);
    // Ignores the first value in the line
    ss >> temp;
    while (ss >> temp)
      codeVal->pos.push_back(temp);

    socketThreadData *data =
        new socketThreadData(codeVal, hostname, portno, emptyMessageVec);

    args.push_back(data);
    std::cout << args.at(i)->codeVal->data << std::endl;
  }

  for (auto &i : args) {
    // Create the thread and push it to the threads vector
    pthread_t thread;
    pthread_create(&thread, nullptr, socketDecodeThread, i);
    threads.push_back(thread);
  }

  for (auto &i : threads) {
    // Join the threads

    pthread_join(i, nullptr);
  }

  for (auto &i : args)
    for (int j = 0; j < i->decMessage.size(); j++)
      if (i->decMessage[j] != '*')
        finalMessage[j] = i->decMessage[j];

  string finalMessageString = "";
  for (auto i : finalMessage)
    finalMessageString += i;

  cout << "Decoded message: " << finalMessageString << std::endl;

  return 0;
}