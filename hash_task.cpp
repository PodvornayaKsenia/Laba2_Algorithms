#include "hash_task.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <stdexcept>

using namespace std;

BloomFilter::BloomFilter(int size) : bitCount(size) {
    if (bitCount <= 0) {
        throw invalid_argument("Invalid Bloom filter size");
    }

    MInit(bits);

    for (int i = 0; i < bitCount; i++) {
        MPushBack(bits, "0");
    }
}

BloomFilter::~BloomFilter() {
    delete[] bits.data;
}

uint64_t BloomFilter::hash1(
    const string& key) const {

    uint64_t hash = UINT64_C(14695981039346656037);

    for (unsigned char c : key) {
        hash ^= c;
        hash *= UINT64_C(1099511628211);
    }

    return hash;
}

uint64_t BloomFilter::hash2(
    const string& key) const {

    uint64_t hash = 5381;

    for (unsigned char c : key) {
        hash = hash * 33 + c;
    }

    return hash;
}

void BloomFilter::add(const string& key) {
    uint64_t h1 = hash1(key);
    uint64_t h2 = hash2(key) | uint64_t{1};

    for (int i = 0; i < 3; i++) {
        int index = static_cast<int>(
            (h1 + i * h2) % bitCount
        );

        MReplace(bits, index, "1");
    }
}

bool BloomFilter::possiblyContains(
    const string& key) const {

    uint64_t h1 = hash1(key);
    uint64_t h2 = hash2(key) | uint64_t{1};

    for (int i = 0; i < 3; i++) {
        int index = static_cast<int>(
            (h1 + i * h2) % bitCount
        );

        if (MGet(bits, index) == "0") {
            return false;
        }
    }

    return true;
}

HashTable::HashTable(bool useDoubleHashing, int size)
    : filter(8192),
      capacity(size),
      count(0),
      doubleHashing(useDoubleHashing),
      probes(0) {

    if (capacity < 2) {
        throw invalid_argument("Invalid table capacity");
    }

    MInit(keys);
    MInit(values);
    MInit(states);

    for (int i = 0; i < capacity; i++) {
        MPushBack(keys, "");
        MPushBack(values, "");
        MPushBack(states, "0");
    }
}

HashTable::~HashTable() {
    delete[] keys.data;
    delete[] values.data;
    delete[] states.data;
}

uint64_t HashTable::hash1(
    const string& key) const {

    uint64_t hash = UINT64_C(14695981039346656037);

    for (unsigned char c : key) {
        hash ^= c;
        hash *= UINT64_C(1099511628211);
    }

    return hash;
}

uint64_t HashTable::hash2(
    const string& key) const {

    uint64_t hash = 5381;

    for (unsigned char c : key) {
        hash = hash * 33 + c;
    }

    return hash;
}

int HashTable::position(
    const string& key, int attempt) const {

    uint64_t start = hash1(key) % capacity;
    uint64_t step = 1;

    if (doubleHashing) {
        step = 1 + hash2(key) % (capacity - 1);
    }

    return static_cast<int>(
        (start + static_cast<uint64_t>(attempt)
        * step) % capacity
    );
}

int HashTable::findIndex(const string& key) const {
    for (int i = 0; i < capacity; i++) {
        int index = position(key, i);
        probes++;

        string state = MGet(states, index);

        if (state == "0") {
            return -1;
        }

        if (state == "1" &&
            MGet(keys, index) == key) {
            return index;
        }
    }

    return -1;
}

bool HashTable::insert(const string& key, const string& value) {
    int firstDeleted = -1;

    for (int i = 0; i < capacity; i++) {
        int index = position(key, i);
        probes++;

        string state = MGet(states, index);

        if (state == "1") {
            if (MGet(keys, index) == key) {
                MReplace(values, index, value);
                return false;
            }
        }
        else if (state == "2") {
            if (firstDeleted == -1) {
                firstDeleted = index;
            }
        }
        else {
            int target = firstDeleted == -1
                ? index : firstDeleted;

            MReplace(keys, target, key);
            MReplace(values, target, value);
            MReplace(states, target, "1");

            count++;
            filter.add(key);
            return true;
        }
    }

    if (firstDeleted != -1) {
        MReplace(keys, firstDeleted, key);
        MReplace(values, firstDeleted, value);
        MReplace(states, firstDeleted, "1");

        count++;
        filter.add(key);
        return true;
    }

    return false;
}

bool HashTable::get(const string& key, string& value) const {
    if (!filter.possiblyContains(key)) {
        return false;
    }

    int index = findIndex(key);
    if (index == -1) {
        return false;
    }

    value = MGet(values, index);
    return true;
}

bool HashTable::containsWithoutBloom(const string& key) const {
    return findIndex(key) != -1;
}

bool HashTable::contains(const string& key) const {
    if (!filter.possiblyContains(key)) {
        return false;
    }

    return findIndex(key) != -1;
}

bool HashTable::remove(const string& key) {
    if (!filter.possiblyContains(key)) {
        return false;
    }

    int index = findIndex(key);

    if (index == -1) {
        return false;
    }

    MReplace(keys, index, "");
    MReplace(values, index, "");
    MReplace(states, index, "2");

    count--;
    return true;
}

bool HashTable::bloomCheck(const string& key) const {
    return filter.possiblyContains(key);
}

int HashTable::getCount() const {
    return count;
}

int HashTable::getCapacity() const {
    return capacity;
}

uint64_t HashTable::getProbes() const {
    return probes;
}

