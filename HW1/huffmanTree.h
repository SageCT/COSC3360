#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <map>

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
  ~huffmanTree();
  void buildHuffmanTree(map<int, char> &m);
  void print();
};

void huffmanTree::buildHuffmanTree(map<int, char> &m) {
  auto it = m.end();
  auto stopAt = ++m.begin();

  for (it; it != stopAt; it--) {
    // node *left = new node;
  }
}

#endif