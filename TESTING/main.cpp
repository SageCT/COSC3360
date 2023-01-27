#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct node {
  string data;
  int priority;
  node() : data(""), priority(0) {}
  node(string data, int priority) : data(data), priority(priority) {}
};

class compare {
 public:
  bool operator()(string a, string b) { return a > b; }
  bool operator()(node *a, node *b) {
    if (a->priority == b->priority) {
      if (a->data == b->data) return a > b;
      return a->data > b->data;
    }
    return a->priority > b->priority;
  }
};

int main() {
  priority_queue<node *, vector<node *>, compare> pq;
  node *a = new node("A", 1);
  node *b = new node("B", 1);
  node *c = new node("C", 2);
  node *d = new node("D", 1);
  node *nul = new node("\0", 1);
  node *d2 = new node("D", 1);

  pq.push(a);
  pq.push(b);
  pq.push(c);
  pq.push(d);
  pq.push(nul);
  pq.push(d2);

  // cout << ("B" > "B") << endl;
  int count = 0;
  while (!pq.empty()) {
    if (pq.top()->data == "\0")
      cout << "NULL" << endl;
    else if (pq.top()->data == "D")
      cout << "D: " << count++ << endl;
    else
      cout << pq.top()->data << endl;
    pq.pop();
  }
  return 0;
}