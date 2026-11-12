#ifndef HASH_TASK_H
#define HASH_TASK_H

#include <string>
#include <cstdint>
#include "array.h"

using namespace std;

class BloomFilter {
private:
    MyArray bits;
    int bitCount;

    uint64_t hash1(const string& key) const;
    uint64_t hash2(const string& key) const;

public:
    BloomFilter(int size = 8192);
    ~BloomFilter();

    BloomFilter(const BloomFilter&) = delete;
    BloomFilter& operator=(const BloomFilter&) = delete;

    void add(const string& key);
    bool possiblyContains(const string& key) const;
};

class HashTable {
private:
    MyArray keys;
    MyArray values;
    MyArray states;
    BloomFilter filter;

    int capacity;
    int count;
    bool doubleHashing;
    mutable uint64_t probes;

    uint64_t hash1(const string& key) const;
    uint64_t hash2(const string& key) const;
    int position(const string& key, int attempt) const;
    int findIndex(const string& key) const;

public:
    HashTable(bool useDoubleHashing, int size = 1009);
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool insert(const string& key, const string& value);
    bool get(const string& key, string& value) const;
    bool contains(const string& key) const;
    bool containsWithoutBloom(const string& key) const;
    bool remove(const string& key);
    bool bloomCheck(const string& key) const;

    int getCount() const;
    int getCapacity() const;
    uint64_t getProbes() const;
    void resetProbes();
    void print() const;
};

void RunHashTask();

#endif
