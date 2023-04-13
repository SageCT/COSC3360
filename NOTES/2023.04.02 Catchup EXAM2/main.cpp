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

using namespace std;

struct ThreadArgs {
  int id;
  pthread_cond_t *waitTurn;
  pthread_mutex_t *mutex;
};

void *addOneHundred(void *id) {
  int *id_ = (int *)id;
  for (int i = 0; i < 100; i++)
    cout << *id_ << " " << i << endl;
}

void addOneHundred(int id, pthread_mutex_t &mutex) {
  for (int i = 0; i < 100; i++) {
    cout << id << " " << i << endl;
  }
}

int main() {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  int one = 1, two = 2;
  pthread_mutex_t mutex;

  pthread_t t1, t2;
  ThreadArgs *args1 = new ThreadArgs;
  args1->id = 1;
  args1->mutex = &mutex;

  pthread_create(&t1, NULL, addOneHundred, (void *)&args1);

  return 0;
}