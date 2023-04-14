#include <cmath>
#include <fcntl.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

static pthread_mutex_t bsem; // Mutex semaphore
static pthread_cond_t waitTurn =
    PTHREAD_COND_INITIALIZER; // Condition variable to control the turn
static int turn;              // Index to control access to the turn array
static int maxEvenThread;

void *printEvenOdd(void *void_ptr_argv) {

  int num = *(int *)void_ptr_argv;

  pthread_mutex_lock(&bsem);

  while (num != turn) {
    pthread_cond_wait(&waitTurn, &bsem);
  }

  pthread_mutex_unlock(&bsem);

  cout << "I am child thread " << num << endl;

  pthread_mutex_lock(&bsem);

  if (num != maxEvenThread) {
    turn += 2;
  } else {
    turn = 1;
  }
  pthread_cond_broadcast(&waitTurn);
  pthread_mutex_unlock(&bsem);

  return nullptr;
}

int main() {
  int nThreads;
  std::cin >> nThreads; // number of Threads from STDIN

  pthread_mutex_init(&bsem, NULL); // Initialize bsem to 1

  pthread_t *tid = new pthread_t[nThreads];
  int *threadNumber = new int[nThreads];
  turn = 0; // initialize the turn here

  // Determine the max thread number for even threads
  if ((nThreads - 1) % 2 == 0)
    maxEvenThread = nThreads - 1;
  else
    maxEvenThread = nThreads - 2;

  for (int i = 0; i < nThreads; i++) {

    threadNumber[i] =
        i; // initialize the thread number here (remember to follow the rules
           // from the specifications of the assignment)
    // call pthread_create here
    if (pthread_create(&tid[i], NULL, printEvenOdd, &threadNumber[i])) {
      cerr << "Error" << endl;
      exit(1);
    }
  }
  // Call pthread_join
  for (int i = 0; i < nThreads; i++) {
    pthread_join(tid[i], NULL);
  }

  delete[] threadNumber;
  delete[] tid;
  return 0;
}