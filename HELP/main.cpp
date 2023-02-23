#include <fstream>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <string>
#include <unordered_map>
using namespace std;

struct ThreadInfo {
  std::unordered_map<std::string, char> huffman_tree; // The Huffman tree
  std::string binary_code;    // The binary code for the symbol to decompress
  std::vector<int> positions; // The positions of the symbol to decompress
  char *output;               // Pointer to the output memory location
};

// Define the function that each child thread will execute
void *decompress_symbol(void *arg) {
  // Cast the void* argument to a ThreadInfo pointer
  ThreadInfo *thread_info = static_cast<ThreadInfo *>(arg);

  // Use the Huffman tree to determine the character from the binary code
  char symbol = thread_info->huffman_tree[thread_info->binary_code];

  // Store the decompressed character in the output memory location
  for (int pos : thread_info->positions) {
    thread_info->output[pos] = symbol;
  }

  // Clean up the thread_info struct (optional)
  delete thread_info;

  return nullptr;
}

// A Tree node
struct Node {
  char ch;
  int freq;
  Node *left, *right;
  // Constructor for leaf nodes
  Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}

  // Constructor for internal nodes
  Node(int f) : ch('\0'), freq(f), left(nullptr), right(nullptr) {}
};

// Function to allocate a new tree node
/*Node* getNode(char ch, int freq, Node* left, Node* right)
{
        Node* node = new Node();

        node->ch = ch;
        node->freq = freq;
        node->left = left;
        node->right = right;

        return node;
}*/

// Comparison object to be used to order the heap
struct comp {
  bool operator()(Node *l, Node *r) {
    // {
    // 	// highest priority item has lowest freq
    // 	if(l->freq == r->freq){
    //         return l->ch < r->ch;
    //     }

    //     return l->freq > r->freq;
    // }
    if (l->freq == r->freq) {
      if (l->ch == r->ch) {
        return l < r;
      }
      return l->ch > r->ch;
    }

    return l->freq > r->freq;
  }
};

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encode(Node *root, string str, unordered_map<char, string> &huffmanCode) {
  if (root == nullptr)
    return;

  // found a leaf node
  if (!root->left && !root->right) {
    huffmanCode[root->ch] = str;
  }

  /// idkn not sure left to right but it wont print right
  encode(root->right, str + "1", huffmanCode);
  encode(root->left, str + "0", huffmanCode);
  // encode(root->right, str + "1", huffmanCode);
}

// traverse the Huffman Tree and decode the encoded string
void decode(Node *root, int &index, string str) {
  if (root == nullptr) {
    return;
  }

  // found a leaf node
  if (!root->left && !root->right) {
    cout << root->ch;
    return;
  }

  index++;

  if (str[index] == '0')
    decode(root->left, index, str);
  else
    decode(root->right, index, str);
}

