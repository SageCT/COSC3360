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
  bool operator()(node *L, node *R) {
    if (L->freq == R->freq) return (L->data > R->data);
    return (L->freq > R->freq);
  }
};

class huffmanTree {
 private:
  priority_queue<node *, vector<node *>, Compare> pq;
  node *root;

 public:
  huffmanTree() : root(nullptr) {}
  huffmanTree(vector<node *> &n) : root(nullptr) { buildHuffmanTree(n); };
  void buildHuffmanTree(vector<node *> &);
  friend bool operator<(const pair<int, char> &, const pair<int, char> &);
  void decode(vector<code *> &);
  void print();
};

void huffmanTree::buildHuffmanTree(vector<node *> &n) {
  // Add nodes to a priority queue to create the Huffman tree
  for (int i = 0; i < n.size(); i++) pq.push(n.at(i));

  // Building the Huffman tree

  // While there is more than one node in the priority queue
  while (pq.size() > 1) {
    // Set left and right nodes as the smallest in the pq (the first two nodes)
    node *left = pq.top();
    pq.pop();
    node *right = pq.top();
    pq.pop();

    // Create a new node with the sum of the frequencies of the two smaller
    // nodes, this will be the parent node
    node *parent = new node("", left->freq + right->freq);
    parent->left = left;
    parent->right = right;
    // Because the nodes will always be sorted in the priority queue, we can
    // just add the parent back the the queue
    pq.push(parent);
    // set the parent as the root
    parent = root;
  }

  // TESTING PRINTING OUT THE PRIORITY QUEUE
  print();
}

void huffmanTree::decode(vector<code *> &c) {
  string result(c.size(), '$');
  cout << "Result: " << result << endl;
  for (int i = 0; i < c.size(); i++) {
    int currCode = c.at(i)->data;
    for (auto position : c.at(i)->pos) {
    }
  }
}

void huffmanTree::print() {
  while (!pq.empty()) {
    cout << "Symbol: " << pq.top()->data << ", Frequency : " << pq.top()->freq
         << ", Code: N/A" << endl;
    pq.pop();
  }
}

#endif