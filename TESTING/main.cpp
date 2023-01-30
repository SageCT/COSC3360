#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct node {
  string data;
  int freq;
  node *left;
  node *right;
  node() : data(""), freq(0), left(nullptr), right(nullptr) {}
  node(string data, int freq)
      : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

class compare {
public:
  bool operator()(string a, string b) { return a > b; }
  bool operator()(node *L, node *R) {
    if (L->freq == R->freq) {
      if (L->data == R->data)
        return (L > R);
      return L->data > R->data;
    }
    return L->freq > R->freq;
  }
};

int main() {
  ifstream file("../HW1/input2.txt");
  priority_queue<node *, vector<node *>, compare> pq;
  vector<node *> nodes;

  for (string line; getline(file, line);)
    nodes.push_back(new node(line.substr(0, 1), stoi(line.substr(2, 1))));

  std::sort(nodes.begin(), nodes.end(), compare());
  file.close();
  // cout << ("B" > "B") << endl;

  while (!nodes.empty()) {
    pq.push(nodes[0]);
    nodes.erase(nodes.begin());
  }

  while (!pq.empty()) {
    cout << pq.top()->data << " " << pq.top()->freq << endl;
    pq.pop();
  }
  return 0;
}