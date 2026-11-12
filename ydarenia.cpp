
#include "ydarenia.h"
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

// Перевод слова в нижний регистр
string ToLower(const string& word) {
    string result = word;

    for (int i = 0; i < (int)result.length(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] - 'A' + 'a';
        }
    }

    return result;
}

// Подсчёт количества ударений
int CountStresses(const string& word) {
    int count = 0;

    for (char c : word) {
        if (c >= 'A' && c <= 'Z') {
            count++;
        }
    }

    return count;
}

int CheckStresses(const MyArray& dictionary,
                  const string& text) {

    istringstream ss(text);
    string word;

    int errors = 0;

    while (ss >> word) {
        bool found = false;
        bool correct = false;

        string lowerWord = ToLower(word);

        for (int i = 0; i < MLength(dictionary); i++) {
            string dictWord = MGet(dictionary, i);

            if (ToLower(dictWord) == lowerWord) {
                found = true;

                if (dictWord == word) {
                    correct = true;
                    break;
                }
            }
        }

        if (found) {
            if (!correct) {
                errors++;
            }
        }
        else {
            if (CountStresses(word) != 1) {
                errors++;
            }
        }
    }

    return errors;
}

void RunArrayTask() {
    int n;

    cout << "Введите количество слов в словаре: ";

    if (!(cin >> n) || n < 0) {
        cout << "Некорректное количество слов.\n";
        return;
    }

    MyArray dictionary;
    MInit(dictionary);

    cout << "Введите слова словаря:\n";

    for (int i = 0; i < n; i++) {
        string word;
        cin >> word;
        MPushBack(dictionary, word);
    }

    char c;

    while (cin.get(c) && c != '\n') {
    }
    string text;

    cout << "Введите текст для проверки:\n";
    getline(cin, text);

    int errors = CheckStresses(dictionary, text);

    cout << "Количество ошибок: " << errors << "\n";

    delete[] dictionary.data;
}
