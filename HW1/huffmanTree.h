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
  huffmanTree(map<char, int> &m) : size(m.size()), head(nullptr) {
    buildHuffmanTree(m);
  };
  ~huffmanTree();
  void buildHuffmanTree(map<char, int> &m);
};

void huffmanTree::buildHuffmanTree(map<char, int> &m) {
  auto it = m.end();
  head = new node;
  head->data = it->first;
  head->freq = it->second;
  it--;
  for (it; it != m.begin(); it--)
    cout << it->first << " " << it->second << endl;
}

#endif