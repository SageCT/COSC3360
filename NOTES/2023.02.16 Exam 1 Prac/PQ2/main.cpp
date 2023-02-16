#include <cmath>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

struct threadData {
  int size;
  int pos;
  int *vals;
  threadData(int s, int p, int *v) : size(s), pos(p), vals(v) {}
};

void *divide2(void *arg) {
  threadData *c = (threadData *)arg;
  int temp = c->vals[c->pos];
  c->vals[c->pos] = c->vals[c->size - 1 - c->pos];
  c->vals[c->size - 1 - c->pos] = temp;
  return nullptr;
}

int main() {
  int size;
  int *v;
  vector<threadData *> args;
  vector<pthread_t> threads;

  cin >> size;
  v = new int[size];
  for (int i = 0; i < size; i++) {
    int temp;
    cin >> temp;
    v[i] = temp;
  }

  for (int i = 0; i < floor(size / 2); i++) {
    pthread_t thread;
    threadData *arg = new threadData(size, i, v);

    args.push_back(arg);
    pthread_create(&thread, nullptr, divide2, arg);
    threads.push_back(thread);
  }

  for (int i = 0; i < threads.size(); i++) {
    pthread_join(threads[i], nullptr);
  }

  for (int i = 0; i < size; i++) {
    cout << v[i] << " ";
  }

  return 0;
}