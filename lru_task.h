#ifndef LRU_TASK_H
#define LRU_TASK_H

#include "double_list.h"

class LRUCache {
private:
    DNode* head;
    DNode* tail;
    int capacity;
    int size;

    DNode* findNode(int key) const;

public:
    LRUCache(int cap);
    ~LRUCache();

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    void set(int key, int value);
    int get(int key);
};

void RunLRUTask();

#endif
