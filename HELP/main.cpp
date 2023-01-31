#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

bool found(int r, int c, char **wordSoup, bool **visited) {
  if (r < 0 || c < 0 || r >= sizeof(wordSoup) || c >= sizeof(wordSoup) ||
      visited[r][c] == true) {
    return false; // base case
  }
  visited[r][c] = true; // mark visited
  bool foundSequence = found(r - 1, c, wordSoup, visited) ||
                       found(r + 1, c, wordSoup, visited) ||
                       found(r, c - 1, wordSoup, visited) ||
                       found(r, c + 1, wordSoup, visited); // recursion
  visited[r][c] = false;
  return foundSequence;
}

int main(int argc, char *argv[]) {
  ifstream input("input1.txt");
  ofstream out("output1.txt");

  string in = "";
  string m = "";
  string n = "";
  string word = "";
  int counter = 0;
  bool isPossible = true;

  if (input.peek() != EOF) {
    input >> m;
    input >> n;
    int rows = stoi(m);
    int columns = stoi(n);
    char **wordSoup;
    bool **visited;
    for (int x = 0; x < rows; x++) {
      for (int y = 0; y < columns; y++) {
        input >> wordSoup[x][y];
      }
    }
    input >> word;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        if (wordSoup[i][j] == word[counter] && found(i, j, wordSoup, visited)) {
          isPossible = true;
          counter++;
          if (counter >= word[-1]) {
            break;
          }
        } else {
          isPossible = false;
        }
      }
    }
  }
  if (isPossible == true) {
    out << "true";
  } else {
    out << "false";
  }
  return 0;
}