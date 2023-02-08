// Forking and pid stuff
#include <iostream>
#include <memory>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main() {

  cout << "I am the parent process" << endl;
  int pid;

  // for (int x = 0; x < 4; x++) {
  //   pid = fork();
  //   if (pid == 0) {
  //     cout << "I am the child process " << x << endl;
  //     if (x == 1 || x == 2) {
  //       pid = fork();
  //       if (pid == 0) {
  //         cout << "I am the grandchild process from child process " << x
  //              << endl;
  //         _exit(0); // Exit the grandchild process
  //       }
  //       wait(nullptr); // Wait for the grandchild processes to finish
  //     }
  //     _exit(0); // Exit the child process
  //   }
  //   wait(nullptr);  // Wait for the child processes to finish
  // }

  for (int x = 0; x < 4; x++) {
    pid = fork();
    if (pid == 0) {
      cout << "I am the child process " << x << endl;
      if (x == 1 || x == 2) {
        pid = fork();
        if (pid == 0) {
          cout << "I am the grandchild process from child process " << x
               << endl;
          _exit(0); // Exit the grandchild process
        }
        wait(nullptr); // Wait for the grandchild processes to finish
      }
      _exit(0); // Exit the child process
    }
    wait(nullptr); // Wait for the child processes to finish
  }
  
  /*
    If greater than 0, the return value is the process ID of the child process
    If less than 0, the fork failed
    If 0, the return value is 0 for the child process
  */

  /*
 i am the parent process
 i am the child process 0
 i am the child process 1
 i am the child process 2
 i am the grand child process from child process 0
 i am the grand child process from child process 1
 i am the grand child process from child process 2
  */

  return 0;
}