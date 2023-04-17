#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <queue>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;
struct node {
  string data;
  int freq;
  int nodeNum;
  shared_ptr<node> left, right;

  node() : data(""), freq(0), left(nullptr), right(nullptr) {}
  node(string data, int freq, int nodeNum)
      : data(data), freq(freq), nodeNum(nodeNum), left(nullptr),
        right(nullptr) {}
  node(string data, int freq, int nodeNum, shared_ptr<node> left,
       shared_ptr<node> right)
      : data(data), freq(freq), left(left), right(right), nodeNum(nodeNum) {}
  node(shared_ptr<node> n)
      : data(n->data), freq(n->freq), left(n->left), right(n->right),
        nodeNum(n->nodeNum) {}
};

struct code {
  string data;
  vector<int> pos;
  code(string data, vector<int> pos) : data(data), pos(pos) {}
};

//* PA1 THREAD DATA STRUCT *//
struct threadData {
  int numChars;
  shared_ptr<node> root;
  shared_ptr<code> codeVal;
  shared_ptr<vector<char>> decMessage;
  threadData(shared_ptr<node> r, shared_ptr<code> c, int nC,
             shared_ptr<vector<char>> dC)
      : root(r), codeVal(c), numChars(nC), decMessage(dC) {}
};

//* PA2 THREAD DATA STRUCT *//
// struct socketThreadData {
//   code *codeVal;
//   char *hostname;
//   int portno;
//   vector<char> decMessage;
//
//   socketThreadData(code *c, char *hn, int pn, vector<char> dC)
//       : codeVal(c), hostname(hn), portno(pn), decMessage(dC) {}
//   ~socketThreadData() { delete codeVal; }
// };

//* PA3 THREAD DATA STRUCT *//
struct mutexThreadData {
  node *root;
  vector<shared_ptr<code>> codeVals;
  shared_ptr<vector<char>> decMessage;
  pthread_mutex_t *mutex;
  pthread_cond_t *waitTurn;
  int *turn;
  int nThreads;
  mutexThreadData(node *r, vector<shared_ptr<code>> c,
                  shared_ptr<vector<char>> dC, pthread_mutex_t *m,
                  pthread_cond_t *wT, int *t, int nT)
      : root(r), codeVals(c), decMessage(dC), mutex(m), waitTurn(wT), turn(t),
        nThreads(nT) {}

  mutexThreadData() {
    root = nullptr;
    mutex = nullptr;
    waitTurn = nullptr;
    turn = nullptr;
    nThreads = 0;
  }
};

class huffmanCompare {
public:
  bool operator()(shared_ptr<node> &L, shared_ptr<node> &R) {
    if (L->freq == R->freq) {
      if (L->data == R->data) {
        return (L->nodeNum < R->nodeNum);
      }
      return L->data > R->data;
    }
    return L->freq > R->freq;
  }
};

class huffmanTree {
private:
  priority_queue<shared_ptr<node>, vector<shared_ptr<node>>, huffmanCompare> pq;
  vector<shared_ptr<node>> nodes;
  shared_ptr<node> root;
  string decodedMessage;

public:
  huffmanTree() : root(nullptr) {}
  huffmanTree(vector<shared_ptr<node>> &n) : nodes(n), root(nullptr) {
    buildHuffmanTree(n);
  };

  void buildHuffmanTree(vector<shared_ptr<node>> &);
  shared_ptr<node> getRoot() const { return root; };
  void decode(vector<shared_ptr<code>> &, int type = 0);
  // shared_ptr<node> printInOrder(shared_ptr<node> &, string = "");
  // void print(bool socket = false) {
  //   printInOrder(root);
  //   if (!socket)
  //     std::cout << "Original message: " << decodedMessage << std::endl;
  // };
};

