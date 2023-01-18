#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "huffmanTree.h"

using namespace std;

int main() {
  string in;
  char c;
  int n;
  map<char, int> m;

  getline(cin, in);
  if (isalpha(in.at(0)))
    c = in.at(0);
  else
    c = in.at(1);

  m[c] = n;

  return 0;
}