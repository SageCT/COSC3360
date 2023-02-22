#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

struct threadData {
  int pos;
  string *octalNum;

  threadData(int p, string *oN) : pos(p), octalNum(oN) {}
};

void *octalBinary(void *args) {
  threadData *data = (threadData *)args;
  char toConvert = data->octalNum->at(data->pos);

  return nullptr;
}

int main() {
  int n;
  stack<char> s;
  vector<pthread_t> threads;
  vector<threadData> *args;

  cin >> n;
  int *vals = new int[n];
  for (int i = 0; i < n; n++)
    cin >> vals[i];

  for (int i = 0; i < n; i++) {
  }

  return 0;
}