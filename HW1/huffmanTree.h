#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <pthread.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
struct node {
  string data;
  int freq;
  shared_ptr<node> left, right;

  node() : data(""), freq(0), left(nullptr), right(nullptr) {}
  node(string data, int freq)
      : data(data), freq(freq), left(nullptr), right(nullptr) {}
  node(string data, int freq, shared_ptr<node> left, shared_ptr<node> right)
      : data(data), freq(freq), left(left), right(right) {}
  node(shared_ptr<node> n)
      : data(n->data), freq(n->freq), left(n->left), right(n->right) {}
};

struct code {
  string data;
  vector<int> pos;
  code(string data, vector<int> pos) : data(data), pos(pos) {}
};

class huffmanCompare {
public:
  bool operator()(shared_ptr<node> &L, shared_ptr<node> &R) {
    if (L->freq == R->freq) {
      if (L->data == R->data)
        return (L < R);
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

  shared_ptr<node> printInOrder(shared_ptr<node> &, string = "");

public:
  huffmanTree() : root(nullptr) {}
  huffmanTree(vector<shared_ptr<node>> &n) : nodes(n), root(nullptr) {
    buildHuffmanTree(n);
  };
  void buildHuffmanTree(vector<shared_ptr<node>> &);
  void decode(vector<shared_ptr<code>> &);

  void print() {
    printInOrder(root);
    std::cout << "Original message: " << decodedMessage << std::endl;
  };
};

void huffmanTree::buildHuffmanTree(vector<shared_ptr<node>> &n) {
  // Make sure that the vector is the correct one passed
  if (nodes != n)
    nodes = n;

  // Add nodes to an overridden priority queue
  for (int i = 0; i < n.size(); i++)
    pq.push(nodes.at(i));

  // Building the Huffman tree

  // While there is more than one node in the priority queue
  while (pq.size() > 1) {
    // Set left and right nodes as the smallest in the pq (the first two nodes)
    shared_ptr<node> left(pq.top());
    pq.pop();
    shared_ptr<node> right(pq.top());
    pq.pop();

    // Create a new node with the sum of the frequencies of the two smaller
    // nodes, this will be the "parent" node
    shared_ptr<node> parent(
        make_shared<node>(node("\0", left->freq + right->freq, left, right)));

    // Because the nodes will always be sorted in the priority queue, we can
    // just add the parent back the the queue
    pq.push(parent);
    root = parent;
  }
}

void huffmanTree::decode(vector<shared_ptr<code>> &c) {
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
      // If current char is 0, go left
      //  If current char is 1, go right
      curr == '0' ? cu = cu->left : cu = cu->right;
    }

    // Once you get the char from the decode, set the data at the given position
    // in the result string
    for (int position : c.at(i)->pos) {
      result.at(position) = cu->data.at(0);
    }
  }
  decodedMessage = result;
}

shared_ptr<node> huffmanTree::printInOrder(shared_ptr<node> &n, string c) {
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