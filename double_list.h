#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

#include <string>
#include <iostream>

using namespace std;

struct DNode {
    string data;
    DNode* next;
    DNode* prev;
    DNode(string val);
};

void LPushFront(DNode* &head, DNode* &tail, string val);
void LPushBack(DNode* &head, DNode* &tail, string val);
void LInsertAfter(DNode* &head, DNode* &tail, string target, string val);
void LInsertBefore(DNode* &head, DNode* &tail, string target, string val);

void LPopFront(DNode* &head, DNode* &tail);
void LPopBack(DNode* &head, DNode* &tail);
void LRemoveByValue(DNode* &head, DNode* &tail, string val);
void LDeleteAfter(DNode* &head, DNode* &tail, string target);
void LDeleteBefore(DNode* &head, DNode* &tail, string target);

void LPrintForward(DNode* head);
void LPrintBackward(DNode* tail);
bool LSearch(DNode* head, string val);

#endif
