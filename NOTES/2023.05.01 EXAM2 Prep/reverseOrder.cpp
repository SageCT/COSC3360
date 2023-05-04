//reverse order
#include <pthread.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
using namespace std;

static pthread_mutex_t bsem;
static pthread_cond_t waitTurn = PTHREAD_COND_INITIALIZER;
static int turn;
static int nthreads;

/*
I am Thread 4
I am Thread 3
I am Thread 2
I am Thread 1
I am Thread 0
*/

void *print_in_reverse_order(void *void_ptr_argv)
{
    int myTurn = *(int *) void_ptr_argv;
    //as long as its not my turn.. wait!
    pthread_mutex_lock(&bsem);

    while (turn > myTurn)
    {
        pthread_cond_wait(&waitTurn, &bsem);
    }

    pthread_mutex_unlock(&bsem);

    //if its my turn, print and decrement turn
    pthread_mutex_lock(&bsem);

    cout << "I am Thread " << myTurn << endl;
    turn = turn - 1;

    pthread_cond_broadcast(&waitTurn);
    pthread_mutex_unlock(&bsem);

    return NULL;
}

int main()
{
    int nthreads;
    std::cin >> nthreads;
    pthread_mutex_init(&bsem, NULL); //initialize access to 1
    pthread_t *tid = new pthread_t[nthreads];
    int *threadNumber = new int[nthreads];

    //nthreads is 5, set turn to 4 since we want to print Thread 4 first
    turn = nthreads - 1;

    for(int i = 0; i < nthreads; i++){
        threadNumber[i] = i;
        if(pthread_create(&tid[i], NULL, print_in_reverse_order, &threadNumber[i])){
            cerr << "Failed to create thread";
            return 1;
        }
    }
    //wait for the other threads to finish 
    for (int i = 0; i < nthreads; i++){
        pthread_join(tid[i], NULL);
    }
    delete [] threadNumber;
    delete [] tid;
    return 0;
}