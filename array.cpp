#include "array.h"

MyArray::MyArray() : data(nullptr), size(0), capacity(0) {}

void MInit(MyArray &arr, int initCapacity) {
    arr.size = 0;
    arr.capacity = initCapacity;
    arr.data = new string[arr.capacity];
}

// увеличение
void MResize(MyArray &arr) {
    int newCapacity = (arr.capacity == 0) ? 2 : arr.capacity * 2;
    string* newData = new string[newCapacity];
    for (int i = 0; i < arr.size; i++) {
        newData[i] = arr.data[i];
    }
    delete[] arr.data;
    arr.data = newData;
    arr.capacity = newCapacity;
}

//вставка
void MPushBack(MyArray &arr, string val) {
    if (arr.capacity == 0) MInit(arr);
    if (arr.size == arr.capacity) MResize(arr);
    arr.data[arr.size++] = val;
}

void MInsert(MyArray &arr, int index, string val) {
    if (index < 0 || index > arr.size) return;
    if (arr.size == arr.capacity) MResize(arr);
    
    for (int i = arr.size; i > index; i--) {
        arr.data[i] = arr.data[i - 1];
    }
    arr.data[index] = val;
    arr.size++;
}

//получение по индексу
string MGet(const MyArray &arr, int index) {
    if (index < 0 || index >= arr.size) return "Index error";
    return arr.data[index];
}

//удаление
void MRemove(MyArray &arr, int index) {
    if (index < 0 || index >= arr.size) return;
    for (int i = index; i < arr.size - 1; i++) {
        arr.data[i] = arr.data[i + 1];
    }
    arr.size--;
}

void MReplace(MyArray &arr, int index, string val) {
    if (index < 0 || index >= arr.size) return;
    arr.data[index] = val;
}

int MLength(const MyArray &arr) {
    return arr.size;
}

void MPrint(const MyArray &arr) {
    if (arr.size == 0) { 
        cout << "[Массив пуст]" << endl; 
        return; 
    }
    for (int i = 0; i < arr.size; i++) {
        cout << arr.data[i] << (i == arr.size - 1 ? "" : ", ");
    }
    cout << endl;
}
