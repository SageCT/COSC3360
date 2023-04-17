#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <cmath>
#include <algorithm>

//this one looks stable, could work, just add waitTurn
#include "huffmanTree.h"
using namespace std;

//struct for threading
template <class T>
struct thread_struct {
    PNode<T> *root;
    string line;
    int turn = 0;
    pthread_mutex_t *bsem;
    pthread_cond_t *waitTurn;
    vector<pair<int, char> > list;// char with their position in the string
};

// Function prototypes
vector<string> getPos(string input);

template <class T>
char getChar(PNode<T> *node, string path, int index);

template <class T>
int getFreq(PNode<T> *node, string path, int index);

template <class T>
void *decode(void *thread_arg) {
    thread_struct<T> *thread_point = (thread_struct<T> *)thread_arg;

    vector<string> posNum = getPos(thread_point->line);
    char dataChar = getChar(thread_point->root, posNum[0], 0);
    int Freq = getFreq(thread_point->root, posNum[0], 0);

    int turn = thread_point->turn;
    while (turn != thread_point->turn) {
        pthread_cond_wait(thread_point->waitTurn, thread_point->bsem);
    }
    pthread_mutex_unlock(thread_point->bsem);
    pthread_mutex_lock(thread_point->bsem);
    cout << "Symbol: " << dataChar << ", Frequency: " << Freq << ", Code: " << posNum[0] << endl;
    thread_point->turn += 1;
    pthread_cond_broadcast(thread_point->waitTurn);
    pthread_mutex_unlock(thread_point->bsem);

    for (int i = 1; i < posNum.size(); i++) {
        int n = stoi(posNum[i]);
        thread_point->list.push_back(make_pair(n, dataChar));
    }
    return (NULL);
}


int main(int argc, char** argv){
    string fileName, commandName;
    string inputString, inputCommand;
    int arr_size=0, num_threads=0;

    //getting first size number input from files
    string size_num="";
    cin >> size_num;
    cin.ignore();

    //number version of size_num
    int num = stoi(size_num);

    //n for while loop increment
    int n=0;

    //getting input from input file and push into huffmanTree
    prio_queue<char> pq;
    while(getline(cin, inputString)){
        pq.enqueue(inputString[0], stoi(inputString.substr(2, inputString.size() - 1)));
        arr_size += stoi(inputString.substr(2, inputString.size() - 1));// getting original message size
        
        if(n == num-1){
            break;
        }
        n++;
    }

    //processing tree
    pq.process();

    // Initialize bsem mutex
    pthread_mutex_t bsem;
    pthread_mutex_init(&bsem, NULL);
    pthread_cond_t waitTurn = PTHREAD_COND_INITIALIZER;

    // Create and initialize thread_struct
    thread_struct<char> *thread_data = new thread_struct<char>();
    thread_data->bsem = &bsem;
    thread_data->waitTurn = &waitTurn;

    // Create and initialize threads vector
    vector<pthread_t> threads;

    //reset n
    n=0;

    //create turn;
    int turn = 0;

    //getting input from command file with multiple threads, every input will have a dedicated thread
    while(getline(cin, inputCommand)){
        num_threads+=1;

        pthread_t thread;

        pthread_mutex_lock(&bsem);

        thread_data->line = inputCommand;
        thread_data->turn = turn;
        thread_data->root = pq.getFront();
        turn++;
        
        pthread_create(&thread, NULL, &decode<char>, (void*)thread_data);
        threads.push_back(thread);

        if(n == num-1){
            break;
        }
        n++;
        // turn++;// note this
    }

    //joining threads
    for (int i = 0; i < threads.size(); i++) {
        pthread_join(threads[i], NULL);
    }

    // Sort the list
    sort(thread_data->list.begin(), thread_data->list.end());

    //printing out the original message
    cout<<"Original message: ";
    for (int i = 0; i < arr_size; i++) {
        cout << thread_data->list[i].second;
    }

    // Free the memory for thread_data
    delete thread_data;

    return 0;
}

//getting position from string
vector<string> getPos(string input){
    vector<string> result;
    string s;
    stringstream ss(input);
    while(getline(ss, s, ' ')){
        result.push_back(s);
    }
    return result;
}

template <class T>
char getChar(PNode<T> *node, string path, int index) {
    if (path.empty()) {
        return '\0';
    }
    if (index == path.length()) {
        return node->data;
    }
    if (path[index] == '0') {
        return getChar(node->left, path, index + 1);
    }
    else if (path[index] == '1') {
        return getChar(node->right, path, index + 1);
    }
    return '\0';
}

template <class T>
int getFreq(PNode<T> *node, string path, int index) {
    if (path.empty()) {
        return 0;
    }
    if (index == path.length()) {
        return node->prio;
    }
    if (path[index] == '0') {
        return getFreq(node->left, path, index + 1);
    }
    else if (path[index] == '1') {
        return getFreq(node->right, path, index + 1);
    }
    return 0;
}

