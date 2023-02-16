#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <queue>
#include <string>
#include <vector>

using namespace std;
int checker(int *arr) { return arr[0]; }

int main() {
  int *arr = new int[10];
  arr[0] = 123;
  cout << checker(arr);

  return 0;
}