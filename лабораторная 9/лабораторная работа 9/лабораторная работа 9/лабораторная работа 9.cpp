#include <iostream>
#include <queue>
#include <random>
#include <climits>
#include <algorithm>
using namespace std;

// Структура узла
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};


// Часть 1. бинарное дерево и обходы + подсчет узлов
class BinaryTree {
private:
    Node* root;
    // Прямой обход: Корень -> Левый -> Правый
    void preOrder(Node* node) {
        if (node == nullptr) return;
        cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }

    // Симметричный обход: Левый -> Корень -> Правый
    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }

    // Обратный обход: Левый -> Правый -> Корень
    void postOrder(Node* node) {
        if (node == nullptr) return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->data << " ";
    }

    // Подсчёт количества узлов (рекурсивно)
    int countNodes(Node* node) const {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    // Освобождение памяти (пост-обход)
    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() { destroy(root); }

    // Построение тестового дерева
    void buildSample() {
        root = new Node(10);
        root->left = new Node(5);
        root->right = new Node(15);
        root->left->left = new Node(3);
        root->left->right = new Node(7);
        root->right->right = new Node(20);
        root->left->right->left = new Node(6);
        root->right->right->left = new Node(18);
    }

    void preOrder() { preOrder(root);  cout << endl; }
    void inOrder() { inOrder(root);   cout << endl; }
    void postOrder() { postOrder(root); cout << endl; }

    int countNodes() const { return countNodes(root); }
};


// Часть 2. бинарное дерево поиска (BST)
// Операции: insert, search, inOrder + remove (все случаи)

class BST {
private:
    Node* root;

    Node* insert(Node* node, int key) {
        if (node == nullptr) return new Node(key);
        if (key < node->data)      node->left = insert(node->left, key);
        else if (key > node->data) node->right = insert(node->right, key);
        // если key == node->data — дубликат, ничего не делаем
        return node;
    }

    Node* search(Node* node, int key) {
        if (node == nullptr || node->data == key) return node;
        if (key < node->data) return search(node->left, key);
        return search(node->right, key);
    }

    // Минимальный элемент в поддереве (самый левый узел)
    Node* findMin(Node* node) {
        while (node && node->left != nullptr) node = node->left;
        return node;
    }

    // Удаление — все три случая
    Node* remove(Node* node, int key) {
        if (node == nullptr) return nullptr;

        if (key < node->data) {
            node->left = remove(node->left, key);
        }
        else if (key > node->data) {
            node->right = remove(node->right, key);
        }
        else {
            // Нашли узел — три случая
            // Случай 1: лист
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            // Случай 2: один потомок (левый отсутствует)
            else if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            // Случай 2: один потомок (правый отсутствует)
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // Случай 3: два потомка — заменяем на минимум из правого поддерева
            else {
                Node* minNode = findMin(node->right);
                node->data = minNode->data;
                node->right = remove(node->right, minNode->data);
            }
        }
        return node;
    }

    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }

    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    int height(Node* node) const {
        if (node == nullptr) return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    Node* findMax(Node* node) const {
        while (node && node->right != nullptr) node = node->right;
        return node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }

    void insert(int key) { root = insert(root, key); }
    bool search(int key) { return search(root, key) != nullptr; }
    void remove(int key) { root = remove(root, key); }

    void print() {
        inOrder(root);
        cout << endl;
    }

    int getHeight() const { return height(root); }

    // Минимум и максимум для Части 3
    bool getMin(int& out) const {
        Node* m = root;
        while (m && m->left) m = m->left;
        if (!m) return false;
        out = m->data;
        return true;
    }
    bool getMax(int& out) const {
        Node* m = findMax(root);
        if (!m) return false;
        out = m->data;
        return true;
    }
};


int main() {
    setlocale(LC_ALL, "rus");
    cout << "Часть 1. Бинарное дерево и обходы (вариант 1)\n";
    BinaryTree tree;
    tree.buildSample();

    cout << "Прямой обход (pre-order):  ";
    tree.preOrder();

    cout << "Симметричный (in-order):   ";
    tree.inOrder();

    cout << "Обратный (post-order):     ";
    tree.postOrder();

    cout << "Количество узлов:          " << tree.countNodes() << "\n\n";

   
    cout << "Часть 2. BST: insert, search, inOrder, remove\n";
    BST bst;
    int keys[] = { 50, 30, 70, 20, 40, 60, 80 };
    for (int k : keys) bst.insert(k);

    cout << "Симметричный обход BST: ";
    bst.print();

    cout << "Высота дерева: " << bst.getHeight() << endl;

    cout << "Поиск 40:  " << (bst.search(40) ? "найден" : "не найден") << endl;
    cout << "Поиск 100: " << (bst.search(100) ? "найден" : "не найден") << endl;

    // Удаляем узел с двумя потомками (30)
    cout << "Удаляем 30 (узел с двумя потомками)...\n";
    bst.remove(30);
    cout << "После удаления 30: ";
    bst.print();
    cout << endl;

    // Дополнительно: удаление листа и узла с одним потомком
    cout << "Удаляем 20 (лист)...\n";
    bst.remove(20);
    cout << "После удаления 20: ";
    bst.print();

    cout << "Удаляем 70 (узел с одним потомком)...\n";
    bst.remove(70);
    cout << "После удаления 70: ";
    bst.print();
    cout << endl;

    cout << "ЧАСТЬ 3. Применение BST\n";
    BST rnd;
    mt19937 rng(42);                      // фиксированный seed
    uniform_int_distribution<int> dist(1, 100);
    cout << "Исходные 20 случайных чисел: ";
    for (int i = 0; i < 20; ++i) {
        int v = dist(rng);
        cout << v << " ";
        rnd.insert(v);
    }
    cout << "\n";

    cout << "Симметричный обход (отсортировано): ";
    rnd.print();

    int mn, mx;
    if (rnd.getMin(mn)) cout << "Минимальный элемент: " << mn << "\n";
    if (rnd.getMax(mx)) cout << "Максимальный элемент: " << mx << "\n";
    cout << "Высота дерева: " << rnd.getHeight() << "\n";

    int toRemove;
    cout << "Введите элемент для удаления: ";
    if (cin >> toRemove) {
        if (rnd.search(toRemove)) {
            rnd.remove(toRemove);
            cout << "После удаления " << toRemove << ": ";
            rnd.print();
        }
        else {
            cout << "Элемент " << toRemove << " не найден в дереве.\n";
        }
    }

    return 0;
}