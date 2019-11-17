//3_2. Сортировка почти упорядоченной последовательности.
//Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k, то a[i] <= a[j].
//Требуется отсортировать последовательность. Последовательность может быть очень длинной.
//Время работы O(n * log(k)). Память O(k). Использовать слияние.


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

// MergeSort с лекции
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
