#include <cmath>
#include <fcntl.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
static pthread_mutex_t bsem; // Mutex semaphore
static pthread_cond_t waitTurn =
    PTHREAD_COND_INITIALIZER; // Condition variable to control the turn
static int turn;              // Index to control access to the turn array
static int maxEvenThread;
void *printEvenOdd(void *void_ptr_argv) {
  // std::cout << "I am child thread " << /*variable name*/ << std::endl;
  return nullptr;
}
int main() {
  int nThreads;
  std::cin >> nThreads;            // number of Threads from STDIN
  pthread_mutex_init(&bsem, NULL); // Initialize bsem to 1
  pthread_t *tid = new pthread_t[nThreads];
  int *threadNumber = new int[nThreads];
  turn = 0; // initialize the turn here
            // Determine the max thread number for even threads
  (nThreads - 1) % 2 == 0 ? maxEvenThread = nThreads - 1
                          : maxEvenThread = nThreads - 2;
  for (int i = 0; i < nThreads; i++) {
    threadNumber[i] =
        i; // initialize the thread number here (remember to follow the rules
           // from the specifications of the assignment)
    // call pthread_create here
  }
  // Call pthread_join
  delete[] threadNumber;
  delete[] tid;
  return 0;
}