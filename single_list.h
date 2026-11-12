#ifndef SINGLE_LIST_H
#define SINGLE_LIST_H

#include <string>
#include <iostream>

using namespace std;

struct Node {
    string data;
    Node* next;
    Node(string val);
};

void pushFront(Node* &head, string val);
void pushBack(Node* &head, string val);
void insertAfter(Node* head, string target, string val);
void insertBefore(Node* &head, string target, string val);

void popFront(Node* &head);
void popBack(Node* &head);
void removeByValue(Node* &head, string val);
void deleteAfter(Node* head, string target);
void deleteBefore(Node* &head, string target);

void printList(Node* head);
void printRecursive(Node* head); 
Node* search(Node* head, string val);

#endif
