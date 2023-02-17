#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

// int main() {
//   //  std::cout << "I am the parent process" << std::endl;
//   //  std::cout << "I am child process " << /*Variable Identifier*/ <<
//   //  std::endl; std::cout << "I am a grandchild process from child process " <<
//   //  /*Variable Identifier*/ << std::endl;
//   return 0;
// }

/*
            FORK EXAMPLE CODE WE WROTE DURING THE PREVIOUS LECTURE
        YOU CAN USE THIS CODE TO WRITE YOUR SOLUTION FOR THIS QUESTION
*/
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int pid;
  std::cout << "I am the parent process " << std::endl;
  for (int i = 0; i < 4; i++) {
    pid = fork();
    if (pid == 0) {
      std::cout << "I am child process " << i << std::endl;
      for (int j = 0; j < i + 1; j++) {

        pid = fork();
        if (pid == 0) {
          std::cout << "I am a grandchild process from child process " << i
                    << std::endl;
          _exit(0);
        }
        wait(nullptr);
      }
      _exit(0);
    }
    wait(nullptr);
  }
  //  for (int i = 0; i < 3; i++)
  //      wait(nullptr);
  return 0;
}
