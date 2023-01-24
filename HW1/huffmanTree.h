#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <map>
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
  int data;
  vector<int> pos;
  code(int data, vector<int> pos) : data(data), pos(pos) {}
};

class Compare {
public:
  bool operator()(node *L, node *r) {
    if (L->freq == r->freq)
      return (L->data < r->data);
    return (L->freq < r->freq);
  }
};

class huffmanTree {
private:
  priority_queue<node *, vector<node *>, Compare> pq;

public:
  huffmanTree(vector<node *> &n) { buildHuffmanTree(n); };
  void buildHuffmanTree(vector<node *> &);
  friend bool operator<(const pair<int, char> &, const pair<int, char> &);
  void decode(vector<code *> &);
  void print();
};

// Overload the < operator for the priority queue
bool operator<(const pair<int, char> &a, const pair<int, char> &b) {
  return (a.first > b.first);
}

void huffmanTree::buildHuffmanTree(vector<node *> &n) {
  // Add nodes to a priority queue to create the Huffman tree
  for (int i = 0; i < n.size(); i++)
    pq.push(n.at(i));


  // TESTING PRINTING OUT THE PRIORITY QUEUE
  print();
}

void huffmanTree::decode(vector<code *> &c) {}

void huffmanTree::print() {
  while (!pq.empty()) {
    cout << "Symbol: " << pq.top()->data << ", Frequency : " << pq.top()->freq
         << ", Code: N/A" << endl;
    pq.pop();
  }
}

#endif