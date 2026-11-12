
#include "set_commands.h"
#include "myset.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool ExecuteSetCommand(const string& filename,
                       const string& query) {
    istringstream ss(query);

    string command, name, value;
    ss >> command >> name;

    if (name.empty()) {
        cerr << "Ошибка: не указано имя множества\n";
        return false;
    }

    if (command != "SETADD" &&
        command != "SETDEL" &&
        command != "SET_AT" &&
        command != "SETPRINT") {
        cerr << "Неизвестная команда множества\n";
        return false;
    }

    if (command != "SETPRINT" && !(ss >> value)) {
        cerr << "Ошибка: не указан элемент\n";
        return false;
    }

    string extra;
    if (ss >> extra) {
        cerr << "Ошибка: лишние аргументы\n";
        return false;
    }

    MySet set;
    vector<string> lines;
    int setPosition = -1;

    // Читаем существующие данные из файла
    ifstream input(filename);
    string line;

    while (getline(input, line)) {
        istringstream row(line);
        string type, savedName;

        row >> type >> savedName;

        if (type == "SET" && savedName == name) {
            if (setPosition != -1) {
                cerr << "Ошибка: множество записано дважды\n";
                return false;
            }

            setPosition = static_cast<int>(lines.size());

            string element;
            while (row >> element) {
                SETADD(set, element);
            }
        }

        lines.push_back(line);
    }

    if (input.bad()) {
        cerr << "Ошибка чтения файла\n";
        return false;
    }

    input.close();

    bool changed = false;

    if (command == "SETADD") {
        changed = SETADD(set, value);
        cout << (changed ? "OK" : "ALREADY EXISTS") << endl;
    }
    else if (command == "SETDEL") {
        changed = SETDEL(set, value);
        cout << (changed ? "OK" : "NOT FOUND") << endl;
    }
    else if (command == "SET_AT") {
        cout << (SET_AT(set, value) ? "TRUE" : "FALSE")
             << endl;
    }
    else if (command == "SETPRINT") {
        SETPrint(set);
    }

    // Сохраняем множество только после изменения
    if (changed) {
        ostringstream result;
        result << "SET " << name;

        for (int i = 0; i < set.capacity; i++) {
            Node* current = set.buckets[i];

            while (current != nullptr) {
                result << " " << current->data;
                current = current->next;
            }
        }

        if (setPosition == -1) {
            lines.push_back(result.str());
        }
        else {
            lines[setPosition] = result.str();
        }

        ofstream output(filename);

        if (!output) {
            cerr << "Ошибка записи файла\n";
            return false;
        }

        for (const string& savedLine : lines) {
            output << savedLine << '\n';
        }

        if (!output) {
            cerr << "Ошибка сохранения данных\n";
            return false;
        }
    }

    return true;
}
