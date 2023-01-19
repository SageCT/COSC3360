#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "huffmanTree.h"

using namespace std;

int main() {
  string in, toAdd;
  int counter = 0;
  // map with the huffman codes and their positions in the encoded string
  map<int, vector<int>> codes;
  // map with the characters and their frequencies
  map<char, int> m;

  cin >> in;
  cin >> in;
  in.clear();

  // Gets line from STDIN, and checks if the first character is a digit,
  // if not a digit adds to map
  getline(cin, in);
  while (isalpha(in.at(0))) {
    m.insert(pair<char, int>(in.at(0), in.at(3))), counter++;

    getline(cin, in);
  }
  while (getline(cin, in)) {
    if (isdigit(in.at(0))) {
      toAdd = "";
      vector<int> temp;
      // Find the code in the string
      toAdd += in.substr(0, in.find(" ") - 1);

      // Erase the code from the input string
      in.erase(0, in.find(" ") + 1);

      stringstream ss(in);
      int i;

      while (ss >> i) temp.push_back(i);

      codes.insert(pair<int, vector<int>>(stoi(toAdd), temp));
    }
  }

  // huffmanTree tree(m);
  // tree.printTree();

  return 0;
}