#include <pthread.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

static pthread_mutex_t bsem;
static pthread_cond_t waitTurn = PTHREAD_COND_INITIALIZER;
static int turn;
static int nthreads;

/*
I am Thread 3
I am Thread 1
I am Thread 0
I am Thread 2
I am Thread 4
*/

void *print_in_increasing_order_even_odd(void *void_ptr_argv)
{
    int *num = (int *)void_ptr_argv;

    pthread_mutex_lock(&bsem); //start critical section

    while (turn != (*num))     //while its not our turn, we wait
    {
        pthread_cond_wait(&waitTurn, &bsem);
    }

    pthread_mutex_unlock(&bsem);

    //print outside the critical section
    std::cout << "I am Thread " << (*num) << std::endl;


    pthread_mutex_lock(&bsem);
    
    if (turn % 2 == 1 && turn - 1 == 0) //means we finished odds
    {
        turn = 0; 
    }

    else if (turn % 2 == 0 && turn + 1 == nthreads) //means we finished printing evens
    {
        turn = -1;
    }

    else
    {
        //if odd, decrement by 2
        if(turn % 2 == 1)
            turn -= 2;

        //if even, increment by 2
        else
            turn += 2;
    }
    
    pthread_cond_broadcast(&waitTurn);
    pthread_mutex_unlock(&bsem);

    return NULL;
}

int main()
{
    std::cin >> nthreads;
    pthread_mutex_init(&bsem, NULL); // Initialize access to 1
    pthread_t *tid = new pthread_t[nthreads];
    int *threadNumber = new int[nthreads];

    if(nthreads % 2 == 0)
        turn = nthreads - 1;

    else
        turn = nthreads - 2;

    for (int i = 0; i < nthreads; i++)
    {
        threadNumber[i] = i;
        pthread_create(&tid[i], NULL, print_in_increasing_order_even_odd, (void *)&threadNumber[i]);
    }

    // Call pthread_join
    for (int i = 0; i < nthreads; i++)
    {
        pthread_join(tid[i], NULL);
    }

    delete[] threadNumber;
    delete[] tid;
    return 0;
}
