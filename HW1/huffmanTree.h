#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <map>
#include <queue>

using namespace std;
struct node {
  char data;
  int freq;
  node *left;
  node *right;
};

class huffmanTree {
public:
  int size;
  node *head;
  huffmanTree() : size(0), head(nullptr){};
  huffmanTree(map<int, char> &m) : size(m.size()), head(nullptr) {
    buildHuffmanTree(m);
  };
  void buildHuffmanTree(map<int, char> &m);
  void print();
};

void huffmanTree::buildHuffmanTree(map<int, char> &m) {
  // Flip the input map from char int to int char

  // Create a priority queue to store live nodes of Huffman tree
  // priority_queue priQueue(m.begin(), m.end(), std::greater<int>());
}

void huffmanTree::print() {}

#endif