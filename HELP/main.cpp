// Write your program here

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <fstream>
#include "huffmanTree.h"

using namespace std;

// Huffman coding algorithm
int main(int argc, char *argv[])
{
    // string text = "Huffman coding is a data compression algorithm.";

    // buildHuffmanTree(text);

    string file;
    string compressed_file;
    ifstream ifs("input1.txt");
    ifstream cfs;

    // cin >> file >> compressed_file;

    // ifs.open("input1.txt");
    cfs.open("compressed_file1.txt");

    string line;
    string letter;
    string frequency;

    while (getline(ifs, line))
    {
        letter = line.substr(0, line.find(" "));
        frequency = line.substr(line.find(" ") + 1);
        // cout << line << endl;
        cout << letter << endl;
        cout << frequency << endl;
    }

    string comp_file;

    while (getline(cfs, comp_file))
    {
        cout << comp_file << endl;
    }

    // buildHuffmanTree(letter);

    // Close the file
    ifs.close();
    cfs.close();

    return 0;
}