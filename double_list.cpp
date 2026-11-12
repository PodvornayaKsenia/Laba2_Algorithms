#include "double_list.h"

DNode::DNode(string val) : data(val), next(nullptr), prev(nullptr) {}

// вставка

void LPushFront(DNode* &head, DNode* &tail, string val) {
    DNode* newNode = new DNode(val);
    if (!head) {
        head = tail = newNode;
        return;
    }
    newNode->next = head;
    head->prev = newNode;
    head = newNode;
}

void LPushBack(DNode* &head, DNode* &tail, string val) {
    DNode* newNode = new DNode(val);
    if (!tail) {
        head = tail = newNode;
        return;
    }
    newNode->prev = tail;
    tail->next = newNode;
    tail = newNode;
}

void LInsertAfter(DNode* &head, DNode* &tail, string target, string val) {
    DNode* temp = head;
    while (temp && temp->data != target) temp = temp->next;
    if (temp) {
        DNode* newNode = new DNode(val);
        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next) temp->next->prev = newNode;
        else tail = newNode;
        temp->next = newNode;
    }
}

void LInsertBefore(DNode* &head, DNode* &tail, string target, string val) {
    DNode* temp = head;
    while (temp && temp->data != target) temp = temp->next;
    if (temp) {
        if (temp == head) {
            LPushFront(head, tail, val);
            return;
        }
        DNode* newNode = new DNode(val);
        newNode->prev = temp->prev;
        newNode->next = temp;
        temp->prev->next = newNode;
        temp->prev = newNode;
    }
}

// удаление 

void LPopFront(DNode* &head, DNode* &tail) {
    if (!head) return;
    DNode* toDelete = head;
    head = head->next;
    if (head) head->prev = nullptr;
    else tail = nullptr;
    delete toDelete;
}

void LPopBack(DNode* &head, DNode* &tail) {
    if (!tail) return;
    DNode* toDelete = tail;
    tail = tail->prev;
    if (tail) tail->next = nullptr;
    else head = nullptr;
    delete toDelete;
}

void LRemoveByValue(DNode* &head, DNode* &tail, string val) {
    DNode* temp = head;
    while (temp && temp->data != val) temp = temp->next;
    if (!temp) return;

    if (temp == head) LPopFront(head, tail);
    else if (temp == tail) LPopBack(head, tail);
    else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        delete temp;
    }
}

void LDeleteAfter(DNode* &head, DNode* &tail, string target) {
    DNode* temp = head;
    while (temp && temp->data != target) temp = temp->next;
    if (temp && temp->next) {
        DNode* toDelete = temp->next;
        temp->next = toDelete->next;
        if (toDelete->next) toDelete->next->prev = temp;
        else tail = temp;
        delete toDelete;
    }
}

void LDeleteBefore(DNode* &head, DNode* &tail, string target) {
    DNode* temp = head;
    while (temp && temp->data != target) temp = temp->next;
    if (temp && temp->prev) {
        DNode* toDelete = temp->prev;
        if (toDelete == head) LPopFront(head, tail);
        else {
            temp->prev = toDelete->prev;
            toDelete->prev->next = temp;
            delete toDelete;
        }
    }
}

//поиск и чтение
void LPrintForward(DNode* head) {
    if (!head) { cout << "[Пустой список]" << endl; return; }
    DNode* temp = head;
    while (temp) {
        cout << temp->data << (temp->next ? " <-> " : "");
        temp = temp->next;
    }
    cout << endl;
}

void LPrintBackward(DNode* tail) {
    if (!tail) { cout << "[Пустой список]" << endl; return; }
    DNode* temp = tail;
    while (temp) {
        cout << temp->data << (temp->prev ? " " : "");
        temp = temp->prev;
    }
    cout << endl;
}

bool LSearch(DNode* head, string val) {
    DNode* temp = head;
    while (temp) {
        if (temp->data == val) return true;
        temp = temp->next;
    }
    return false;
}
