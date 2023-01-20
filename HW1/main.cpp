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

  getline(cin, in);
  getline(cin, in);
  in.clear();

  // Gets line from STDIN, and checks if the first character is a digit,
  // if not a digit adds to map
  getline(cin, in);
  while (isalpha(in.at(0))) {
    string temp = "";
    temp += in.at(2);
    m.insert(pair<char, int>(in.at(0), stoi(temp))), counter++;
    getline(cin, in);
  }
  while (counter > 0 && isdigit(in.at(0))) {
    toAdd = "";
    vector<int> temp;
    // Find the code in the string
    toAdd += in.substr(0, in.find(" "));

    // Erase the code from the input string
    in.erase(0, in.find(" ") + 1);

    stringstream ss(in);
    int i;

    while (ss >> i) temp.push_back(i);
    codes.insert(pair<int, vector<int>>(stoi(toAdd), temp));
    counter--;
    getline(cin, in);
  }

  cout << "Letters and their frequencies: " << endl;
  for (auto it = m.begin(); it != m.end(); ++it) {
    cout << it->first << " " << it->second << endl;
  }

  cout << "Huffman codes and their positions: " << endl;
  for (auto it = codes.begin(); it != codes.end(); ++it) {
    cout << it->first << " ";
    for (int i = 0; i < it->second.size(); i++) {
      cout << it->second.at(i) << " ";
    }
    cout << endl;
  }

  // huffmanTree tree(m);
  // tree.printTree();

  return 0;
}