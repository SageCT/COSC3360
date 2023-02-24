#include <fstream>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <sstream>
#include <unistd.h>
#include <vector>

using namespace std;

// node structure for the Huffman tree
struct Node {
  char symbol;
  int frequency;
  Node *left;
  Node *right;

  Node(char s, int f)
      : symbol(s), frequency(f), left(nullptr), right(nullptr) {}
  Node(Node *l, Node *r)
      : symbol(0), frequency(l->frequency + r->frequency), left(l), right(r) {}
};

// comparator for priority queue
struct Compare {
  bool operator()(Node *l, Node *r) {
    if (l->frequency == r->frequency) {
      if (l->symbol == r->symbol) {
        return l < r;
      }
      return l->symbol > r->symbol;
    }
    return l->frequency > r->frequency;
  }
};

struct arguments {
  char alph;
  string symb;
  vector<int> pos;
  char *message;
  Node *root;
};

// function to build the Huffman tree
Node *buildHuffmanTree(vector<char> &symbols, vector<int> &frequencies) {
  // create min heap priority queue
  priority_queue<Node *, vector<Node *>, Compare> pq;

  // insert leaf nodes into priority queue
  for (size_t i = 0; i < symbols.size(); ++i) {
    pq.push(new Node(symbols[i], frequencies[i]));
  }

  // build the Huffman tree
  while (pq.size() > 1) {
    Node *l = pq.top();
    pq.pop();

    Node *r = pq.top();
    pq.pop();

    Node *parent = new Node(l, r);
    pq.push(parent);
  }

  return pq.top();
}

// function to print the Huffman codes for each symbol

char decodeHuffmanCodes(Node *root, string encodedString) {
  Node *currentNode = root;

  for (char c : encodedString) {
    if (c == '0') {
      currentNode = currentNode->left;
    } else {
      currentNode = currentNode->right;
    }

    if (currentNode->left == nullptr && currentNode->right == nullptr) {
      return currentNode->symbol;
      currentNode = root;
    }
  }
}
void printHuffmanCodes(Node *root, string code = "") {
  if (root == nullptr) {
    return;
  }

  if (root->symbol != 0) {
    cout << "Symbol: " << root->symbol << ", Frequency: " << root->frequency
         << ", Code: " << code << endl;
  }

  printHuffmanCodes(root->left, code + "0");
  printHuffmanCodes(root->right, code + "1");
}

void *decode(void *void_ptr) {
  arguments *arg_ptr = (arguments *)void_ptr;

  decodeHuffmanCodes(arg_ptr->root, arg_ptr->symb);
  for (int i = 0; i < arg_ptr->pos.size(); i++) {
    int index = arg_ptr->pos[i];
    arg_ptr->alph = decodeHuffmanCodes(arg_ptr->root, arg_ptr->symb);
    arg_ptr->message[index] = arg_ptr->alph;
  }
  return nullptr;
}
int main() {
  // input symbols and their frequencies
  // vector<char> symbols = {'0', '2', '3', '6', ' ', 'C', 'O', 'S', 'P', 'R',
  // 'I', 'N' ,'G'}; vector<int> frequencies = {2,2,3,1,3,2,1,2,1,1,1,1,1};

  vector<char> symbols;
  vector<int> frequencies;

  vector<string> code;
  vector<int> positions;

  // for(int i = 0; i < symbols.size(); ++i){
  //     cout << symbols[i] << " " << frequencies[i] << endl;
  // }
  // string filename;
  // cin >> filename;
  int totalFreq = 0;
  ifstream input("input.txt");

  string line;
  while (getline(input, line)) {
    char ch;
    int freq;

    if (line[0] == ' ') {
      ch = ' ';
      freq = stoi(line.substr(1));
      totalFreq += freq;

    } else {
      ch = line[0];
      freq = stoi(line.substr(2));
      totalFreq += freq;
    }

    symbols.push_back(ch);
    frequencies.push_back(freq);
  }

  input.close();

  // build Huffman tree
  Node *root = buildHuffmanTree(symbols, frequencies);

  // print Huffman codes
  printHuffmanCodes(root);

  int nthreads = symbols.size();
  pthread_t *tid = new pthread_t[nthreads];
  arguments *arg = new arguments[nthreads];
  char *message = new char[totalFreq];

  ifstream comparfile("STDIN.txt");

  string line1;
  for (int i = 0; i < nthreads; i++) {
    arg[i].root = root;
    while (getline(comparfile, line1)) {
      string co;
      int pos;
      int wspace = 0;
      for (int i = 0; i < line1.size(); ++i) {
        if (line1[i] == ' ') {
          wspace++;
        }
      }
      istringstream iss(line1);

      iss >> co;
      arg[i].symb = co;
      while (wspace != 0) {
        iss >> pos;
        arg[i].pos.push_back(pos);
        wspace--;
      }
    }
  }

  comparfile.close();

  /*for(int i = 0; i < code.size(); ++i){

      cout << code[i] << endl;
  }*/
  for (int i = 0; i < nthreads; i++) {
    arg[i].message = message;
    if (pthread_create(&tid[i], nullptr, decode, &arg[i])) {
      fprintf(stderr, "Error creating thread\n");
      return 1;
    }
  }
  for (int i = 0; i < nthreads; i++) {
    pthread_join(tid[i], nullptr);
  }
  cout << "Original message: ";
  for (int i = 0; i < totalFreq; i++) {
    std::cout << message[i];
  }
  std::cout << std::endl;

  delete[] tid;
  delete[] arg;
  delete[] message;

  return 0;
}