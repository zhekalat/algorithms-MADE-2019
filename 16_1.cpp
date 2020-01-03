// Латышев Евгений. Задача 16. Вариант 1
// https://contest.yandex.ru/contest/16186/run-report/29330289/
// Найдите все вхождения шаблона в строку.
// Длина шаблона – p, длина строки ­– n. Время O(n + p), доп. память – O(p)
// Вариант 1. С помощью префикс-функции (алгоритм Кнута-Морриса-Пратта)

#include <iostream>
#include <string>
#include <vector>

std::vector<int> prefixFunction(std::string fullString) {
    std::vector<int> prefixValues;
    prefixValues.push_back(0);
    for (int i = 1; i < fullString.length(); i++) {
        int currValue = prefixValues[i - 1];
        while (currValue > 0 and fullString[i] != fullString[currValue])
            currValue = prefixValues[currValue - 1];
        if (fullString[i] == fullString[currValue])
            currValue++;
        prefixValues.push_back(currValue);
    }
    return prefixValues;
}

void find_all_matches(const std::string& pattern, const std::string& inputString) {
    std::vector<int> matchPositions;
    std::vector<int> prefixValues = prefixFunction(pattern + "$" + inputString);
    for (int i = 0; i < inputString.length(); i++) {
        if (prefixValues[pattern.length() + 1 + i] == pattern.length())
            std::cout << i - pattern.length() + 1 << ' ';
    }
}

int main(const int argc, const char* argv[]) {
    std::string pattern, inputString;
    std::cin >> pattern;
    std::cin >> inputString;
    std::vector<int> result;
    find_all_matches(pattern, inputString);
    return 0;
}
