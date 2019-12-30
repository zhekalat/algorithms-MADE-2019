// Латышев Евгений. Задача 8 вариант 2.
// https://contest.yandex.ru/contest/15766/run-report/27122481/
// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
// когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и
// проверки принадлежности данной строки множеству.
// 1_2. Для разрешения коллизий используйте двойное хеширование.

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// Элемент хеш-таблицы
struct HashTableNode {
    std::string key = "";
    bool deleted = false;
    explicit HashTableNode(std::string key_) : key(std::move(key_)) {}
};

// Хеш-таблица
class HashTable {
public:
    explicit HashTable(size_t initial_size) : table(initial_size, HashTableNode("")) {}
    bool Has(const std::string& key) const;
    bool Add(const std::string& key);
    bool Remove(const std::string& key);

private:
    std::vector<HashTableNode> table;
    int static hornerHash(const std::string& key);
    int static secondaryHash(const std::string& key);
    void resize();
    int currNodes = 0;
};

// Подсчёт хеша по методу Горнера
int HashTable::hornerHash(const std::string& key) {
    size_t hash = 0;
    size_t a = 149;
    for (char const &c: key) {
        hash = (hash * a + c);
    }
    return hash;
}

// Подсчёт простого хеша для двойного хеширования
int HashTable::secondaryHash(const std::string& key) {
    size_t hash = 0;
    for (char const &c: key) {
        hash = hash + c;
    }
    hash = (hash * 2 + 1);
    return hash;
}

// Увеличение размера хеш-таблицы в 2 раза
void HashTable::resize() {
    currNodes = 0;  // Пересчитываем количество элементов, т.к. не переносим элменты с проставленным флагом удаления
    for(auto & i : table) {
        if (!i.deleted) {
            currNodes++;
        }
    }
    size_t new_table_size;
    if (currNodes >= table.size() * 0.75) {
        new_table_size = table.size() * 2;
    } else {
        new_table_size = table.size();
    }
    std::vector<HashTableNode> new_table(new_table_size, HashTableNode(""));

    for(auto & i : table) {
        if (!i.deleted) { // Переносим только неудалённые элементы
            size_t hash = hornerHash(i.key) % new_table.size(); // Заново рассчитываем хеш, так как изменился размер таблицы
            if (new_table[hash].key == "") {
                new_table[hash].key = i.key;
            } else {
                while(new_table[hash].key != "") {
                    hash = (hash + secondaryHash(i.key)) % new_table.size(); // Если место занято, пробируем двойным хешированием
                }
                new_table[hash].key = i.key;
            }
        }
    }
    table = new_table;
}

// Проверка наличия ключа в хеш-таблице
bool HashTable::Has(const std::string& key) const {
    assert(!key.empty());

    size_t hash = hornerHash(key) % table.size();
    while (table[hash].key != "") {
        if (table[hash].key == key and !table[hash].deleted) {
            return true;
        }
        hash = (hash + secondaryHash(key)) % table.size(); // Если место занято, пробируем двойным хешированием
    }
    return false;
}

// Добавление ключа в хеш-таблицу
bool HashTable::Add(const std::string& key) {
    assert(!key.empty());

    if (currNodes >= table.size() * 0.75) {  // Если заполнили 3/4 хеш-таблицы, перестраиваем её
        resize();
    }
    size_t hash = hornerHash(key) % table.size();
    size_t first_deleted = 0;
    bool found_deleted = false;
    while (table[hash].key != "") {
        if (table[hash].key == key) {
            if (table[hash].deleted) {
                table[hash].deleted = false;
                return true;
            } else {
                return false;
            }
        }
        if (table[hash].deleted and !found_deleted) {
            first_deleted = hash;
            found_deleted = true;
        }
        hash = (hash + secondaryHash(key)) % table.size(); // Если место занято, пробируем двойным хешированием
    }
    if (found_deleted) { // если по пути до 1 пустого встретили удалённый элемент, вставляем на его место
        table[first_deleted].key = key;
        table[first_deleted].deleted = false;
    } else {  // если не встретили удалённого, то вставляем на место пустого и увеличиваем счётчик количества узлов
        table[hash].key = key;
        table[hash].deleted = false;
        currNodes++;  // Подсчитываем количество узлов в дереве, чтобы в хеш-таблице всегда было достаточно места
    }
    return true;
}

// Удаление ключа из хеш-таблицы
bool HashTable::Remove(const std::string& key) {
    assert(!key.empty());

    size_t hash = hornerHash(key) % table.size();
    // Идём до первого пустого элемента, либо до нужного ключа без флага удаления
    while (table[hash].key != key or table[hash].deleted) {
        if (table[hash].key == "") {
            return false;
        }
        hash = (hash + secondaryHash(key)) % table.size(); // Если место занято, пробируем двойным хешированием
    }
    table[hash].deleted = true;
    return true;
}

int main() {
    HashTable table(8);
    char command = ' ';
    std::string value;
    while (std::cin >> command >> value) {
        switch (command) {
            case '?':
                std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '+':
                std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                break;
        }
    }
    return 0;
}
