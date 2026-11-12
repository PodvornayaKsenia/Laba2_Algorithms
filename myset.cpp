
#include "myset.h"
#include <iostream>
#include <cstdint>

using namespace std;

MySet::MySet() {
    capacity = 16;
    size = 0;

    buckets = new Node*[capacity];

    for (int i = 0; i < capacity; i++) {
        buckets[i] = nullptr;
    }
}

MySet::~MySet() {
    for (int i = 0; i < capacity; i++) {
        while (buckets[i] != nullptr) {
            popFront(buckets[i]);
        }
    }

    delete[] buckets;
}

int SetHash(const string& value, int capacity) {
    uint64_t hash = 0;

    for (unsigned char c : value) {
        hash = hash * 31 + c;
    }

    return static_cast<int>(hash % capacity);
}

bool SET_AT(const MySet& set, const string& value) {
    int index = SetHash(value, set.capacity);

    return search(set.buckets[index], value) != nullptr;
}

void SetResize(MySet& set) {
    int newCapacity = set.capacity * 2;

    Node** newBuckets = new Node*[newCapacity];

    for (int i = 0; i < newCapacity; i++) {
        newBuckets[i] = nullptr;
    }

    for (int i = 0; i < set.capacity; i++) {
        Node* current = set.buckets[i];

        while (current != nullptr) {
            Node* next = current->next;

            int index = SetHash(current->data, newCapacity);

            current->next = newBuckets[index];
            newBuckets[index] = current;

            current = next;
        }
    }

    delete[] set.buckets;

    set.buckets = newBuckets;
    set.capacity = newCapacity;
}

bool SETADD(MySet& set, const string& value) {
    if (SET_AT(set, value)) {
        return false;
    }

    // Расширяем таблицу при заполнении более 75%
    if ((set.size + 1) * 4 > set.capacity * 3) {
        SetResize(set);
    }

    int index = SetHash(value, set.capacity);

    pushFront(set.buckets[index], value);
    set.size++;

    return true;
}

bool SETDEL(MySet& set, const string& value) {
    if (!SET_AT(set, value)) {
        return false;
    }

    int index = SetHash(value, set.capacity);

    removeByValue(set.buckets[index], value);
    set.size--;

    return true;
}

void SETPrint(const MySet& set) {
    cout << "{";

    bool first = true;

    for (int i = 0; i < set.capacity; i++) {
        Node* current = set.buckets[i];

        while (current != nullptr) {
            if (!first) {
                cout << ", ";
            }

            cout << current->data;

            first = false;
            current = current->next;
        }
    }

    cout << "}" << endl;
}
