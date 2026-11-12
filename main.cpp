
#include <iostream>
#include <string>
#include <limits>

#include "xml.h"
#include "set_commands.h"
#include "genom.h"
#include "ydarenia.h"
#include "huffman_task.h"
#include "hash_task.h"
#include "lru_task.h"

using namespace std;

void RunXML() {
    string xml;

    cout << "\nЗадание 1. Восстановление XML\n";
    cout << "Введите XML-строку:\n";

    getline(cin, xml);

    string restored = XMLRestore(xml);

    cout << "Результат: " << restored << "\n";
}

void RunSet() {
    string filename;
    string query;

    cout << "\nЗадание 2. АТД Множество\n";
    cout << "Введите имя файла с данными:\n";
    getline(cin, filename);

    if (filename.empty()) {
        cout << "Имя файла не может быть пустым.\n";
        return;
    }

    cout << "Введите команду:\n";
    getline(cin, query);

    if (query.empty()) {
        cout << "Команда не может быть пустой.\n";
        return;
    }

    if (!ExecuteSetCommand(filename, query)) {
        cout << "Не удалось выполнить команду.\n";
    }
}

void RunGenome() {
    string first;
    string second;

    cout << "\nЗадание 3. Инопланетный геном\n";

    cout << "Введите первый геном:\n";
    getline(cin, first);

    cout << "Введите второй геном:\n";
    getline(cin, second);

    cout << "Степень близости: "
         << GenomeSimilarity(first, second)
         << "\n";
}

void ShowMenu() {
    cout << "      ЛАБОРАТОРНАЯ РАБОТА №2\n";

    cout << "1. Восстановление XML\n";
    cout << "2. АТД Множество\n";
    cout << "3. Инопланетный геном\n";
    cout << "4. Контрольная по ударениям\n";
    cout << "5. Алгоритм Хаффмана\n";
    cout << "6. Хеш-таблица и фильтр Блума\n";
    cout << "7. LRU-кэш\n";
    cout << "0. Выход\n";
    cout << "Выберите задание: ";
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        string filename;
        string query;

        for (int i = 1; i < argc; i++) {
            string argument = argv[i];

            if (argument == "--file" && i + 1 < argc) {
                filename = argv[++i];
            }
            else if (argument == "--query" && i + 1 < argc) {
                query = argv[++i];
            }
            else {
                cerr << "Некорректные аргументы запуска.\n";
                cerr << "Пример:\n";
                cerr << "./dbms --file file.data "
                        "--query 'SETADD myset 10'\n";
                return 1;
            }
        }

        if (filename.empty() || query.empty()) {
            cerr << "Необходимо указать --file и --query.\n";
            return 1;
        }

        return ExecuteSetCommand(filename, query) ? 0 : 1;
    }

    int choice;

    while (true) {
        ShowMenu();

        if (!(cin >> choice)) {
            if (cin.eof()) {
                break;
            }

            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cout << "Введите число от 0 до 7.\n";
            continue;
        }

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        switch (choice) {
            case 1:
                RunXML();
                break;

            case 2:
                RunSet();
                break;

            case 3:
                RunGenome();
                break;

            case 4:
                RunArrayTask();
                break;

            case 5:
                RunHuffmanTask();
                break;

            case 6:
                RunHashTask();
                break;

            case 7:
                RunLRUTask();
                break;

            case 0:
                cout << "Завершение программы.\n";
                return 0;

            default:
                cout << "Такого задания нет.\n";
                break;
        }
    }

    return 0;
}

