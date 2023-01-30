#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "huffmanTree.h"

using namespace std;

int main() {
  string in, toAdd;
  int counter = 0;
  // struct with the codes and their positions in a vector of ints
  vector<code *> codes;
  // vector with the characters and their frequencies
  vector<node *> freq;

  string inputFileName, compressedFileName;
  cin >> inputFileName >> compressedFileName;
  ifstream inputFile(inputFileName), compressedFile(compressedFileName);

  // Gets line from inputFile, and checks if the first character is a digit,
  // if not a digit adds to a vector of nodes
  while (getline(inputFile, in)) {
    string temp = "";
    temp += in.at(2);

    // Make node and push to vector
    node *newNode = new node(in.substr(0, 1), stoi(temp));
    freq.push_back(newNode), counter++;
  }
  sort(freq.begin(), freq.end(), huffmanCompare());

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
      code *newCode = new code(toAdd, temp);

      codes.push_back(newCode);

      counter--;
      if (counter == 0)
        break;
    }
  }

  compressedFile.close();

  // Create and print the Huffman Tree
  huffmanTree tree(freq);
  tree.decode(codes);
  // tree.print();
  return 0;
}