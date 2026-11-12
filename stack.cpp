#include "stack.h"
//вставка

void SPush(SNode* &top, string val) {
    SNode* newNode = new SNode(val);
    newNode->next = top;
    top = newNode;
}
//удаление 
string SPop(SNode* &top) {
    if (top == nullptr) return "";
    
    SNode* toDelete = top;
    string value = toDelete->data;
    top = top->next;
    delete toDelete;
    return value;
}
//вывод
void SPrint(SNode* top) {
    if (!top) {
        cout << "[Стек пуст]" << endl;
        return;
    }
    SNode* temp = top;
    while (temp) {
        cout << temp->data << endl; 
        temp = temp->next;
    }
}
