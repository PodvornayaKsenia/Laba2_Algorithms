#include "lru_task.h"

#include <iostream>
#include <string>

using namespace std;

LRUCache::LRUCache(int cap)
    : head(nullptr),
      tail(nullptr),
      capacity(cap > 0 ? cap : 0),
      size(0) {
}

LRUCache::~LRUCache() {
    while (head != nullptr) {
        LPopFront(head, tail);
    }
}

DNode* LRUCache::findNode(int key) const {
    string wantedKey = to_string(key);

    DNode* current = head;

    while (current != nullptr) {
        size_t separator = current->data.find(' ');

        string currentKey =
            current->data.substr(0, separator);

        if (currentKey == wantedKey) {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

void LRUCache::set(int key, int value) {
    if (capacity == 0) {
        return;
    }

    DNode* node = findNode(key);

    if (node != nullptr) {
        string oldData = node->data;

        LRemoveByValue(head, tail, oldData);

        LPushBack(
            head,
            tail,
            to_string(key) + " " + to_string(value)
        );

        return;
    }

    if (size == capacity) {
        LPopFront(head, tail);
        size--;
    }

    LPushBack(
        head,
        tail,
        to_string(key) + " " + to_string(value)
    );

    size++;
}

int LRUCache::get(int key) {
    DNode* node = findNode(key);

    if (node == nullptr) {
        return -1;
    }

    string data = node->data;
    size_t separator = data.find(' ');

    int value = stoi(data.substr(separator + 1));

    LRemoveByValue(head, tail, data);
    LPushBack(head, tail, data);

    return value;
}

void RunLRUTask() {
    int capacity;
    int Q;

    cout << "Введите емкость кэша и количество запросов:\n";

    if (!(cin >> capacity >> Q)) {
        return;
    }

    if (capacity < 0 || Q < 0) {
        cout << "Некорректные входные данные.\n";
        return;
    }

    LRUCache cache(capacity);

    cout << "Введите команды SET x y или GET x:\n";

    bool firstOutput = true;

    for (int i = 0; i < Q; i++) {
        string command;
        cin >> command;

        if (command == "SET") {
            int key;
            int value;

            if (!(cin >> key >> value)) {
                return;
            }

            cache.set(key, value);
        }
        else if (command == "GET") {
            int key;

            if (!(cin >> key)) {
                return;
            }

            int result = cache.get(key);

            if (!firstOutput) {
                cout << " ";
            }

            cout << result;
            firstOutput = false;
        }
        else {
            cout << "\nНеизвестная команда.\n";
            return;
        }
    }

    if (!firstOutput) {
        cout << "\n";
    }
}
