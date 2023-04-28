#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
using namespace std;

struct node {
  char letter;
  int freq;
  node *left;
  node *right;
  int age;
  node() {
    letter = '\0';
    freq = 0;
    left = nullptr;
    right = nullptr;
  }
  node(char let, int num) {
    letter = let;
    freq = num;
    left = nullptr;
    right = nullptr;
  }
  ~node() { // deconstructor to delete nodes
    delete left;
    delete right;
  }
};

// custom comparator, referenced from geeks for geeks:
// https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/
struct compareNode {
  bool operator()(node *const &n1, node *const &n2) {
    return n1->freq == n2->freq
               ? (n1->letter == n2->letter ? n1->age < n2->age
                                           : n1->letter > n2->letter)
               : n1->freq > n2->freq;
  }
};

class huffmanTree {
private:
  node *root;
  map<string, pair<char, int>> codeMap;
  int nodeAge;

public:
  huffmanTree() {
    root = nullptr;
    nodeAge = 0;
  }
  ~huffmanTree() { // deconstructor deletes root
    delete root;
  }

  void makeTree(priority_queue<node *, vector<node *>, compareNode> q) {
    while (q.size() > 1) {
      node *temp = new node();
      nodeAge++;
      temp->age = nodeAge; // age is assigned to node
      temp->left = q.top();
      q.pop();
      temp->right = q.top();
      q.pop();
      temp->freq = temp->left->freq + temp->right->freq;
      q.push(temp);
    }
    root = q.top();
  }

  void findCode() { findCodeHelper(root, "", codeMap); }

  void findCodeHelper(node *curr, string code,
                      map<string, pair<char, int>> &codeMap) {
    if (curr->letter != '\0') {
      pair<char, int> letterFreq(curr->letter, curr->freq);
      codeMap[code] = letterFreq;
    }
    if (curr->left) {
      code += '0';
      findCodeHelper(curr->left, code, codeMap);
      code.pop_back();
    }
    if (curr->right) {
      code += '1';
      findCodeHelper(curr->right, code, codeMap);
      code.pop_back();
    }
  }

  char decoder(string code) { return codeMap[code].first; }

  char getFreq(string code) { return codeMap[code].second; }
};
