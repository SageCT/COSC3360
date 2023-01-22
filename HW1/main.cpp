#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "huffmanTree.h"

using namespace std;

int main() {
  string in, toAdd;
  int counter = 0;
  // map with the huffman codes and their positions in the encoded string
  map<int, vector<int>> codes;
  // vector with the characters and their frequencies
  vector<pair<int, char>> freq;

  getline(cin, in);
  getline(cin, in);
  in.clear();

  // Gets line from STDIN, and checks if the first character is a digit,
  // if not a digit adds to map
  getline(cin, in);
  while (isalpha(in.at(0))) {
    string temp = "";
    temp += in.at(2);
    freq.push_back(make_pair(stoi(temp), in.at(0))), counter++;
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

    while (ss >> i)
      temp.push_back(i);
    codes.insert(pair<int, vector<int>>(stoi(toAdd), temp));
    counter--;
    if (counter == 0)
      break;
    getline(cin, in);
  }

  cout << "Letters and their frequencies: " << endl;
  for (auto it = freq.begin(); it != freq.end(); ++it) {
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

  huffmanTree tree(freq);
  // tree.printTree();

  return 0;
}