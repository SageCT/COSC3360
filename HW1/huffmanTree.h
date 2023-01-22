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

class Compare {
public:
  bool operator()(node *L, node *r) { return (L->freq > r->freq); }
};

class huffmanTree {
private:
  priority_queue<node *, vector<node *>, Compare> pq;

public:
  huffmanTree(vector<pair<int, char>> &v) { buildHuffmanTree(v); };
  void buildHuffmanTree(vector<pair<int, char>> &);
  friend bool operator<(const pair<int, char> &, const pair<int, char> &);
  void print();
};

// Overload the < operator for the priority queue
bool operator<(const pair<int, char> &a, const pair<int, char> &b) {
  return (a.first > b.first);
}

void huffmanTree::buildHuffmanTree(vector<pair<int, char>> &v) {
  // Create nodes from the pairs passed
  for (int i = 0; i < v.size(); i++) {
    node *temp = new node;
    temp->data = v.at(i).second;
    temp->freq = v.at(i).first;
    temp->left = nullptr;
    temp->right = nullptr;
    pq.push(temp);
  }

  // Create a priority queue to store nodes for Huffman tree

  for (int i = 0; i < v.size(); i++) {
    pq.push(v.at(i));
  }

  // TESTING PRINTING OUT THE PRIORITY QUEUE
  print();
}

void huffmanTree::print() {
  while (!pq.empty()) {
    cout << "Symbol: " << pq.top().second << ", Frequency : " << pq.top().first
         << ", Code: N/A" << endl;
    pq.pop();
  }
}

#endif