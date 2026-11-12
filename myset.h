#ifndef MYSET_H
#define MYSET_H

#include "single_list.h"
#include <string>

using namespace std;

struct MySet {
    Node** buckets;
    int capacity;
    int size;

    MySet();
    ~MySet();

    MySet(const MySet&) = delete;
    MySet& operator=(const MySet&) = delete;
};

int SetHash(const string& value, int capacity);

bool SET_AT(const MySet& set, const string& value);
bool SETADD(MySet& set, const string& value);
bool SETDEL(MySet& set, const string& value);

void SetResize(MySet& set);
void SETPrint(const MySet& set);

#endif