// Builds Huffman Tree and decode given input text
Node *buildHuffmanTree(string &text) {
  // count freq of appearance of each character
  // and store it in a map
  ifstream input(text);
  if (!input.is_open()) {
    cerr << "Error opening file " << text << endl;
    return nullptr;
  }
  unordered_map<char, int> freq;
  // char ch;
  // int fre;

  string line;
  /*while (input >> ch >> fre && (!input.eof())) {


              string line;
              if(!input.eof()){
                      getline(input, line);
                      if(line[0] == ' ')
                              ch = ' ';
                              freq[ch] = fre;
              }
              cout << ch <<endl;

      freq[ch] = fre;
  }*/
  // vector <int> pos_forinput;
  while (getline(input, line)) {
    char ch;
    int fre;

    int count_wspace = 0;
    if (line[0] == ' ') {
      ch = ' ';
      string free;
      free = line.substr(1, line.length() - 1);
      cout << free << endl;
      fre = stoi(free);
      cout << "freq" << fre << endl;
      freq[ch] = fre;
    } else {
      ch = line[0];
      string free;
      free = line.substr(2, line.length() - 1);
      cout << free << "done" << endl;
      cout << free[0] << endl;
      cout << free[1] << endl;
      cout << free[2] << endl;
      fre = stoi(free);
      cout << "freq" << fre << endl;
      freq[ch] = fre;
    }
  }

  cout << "ben" << endl;
  input.close();

  // Create a priority queue to store live nodes of
  // Huffman tree;
  priority_queue<Node *, vector<Node *>, comp> pq;

  // Create a leaf node for each character and add it
  // to the priority queue.
  for (auto pair : freq) {
    // pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    Node *node = new Node(pair.first, pair.second);
    pq.push(node);
  }

  // do till there is more than one node in the queue
  while (pq.size() > 1) {
    // Remove the two nodes of highest priority
    // (lowest freq) from the queue
    Node *left = pq.top();
    pq.pop();
    Node *right = pq.top();
    pq.pop();

    Node *parent = new Node(left->freq + right->freq);
    parent->left = left;
    parent->right = right;
    pq.push(parent);
    // Create a new internal node with these two nodes
    // as children and with freq equal to the sum
    // of the two nodes' frequencies. Add the new node
    // to the priority queue.
    // int sum = left->freq + right->freq;
    // pq.push(getNode('\0', sum, left, right));
  }

  // root stores pointer to root of Huffman Tree
  Node *root = pq.top();

  // traverse the Huffman Tree and store Huffman Codes
  // in a map. Also prints them
  unordered_map<char, string> huffmanCode;
  encode(root, "", huffmanCode);

  cout << "Huffman Codes are :\n" << '\n';
  for (auto pair : huffmanCode) {
    cout << "Symbol: " << pair.first << ", Frequency: " << freq[pair.first]
         << ", Code: " << pair.second << endl;
  }

  /*cout << "\nOriginal string was :\n" << text << '\n';

  // print encoded string
  string str = "";
  for (char ch: text) {
          str += huffmanCode[ch];
  }

  cout << "\nEncoded string is :\n" << str << '\n';

  // traverse the Huffman Tree again and this time
  // decode the encoded string
  int index = -1;
  cout << "\nDecoded string is: \n";
  while (index < (int)str.size() - 2) {
          decode(root, index, str);
  }*/
  return 0;
}

// Huffman coding algorithm
int main() {
  // string text = "Huffman coding is a data compression algorithm.";

  string text = "input.txt";
  Node *root = buildHuffmanTree(text);
  // ifstream compressed_file("STDIN.txt");
  // vector<ThreadInfo> symbol_infos;
  // while (!compressed_file.eof()) {
  // 	string bincode;
  // 	compressed_file >> bincode;

  // }

  return 0;
}

/*vector <int> pos_forinput;
        while (getline(input, line)) {
        {
                int count_wspace = 0;
                if(line[0] == ' ')
                                ch = ' ';
                                for(int i =1; i < line.length(); i++){
                                        if(line[i] == ' '){
                                                count_wspace++;
                                                pos_forinput.push_back(i);
                                        }
                                }
                                freq[ch] = fre;
                }*/

/*void huffmanTree::decode(vector<shared_ptr<code>> &c, bool threaded) {
if (threaded) {
int max = 0;

for (int i = 0; i < c.size(); i++)
for (int j = 0; j < c.at(i)->pos.size(); j++)
if (max < c.at(i)->pos.at(j))
max = c.at(i)->pos.at(j);

string result = "";
static vector<pthread_t> threads;
shared_ptr<vector<char>> message(
make_shared<vector<char>>(vector<char>(max + 1)));

for (auto i : c) {
threadData *arg = new threadData(root, i, max + 1, message);
pthread_t thread;
pthread_create(&thread, nullptr, decodethread, arg);
threads.push_back(thread);
}

for (auto &i : threads)
pthread_join(i, nullptr);
for (int i = 0; i < message->size(); i++)
result += message->at(i);

decodedMessage = result;
}
}*/