#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <queue>
#include <string>
using namespace std;

/*
 * based on pseudocode from: https://www.programiz.com/dsa/huffman-coding
 * tree node allocation and construction loosely modfied from:
 * https://gist.github.com/pwxcoo/72d7d3c5c3698371c21e486722f9b34b
 */

// node of a huffman tree, containing character, frequency and two childeren
// memory is freed in destructor, tree will be delete when objects are destroyed
class HuffmanNode {
public:
  char character;
  int frequency;
  int age;

  HuffmanNode *left;
  HuffmanNode *right;

  ~HuffmanNode();
};

// structure to customize the ordering of the items in the priority queue
// code from:
// https://www.geeksforgeeks.org/priority-queue-of-pairs-in-c-with-ordering-by-first-and-second-element/
// and:
// https://stackoverflow.com/questions/45097171/the-body-of-constexpr-function-not-a-return-statement
struct huffmanComparator {
  bool operator()(HuffmanNode *a, HuffmanNode *b) {
    return (a->frequency != b->frequency)   ? (a->frequency > b->frequency)
           : (a->character == b->character) ? (a->age < b->age)
                                            : (a->character > b->character);
  }
};

HuffmanNode *createTreeNode(char nodeCh, int nodeFreq, int nodeAge,
                            HuffmanNode *leftChild, HuffmanNode *rightChild);
HuffmanNode *createHuffmanTree(
    priority_queue<HuffmanNode *, vector<HuffmanNode *>, huffmanComparator> pq,
    int currAge);
void printHuffmanTree(HuffmanNode *root, string code);
char getCharacterFromHuffmanCode(HuffmanNode *root, string code, int codeIndex);

#endif