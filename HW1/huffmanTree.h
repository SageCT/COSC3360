#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <pthread.h>

#include <iostream>
#include <queue>

using namespace std;
struct node {
  string data;
  int freq;
  node *left;
  node *right;
  node() : data(""), freq(0), left(nullptr), right(nullptr) {}
  node(string data, int freq)
      : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

struct code {
  string data;
  vector<int> pos;
  code(string data, vector<int> pos) : data(data), pos(pos) {}
};

// class huffmanQueue {
// private:
//   vector<node *> pq;
//   void balance();
//   void add(node *n);

// public:
//   huffmanQueue() {}
//   huffmanQueue(vector<node *> &n) : pq(n) {}
//   void push(node *n) { add(n); }
//   node *pop() {
//     node *temp = pq[0];
//     pq.erase(pq.begin());
//     return temp;
//   }
//   node *top() { return pq.at(0); }
//   int length() { return pq.size(); }
// };

// void huffmanQueue::add(node *n) {
//   for (int i = pq.size(); i > 0; i--) {
//     if (pq[i]->freq == n->freq) {
//       if (pq[i]->data == n->data) {
//         if (pq[i] < n) {
//           pq.insert(pq.begin() + i, n);
//           return;
//         }
//       }
//     } else if (pq[i]->freq < n->freq) {
//       pq[i] = n;
//       return;
//     }
//   }
// }

// class priQueue {
// private:
//   int size;
//   vector<node *> pq;

// public:
//   priQueue() { size = 0; }
//   ~priQueue() {}
//   void push(node *);
//   node *pop();
//   node *top() const {
//     if (!isEmpty())
//       return pq.at(0);
//     return nullptr;
//   };
//   int getSize() const { return size; }
//   bool isEmpty() const { return size == 0; }
// };

// void priQueue::push(node *n) {
//   if (isEmpty()) {
//     pq.push_back(n);
//     size++;
//   } else {
//     int index = size - 1;
//     while (index > -1) {
//       if (index == 0 && n->freq < pq[index]->freq) {
//         pq.insert(pq.begin() + index, toAdd);
//         index--, size++;
//       } else if (pri < pq[index].priority) {
//         index--;
//       } else if (pri >= pq[index].priority) {
//         pq.insert(pq.begin() + index + 1, toAdd);
//         index = -1;
//         size++;
//       }
//     }
//   }
// }

// node *priQueue::pop() {
//   if (!isEmpty()) {
//     node *temp = pq[0];
//     pq.erase(pq.begin() + 0);
//     size--;
//     return temp;
//   }
//   return "-1";
// }

class huffmanCompare {
public:
  bool operator()(node *L, node *R) {
    if (L->freq == R->freq) {
      if (L->data == R->data)
        return (L > R);
      return L->data > R->data;
    }
    return L->freq > R->freq;
  }
};

class huffmanTree {
private:
  priority_queue<node *, vector<node *>, huffmanCompare> pq;
  node *root;
  string decodedMessage;
  node *printInOrder(node *, string = "");

public:
  huffmanTree() : root(nullptr) {}
  huffmanTree(vector<node *> &n) : root(nullptr) { buildHuffmanTree(n); };
  void buildHuffmanTree(vector<node *> &);
  void decode(vector<code *>);

  void print();
};

void huffmanTree::buildHuffmanTree(vector<node *> &n) {
  // Add nodes to an overridden priority queue
  for (int i = 0; i < n.size(); i++)
    pq.push(n.at(i));

  // Building the Huffman tree

  // While there is more than one node in the priority queue
  while (pq.size() > 1) {
    // Set left and right nodes as the smallest in the pq (the first two nodes)
    node *left = pq.top();
    pq.pop();
    node *right = pq.top();
    pq.pop();

    // TESTING print out left and right

    // Create a new node with the sum of the frequencies of the two smaller
    // nodes, this will be the "parent" node
    node *parent = new node("\0", left->freq + right->freq);
    parent->left = left;
    parent->right = right;
    // Because the nodes will always be sorted in the priority queue, we can
    // just add the parent back the the queue
    pq.push(parent);
  }
  // Set the remaining node as the root
  root = pq.top();
  pq.pop();

  printInOrder(root);
}

// HEISENBUG HERE

void huffmanTree::decode(vector<code *> c) {
  // Find the largest position to find the length of the final string
  int max = 0;

  for (int i = 0; i < c.size(); i++)
    for (int j = 0; j < c.at(i)->pos.size(); j++) {
      if (max < c.at(i)->pos.at(j))
        max = c.at(i)->pos.at(j);
    }

  cout << "Max: " << max << endl;
  string result(max + 1, '*');

  for (int i = 0; i < c.size(); i++) {

    string currCode = c.at(i)->data;
    node *cu = root;
    cout << "Current code: " << currCode << endl;

    for (auto curr : currCode) {
      // If current char is 0, go left
      //  If current char is 1, go right
      if (curr == '0' && cu->left)
        cu = cu->left;
      else if (curr == '1' && cu->right)
        cu = cu->right;
    }

    // Once you get the char from the decode, set the data at the given position
    // in the result string
    for (int position : c.at(i)->pos) {
      result.at(position) = cu->data.at(0);
    }
  }

  decodedMessage = result;
}

void huffmanTree::print() {
  // printInOrder(root);
  // cout << "Original message: " << decodedMessage << endl;
}

node *huffmanTree::printInOrder(node *n, string c) {
  if (!n)
    return nullptr;
  printInOrder(n->left, c + "0");
  if (n->data != "\0") {
    std::cout << "Symbol: " << n->data << ", Frequency : " << n->freq
              << ", Code: " << c << endl;
  }
  printInOrder(n->right, c + "1");
  return n;
}

#endif