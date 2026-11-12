#include "xml.h"
#include "stack.h"

bool XMLCheck(const string& xml) {
    SNode* top = nullptr;
    int i = 0;
    int n = xml.length();
    bool correct = true;

    while (i < n) {
        if (xml[i] != '<') {
            correct = false;
            break;
        }

        i++;

        bool closing = false;

        if (i < n && xml[i] == '/') {
            closing = true;
            i++;
        }

        string name = "";

        while (i < n && xml[i] >= 'a' && xml[i] <= 'z') {
            name += xml[i];
            i++;
        }

        if (name.empty() || i >= n || xml[i] != '>') {
            correct = false;
            break;
        }

        i++;

        if (!closing) {
            SPush(top, name);
        }
        else {
            if (top == nullptr || top->data != name) {
                correct = false;
                break;
            }

            SPop(top);
        }
    }

    if (top != nullptr) {
        correct = false;
    }

    while (top != nullptr) {
        SPop(top);
    }

    return correct;
}

string XMLRestore(const string& xml) {
    string symbols = "abcdefghijklmnopqrstuvwxyz<>/";
    string result = xml;

    // При нескольких возможных исправлениях предпочитаем более поздний символ.
    for (int i = static_cast<int>(xml.length()) - 1; i >= 0; i--) {
        char original = result[i];

        // Пробуем все допустимые символы
        for (char c : symbols) {
            if (c == original) {
                continue;
            }

            result[i] = c;

            // Проверяем строку с заменённым символом
            if (XMLCheck(result)) {
                return result;
            }
        }

        // Возвращаем исходный символ
        result[i] = original;
    }

    return "Incorrect";
}
