#include <fstream>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <sstream>
#include <unistd.h>
#include <vector>

using namespace std;

// node structure for the Huffman tree
struct Node {
  char symbol;
  int frequency;
  Node *left;
  Node *right;

  Node(char s, int f)
      : symbol(s), frequency(f), left(nullptr), right(nullptr) {}
  Node(Node *l, Node *r)
      : symbol(0), frequency(l->frequency + r->frequency), left(l), right(r) {}
};

// comparing for priority queue
struct Compare {
  bool operator()(Node *l, Node *r) {
    if (l->frequency == r->frequency) {
      if (l->symbol == r->symbol) {
        return l < r;
      }
      return l->symbol > r->symbol;
    }
    return l->frequency > r->frequency;
  }
};

struct arguments {
  char alph;
  char letter;
  int frequency;
  string symb;
  vector<int> pos;
  char *message;
  Node *root;
  vector<vector<int>> positions;
  int rowsize;
  pthread_mutex_t *bsemPTR;
  pthread_mutex_t *bsemPTR2;
  pthread_cond_t *condVar;
  // ! TURN NEEDS TO BE POINTER BC IT IS A SHARED RESOURCE !//
  int *turn;
  int threadnumber;
};

// function to build the Huffman tree using chat gpt
Node *buildHuffmanTree(vector<char> &symbols, vector<int> &frequencies) {

  priority_queue<Node *, vector<Node *>, Compare> pq;

  for (size_t i = 0; i < symbols.size(); ++i) {
    pq.push(new Node(symbols[i], frequencies[i]));
  }

  while (pq.size() > 1) {
    Node *l = pq.top();
    pq.pop();

    Node *r = pq.top();
    pq.pop();

    Node *parent = new Node(l, r);
    pq.push(parent);
  }

  return pq.top();
}

// function that traverses the huffman tree to find the right symbol
char decodeHuffmanCodes(Node *root, string encodedString, int index) {
  Node *currentNode = root;

  if (currentNode->left == nullptr && currentNode->right == nullptr) {
    return currentNode->symbol;
  }

  if (encodedString[index] == '0') {
    currentNode = currentNode->left;
  } else {
    currentNode = currentNode->right;
  }

  if (currentNode->left == nullptr && currentNode->right == nullptr) {
    return currentNode->symbol;
  }

  return decodeHuffmanCodes(currentNode, encodedString, index + 1);
}
// function to print the Huffman codes for each symbol
void printHuffmanCodes(Node *root, string code = "") {
  if (root == nullptr) {
    return;
  }

  if (root->symbol != 0) {
    cout << "Symbol: " << root->symbol << ", Frequency: " << root->frequency
         << ", Code: " << code << endl;
  }

  printHuffmanCodes(root->left, code + "0");
  printHuffmanCodes(root->right, code + "1");
}

