#ifndef HUFFMAN_TASK_H
#define HUFFMAN_TASK_H

#include <string>
#include "array.h"

using namespace std;

struct HuffmanNode {
    unsigned char symbol;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
};

class HuffmanCoder {
private:
    HuffmanNode* root;
    string codes[256];

    void buildCodes(HuffmanNode* node, const string& code);
    void clearTree(HuffmanNode* node);

public:
    HuffmanCoder();
    ~HuffmanCoder();

    HuffmanCoder(const HuffmanCoder&) = delete;
    HuffmanCoder& operator=(const HuffmanCoder&) = delete;

    void buildTree(const string& text);
    void printCodes() const;

    string encode(const string& text) const;
    string decode(const string& bits) const;
};

void RunHuffmanTask();

#endif
