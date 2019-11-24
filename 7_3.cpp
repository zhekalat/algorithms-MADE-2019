// Латышев Евгений
// https://contest.yandex.ru/contest/15368/run-report/26535417/
// Множество натуральных чисел постоянно меняется. Элементы в нем добавляются и удаляются по одному.
// Вычислите указанные порядковые статистики после каждого добавления или удаления.
// Требуемая скорость выполнения запроса - O(log n) в среднем.
// В реализации используйте декартовы деревья.

#include <iostream>
#include <stack>

// узел декартова дерева
struct TreapNode {
    explicit TreapNode(int _value) :
            value(_value), priority(rand()) {}

    int value = 0;
    int priority = 0;
    int total_children = 0;
    TreapNode* left = nullptr;
    TreapNode* right = nullptr;
};

// декартово дерево
class Treap {
public:
    explicit Treap() = default;
    ~Treap();
    Treap(const Treap& other);
    Treap(Treap&& other) noexcept;
    Treap& operator=(const Treap& other) = delete;
    Treap& operator=(Treap&& other) noexcept = delete;
    void Push(int value);
    void Pop(int value);
    int GetKStat(int k) const;
private:
    TreapNode* root = nullptr;
    void delete_subtree(TreapNode* node);
    std::pair<TreapNode*, TreapNode*> split(TreapNode* node, int key);
    TreapNode* merge(TreapNode*, TreapNode*);
    void update_child_count(TreapNode* currNode);
};

// глубокая копия Treap'а
Treap::Treap(const Treap& other) {
    std::stack<TreapNode*> nodesToCopy;
    std::stack<TreapNode*> nodesToCopyNew;
    root = new TreapNode(other.root->value);
    nodesToCopy.push(other.root);
    nodesToCopyNew.push(root);
    while (!nodesToCopy.empty()) {
        TreapNode* currNode = nodesToCopy.top();
        TreapNode* currNodeNew = nodesToCopyNew.top();
        nodesToCopy.pop();
        nodesToCopyNew.pop();
        if (currNode->right != nullptr) {
            currNodeNew->right = new TreapNode(currNode->right->value);
            nodesToCopy.push(currNode->right);
            nodesToCopyNew.push(currNodeNew->right);
        }
        if (currNode->left != nullptr) {
            currNodeNew->left = new TreapNode(currNode->left->value);
            nodesToCopy.push(currNode->left);
            nodesToCopyNew.push(currNodeNew->left);
        }
    }
}

Treap::Treap(Treap&& other) noexcept {
    root = other.root;
}

// разрезаем декартово дерево по ключу
std::pair<TreapNode*, TreapNode*> Treap::split(TreapNode* node, int key) {
    if (!node) {
        return std::make_pair(nullptr, nullptr);
    }
    if (node->value <= key) {
        const auto right_pair = split(node->right, key);
        node->right = right_pair.first;
        update_child_count(node);
        return std::make_pair(node, right_pair.second);
    } else {
        const auto left_pair = split(node->left, key);
        node->left = left_pair.second;
        update_child_count(node);
        return std::make_pair(left_pair.first, node);
    }
}

// склеиваем 2 декартовых дерева
TreapNode* Treap::merge(TreapNode* left, TreapNode* right) {
    if (!left || !right) {
        if (left == nullptr) {
            return right;
        } else {
            return left;
        }
    }
    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        update_child_count(left);
        return left;
    } else {
        right->left = merge(left, right->left);
        update_child_count(right);
        return right;
    }
}

// добавляем элемент в декартово дерево
void Treap::Push(int value) {
    std::pair<TreapNode*, TreapNode*> pair;
    pair = split(root, value);
    TreapNode* newNode;
    newNode = new TreapNode(value);
    TreapNode* leftRoot;
    leftRoot = merge(pair.first, newNode);
    root = merge(leftRoot, pair.second);
}

// удаляем элемент из декартова дерева
void Treap::Pop(int value) {
    std::pair<TreapNode*, TreapNode*> pair;
    pair = split(root, value);
    std::pair<TreapNode*, TreapNode*> leftPair;
    leftPair = split(pair.first, value-1); // так как наши значения имеют тип int, берем epsilon = 1
    root = merge(leftPair.first, pair.second);
}

// при изменении структуры дерева пересчитываем его детей
void Treap::update_child_count(TreapNode* currNode) {
    if (currNode->left != nullptr) {
        if (currNode->right != nullptr) {
            currNode->total_children = currNode->left->total_children + 1 + currNode->right->total_children + 1;
        } else {
            currNode->total_children = currNode->left->total_children + 1;
        }
    } else {
        if (currNode->right != nullptr) {
            currNode->total_children = currNode->right->total_children + 1;
        } else {
            currNode->total_children = 0;
        }
    }
}

// возвращаем k-ую статистику
int Treap::GetKStat(int k) const {
    TreapNode* currNode = root;
    while (currNode) {
        int currLeft = 0;
        if (currNode->left != nullptr) {
            currLeft = currNode->left->total_children + 1;
        }
        if (k == currLeft) {
            return currNode->value;
        }
        else if (k > currLeft) {
            // вычитаем из k количество элементов левого поддерева (+ текущий), чтобы учесть их при проходе правого
            k -= currLeft + 1;
            currNode = currNode->right;
        }
        else {
            currNode = currNode->left;
        }
    }
    return 0;
}

Treap::~Treap() {
    delete_subtree(root);
}

void Treap::delete_subtree(TreapNode* node) {
    if (!node) return;
    delete_subtree(node->left);
    delete_subtree(node->right);
    delete node;
}

int main() {
    srand(42);
    int element_count = 0;
    int k;
    std::cin >> element_count;

    Treap treap;
    for (int i = 0; i < element_count; i++) {
        int element = 0;
        std::cin >> element >> k;
        if (element >= 0) {
            treap.Push(element);
            std::cout << treap.GetKStat(k) << "\n";
        } else {
            treap.Pop(-element);
            std::cout << treap.GetKStat(k) << "\n";
        }
    }
    return 0;
}
