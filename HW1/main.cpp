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
  int counter = 0;
  map<int, vector<int>> codes;
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

  if (isdigit(in.at(0))) {
    toAdd = "";
    vector<int> temp;
    // Find the code in the string
    toAdd += in.substr(0, in.find(" ") - 1);
    // Erase the code from the input string
    in.erase(0, in.find(" ") + 1);

    // While the input string is not empty push back the digits until you find a
    // space or the end of the string
    while (in.size() > 0) {
      if (in.find(" ") == string::npos) {
        temp.push_back(stoi(in));
      } else if (in.find(" ") != string::npos)
        temp.push_back(stoi(in.substr(0, in.find(" ") - 1)));
    }
    temp.push_back(stoi());

    while (getline(cin, in)) {
    }
  }

  // huffmanTree tree(m);
  toAdd = "";
  while (counter > 0) {
    in.find(" ");

    counter--;
  }

  return 0;
}