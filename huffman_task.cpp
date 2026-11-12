
#include "huffman_task.h"

#include <iostream>
#include <stdexcept>

using namespace std;

HuffmanCoder::HuffmanCoder() : root(nullptr) {}

HuffmanCoder::~HuffmanCoder() {
    clearTree(root);
}

void HuffmanCoder::clearTree(HuffmanNode* node) {
    if (node == nullptr) {
        return;
    }

    clearTree(node->left);
    clearTree(node->right);

    delete node;
}

void HuffmanCoder::buildCodes(HuffmanNode* node,
                              const string& code) {
    if (node == nullptr) {
        return;
    }

    if (node->left == nullptr &&
        node->right == nullptr) {

        codes[node->symbol] = code.empty() ? "0" : code;
        return;
    }

    buildCodes(node->left, code + "0");
    buildCodes(node->right, code + "1");
}

void HuffmanCoder::buildTree(const string& text) {
    clearTree(root);
    root = nullptr;

    for (int i = 0; i < 256; i++) {
        codes[i].clear();
    }

    MyArray frequencies;
    MInit(frequencies);

    for (int i = 0; i < 256; i++) {
        MPushBack(frequencies, "0");
    }

    for (unsigned char symbol : text) {
        int count = stoi(MGet(frequencies, symbol));
        frequencies.data[symbol] = to_string(count + 1);
    }

    HuffmanNode* nodes[512] = {};
    int nodeCount = 0;

    MyArray freeNodes;
    MInit(freeNodes);

    for (int i = 0; i < 256; i++) {
        int frequency = stoi(MGet(frequencies, i));

        if (frequency == 0) {
            continue;
        }

        nodes[nodeCount] = new HuffmanNode{
            static_cast<unsigned char>(i),
            frequency,
            nullptr,
            nullptr
        };

        MPushBack(freeNodes, to_string(nodeCount));
        nodeCount++;
    }

    while (MLength(freeNodes) > 1) {
        int firstPosition = 0;

        for (int i = 1; i < MLength(freeNodes); i++) {
            int currentIndex = stoi(MGet(freeNodes, i));
            int firstIndex = stoi(MGet(freeNodes, firstPosition));

            if (nodes[currentIndex]->frequency <
                nodes[firstIndex]->frequency) {
                firstPosition = i;
            }
        }

        int firstIndex = stoi(MGet(freeNodes, firstPosition));
        HuffmanNode* left = nodes[firstIndex];

        MRemove(freeNodes, firstPosition);

        int secondPosition = 0;

        for (int i = 1; i < MLength(freeNodes); i++) {
            int currentIndex = stoi(MGet(freeNodes, i));
            int secondIndex = stoi(MGet(freeNodes, secondPosition));

            if (nodes[currentIndex]->frequency <
                nodes[secondIndex]->frequency) {
                secondPosition = i;
            }
        }

        int secondIndex = stoi(MGet(freeNodes, secondPosition));
        HuffmanNode* right = nodes[secondIndex];

        MRemove(freeNodes, secondPosition);

        nodes[nodeCount] = new HuffmanNode{
            0,
            left->frequency + right->frequency,
            left,
            right
        };

        MPushBack(freeNodes, to_string(nodeCount));
        nodeCount++;
    }

    if (MLength(freeNodes) == 1) {
        int rootIndex = stoi(MGet(freeNodes, 0));
        root = nodes[rootIndex];
        buildCodes(root, "");
    }

    delete[] frequencies.data;
    delete[] freeNodes.data;
}

void HuffmanCoder::printCodes() const {
    for (int i = 0; i < 256; i++) {
        if (codes[i].empty()) {
            continue;
        }

        if (i == ' ') {
            cout << "[пробел]";
        }
        else if (i == '\t') {
            cout << "[табуляция]";
        }
        else if (i == '\n') {
            cout << "[перевод строки]";
        }
        else {
            cout << static_cast<char>(i);
        }

        cout << " : " << codes[i] << endl;
    }
}

string HuffmanCoder::encode(const string& text) const {
    string result;

    for (unsigned char symbol : text) {
        if (codes[symbol].empty()) {
            throw invalid_argument(
                "Символ отсутствует в таблице кодов"
            );
        }

        result += codes[symbol];
    }

    return result;
}

string HuffmanCoder::decode(const string& bits) const {
    string result;

    if (root == nullptr) {
        if (!bits.empty()) {
            throw invalid_argument("Дерево пустое");
        }

        return result;
    }

    if (root->left == nullptr &&
        root->right == nullptr) {

        for (char bit : bits) {
            if (bit != '0') {
                throw invalid_argument(
                    "Некорректный двоичный код"
                );
            }

            result += static_cast<char>(root->symbol);
        }

        return result;
    }

    HuffmanNode* current = root;

    for (char bit : bits) {
        if (bit == '0') {
            current = current->left;
        }
        else if (bit == '1') {
            current = current->right;
        }
        else {
            throw invalid_argument(
                "Код должен содержать только 0 и 1"
            );
        }

        if (current == nullptr) {
            throw invalid_argument(
                "Некорректный двоичный код"
            );
        }

        if (current->left == nullptr &&
            current->right == nullptr) {

            result += static_cast<char>(current->symbol);
            current = root;
        }
    }

    if (current != root) {
        throw invalid_argument(
            "Двоичный код обрывается"
        );
    }

    return result;
}

void RunHuffmanTask() {
    string text;

    cout << "Введите строку для кодирования:\n";
    getline(cin, text);

    HuffmanCoder coder;
    coder.buildTree(text);

    cout << "\nТаблица кодов:\n";
    coder.printCodes();

    string encoded = coder.encode(text);

    cout << "\nЗакодированная строка:\n";
    cout << encoded << endl;

    cout << "\nДекодированная строка:\n";
    cout << coder.decode(encoded) << endl;

    cout << "\nВведите двоичный код для декодирования "
            "(Enter — использовать полученный):\n";

    string bits;
    getline(cin, bits);

    if (bits.empty()) {
        bits = encoded;
    }

    try {
        cout << "Результат декодирования: "
             << coder.decode(bits) << endl;
    }
    catch (const invalid_argument& error) {
        cout << "Ошибка: " << error.what() << endl;
    }
}

