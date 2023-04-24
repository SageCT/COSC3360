#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "huffmanTree.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

#define MAX_SIZE 100
class huffmantree {
public:
  char data;
  int freq;
  huffmantree *left;
  huffmantree *right;
  huffmantree(char character, int frequency) {
    data = character;
    freq = frequency;
    left = right = NULL;
  }
};

class Compare {
public:
  bool operator()(huffmantree *a, huffmantree *b) {
    if (a->freq == b->freq) {
      if (a->data == b->data)
        return a < b;
      return int(a->data) > int(b->data);
    }
    return a->freq > b->freq;
  }
};
huffmantree *
generateTree(priority_queue<huffmantree *, vector<huffmantree *>, Compare> pq) {
  while (pq.size() != 1) {
    huffmantree *left = pq.top();
    pq.pop();
    huffmantree *right = pq.top();
    pq.pop();
    huffmantree *node = new huffmantree('\0', left->freq + right->freq);
    node->left = left;
    node->right = right;
    pq.push(node);
  }

  return pq.top();
}
void printCodes(huffmantree *root, int arr[], int top) {
  if (root->left) {
    arr[top] = 0;
    printCodes(root->left, arr, top + 1);
  }
  if (root->right) {
    arr[top] = 1;
    printCodes(root->right, arr, top + 1);
  }
  if (!root->left && !root->right) {
    cout << "Symbol: " << root->data << ", Frequency: " << root->freq
         << ", Code: ";
    string temp1 = "";
    for (int i = 0; i < top; i++) {
      temp1 = temp1 + to_string(arr[i]);
    }
    cout << temp1 << endl;
  }
}

char findLetter(huffmantree *curr, string compressed, string code,
                char &letter) {
  if (code == compressed)
    letter = curr->data;
  if (curr != nullptr) {
    findLetter(curr->left, compressed, code + "0", letter);
    findLetter(curr->right, compressed, code + "1", letter);
  }
  return letter;
}
huffmantree *getRoot(vector<char> data, vector<int> freq, int size) {
  priority_queue<huffmantree *, vector<huffmantree *>, Compare> pq;
  for (int i = 0; i < size; i++) {
    huffmantree *newNode = new huffmantree(data[i], freq[i]);
    pq.push(newNode);
  }
  huffmantree *root = generateTree(pq);
  return root;
}
void HuffmanCodes(vector<char> data, vector<int> freq, int size) {
  priority_queue<huffmantree *, vector<huffmantree *>, Compare> pq;
  for (int i = 0; i < size; i++) {
    huffmantree *newNode = new huffmantree(data[i], freq[i]);
    pq.push(newNode);
  }
  huffmantree *root = generateTree(pq);
  int arr[MAX_SIZE], top = 0;
  // printCodes(root, arr, top);
}
#endif