#include "huffmanTree.h"
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;
char getChar(string in) {
  char fixed = in[0];
  return fixed;
}
int getNum(string in) {
  int num;
  num = stoi(in.substr(2));
  return num;
}
struct args {
  char symbol;
  string bcode;
  int turn;
  int tnum;
  vector<int> positions;
  vector<char> *message;
  pthread_mutex_t *bsem;
  pthread_cond_t *waitTurn;

  args() {
    symbol = '0';
    bcode = "";
    turn = 0;
    tnum = 0;
    positions = {0};
    bsem = nullptr;
    waitTurn = nullptr;
  }
};
void *decodemssg(void *pos_void_ptr) {
  args *pos_ptr = (struct args *)pos_void_ptr;

  vector<int> temppos = pos_ptr->positions;
  int indext = pos_ptr->tnum;
  string tempbcode = pos_ptr->bcode;
  char tempsymb = pos_ptr->symbol;

  while (indext != pos_ptr->turn) {
    pthread_cond_wait(pos_ptr->waitTurn, pos_ptr->bsem);
  }
  pthread_mutex_unlock(pos_ptr->bsem);

  for (int i = 0; i < pos_ptr->positions.size(); i++) {
    (*pos_ptr->message)[temppos[i]] = tempsymb;
  }

  cout << "Symbol: " << pos_ptr->symbol
       << ", Frequency: " << pos_ptr->positions.size()
       << ", Code: " << pos_ptr->bcode << endl;

  pthread_mutex_lock(pos_ptr->bsem);
  (pos_ptr->turn)++;
  pthread_cond_broadcast(pos_ptr->waitTurn);
  pthread_mutex_unlock(pos_ptr->bsem);
  return nullptr;
}

int main() {
  string infile, line, binaryc, empt = "", temp_pos;
  char letr, letter;
  int totalsymb = 0, letpr = 0, freq = 0, size = 0, cmpsize = 0;
  vector<int> freqc;  // frequency vector
  vector<char> inltr; // character vector
  huffmantree *root;
  getline(cin, line);
  totalsymb = stoi(line); // use totalsymb as the for loop iterator
  // cout << totalsymb << endl;

  for (int i = 0; i < totalsymb; i++) {
    getline(cin, line);
    letr = getChar(line);
    letpr = getNum(line);
    // cout << "Letter " << letr << " Frequency " << letpr << endl;
    freq = freq + letpr;
    freqc.push_back(letpr);
    inltr.push_back(letr);
  }

  size = inltr.size();
  HuffmanCodes(inltr, freqc, size);

  // vector for args and threads
  args argst;
  int turn = 0;
  static pthread_mutex_t bsem;
  pthread_mutex_init(&bsem, NULL);
  static pthread_cond_t waitTurn_ = PTHREAD_COND_INITIALIZER;
  int tnum = totalsymb;

  vector<pthread_t> tid;
  // vector <pthread_t> temptid;
  // int *threadNumber = new int[totalsymb];

  root = getRoot(inltr, freqc, size);
  vector<char> finalmsg(root->freq);
  vector<int> positions;

  // pthread_mutex_init(&bsem, NULL);

  for (int i = 0; i < totalsymb; i++) {
    getline(cin, line);
    pthread_t newthread;
    // threadNumber[i] = i;
    // cout << line << endl;
    istringstream iss(line);
    iss >> binaryc;

    // vector<char> *msg(root->freq);
    positions.clear();
    letter = findLetter(root, binaryc, empt, letter);
    while (iss >> temp_pos) {
      int post;
      post = stoi(temp_pos);
      positions.push_back(post);
    }
    pthread_mutex_lock(&bsem);

    argst.positions = positions;
    argst.symbol = letter;
    argst.message = &finalmsg;
    argst.bcode = binaryc;
    argst.waitTurn = &waitTurn_;
    argst.bsem = &bsem;
    argst.tnum = i;

    // cout << argst.symbol << endl;
    pthread_create(&newthread, NULL, decodemssg, &argst);
    tid.push_back(newthread);

    // args* newargs = new args(letter, positions, msg, binaryc);
  }
  for (int i = 0; i < size; i++) {
    pthread_join(tid[i], NULL);
  }
  cout << "Original message: ";
  for (int i = 0; i < finalmsg.size(); i++) {
    cout << finalmsg[i];
  }
}
