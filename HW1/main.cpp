#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "huffmanTree.h"

using namespace std;

int main() {
  string in, toAdd;
  map<char, vector<char>> codes;
  int counter = 0;
  map<int, char> m;

  // Gets line from STDIN, and checks if the first character is a digit
  getline(cin, in);
  while (!isdigit(in.at(0))) {
    if (isalpha(in.at(0)))
      m.insert(pair<int, char>(in.at(0), in.at(3))), counter++;

    in.clear();
    getline(cin, in);
  }

  // huffmanTree tree(m);
  toAdd = "";
  while (counter > 0) {
    if (isdigit(in.at(0))) {
      toAdd += in.at(0);
      in.erase(0, 1);
      while (isdigit(in.at(0))) {
        toAdd += in.at(0);
        in.erase(0, 1);
      }
    }
    in.erase(0, 1);

    counter--;
  }

  return 0;
}