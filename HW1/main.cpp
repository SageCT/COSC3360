#include <sstream>

#include "huffmanTree.h"

using namespace std;

int main() {
  string in, toAdd;
  int counter = 0;
  // struct with the codes and their positions in a vector of ints
  vector<shared_ptr<code>> codes;
  // vector with the characters and their frequencies
  vector<shared_ptr<node>> freq;

  string inputFileName, compressedFileName;
  cin >> inputFileName >> compressedFileName;

  ifstream inputFile(inputFileName), compressedFile(compressedFileName);

  // Gets line from inputFile, and checks if the first character is a digit,
  // if not a digit adds to a vector of nodes
  while (getline(inputFile, in)) {
    string temp = "";
    temp += in.at(2);

    // Make node and push to vector
    shared_ptr<node> newNode(
        make_shared<node>(node(in.substr(0, 1), stoi(temp))));
    freq.push_back(newNode), counter++;
  }
  sort(freq.begin(), freq.end(), huffmanCompare());
  reverse(freq.begin(), freq.end());

  inputFile.close();

  while (getline(compressedFile, in)) {
    if (counter > 0 && isdigit(in.at(0))) {
      toAdd = "";
      vector<int> temp;
      // Find the code in the string
      toAdd += in.substr(0, in.find(" "));

      // Erase the code from the input string
      in.erase(0, in.find(" ") + 1);

      stringstream ss(in);
      int i;

      while (ss >> i)
        temp.push_back(i);
      shared_ptr<code> newCode(make_shared<code>(code(toAdd, temp)));
      codes.push_back(newCode);

      counter--;
      if (counter == 0)
        break;
    }
  }

  compressedFile.close();

  // Create and print the Huffman Tree
  huffmanTree tree(freq);
  tree.decode(codes, true);
  tree.print();
  return 0;
}

string *decodethread(void *ptr) {
  // Change the void pointer to a shared_ptr<code> pointer
  threadData *c = (threadData *)ptr;
  string result(c->numChars, '*');

  for (int i = 0; i < c->codeVal->pos.size(); i++) {

    shared_ptr<node> cu(c->root);
    string currCode = c->codeVal->data;

    for (char curr : currCode) {
      // If current char is 0, go left
      // If current char is 1, go right
      curr == '0' ? cu = cu->left : cu = cu->right;
    }
    // Once you get the char from the decode, set the data at the given position
    // in the result string
    for (int position : c->codeVal->pos) {
      result.at(position) = cu->data.at(0);
    }
  }
  return &result;
}