void huffmanTree::buildHuffmanTree(vector<shared_ptr<node>> &n) {
  // Make sure that the vector is the correct one passed
  if (nodes != n)
    nodes = n;

  // Counts number of nodes for comparions
  int nodeNum = n.size();

  // Add nodes to an overridden priority queue
  for (int i = 0; i < n.size(); i++)
    pq.push(nodes.at(i));

  // Building the Huffman tree
  // While there is more than one node in the priority queue
  while (pq.size() > 1) {
    // Set left and right nodes as the smallest in the pq (the first two
    // nodes)
    shared_ptr<node> left(pq.top());
    pq.pop();
    shared_ptr<node> right(pq.top());
    pq.pop();

    //* Create a new node with the sum of the frequencies of the two smaller
    //* nodes, this will be the "parent" node
    shared_ptr<node> parent(make_shared<node>(
        node("\0", left->freq + right->freq, nodeNum++, left, right)));

    // Because the nodes will always be sorted in the priority queue, we can
    // just add the parent back the the queue
    pq.push(parent);
    root = parent;
  }
}

//* PA2 DECODE VOID FUNCTION *//
void *decodethread(void *ptr) {
  // Change the void pointer to a shared_ptr<code> pointer
  threadData *c = (threadData *)ptr;

  shared_ptr<node> cu(c->root);
  string currCode = c->codeVal->data;

  for (char curr : currCode) {
    //* If current char is 0, go left
    //* If current char is 1, go right
    curr == '0' ? cu = cu->left : cu = cu->right;
  }
  //* Once you get the char from the decode, set the data at the given position
  //* in the result string
  for (int position : c->codeVal->pos) {
    c->decMessage->at(position) = cu->data.at(0);
  }

  return nullptr;
}

//* PA3 DECODE VOID FUNCTION *//
void *decodeThreadMutex(void *arg) {

  mutexThreadData *data = (mutexThreadData *)arg;
  mutexThreadData localData = *data;

  pthread_mutex_unlock(localData.mutex);
  //! End Critical Section 1 !//

  pthread_mutex_lock(localData.mutex);
  //! Start Critical Section 2 !//

  while (*localData.turn != localData.nThreads)
    pthread_cond_wait(localData.waitTurn, localData.mutex);

  pthread_mutex_unlock(localData.mutex);
  //! End Critical Section 2 !//

  int currThread = localData.nThreads;
  node *cu = localData.root;

  // Find the correct node to decode the message
  for (char curr : localData.codeVals.at(currThread)->data)
    curr == '0' ? cu = cu->left.get() : cu = cu->right.get();

  pthread_mutex_lock(localData.mutex);
  //! Start Critical Section 3 !//

  // Once you get the char from the decode, set the data at the given position
  // in the shared string
  for (int position : localData.codeVals.at(currThread)->pos)
    localData.decMessage->at(position) = cu->data.at(0);
  (*localData.turn)++;

  std::cout << "Symbol: " << cu->data << ", Frequency: " << cu->freq
            << ", Code: " << localData.codeVals.at(currThread)->data
            << std::endl;

  pthread_cond_broadcast(localData.waitTurn);
  pthread_mutex_unlock(localData.mutex);
  //! End Critical Section 3 !//

  return nullptr;
}

