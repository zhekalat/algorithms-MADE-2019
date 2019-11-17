// 1_2. Реализовать дек с динамическим зацикленным буфером.

#include <cassert>
#include <iostream>

class Deque {
public:
    // Проверка дека на пустоту
    bool empty() const;
    // Добавление элемента в начало
    void push_front(int value);
    // Извлечение элемента с начала
    int pop_front();
    // Добавление элемента в конец
    void push_back(int value);
    // Извлечение элемента с конца
    int pop_back();

private:
    int head = 0;
    int tail = 0;
    int array_size = 0;
    int *arr = new int[1];
    // Изменение размера динамического буфера
    void resize(int new_size);
};

void Deque::resize(int new_size) {
    int *new_arr = new int[new_size];
    int counter = 0;
    if (new_size > array_size) {
        // если буфер увеличивается
        tail = array_size - 1;
        counter = array_size;
    } else {
        // если буфер уменьшается
        if (tail >= head) {
            tail = tail - head;
        } else {
            tail = array_size - head + tail;
        }
        counter = new_size;
    }

    // переносим элементы начиная с 0 элемента нового буфера
    for (int i = 0; i < counter; i++) {
        if (head + i < array_size) {
            new_arr[i] = arr[head + i];
        } else {
            new_arr[i] = arr[head + i - array_size];
        }
    }
    delete [] arr;
    arr = new_arr;
    head = 0;
    array_size = new_size;
}

bool Deque::empty() const {
    return array_size == 0;
}

void Deque::push_front(int value) {
    if (empty()) {
        head = tail = 0;
        arr = new int[1];
        arr[head] = value;
        array_size = 1;
    } else {
        int new_head = 0;
        if (head == 0) {
            new_head = array_size - 1;
        } else {
            new_head = head - 1;
        }
        if (new_head == tail) {
            resize(array_size * 2);
            head = array_size - 1;
        } else {
            head = new_head;
        }
        arr[head] = value;
    }
}

void Deque::push_back(int value) {
    if (empty()) {
        head = tail = 0;
        arr = new int[1];
        arr[head] = value;
        array_size = 1;
    } else {
        int new_tail = 0;
        if (tail == array_size - 1) {
            new_tail = 0;
        } else {
            new_tail = tail + 1;
        }
        if (head == new_tail) {
            resize(array_size * 2);
            tail += 1;
        } else {
            tail = new_tail;
        }
        arr[tail] = value;
    }
}

int Deque::pop_front() {
    assert(!empty());
    int result = arr[head];
    if (head == tail) {
        delete [] arr;
        head = tail = 0;
        array_size = 0;
    } else {
        if (head == array_size - 1) {
            head = 0;
        } else {
            head += 1;
        }

        // уменьшаем буфер, если он используется меньше, чем на 1/4
        int curr_elements = 0;
        if (head < tail) {
            curr_elements = tail - head + 1;
        } else {
            curr_elements = array_size - head + tail + 1;
        }
        if (array_size > 1 & curr_elements * 4 < array_size) {
            resize(array_size / 2);
        }
    }
    return result;
}

int Deque::pop_back() {
    assert(!empty());
    int result = arr[tail];
    if (head == tail) {
        delete [] arr;
        head = tail = 0;
        array_size = 0;
    } else {
        if (tail == 0) {
            tail = array_size - 1;
        } else {
            tail -= 1;
        }

        // уменьшаем буфер, если он используется меньше, чем на 1/4
        int curr_elements = 0;
        if (head < tail) {
            curr_elements = tail - head + 1;
        } else {
            curr_elements = array_size - head + tail + 1;
        }
        if (array_size > 1 & curr_elements * 4 < array_size) {
            resize(array_size / 2);
        }
    }
    return result;
}

int main() {
    int commands_count = 0;
    std::cin >> commands_count;

    Deque deque;
    for (int i = 0; i < commands_count; ++i) {
        int command = 0;
        int value = 0;
        std::cin >> command >> value;
        switch (command) {
            case 1:  // push front
                deque.push_front(value);
                break;
            case 2:  // pop front
                if (deque.empty()) {
                    if (value != -1) {
                        std::cout << "NO";
                        return 0;
                    }
                } else if (deque.pop_front() != value) {
                    std::cout << "NO";
                    return 0;
                }
                break;
            case 3:  // push back
                deque.push_back(value);
                break;
            case 4:  // pop back
                if (deque.empty()) {
                    if (value != -1) {
                        std::cout << "NO";
                        return 0;
                    }
                } else if (deque.pop_back() != value) {
                    std::cout << "NO";
                    return 0;
                }
                break;
            default:
                break;
        }
    }
    std::cout << "YES";
    return 0;
}
