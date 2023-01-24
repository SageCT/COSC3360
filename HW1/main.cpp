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

  getline(cin, in);
  getline(cin, in);
  in.clear();

  // Gets line from STDIN, and checks if the first character is a digit,
  // if not a digit adds to map
  getline(cin, in);
  while (isalpha(in.at(0))) {
    string temp = "";
    temp += in.at(2);

    // Make node and push to vector
    node *newNode = new node(in.substr(0, 1), stoi(temp));
    freq.push_back(newNode), counter++;
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
    code *newCode = new code(stoi(toAdd), temp);

    codes.push_back(newCode);

    counter--;
    if (counter == 0)
      break;
    getline(cin, in);
  }

  cout << "Letters and their frequencies: " << endl;
  for (int i = 0; i < freq.size(); i++)
    cout << freq.at(i)->data << " " << freq.at(i)->freq << endl;

  cout << "Huffman codes and their positions: " << endl;
  for (int i = 0; i < codes.size(); i++) {
    cout << codes.at(i)->data << " ";
    for (int j = 0; j < codes.at(i)->pos.size(); j++)
      cout << codes.at(i)->pos.at(j) << " ";
    cout << endl;
  }

  huffmanTree tree(freq);
  tree.decode(codes);
  // tree.printTree();

  return 0;
}