#include "single_list.h"

Node::Node(string val) {
    data = val;
    next = nullptr;
}

//вставка

void pushFront(Node* &head, string val) {
    Node* newNode = new Node(val);
    newNode->next = head;
    head = newNode;
}

void pushBack(Node* &head, string val) {
    Node* newNode = new Node(val);
    if (!head) {
        head = newNode;
        return;
    }
    Node* temp = head;
    while (temp->next) temp = temp->next;
    temp->next = newNode;
}

void insertAfter(Node* head, string target, string val) {
    Node* temp = head;
    while (temp && temp->data != target) temp = temp->next;
    if (temp) {
        Node* newNode = new Node(val);
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

void insertBefore(Node* &head, string target, string val) {
    if (!head) return;
    if (head->data == target) {
        pushFront(head, val);
        return;
    }
    Node* temp = head;
    while (temp->next && temp->next->data != target) temp = temp->next;
    if (temp->next) {
        Node* newNode = new Node(val);
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

// удаление
void popFront(Node* &head) {
    if (!head) return;
    Node* nodeToDelete = head;
    head = head->next;
    delete nodeToDelete;
}

void popBack(Node* &head) {
    if (!head) return;
    if (!head->next) {
        delete head;
        head = nullptr;
        return;
    }
    Node* temp = head;
    while (temp->next->next) temp = temp->next;
    delete temp->next;
    temp->next = nullptr;
}

void removeByValue(Node* &head, string val) {
    if (!head) return;
    if (head->data == val) {
        popFront(head);
        return;
    }
    Node* temp = head;
    while (temp->next && temp->next->data != val) temp = temp->next;
    if (temp->next) {
        Node* nodeToDelete = temp->next;
        temp->next = nodeToDelete->next;
        delete nodeToDelete;
    }
}

void deleteAfter(Node* head, string target) {
    Node* temp = head;
    while (temp && temp->data != target) temp = temp->next;
    if (temp && temp->next) {
        Node* nodeToDelete = temp->next;
        temp->next = nodeToDelete->next;
        delete nodeToDelete;
    }
}

void deleteBefore(Node* &head, string target) {
    if (!head || !head->next || head->data == target) return;
    if (head->next->data == target) {
        popFront(head);
        return;
    }
    Node* temp = head;
    while (temp->next && temp->next->next && temp->next->next->data != target) {
        temp = temp->next;
    }
    if (temp->next && temp->next->next) {
        Node* nodeToDelete = temp->next;
        temp->next = nodeToDelete->next;
        delete nodeToDelete;
    }
}

// чтение и поиск
void printList(Node* head) {
    if (!head) {
        cout << "[Пустой список]" << endl;
        return;
    }
    Node* temp = head;
    while (temp) {
        cout << temp->data << (temp->next ? " -> " : "");
        temp = temp->next;
    }
    cout << endl;
}

void printRecursive(Node* head) {
    if (!head) {
        cout << "NULL" << endl;
        return;
    }
    cout << head->data << " -> ";
    printRecursive(head->next);
}

Node* search(Node* head, string val) {
    Node* temp = head;
    while (temp) {
        if (temp->data == val) return temp;
        temp = temp->next;
    }
    return nullptr;
}
