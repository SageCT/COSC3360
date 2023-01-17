#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

struct node {
  char data;
  int freq;
  node *left;
  node *right;
};


class huffmanTree {
 public:
  huffmanTree();
  ~huffmanTree();
};

huffmanTree::huffmanTree(){

}

#endif