void huffmanTree::decode(vector<shared_ptr<code>> &c, int progAssign) {
  switch (progAssign) {
  case 1: {
    // Find the largest position to find the length of the final string
    int max = 0;

    for (int i = 0; i < c.size(); i++)
      for (int j = 0; j < c.at(i)->pos.size(); j++) {
        if (max < c.at(i)->pos.at(j))
          max = c.at(i)->pos.at(j);
      }
    string result(max + 1, '*');

    for (int i = 0; i < c.size(); i++) {
      string currCode = c.at(i)->data;
      shared_ptr<node> cu(make_shared<node>(root));

      for (auto curr : currCode) {
        //* If current char is 0, go left
        //* If current char is 1, go right
        curr == '0' ? cu = cu->left : cu = cu->right;
      }

      // Once you get the char from the decode, set the data at the given
      // position in the result string
      for (int position : c.at(i)->pos)
        result.at(position) = cu->data.at(0);
    }
    decodedMessage = result;
  } break;

  case 2: {
    int max = 0;

    for (int i = 0; i < c.size(); i++)
      for (int j = 0; j < c.at(i)->pos.size(); j++)
        if (max < c.at(i)->pos.at(j))
          max = c.at(i)->pos.at(j);

    string result = "";
    static vector<pthread_t> threads;
    static shared_ptr<vector<char>> message(
        make_shared<vector<char>>(vector<char>(max + 1)));

    for (auto i : c) {
      threadData *arg = new threadData(root, i, max + 1, message);
      pthread_t thread;
      pthread_create(&thread, nullptr, decodethread, arg);
      threads.push_back(thread);
    }

    for (auto &i : threads)
      pthread_join(i, nullptr);

    for (int i = 0; i < message->size(); i++)
      result += message->at(i);

    decodedMessage = result;
  } break;

  case 3: {
    int max = 0;

    for (int i = 0; i < c.size(); i++)
      for (int j = 0; j < c.at(i)->pos.size(); j++)
        if (max < c.at(i)->pos.at(j))
          max = c.at(i)->pos.at(j);

    string result = "";
    vector<pthread_t> threads;
    static shared_ptr<vector<char>> message(
        make_shared<vector<char>>(vector<char>(max + 1)));

    for (int i = 0; i < max + 1; i++) {
      message->at(i) = '*';
    }

    //* Create mutex and initalize
    static pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    //* Create condition variable and initalize
    static pthread_cond_t waitTurn = PTHREAD_COND_INITIALIZER;

    int nThreads = c.size();
    int turn = 0;

    mutexThreadData *arg = new mutexThreadData;

    for (int i = 0; i < c.size(); i++) {

      pthread_mutex_lock(&mutex);
      //! Start Critical Section 1 !//
      pthread_t thread;
      arg->root = root.get();
      arg->codeVals = c;
      arg->decMessage = message;
      arg->mutex = &mutex;
      arg->waitTurn = &waitTurn;
      arg->nThreads = i;
      arg->turn = &turn;

      pthread_create(&thread, nullptr, decodeThreadMutex, arg);
      threads.push_back(thread);
    }

    for (auto &i : threads)
      pthread_join(i, nullptr);

    pthread_mutex_destroy(&mutex);

    for (int i = 0; i < arg->decMessage->size(); i++)
      result += arg->decMessage->at(i);

    cout << "Original message: " << result << endl;
  } break;

  default: {
    // Find the largest position to find the length of the final string
    int max = 0;

    for (int i = 0; i < c.size(); i++)
      for (int j = 0; j < c.at(i)->pos.size(); j++) {
        if (max < c.at(i)->pos.at(j))
          max = c.at(i)->pos.at(j);
      }
    string result(max + 1, '*');

    for (int i = 0; i < c.size(); i++) {
      string currCode = c.at(i)->data;
      shared_ptr<node> cu(make_shared<node>(root));

      for (auto curr : currCode) {
        //* If current char is 0, go left
        //* If current char is 1, go right
        curr == '0' ? cu = cu->left : cu = cu->right;
      }

      // Once you get the char from the decode, set the data at the given
      // position in the result string
      for (int position : c.at(i)->pos) {
        result.at(position) = cu->data.at(0);
      }
    }
    decodedMessage = result;
  } break;
  }
}

// shared_ptr<node> huffmanTree::printInOrder(shared_ptr<node> &n, string c) {
//   if (!n)
//     return nullptr;
//   printInOrder(n->left, c + "0");
//   if (n->data != "\0") {
//     std::cout << "Symbol: " << n->data << ", Frequency: " << n->freq
//               << ", Code: " << c << endl;
//   }
//   printInOrder(n->right, c + "1");
//   return n;
// }

#endif