void HashTable::resetProbes() {
    probes = 0;
}

void HashTable::print() const {
    for (int i = 0; i < capacity; i++) {
        if (MGet(states, i) == "1") {
            cout << "[" << i << "] "
                 << MGet(keys, i) << ": "
                 << MGet(values, i) << "\n";
        }
    }
}

struct TestResult {
    double insertUs = 0;
    double foundUs = 0;
    double missingUs = 0;
    double insertProbes = 0;
    double foundProbes = 0;
    double missingProbes = 0;
};

static TestResult TestHashMethod(bool useDouble, int elements, int repeats) {
    TestResult result;

    for (int repeat = 0; repeat < repeats; repeat++) {
        HashTable table(useDouble, 23);
        auto start = chrono::steady_clock::now();
        for (int i = 0; i < elements; i++) {
            string key = "key" + to_string(i);
            table.insert(key, "value" + to_string(i));
        }
        auto end = chrono::steady_clock::now();
        result.insertUs += chrono::duration<double, micro>(end - start).count();
        result.insertProbes += static_cast<double>(table.getProbes()) / elements;

        table.resetProbes();
        start = chrono::steady_clock::now();
        string value;
        for (int i = 0; i < elements; i++) {
            table.get("key" + to_string(i), value);
        }
        end = chrono::steady_clock::now();
        result.foundUs += chrono::duration<double, micro>(end - start).count();
        result.foundProbes += static_cast<double>(table.getProbes()) / elements;

        table.resetProbes();
        start = chrono::steady_clock::now();
        for (int i = 0; i < elements; i++) {
            table.containsWithoutBloom("missing" + to_string(i));
        }
        end = chrono::steady_clock::now();
        result.missingUs += chrono::duration<double, micro>(end - start).count();
        result.missingProbes += static_cast<double>(table.getProbes()) / elements;
    }

    result.insertUs /= repeats;
    result.foundUs /= repeats;
    result.missingUs /= repeats;
    result.insertProbes /= repeats;
    result.foundProbes /= repeats;
    result.missingProbes /= repeats;
    return result;
}

void RunHashTask() {
    HashTable linear(false);
    HashTable doubleHash(true);

    int choice = -1;

    while (choice != 0) {
        cout << "\nЗадание 6. Хеш-таблица\n";
        cout << "1. Добавить пару ключ-значение\n";
        cout << "2. Найти значение по ключу\n";
        cout << "3. Удалить ключ\n";
        cout << "4. Вывести таблицы\n";
        cout << "5. Эмпирическое сравнение\n";
        cout << "0. Выход\n";
        cout << "Выбор: ";

        if (!(cin >> choice)) {
            return;
        }

        if (choice >= 1 && choice <= 3) {
            string key;
            cout << "Введите ключ: ";
            cin >> key;

            if (choice == 1) {
                string value;
                cout << "Введите значение: ";
                cin >> value;

                bool a = linear.insert(key, value);
                bool b = doubleHash.insert(key, value);

                if (a && b) {
                    cout << "Пара добавлена.\n";
                }
                else if (linear.contains(key) && doubleHash.contains(key)) {
                    cout << "Значение обновлено.\n";
                }
                else {
                    cout << "Таблица заполнена.\n";
                }
            }
            else if (choice == 2) {
                cout << "Фильтр Блума: "
                     << (linear.bloomCheck(key)
                         ? "Возможно присутствует"
                         : "Точно отсутствует")
                     << "\n";

                string value;
                cout << "Линейное пробирование: ";
                if (linear.get(key, value)) {
                    cout << value << "\n";
                } else {
                    cout << "Не найден\n";
                }

                cout << "Двойное хеширование: ";
                if (doubleHash.get(key, value)) {
                    cout << value << "\n";
                } else {
                    cout << "Не найден\n";
                }
            }
            else {
                bool a = linear.remove(key);
                bool b = doubleHash.remove(key);

                cout << ((a && b)
                    ? "Ключ удален.\n"
                    : "Ключ не найден.\n");
            }
        }
        else if (choice == 4) {
            cout << "\nЛинейное пробирование:\n";
            linear.print();

            cout << "\nДвойное хеширование:\n";
            doubleHash.print();
        }
        else if (choice == 5) {
            const int levels[] = {6, 11, 17, 21};
            const int repeats = 3;

            cout << "\nСравнение на таблице из 23 ячеек, 3 повторения\n";
            cout << "Заполнение | Линейное: вставка / успешный / отсутствует"
                 << " | Двойное: вставка / успешный / отсутствует\n";
            cout << fixed << setprecision(2);

            for (int elements : levels) {
                TestResult linearResult = TestHashMethod(false, elements, repeats);
                TestResult doubleResult = TestHashMethod(true, elements, repeats);
                double percent = 100.0 * elements / 23;

                cout << percent << "% (" << elements << " пар): "
                     << linearResult.insertProbes << " / "
                     << linearResult.foundProbes << " / "
                     << linearResult.missingProbes << " | "
                     << doubleResult.insertProbes << " / "
                     << doubleResult.foundProbes << " / "
                     << doubleResult.missingProbes << " проверок\n";

                cout << "  Время серии, мкс: "
                     << linearResult.insertUs << " / "
                     << linearResult.foundUs << " / "
                     << linearResult.missingUs << " | "
                     << doubleResult.insertUs << " / "
                     << doubleResult.foundUs << " / "
                     << doubleResult.missingUs << "\n";
            }
            cout << defaultfloat;
        }
    }
}
