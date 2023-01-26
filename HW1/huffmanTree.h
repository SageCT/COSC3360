#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

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
  int data;
  vector<int> pos;
  code(int data, vector<int> pos) : data(data), pos(pos) {}
};

class Compare {
public:
  bool operator()(node *L, node *R) {
    if (L->freq == R->freq)
      return (L->data > R->data);
    return (L->freq > R->freq);
  }
};

class huffmanTree {
private:
  priority_queue<node *, vector<node *>, Compare> pq;
  node *root;
  node *printInOrder(node *, string = "");

public:
  huffmanTree() : root(nullptr) {}
  huffmanTree(vector<node *> &n) : root(nullptr) { buildHuffmanTree(n); };
  void buildHuffmanTree(vector<node *> &);
  friend bool operator<(const pair<int, char> &, const pair<int, char> &);
  void decode(vector<code *> &);
  void print() { printInOrder(root); };
};

void huffmanTree::buildHuffmanTree(vector<node *> &n) {
  // Add nodes to a priority queue to create the Huffman tree
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

    // Create a new node with the sum of the frequencies of the two smaller
    // nodes, this will be the parent node
    node *parent = new node("\0", left->freq + right->freq);
    parent->left = left;
    parent->right = right;
    // Because the nodes will always be sorted in the priority queue, we can
    // just add the parent back the the queue
    pq.push(parent);
  }
  // Set the remaining node as the root
  root = pq.top();

  // TESTING PRINTING OUT THE PRIORITY QUEUE
  print();
}

void huffmanTree::decode(vector<code *> &c) {

  // Find the largest position to find the length of the final string
  int max = 0;

  for (int i = 0; i < c.size(); i++)
    for (int j = 0; j < c.at(i)->pos.size(); j++)
      if (max < c.at(i)->pos.at(j))
        max = c.at(i)->pos.at(j);

  string result(max, '*');

  for (int i = 0; i < c.size(); i++) {
    string currCode = to_string(c.at(i)->data);
    node *cu = root;

    for (int j = 0; j < currCode.size(); j++) {
      // If current char is 0, go left
      //  If current char is 1, go right
      currCode.at(j) == '0' ? cu = cu->left : cu = cu->right;
    }

    // Once you get the result from the decode, add the data in the position in
    // the string
    for (auto position : c.at(i)->pos)
      result.at(position) = cu->data.at(0);
  }
}

node *huffmanTree::printInOrder(node *n, string c) {
  node *temp = n;
  if (n == nullptr)
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