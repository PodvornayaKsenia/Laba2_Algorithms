#ifndef ARRAY_H
#define ARRAY_H

#include <string>
#include <iostream>

using namespace std;

struct MyArray {
    string* data;
    int size;
    int capacity;

    MyArray(); 
};

void MInit(MyArray &arr, int initCapacity = 2);
void MPushBack(MyArray &arr, string val);
void MInsert(MyArray &arr, int index, string val);
string MGet(const MyArray &arr, int index);
void MRemove(MyArray &arr, int index);
void MReplace(MyArray &arr, int index, string val);
int MLength(const MyArray &arr);
void MPrint(const MyArray &arr);

#endif
