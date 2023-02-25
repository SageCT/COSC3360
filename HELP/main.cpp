#include <fstream>
#include <iostream>
#include <map>
#include <pthread.h>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "huffmanTree.h"

using namespace std;

// extracts all numbers from a string and puts them into a vector
vector<string> getNumbersFromString(string line) {
  vector<string> v;
  stringstream sstream;
  sstream << line;

  string temp;
  string foundInt;
  while (!sstream.eof()) {
    sstream >> temp;
    if (stringstream(temp) >> foundInt)
      v.push_back(foundInt);
    temp = "";
  }
  return v;
}

// thread arguments struct
struct ThreadArgs {
  HuffmanNode *root;
  map<int, char> decodedMap;
  string line;
};

// function to decompress from huffmancode in a thread
void *decompress(void *threadArguments) {
  ThreadArgs *threadArg = (ThreadArgs *)threadArguments;

  vector<string> lineNumbers = getNumbersFromString(threadArg->line);
  char decodedChar =
      getCharacterFromHuffmanCode(threadArg->root, lineNumbers[0], -1);

  for (int i = 1; i < lineNumbers.size(); i++) {
    int pos = stoi(lineNumbers[i]);
    threadArg->decodedMap[pos] = decodedChar;
    // cout << '\t' << threadArg->decodedMap[pos] << endl;
  }

  return (NULL);
}

int main(int argc, char *argv[]) {
  string inputFileName;
  cin >> inputFileName;
  string compressedFileName;
  cin >> compressedFileName;

  int age = 0;
  priority_queue<HuffmanNode *, vector<HuffmanNode *>, huffmanComparator>
      huffmanQueue;
  int messageSize = 0;

  // read input file and store the frequencies of each character in the priority
  // queue
  ifstream inputFile(inputFileName);
  string inputFileLine;
  while (getline(inputFile, inputFileLine)) {
    char ch = inputFileLine[0];
    int freq = stoi(inputFileLine.substr(2, inputFileLine.size() - 1));
    messageSize += freq;

    huffmanQueue.push(createTreeNode(ch, freq, age, nullptr, nullptr));
    age += 1;
    huffmanQueue.top()->age = age;
  }
  inputFile.close();

  // build huffman tree
  HuffmanNode *huffmanTreeRoot = createHuffmanTree(huffmanQueue, age);
  printHuffmanTree(huffmanTreeRoot, "");

  // vector of threads
  vector<pthread_t> threads;
  static vector<ThreadArgs *> threadReturnVals;
  int numThreads = 0;

  // read from compressed file
  ifstream compressedFile(compressedFileName);
  string compFileLine;
  while (getline(compressedFile, compFileLine)) {
    numThreads += 1;
    ThreadArgs *funcArgs = new ThreadArgs();
    funcArgs->root = huffmanTreeRoot;
    funcArgs->line = compFileLine;

    pthread_t thread;
    if (pthread_create(&thread, NULL, decompress, (void *)funcArgs)) {
      cout << "could not make thread" << endl;
      return -1;
    }
    threads.push_back(thread);
    threadReturnVals.push_back(funcArgs);
  }
  compressedFile.close();
  map<int, char> finalMessage;

  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  for (auto vals : threadReturnVals) {
    for (auto tElm : vals->decodedMap) {
      finalMessage[tElm.first] = tElm.second;
    }
  }

  // output message
  cout << "Original message: ";
  for (const auto &elm : finalMessage)
    cout << elm.second;
  cout << endl;
}