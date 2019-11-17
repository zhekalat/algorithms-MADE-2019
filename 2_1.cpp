// 2_1. Жадина.
// Вовочка ест фрукты из бабушкиной корзины. В корзине лежат фрукты разной массы. Вовочка может поднять не более K грамм. Каждый фрукт весит не более K грамм. За раз он выбирает несколько самых тяжелых фруктов, которые может поднять одновременно, откусывает от каждого половину и кладет огрызки обратно в корзину. Если фрукт весит нечетное число грамм, он откусывает большую половину. Фрукт массы 1гр он съедает полностью.
// Определить за сколько подходов Вовочка съест все фрукты в корзине.
// Формат входных данных. Вначале вводится n - количество фруктов и n строк с массами фруктов.
// n ≤ 50000.
// Затем K - "грузоподъемность". K ≤ 1000.
// Формат выходных данных. Неотрицательное число - количество подходов к корзине.

#include <iostream>
#include <vector>

class PriorityQueue {
public:
    bool empty() const;
    int top() const;
    int pop();
    void push(int value);
private:
    std::vector<int> arr;
    void siftDown(int index);
    void siftUp(int index);
};

void PriorityQueue::siftDown(int index) {
    while (2*index + 1 < arr.size()) {  // пока у текущего узла есть потомки
        int max_child_index = 0;
        if (2*index + 2 == arr.size()) {  // если у текущего узла остался 1 потомок
            max_child_index = 2*index + 1;
        } else {
            if (arr[2*index + 1] > arr[2*index + 2]) {  // находим максимального потомка
                max_child_index = 2*index + 1;
            } else {
                max_child_index = 2*index + 2;
            }
        }

        if (arr[index] < arr[max_child_index]) {  // если потомок больше родителя - меняем местами
            int tmp = 0;
            tmp = arr[index];
            arr[index] = arr[max_child_index];
            arr[max_child_index] = tmp;
            index = max_child_index;
        } else {  // иначе больше ничего делать не нужно
            break;
        }
    }

}

void PriorityQueue::siftUp(int index) {
    while (index > 0) {  // пока у текущего узла есть родители
        int parent_index = (index - 1) / 2;
        if (arr[index] > arr[parent_index]) {  // если потомок больше родителя - меняем местами
            int tmp = 0;
            tmp = arr[index];
            arr[index] = arr[parent_index];
            arr[parent_index] = tmp;
            index = parent_index;
        } else {  // иначе больше ничего делать не нужно
            break;
        }
    }
}

bool PriorityQueue::empty() const {
    return arr.empty();
}

int PriorityQueue::top() const {
    return arr.front();
}

int PriorityQueue::pop() {
    int tmp = arr[0];
    arr[0] = arr[arr.size() - 1];
    arr.pop_back();
    siftDown(0);
    return tmp;
}

void PriorityQueue::push(int value) {
    arr.push_back(value);
    siftUp(arr.size() - 1);
}

int main() {
    int fruit_count = 0;
    std::cin >> fruit_count;

    PriorityQueue priorityQueue;
    for (int i = 0; i < fruit_count; i++) {
        int weight = 0;
        std::cin >> weight;
        priorityQueue.push(weight);
    }
    int capacity = 0;
    std::cin >> capacity;

    int attempt = 0;
    // пока остались фрукты, Вовочка продолжает их есть
    while (not priorityQueue.empty()) {
        std::vector<int> curr_fruits;
        int curr_capacity = 0;
        // заполняем грузоподъемность
        while ((capacity - curr_capacity >= priorityQueue.top()) & (not priorityQueue.empty())) {
            curr_fruits.push_back(priorityQueue.top());
            curr_capacity += priorityQueue.top();
            priorityQueue.pop();
        }
        // откусываем от фруктов
        for (int curr_fruit : curr_fruits) {
            int half_fruit = curr_fruit / 2;
            if (half_fruit > 0) {
                priorityQueue.push(half_fruit);
            }
        }
        attempt++;
    }
    std::cout << attempt;
    return 0;
}
