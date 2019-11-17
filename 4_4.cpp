//Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
//Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
//Напишите нерекурсивный алгоритм.
//Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
//Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
//Описание для случая прохода от начала массива к концу:
//Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
//Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного.
//Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы.
//Последним элементом лежит опорный.
//Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
//Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
//Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
//Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
//В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.
//
//4_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
//Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.



#include <iostream>
#include <cstring>

// Слияние двух отсортированных массивов
void Merge(int const* left_arr, int leftLen, int const* right_arr, int rightLen, int* new_arr) {
    int currLeft = 0;
    int currRight = 0;
    int currNew = 0;
    while (currLeft != leftLen and currRight != rightLen) {
        if (left_arr[currLeft] <= right_arr[currRight]) {
            new_arr[currNew] = left_arr[currLeft];
            currLeft++;
        } else {
            new_arr[currNew] = right_arr[currRight];
            currRight++;
        }
        currNew++;
    }
    while (currLeft != leftLen) {
        new_arr[currNew] = left_arr[currLeft];
        currLeft++;
        currNew++;
    }
    while (currRight != rightLen) {
        new_arr[currNew] = right_arr[currRight];
        currRight++;
        currNew++;
    }
}

void MergeSort(int* arr, int len) {
    if (len <= 1) {
        return;
    }
    int firstHalfLen = len / 2;
    int secondHalfLen = len - firstHalfLen;
    MergeSort(arr, firstHalfLen);
    MergeSort(arr + firstHalfLen, secondHalfLen);
    int* tmp = new int[len];
    Merge(arr, firstHalfLen, arr+firstHalfLen, secondHalfLen, tmp);
    std::memcpy(arr, tmp, sizeof(int) * len);
    delete[] tmp;
}

int main() {
    int n = 0;
    int k = 0;
    std::cin >> n >> k;
    int *old_arr = new int [k];
    int *new_arr = new int [k];
    int total_count = 0;

    int curr_count = 0;
    while (curr_count < k) {
        int element = 0;
        std::cin >> element;
        old_arr[curr_count] = element;
        curr_count++;
        total_count++;
    }
    MergeSort(old_arr, curr_count);
//    for (int i = 0; i < curr_count; i++) {
//        std::cout << old_arr[i] << ' ';
//    }
//    std::cout << '\n';
    while (total_count < n) {
        curr_count = 0;
        while (curr_count < k and total_count < n) {
            int element = 0;
            std::cin >> element;
            new_arr[curr_count] = element;
            curr_count++;
            total_count++;
        }
        MergeSort(new_arr, curr_count);
//        for (int i = 0; i < curr_count; i++) {
//            std::cout << new_arr[i] << ' ';
//        }
//        std::cout << '\n';

        int *combined_arr = new int [k + curr_count];
        Merge(old_arr, k, new_arr, curr_count, combined_arr);
        // выводим первые k элементов, гарантированно стоящих на своих местах
        for (int i =0; i < k; i++) {
            std::cout << combined_arr[i];
        }

        // оставшуюся часть переносим для следующей итерации
        for (int i = k; i < k + curr_count; i++) {
            old_arr[i - k] = combined_arr[i];
        }
        delete[] combined_arr;
    }
    for (int i =0; i < curr_count; i++) {
        std::cout << old_arr[i];
    }
    delete[] old_arr;
    delete[] new_arr;

    return 0;
}
