#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>

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
  bool operator()(shared_ptr<node> L, shared_ptr<node> R) {
    if (L->freq == R->freq) {
      if (L->data == R->data)
        return (L.get() < R.get());
      return L->data > R->data;
    }
    return L->freq > R->freq;
  }
};

int main() {
  ifstream file("../HW1/input2.txt");
  priority_queue<shared_ptr<node>, vector<shared_ptr<node>>, compare> pq;
  vector<shared_ptr<node>> nodes;

  for (string line; getline(file, line);)
    pq.push(
        make_shared<node>(node(line.substr(0, 1), stoi(line.substr(2, 1)))));

  std::sort(nodes.begin(), nodes.end(), compare());
  file.close();
  // cout << ("B" > "B") << endl;

  vector<shared_ptr<node>> n;
  // while (!pq.empty()) {
  //   n.push_back(make_shared<node>(pq.top()->data, pq.top()->freq));
  //   pq.pop();
  // }

  n.push_back(make_shared<node>("Second", 2));
  n.push_back(make_shared<node>("Third", 3));
  n.push_back(make_shared<node>("First", 1));

  //  while (auto i : n)
  //   cout << i->data << " " << i->freq << endl;

  while (!pq.empty()) {
    cout << pq.top()->data << " " << pq.top()->freq << endl;
    pq.pop();
  }
  return 0;
}