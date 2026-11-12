#ifndef STACK_H
#define STACK_H

#include <string>
#include <iostream>

using namespace std;

struct SNode {
    string data;
    SNode* next;
    SNode(string val) : data(val), next(nullptr) {}
};

void SPush(SNode* &top, string val);
string SPop(SNode* &top);
string SPeek(SNode* top);
void SPrint(SNode* top);

#endif
