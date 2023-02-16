#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

struct threadData {
  int ASCIIval;
  char *letter;
  threadData(int a, char *c) : ASCIIval(a), letter(c) {}
};

void *printChar(void *arg) {}

int main() {
  string s = "COSC3360";
  vector<char *> v;
  vector<pthread_t> threads;
  for (auto i = 0; i < s.length(); i++) {
    v.push_back(&s[i]);
  }

  for (auto i = 0; i < v.size(); i++) {
    pthread_t thread;
    threadData *data = new threadData(0, v[i]);
    threads.push_back(thread);
    pthread_create(&threads[i], NULL, printChar, v[i]);
  }

  cout << "The word \"" << s
       << "\" is represented with the following ASCII values:" << endl;
       for(auto i = 0; i < v.size(); i++){
           cout << v[i] << " ";
       }

  
  return 0;
}