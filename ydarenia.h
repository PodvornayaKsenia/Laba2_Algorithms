#ifndef ARRAY_TASK_H
#define ARRAY_TASK_H

#include <string>
#include "array.h"

using namespace std;

string ToLower(const string& word);
int CountStresses(const string& word);

int CheckStresses(const MyArray& dictionary,
                  const string& text);

void RunArrayTask();

#endif
