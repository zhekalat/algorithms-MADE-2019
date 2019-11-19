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
    ~BinaryTree();
    void add(int value);
    void print() const;
private:
    TreeNode* root = nullptr;
};

BinaryTree::~BinaryTree() {
    std::stack<TreeNode*> nodes_to_print;
    if (root->right != nullptr) {
        nodes_to_print.push(root->right);
    }
    nodes_to_print.push(root);
    TreeNode* currNode = root->left;
    // всегда идём до упора налево, положив в стек правого сына и текущий узел
    while (!nodes_to_print.empty()) {
        while (currNode != nullptr) {
            if (currNode->right != nullptr) {
                nodes_to_print.push(currNode->right);
            }
            nodes_to_print.push(currNode);
            currNode = currNode->left;
        }
        // когда налево дошли до упора, идём направо
        currNode = nodes_to_print.top();
        nodes_to_print.pop();
        // если есть куда идти направо - спускаемся, иначе - удаляем текущий узел
        if (currNode->right != nullptr and !nodes_to_print.empty() and currNode->right == nodes_to_print.top()) {
            nodes_to_print.pop();
            nodes_to_print.push(currNode);
            currNode = currNode->right;
        } else {
            delete currNode;
            currNode = nullptr;
        }
    }
}

void BinaryTree::add(int value) {
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

void BinaryTree::print() const {
    std::stack<TreeNode*> nodes_to_print;
    if (root->right != nullptr) {
        nodes_to_print.push(root->right);
    }
    nodes_to_print.push(root);
    TreeNode* currNode = root->left;
    // всегда идём до упора налево, положив в стек правого сына и текущий узел
    while (!nodes_to_print.empty()) {
        while (currNode != nullptr) {
            if (currNode->right != nullptr) {
                nodes_to_print.push(currNode->right);
            }
            nodes_to_print.push(currNode);
            currNode = currNode->left;
        }
        // когда налево дошли до упора, идём направо
        currNode = nodes_to_print.top();
        nodes_to_print.pop();
        // если есть куда идти направо - спускаемся, иначе - выводим текущий узел
        if (currNode->right != nullptr and !nodes_to_print.empty() and currNode->right == nodes_to_print.top()) {
            nodes_to_print.pop();
            nodes_to_print.push(currNode);
            currNode = currNode->right;
        } else {
            std::cout << currNode->value << " ";
            currNode = nullptr;
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
        tree.add(element);
    }
    tree.print();
    return 0;
}