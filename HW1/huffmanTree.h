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
  node(char data, int freq)
      : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

struct code {
  int data;
  vector<int> pos;
  code(int data, vector<int> pos) : data(data), pos(pos) {}
};

class Compare {
public:
  bool operator()(node *L, node *r) { return (L->freq > r->freq); }
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
  // Create nodes from the pairs passed
  for (int i = 0; i < n.size(); i++) {
    node *temp = new node;
    temp->data = n.at(i)->data;
    temp->freq = n.at(i)->freq;
    temp->left = nullptr;
    temp->right = nullptr;
    pq.push(temp);
  }

  // Create a priority queue to store nodes for Huffman tree

  for (int i = 0; i < n.size(); i++) {
    pq.push(n.at(i));
  }

  // TESTING PRINTING OUT THE PRIORITY QUEUE
  print();
}

void huffmanTree::decode(vector<code *> &c) {}

void huffmanTree::print() {
  while (!pq.empty()) {
    cout << "Symbol: " << pq.top().second << ", Frequency : " << pq.top().first
         << ", Code: N/A" << endl;
    pq.pop();
  }
}

#endif