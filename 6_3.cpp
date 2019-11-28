// Латышев Евгений
// https://contest.yandex.ru/contest/15368/run-report/25965108/
// Задача 6.3
// Дано число N ≤ 104 и последовательность целых чисел из [-231..231] длиной N. Требуется построить бинарное дерево,
// заданное наивным порядком вставки. Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Выведите элементы в порядке post-order (снизу вверх).
// Рекурсия запрещена.

#include <iostream>
#include <stack>

// узел дерева
struct TreeNode {
    explicit TreeNode(int _value) : value(_value) {}
    int value = 0;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

// Бинарное дерево
class BinaryTree {
public:
    explicit BinaryTree() = default;
    ~BinaryTree() {PostOrder(deleteNode);};
    BinaryTree(const BinaryTree& other);
    BinaryTree(BinaryTree&& other) noexcept;
    BinaryTree& operator=(const BinaryTree& other) = delete;
    BinaryTree& operator=(BinaryTree&& other) noexcept = delete;
    void Add(int value);
    void PrintTree() const {PostOrder(printNode);};
private:
    TreeNode* root = nullptr;
    static void deleteNode(TreeNode* currNode) {delete currNode;};
    static void printNode(TreeNode* currNode) {std::cout << currNode->value << " ";};
    void PostOrder(const std::function<void(TreeNode*)>& operation) const;
    void PreOrder(const std::function<void(TreeNode*)>& operation) const;
};

// глубокая копия дерева
BinaryTree::BinaryTree(const BinaryTree& other) {
    std::stack<TreeNode*> nodesToCopy;
    std::stack<TreeNode*> nodesToCopyNew;
    root = new TreeNode(other.root->value);
    nodesToCopy.push(other.root);
    nodesToCopyNew.push(root);
    while (!nodesToCopy.empty()) {
        TreeNode* currNode = nodesToCopy.top();
        TreeNode* currNodeNew = nodesToCopyNew.top();
        nodesToCopy.pop();
        nodesToCopyNew.pop();
        if (currNode->right != nullptr) {
            currNodeNew->right = new TreeNode(currNode->right->value);
            nodesToCopy.push(currNode->right);
            nodesToCopyNew.push(currNodeNew->right);
        }
        if (currNode->left != nullptr) {
            currNodeNew->left = new TreeNode(currNode->left->value);
            nodesToCopy.push(currNode->left);
            nodesToCopyNew.push(currNodeNew->left);
        }
    }
}

BinaryTree::BinaryTree(BinaryTree&& other) noexcept {
    root = other.root;
}

// обход дерева сверху вниз
void BinaryTree::PreOrder(const std::function<void(TreeNode*)>& operation) const {
    std::stack<TreeNode*> nodesToVisit;
    nodesToVisit.push(root);
    while (!nodesToVisit.empty()) {
        TreeNode* currNode = nodesToVisit.top();
        operation(currNode);
        nodesToVisit.pop();
        if (currNode->right != nullptr) {
            nodesToVisit.push(currNode->right);
        }
        if (currNode->left != nullptr) {
            nodesToVisit.push(currNode->left);
        }
    }
}

// обход дерева снизу вверх
void BinaryTree::PostOrder(const std::function<void(TreeNode*)>& operation) const {
    std::stack<TreeNode*> nodesToVisit;
    if (root->right != nullptr) {
        nodesToVisit.push(root->right);
    }
    nodesToVisit.push(root);
    TreeNode* currNode = root->left;
    // всегда идём до упора налево, положив в стек правого сына и текущий узел
    while (!nodesToVisit.empty()) {
        while (currNode != nullptr) {
            if (currNode->right != nullptr) {
                nodesToVisit.push(currNode->right);
            }
            nodesToVisit.push(currNode);
            currNode = currNode->left;
        }
        // когда налево дошли до упора, идём направо
        currNode = nodesToVisit.top();
        nodesToVisit.pop();
        // если есть куда идти направо - спускаемся, иначе - удаляем текущий узел
        if (currNode->right != nullptr and !nodesToVisit.empty() and currNode->right == nodesToVisit.top()) {
            nodesToVisit.pop();
            nodesToVisit.push(currNode);
            currNode = currNode->right;
        } else {
            operation(currNode);
            currNode = nullptr;
        }
    }
}

void BinaryTree::Add(int value) {
    TreeNode* newNode = nullptr;
    if (root == nullptr) {
        root = new TreeNode(value);
        return;
    }
    TreeNode* currNode = root;
    // ищем место для вставки новго элемента, пока не найдём
    while (newNode == nullptr) {
        if (currNode->value > value) {
            if (currNode->left == nullptr) {
                newNode = new TreeNode(value);
                currNode->left = newNode;
            } else {
                currNode = currNode->left;
            }
        } else {
            if (currNode->right == nullptr) {
                newNode = new TreeNode(value);
                currNode->right = newNode;
            } else {
                currNode = currNode->right;
            }
        }
    }
}

int main() {
    int element_count = 0;
    std::cin >> element_count;

    BinaryTree tree;
    for (int i = 0; i < element_count; i++) {
        int element = 0;
        std::cin >> element;
        tree.Add(element);
    }
    tree.PrintTree();
    return 0;
}