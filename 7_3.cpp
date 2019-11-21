#include <iostream>
#include <stack>

// узел декартова дерева
struct TreapNode {
    explicit TreapNode(int _value) :
            value(_value), priority(rand()) {}

    int value = 0;
    int priority = 0;
    int left_children = 0;  // на позицию текущего узла влияют только левые потомки,
    int total_children = 0;  // но на позицию его родителя влияют и правые потомки, поэтому храним 2 счётчика
    TreapNode* left = nullptr;
    TreapNode* right = nullptr;
};

// декартово дерево
class Treap {
public:
    ~Treap();
    void Add(int value);
    void Pop(int value);
    int GetKStat(int k);
private:
    TreapNode* root = nullptr;
    void delete_subtree(TreapNode* node);
    std::pair<TreapNode*, TreapNode*> split(TreapNode* node, int key);
    TreapNode* merge(TreapNode*, TreapNode*);
    void update_child_count(TreapNode* currNode);
};

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
void Treap::Add(int value) {
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
        currNode->left_children = currNode->left->total_children + 1;
        if (currNode->right != nullptr) {
            currNode->total_children = currNode->left_children + currNode->right->total_children + 1;
        } else {
            currNode->total_children = currNode->left_children;
        }
    } else {
        currNode->left_children = 0;
        if (currNode->right != nullptr) {
            currNode->total_children = currNode->right->total_children + 1;
        } else {
            currNode->total_children = currNode->left_children;
        }
    }
}

// возвращаем k-ую статистику
int Treap::GetKStat(int k) {
    TreapNode* currNode = root;
    while (currNode) {
        if (k == currNode->left_children) {
            return currNode->value;
        }
        else if (k > currNode->left_children) {
            // вычитаем из k количество элементов левого поддерева (+ текущий), чтобы учесть их при проходе правого
            k -= currNode->left_children + 1;
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
            treap.Add(element);
            std::cout << treap.GetKStat(k) << "\n";
        } else {
            treap.Pop(-element);
            std::cout << treap.GetKStat(k) << "\n";
        }
    }
    return 0;
}