// thread function that traverses the tree and decode message
void *decode(void *void_ptr) {
  arguments *arg_ptr = (arguments *)void_ptr;

  int threadnum = arg_ptr->threadnumber;
  string code = arg_ptr->symb;
  int freq = arg_ptr->frequency;
  vector<vector<int>> posi;
  posi.push_back(arg_ptr->pos);
  pthread_mutex_unlock(arg_ptr->bsemPTR);

  pthread_mutex_lock(arg_ptr->bsemPTR2);

  while (threadnum != *arg_ptr->turn) {
    pthread_cond_wait(arg_ptr->condVar, arg_ptr->bsemPTR);
  }
  pthread_mutex_unlock(arg_ptr->bsemPTR2);

  int j = 0;
  arg_ptr->alph = decodeHuffmanCodes(arg_ptr->root, arg_ptr->symb, j);
  // cout << arg_ptr->alph << endl;
  // cout << arg_ptr->positions[threadnum].size() <<endl;
  for (int i = 0; i < arg_ptr->positions[threadnum].size(); i++) {
    int index = arg_ptr->positions[threadnum][i];
    // cout << index << " ";
    arg_ptr->message[index] = arg_ptr->alph;
  }
  cout << "Symbol: " << arg_ptr->alph
       << ", Frequency: " << arg_ptr->positions[threadnum].size()
       << ", Code: " << code << endl;

  // cout << endl;

  pthread_mutex_lock(arg_ptr->bsemPTR);
  (*arg_ptr->turn)++;
  pthread_cond_broadcast(arg_ptr->condVar);
  pthread_mutex_unlock(arg_ptr->bsemPTR);

  return nullptr;
}
int main() {

  vector<char> symbols;
  vector<int> frequencies;

  vector<string> code;
  vector<int> positions;

  int totalFreq = 0;
  string numalph;
  getline(cin, numalph);

  // cout << numalph << endl;
  for (int i = 0; i < stoi(numalph); i++) {
    string line;
    getline(cin, line);
    char ch;
    int freq;

    if (line[0] == ' ') {
      ch = ' ';
      freq = stoi(line.substr(1));
      totalFreq += freq;

    } else {
      ch = line[0];
      freq = stoi(line.substr(2));
      totalFreq += freq;
    }

    symbols.push_back(ch);
    frequencies.push_back(freq);
  }

  // build Huffman tree
  Node *root = buildHuffmanTree(symbols, frequencies);

  // print Huffman codes
  // printHuffmanCodes(root);

  string line1;

  vector<string> symb;
  vector<int> numpos;
  vector<vector<int>> postions;
  vector<int> rowpos;

  int highfreq = 0;

  for (int i = 0; i < stoi(numalph); i++) {
    getline(cin, line1);

    // nthreads++;

    // cout << line1 << endl;
    string co;
    int pos;
    int wspace = 0;
    for (int j = 0; j < line1.size(); ++j) {
      if (line1[j] == ' ') {
        wspace++;
      }
    }

    numpos.push_back(wspace);

    istringstream iss(line1);

    iss >> co;

    symb.push_back(co);

    while (wspace != 0) {
      iss >> pos;
      if (pos > highfreq) {
        highfreq = pos;
      }
      rowpos.push_back(pos);

      // arg[i].pos.push_back(pos);

      wspace--;
    }
    postions.push_back(rowpos);
    rowpos.clear();
  }

  int nthreads = stoi(numalph);
  // static pthread_mutex_t bsem;

  pthread_t *tid = new pthread_t[nthreads];
  //*pthread_mutex_init(&bsem, NULL);

  //! ADDED SECOND SEMAPHORE BC RINCON SAID YOU NEED 2 (HE'S BUGGING) !//
  pthread_mutex_t bsem = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t bsem2 = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
  arguments arg;
  char *message = new char[totalFreq];

  arg.bsemPTR2 = &bsem2;
  arg.bsemPTR = &bsem;
  arg.condVar = &empty;

  //     for( int i = 0 ; i < nthreads; i++){
  //     for(int j = 0; j < numpos[i]; j++){
  //          //arg[i].pos.push_back(postions[i][j]);
  //          //cout << postions[i][j] << endl;
  //     }
  //     //arg[i].symb = symb[i];
  //     //cout << arg[i].alph << endl;
  // }
  int turn = 0;
  for (int i = 0; i < nthreads; i++) {

    pthread_mutex_lock(&bsem);

    arg.message = message;
    arg.positions.push_back(postions[i]);
    arg.symb = symb[i];
    arg.rowsize = numpos[i];
    arg.threadnumber = i;
    //! CHANGED TURN TO BE A REFERENCE TO AN INT !//
    arg.turn = &turn;
    arg.root = root;
    // arg.letter = symbols[i]; //if needed
    // arg.frequency = frequencies[i]; // if needed

    if (pthread_create(&tid[i], nullptr, decode, &arg)) {
      fprintf(stderr, "Error creating thread\n");
      return 1;
    }

    // pthread_mutex_unlock(&bsem);
  }
  for (int i = 0; i < nthreads; i++) {
    pthread_join(tid[i], nullptr);
  }
  cout << "Original message: ";
  for (int i = 0; i < totalFreq; i++) {
    cout << message[i];
  }
  cout << std::endl;

  delete[] tid;
  // delete arg;
  delete[] message;
  //! DESTORY MUTEXES !//
  pthread_mutex_destroy(&bsem);
  pthread_mutex_destroy(&bsem2);

  return 0;
}