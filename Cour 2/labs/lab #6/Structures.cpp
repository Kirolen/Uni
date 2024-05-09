#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <thread>
#include <chrono>
#include <benchmark.h>

#define my_max(a, b) ((a > b) ? a : b)
#define bm_elem 40000
#define bm_max 100.0
#define bm_min -100.0

double randomDouble(double min, double max) {
    double randomFraction = static_cast<double>(rand()) / RAND_MAX;
    return min + randomFraction * (max - min);
}

struct Complex {
    double real;
    double imag;

    Complex(double r, double i) : real(r), imag(i) {}
    Complex() {
        real = INFINITY;
        imag = INFINITY;
    };

    double modulus() const {
        return real * real + imag * imag;
    }

    void print() {
        if (imag >= 0)
            std::cout << real << " + " << imag << "i";
        else
            std::cout << real << " - " << static_cast<int>(-imag) << "i";
        //std::cout << " - " << this->modulus();
    }

    bool operator==(const Complex& other) const {
        return (fabs(real - other.real) < 1e-9) && (fabs(imag - other.imag) < 1e-9);
    }

    bool operator>(const Complex& other) {
        return this->modulus() > other.modulus();
    }

    bool operator<(const Complex& other) {
        return this->modulus() < other.modulus();
    }


    bool operator<=(const Complex& other) {
        return this->modulus() <= other.modulus();
    }
};

class linked_list {
private:
    struct Node {
        Complex data;
        Node* next;

        Node(const Complex& d) : data(d), next(nullptr) {}
    };

    Node* head;

public:

    linked_list() {
        head = nullptr;
    };
    void add(const Complex& z) {
        Node* new_node = new Node(z);
        if (!head || head->data.modulus() >= z.modulus()) {
            new_node->next = head;
            head = new_node;
        }
        else {
            Node* current = head;
            while (current->next && current->next->data.modulus() < z.modulus()) {
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
    }

    void remove(const Complex& z) {
        Node* current = head;
        Node* prev = nullptr;
        while (current) {
            if (current->data.real == z.real && current->data.imag == z.imag) {
                if (prev) {
                    prev->next = current->next;
                    delete current;
                    current = prev->next; 
                }
                else {
                    Node* temp = current;
                    head = current->next;
                    delete temp;
                    current = head;
                }
            }
            else {
                prev = current;
                current = current->next;
            }
        }
    }

    void generate(int count, double min_modulus = -10.0, double max_modulus = 10.0) {
        for (int i = 0; i < count; ++i) {
            add(Complex(randomDouble(min_modulus, max_modulus), randomDouble(min_modulus, max_modulus)));
        }
    }

    void print() {
        Node* current = head;
        while (current) {
            current->data.print();
            if (current->next) std::cout << ", ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    bool find_element_by_val(const Complex& z) {
        Node* current = head;
        while (current) {
            if (current->data.real == z.real && current->data.imag == z.imag) {
                return current;
            }
            current = current->next;
        }
        return false;
    }

    std::vector<Complex> find_elements_by_range(double min_modulus, double max_modulus) {
        std::vector<Complex> result;
        Node* current = head;
        while (current) {
            double modulus = current->data.modulus();
            if (modulus >= min_modulus && modulus <= max_modulus) {
                result.push_back(current->data);
            }
            current = current->next;
        }
        return result;
    }

    ~linked_list() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
};
class array_list {
private:
    std::vector<Complex> elements;

public:
    array_list() {}

    void add(const Complex& z) {
        auto it = std::lower_bound(elements.begin(), elements.end(), z, [](const Complex& a, const Complex& b) {
             return a.modulus() < b.modulus();});
        elements.insert(it, z);
    }

    void remove(const Complex& z) {
        elements.erase(std::remove(elements.begin(), elements.end(), z), elements.end());
    }

    void generate(int count, double min_modulus = -10.0, double max_modulus = 10.0) {
        for (int i = 0; i < count; ++i) {
            add(Complex(randomDouble(min_modulus, max_modulus), randomDouble(min_modulus, max_modulus)));
        }
    }

    void print() {
        for (int i = 0; i < elements.size(); ++i) {
            elements[i].print();
            if (i != elements.size()-1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    bool find_element_by_val(const Complex& z) {
        return std::find(elements.begin(), elements.end(), z) != elements.end();
    }

    std::vector<Complex> find_elements_by_range(double minModulus, double maxModulus) {
        std::vector<Complex> result;
        for (const auto& elem : elements) {
            double modulus = elem.modulus();
            if (modulus >= minModulus && modulus <= maxModulus) {
                result.push_back(elem);
            }
        }
        return result;
    }
};
class binary_tree {
private:
    struct Node {
        Complex data;
        Node* left;
        Node* right;

        Node(const Complex& d) : data(d), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void add_recursive(Node*& current, const Complex& z) {
        if (!current) {
            current = new Node(z);
            return;
        }

        if (z.modulus() <= current->data.modulus()) {
            add_recursive(current->left, z);
        }
        else {
            add_recursive(current->right, z);
        }
    }

    void remove_recursive(Node*& current, const Complex& z) {
        if (!current) return;

        if (z.real == current->data.real && z.imag == current->data.imag) {
            if (!current->left) {
                Node* temp = current->right;
                delete current;
                current = temp;
            }
            else if (!current->right) {
                Node* temp = current->left;
                delete current;
                current = temp;
            }
            else {
                Node* temp = find_min(current->right);
                current->data = temp->data;
                remove_recursive(current->right, temp->data); 
            }
            remove_recursive(current, z);
        }
        else if (z.modulus() <= current->data.modulus()) {
            remove_recursive(current->left, z);
        }
        else {
            remove_recursive(current->right, z);
        }
    }

    Node* find_min(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    void print_recursive(Node* current) {
        if (!current) return;

        print_recursive(current->left);
        current->data.print();
        std::cout << "; ";
        std::cout << std::endl;
        print_recursive(current->right);
    }

    Node* find_el_by_val_recursive(Node* current, const Complex& z) {
        if (!current) return nullptr;

        if (current->data.real == z.real && current->data.imag == z.imag) {
            return current;
        }
        else if (z.modulus() <= current->data.modulus()) {
            return find_el_by_val_recursive(current->left, z);
        }
        else {
            return find_el_by_val_recursive(current->right, z);
        }
    }

    void find_el_by_range_recursive(Node* current, double min_modulus, double max_modulus, std::vector<Complex>& result) {
        if (!current) return;

        double modulus = current->data.modulus();
        if (modulus >= min_modulus && modulus <= max_modulus) {
            result.push_back(current->data);
        }
        if (modulus >= min_modulus) {
            find_el_by_range_recursive(current->left, min_modulus, max_modulus, result);
        }
        if (modulus <= max_modulus) {
            find_el_by_range_recursive(current->right, min_modulus, max_modulus, result);
        }
    }

    void destroy_tree(Node*& node) {
        if (node) {
            destroy_tree(node->left);
            destroy_tree(node->right);
            delete node;
            node = nullptr;
        }
    }

public:
    binary_tree() : root(nullptr) {}

    void add(const Complex& z) {
        add_recursive(root, z);
    }

    void remove(const Complex& z) {
        remove_recursive(root, z);
    }

    void generate(int count, double min_modulus = -10.0, double max_modulus = 10.0) {
        for (int i = 0; i < count; ++i) {
            add(Complex(randomDouble(min_modulus, max_modulus), randomDouble(min_modulus, max_modulus)));
        }
    }

    void print() {
        print_recursive(root);
    }

    bool find_element_by_val(const Complex& z) {
        return find_el_by_val_recursive(root, z);
    }

    std::vector<Complex> find_elements_by_range(double min_modulus, double max_modulus) {
        std::vector<Complex> result;
        find_el_by_range_recursive(root, min_modulus, max_modulus, result);
        return result;
    }

    ~binary_tree() {
        destroy_tree(root);
    }
};
class AVL_tree {
private:
    struct Node {
        Complex data;
        Node* left;
        Node* right;
        int height; 

        Node(const Complex& d) : data(d), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node) {
        if (!node) return 0;
        return node->height;
    }

    int get_balance(Node* node) {
        if (!node) return 0;
        return height(node->left) - height(node->right);
    }

    Node* rotate_right(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = 1 + my_max(height(y->left), height(y->right));
        x->height = 1 + my_max(height(x->left), height(x->right));

        return x;
    }

    Node* rotate_left(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + my_max(height(x->left), height(x->right));
        y->height = 1 + my_max(height(y->left), height(y->right));

        return y;
    }

    Node* insert_recursive(Node* current, const Complex& z) {
        if (!current) return new Node(z);

        if (z.modulus() <= current->data.modulus()) {
            current->left = insert_recursive(current->left, z);
        }
        else {
            current->right = insert_recursive(current->right, z);
        }

        current->height = 1 + my_max(height(current->left), height(current->right));

        int balance = get_balance(current);

        if (balance > 1 && z.modulus() <= current->left->data.modulus())
            return rotate_right(current);

        if (balance < -1 && z.modulus() > current->right->data.modulus())
            return rotate_left(current);

        if (balance > 1 && z.modulus() > current->left->data.modulus()) {
            current->left = rotate_left(current->left);
            return rotate_right(current);
        }

        if (balance < -1 && z.modulus() <= current->right->data.modulus()) {
            current->right = rotate_right(current->right);
            return rotate_left(current);
        }

        return current;
    }

    Node* min_value_node(Node* node) {
        Node* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }

    Node* remove_recursive(Node* root, const Complex& z) {
        if (!root) return root;

        if (z.modulus() < root->data.modulus())
            root->left = remove_recursive(root->left, z);
        else if (z.modulus() > root->data.modulus())
            root->right = remove_recursive(root->right, z);
        else {
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;
                delete temp;
            }
            else {
                Node* temp = min_value_node(root->right);
                root->data = temp->data;
                root->right = remove_recursive(root->right, temp->data);
            }

            root = remove_recursive(root, z);
        }

        if (!root) return root;

        root->height = 1 + my_max(height(root->left), height(root->right));

        int balance = get_balance(root);

        if (balance > 1 && get_balance(root->left) >= 0)
            return rotate_right(root);

        if (balance > 1 && get_balance(root->left) < 0) {
            root->left = rotate_left(root->left);
            return rotate_right(root);
        }

        if (balance < -1 && get_balance(root->right) <= 0)
            return rotate_left(root);

        if (balance < -1 && get_balance(root->right) > 0) {
            root->right = rotate_right(root->right);
            return rotate_left(root);
        }

        return root;
    }

    void print_recursive(Node* current) {
        if (!current) return;

        print_recursive(current->left);
        current->data.print();
        std::cout << "; ";
        std::cout << std::endl;
        print_recursive(current->right);
    }


    Node* find_el_by_val_recursive(Node* current, const Complex& z) {
        if (!current) return nullptr;

        if (current->data.real == z.real && current->data.imag == z.imag) {
            return current;
        }
        else if (z.modulus() <= current->data.modulus()) {
            return find_el_by_val_recursive(current->left, z);
        }
        else {
            return find_el_by_val_recursive(current->right, z);
        }
    }

    void find_el_by_range_recursive(Node* current, double min_modulus, double max_modulus, std::vector<Complex>& result) {
        if (!current) return;

        double modulus = current->data.modulus();
        if (modulus >= min_modulus && modulus <= max_modulus) {
            result.push_back(current->data);
        }
        if (modulus >= min_modulus) {
            find_el_by_range_recursive(current->left, min_modulus, max_modulus, result);
        }
        if (modulus <= max_modulus) {
            find_el_by_range_recursive(current->right, min_modulus, max_modulus, result);
        }
    }

    void destroy_tree(Node*& node) {
        if (node) {
            destroy_tree(node->left);
            destroy_tree(node->right);
            delete node;
            node = nullptr;
        }
    }


public:
    AVL_tree() : root(nullptr) {}

    void add(const Complex& z) {
        root = insert_recursive(root, z);
    }

    void remove(const Complex& z) {
        root = remove_recursive(root, z);
    }

    void generate(int count, double min_modulus = -10.0, double max_modulus = 10.0) {
        for (int i = 0; i < count; ++i) {
            add(Complex(randomDouble(min_modulus, max_modulus), randomDouble(min_modulus, max_modulus)));
        }
    }

    void print() {
        print_recursive(root);
    }

    bool find_element_by_val(const Complex& z) {
        return find_el_by_val_recursive(root, z);
    }

    std::vector<Complex> find_elements_by_range(double min_modulus, double max_modulus) {
        std::vector<Complex> result;
        find_el_by_range_recursive(root, min_modulus, max_modulus, result);
        return result;
    }

    ~AVL_tree() {
        destroy_tree(root);
    }
};
class B23Tree {
private:
    struct TreeNode {
        Complex data[2];
        int size; // number of data items
        TreeNode* children[3];
        TreeNode* parent; //TODO: can be removed

        TreeNode(Complex data1, TreeNode* parent = nullptr) {
            this->data[0] = data1;
            this->size = 1;
            children[0] = children[1] = children[2] = nullptr;
            this->parent = parent;
        }

        TreeNode(Complex data1, Complex data2, TreeNode* parent = nullptr) {
            assert(data1 <= data2);
            this->data[0] = data1;
            this->data[1] = data2;
            this->size = 2;
            children[0] = children[1] = children[2] = nullptr;
            this->parent = parent;
        }

        TreeNode(Complex data1, Complex data2, Complex data3, TreeNode* parent = nullptr) {
            assert(data1 <= data2);
            assert(data2 <= data3);

            this->data[0] = data2;
            this->size = 1;
            this->children[0] = new TreeNode(data1, this);
            this->children[1] = new TreeNode(data3, this);
            this->children[2] = nullptr;

            this->parent = parent;

        }

        void add_single_data(Complex data) {
            assert(size == 1);
            if (data > this->data[0]) {
                this->data[1] = data;
                size = 2;
            }
            else {
                this->data[1] = this->data[0];
                this->data[0] = data;
                size = 2;
            }
        }

        //return nullptr if added, else "4-node" (2-node with both children as 2-nodes)
        TreeNode* add_and_split(Complex new_data) {
            if (children[0] == nullptr) {
                if (size == 1) {
                    add_single_data(new_data);
                    return nullptr;
                }
                else {
                    if (new_data < data[0]) {
                        return new TreeNode(new_data, data[0], data[1]);
                    }
                    else if (new_data < data[1]) {
                        return new TreeNode(data[0], new_data, data[1]);
                    }
                    else {
                        return new TreeNode(data[0], data[1], new_data);
                    }
                }
            }
            TreeNode* extra = nullptr;
            if (size == 1) {
                if (new_data < data[0]) {
                    extra = children[0]->add_and_split(new_data);
                    if (!extra) { return nullptr; }
                    data[1] = data[0];
                    data[0] = extra->data[0];
                    children[2] = children[1];
                    children[0] = extra->children[0];
                    children[1] = extra->children[1];
                    size = 2;
                    return nullptr;
                }
                else {
                    extra = children[1]->add_and_split(new_data);
                    if (!extra) { return nullptr; }
                    data[1] = extra->data[0];
                    children[1] = extra->children[0];
                    children[2] = extra->children[1];
                    size = 2;
                    return nullptr;
                }
            }
            else {
                if (new_data < data[0]) {
                    extra = children[0]->add_and_split(new_data);
                    if (!extra) { return nullptr; }
                    TreeNode* result = new TreeNode(extra->data[0], data[0], data[1]);
                    result->children[0]->children[0] = extra->children[0];
                    result->children[0]->children[1] = extra->children[1];
                    result->children[1]->children[0] = children[1];
                    result->children[1]->children[1] = children[2];
                    return result;
                }
                else if (new_data < data[1]) {
                    extra = children[1]->add_and_split(new_data);
                    if (!extra) { return nullptr; }
                    TreeNode* result = new TreeNode(data[0], extra->data[0], data[1]);
                    result->children[0]->children[0] = children[0];
                    result->children[0]->children[1] = extra->children[0];
                    result->children[1]->children[0] = extra->children[1];
                    result->children[1]->children[1] = children[2];
                    return result;
                }
                else {
                    extra = children[2]->add_and_split(new_data);
                    if (!extra) { return nullptr; }
                    TreeNode* result = new TreeNode(data[0], data[1], extra->data[0]);
                    result->children[0]->children[0] = children[0];
                    result->children[0]->children[1] = children[1];
                    result->children[1]->children[0] = extra->children[0];
                    result->children[1]->children[1] = extra->children[1];
                    return result;
                }
            }
        }


        TreeNode* find_max_subtree() {
            if (children[2]) {
                assert(size == 2);
                return children[2]->find_max_subtree();
            }
            if (children[1]) {
                assert(size == 1);
                return children[1]->find_max_subtree();
            }
            assert(children[0] == nullptr);
            return this; // no children
        }

        Complex get_max_data() {
            if (size == 2) {
                return data[1];
            }
            else {
                return data[0];
            }
        }
        enum RemoveResult { Removed, NotFound, NeedParentRemove };

        //void rebalance(TreeNode* current_child, TreeNode* left_child, TreeNode* right_child) {
        // returns true if rebalance complete - no need to rebalance parent
        bool rebalance(int index_current_child) {
            assert(index_current_child < size + 1);
            TreeNode* current_child = children[index_current_child];
            assert(current_child);
            assert(current_child->size == 0);
            TreeNode* left_child = nullptr;
            if (index_current_child > 0) { left_child = children[index_current_child - 1]; }
            TreeNode* right_child = (index_current_child < size) ?
                children[index_current_child + 1] :
                nullptr;
            assert(left_child != nullptr || right_child != nullptr);

            if (left_child && left_child->size == 2) {
                assert(index_current_child - 1 >= 0); // can access data in next lines
                current_child->data[0] = this->data[index_current_child - 1];
                this->data[index_current_child - 1] = left_child->data[1];

                current_child->children[1] = current_child->children[0];
                current_child->children[0] = left_child->children[2];
                left_child->children[2] = nullptr;

                current_child->size = 1;
                left_child->size = 1;
                return true;
            }

            if (right_child && right_child->size == 2) {
                assert(index_current_child < this->size);
                current_child->data[0] = this->data[index_current_child]; // not index + 1
                this->data[index_current_child] = right_child->data[0];
                right_child->data[0] = right_child->data[1];

                current_child->children[1] = right_child->children[0];
                right_child->children[0] = right_child->children[1];
                right_child->children[1] = right_child->children[2];
                right_child->children[2] = nullptr;

                current_child->size = 1;
                right_child->size = 1;
                return true;
            }

            if (left_child) {
                assert(left_child->size == 1);
                assert(index_current_child - 1 >= 0);

                left_child->data[1] = this->data[index_current_child - 1];

                left_child->children[2] = current_child->children[0];

                left_child->size = 2;
                this->size--;

                if (this->size == 0) {
                    assert(this->children[0] == left_child);
                    assert(this->children[1] == current_child);
                    assert(this->children[2] == nullptr);
                    delete current_child;
                    this->children[1] = nullptr; // maybe not needed - this will be fixed in parent rebalance?
                    return false;
                }
                assert(this->size == 1);
                if (index_current_child == 1) {
                    this->data[0] = this->data[1];
                    assert(this->children[0] == left_child);
                    assert(this->children[1] == current_child);
                    delete current_child;
                    this->children[1] = this->children[2];
                    this->children[2] = nullptr;
                    return true;
                }
                assert(index_current_child == 2); { // to make this case similar to previous (same indent level)
                    assert(this->children[1] == left_child);
                    assert(this->children[2] == current_child);
                    delete current_child;
                    this->children[2] = nullptr;
                    return true;
                }


            }
            assert(right_child != nullptr);
            assert(right_child->size == 1);

            right_child->data[1] = right_child->data[0];
            right_child->data[0] = this->data[index_current_child]; // not index + 1!!
            assert(right_child->data[0] < right_child->data[1]);

            if (current_child->children[0] != nullptr) {
                assert(right_child->children[0] != nullptr);
                assert(right_child->children[1] != nullptr);
                // previous tree was balanced, if current_child had children - then right_child must have children too
                right_child->children[2] = right_child->children[1];
                right_child->children[1] = right_child->children[0];
                right_child->children[0] = current_child->children[0];

            }
            else {
                assert(current_child->children[0] == nullptr);
                assert(right_child->children[0] == nullptr);
                assert(right_child->children[1] == nullptr);
                // in this case, no need to copy nullptrs - previous code would work, but it is extra work
            }
            right_child->size = 2;
            this->size--;

            if (this->size == 0) {
                assert(this->children[0] == current_child);
                assert(this->children[1] == right_child);
                assert(this->children[2] == nullptr);
                delete current_child;
                this->children[0] = this->children[1];
                this->children[1] = nullptr; // maybe not needed - this will be fixed in parent rebalance?
                return false;
            }
            assert(this->size == 1);
            if (index_current_child == 0) {
                assert(this->children[0] == current_child);
                assert(this->children[1] == right_child);
                this->data[0] = this->data[1];
                delete current_child;
                this->children[0] = this->children[1];
                this->children[1] = this->children[2];
                this->children[2] = nullptr;
                return true;
            }
            assert(index_current_child == 1); {
                assert(this->children[1] == current_child);
                assert(this->children[2] == right_child);
                delete current_child;
                this->children[1] = this->children[2];
                this->children[2] = nullptr;
                return true;
            }

        }

        // can set size to 0, this means parent needs to fix it
        RemoveResult remove(Complex data_to_remove) {
            if (children[0] == nullptr) {
                if (size == 1) {
                    if (data[0] == data_to_remove) {
                        size = 0;
                        return NeedParentRemove;
                    }
                    else {
                        return NotFound;
                    }
                }
                else { // size == 2
                    if (data[0] == data_to_remove) {
                        data[0] = data[1];
                        size = 1;
                        return Removed;
                    }
                    else if (data[1] == data_to_remove) {
                        size = 1;
                        return Removed;
                    }
                    else {
                        return NotFound;
                    }
                }
            }
            if (size == 1) {
                if (data_to_remove < data[0]) {
                    RemoveResult result = children[0]->remove(data_to_remove);
                    if (result == NeedParentRemove) {
                        rebalance(0);
                        if (this->size == 0) { return NeedParentRemove; }
                        else { return Removed; }
                    }
                    else {
                        return result;
                    }
                }
                else if (data_to_remove > data[0]) {
                    RemoveResult result = children[1]->remove(data_to_remove);
                    if (result == NeedParentRemove) {
                        rebalance(1);
                        if (this->size == 0) { return NeedParentRemove; }
                        else { return Removed; }
                    }
                    else {
                        return result;
                    }
                }
                else { // removing our only data
                    TreeNode* prev = children[0]->find_max_subtree();
                    assert(prev->children[0] == nullptr);
                    Complex prev_data = prev->get_max_data();
                    data[0] = prev_data;
                    //prev->remove(prev_data, parent???)
                    RemoveResult result = this->children[0]->remove(prev_data); // because we know prev, but don't know full sequence of parents
                    assert(result != NotFound);
                    if (result == Removed) { return Removed; }
                    rebalance(0);
                    if (this->size == 0) { return NeedParentRemove; }
                    else { return Removed; }
                }
            }
            if (size == 2) {
                if (data_to_remove < data[0]) {
                    RemoveResult result = children[0]->remove(data_to_remove);
                    if (result == NeedParentRemove) {
                        rebalance(0);
                        assert(this->size > 0);
                        return Removed;
                    }
                    else {
                        return result;
                    }
                }
                else if (data_to_remove == data[0]) {
                    TreeNode* prev = children[0]->find_max_subtree();
                    assert(prev->children[0] == nullptr);
                    Complex prev_data = prev->get_max_data();
                    data[0] = prev_data;
                    RemoveResult result = this->children[0]->remove(prev_data); // because we know prev, but don't know full sequence of parents
                    assert(result != NotFound);
                    if (result == Removed) { return Removed; }
                    rebalance(0);
                    assert(this->size > 0);
                    return Removed;
                }
                else if (data_to_remove < data[1]) {
                    RemoveResult result = children[1]->remove(data_to_remove);
                    if (result == NeedParentRemove) {
                        rebalance(1);
                        assert(this->size > 0);
                        return Removed;
                    }
                    else {
                        return result;
                    }
                }
                else if (data_to_remove == data[1]) {
                    TreeNode* prev = children[1]->find_max_subtree();
                    assert(prev->children[0] == nullptr);
                    Complex prev_data = prev->get_max_data();
                    data[1] = prev_data;
                    RemoveResult result = this->children[1]->remove(prev_data); // because we know prev, but don't know full sequence of parents
                    assert(result != NotFound);
                    if (result == Removed) { return Removed; }
                    rebalance(1);
                    assert(this->size > 0);
                    return Removed;
                }
                else { // data_to_remove > data[1]
                    RemoveResult result = children[2]->remove(data_to_remove);
                    if (result == NeedParentRemove) {
                        rebalance(2);
                        assert(this->size > 0);
                        return Removed;
                    }
                    else {
                        return result;
                    }
                }
            }

        }

        void print_as_tree() {
            data[0].print();
            if (size == 2) {
                std::cout << ";"; data[1].print();
            }
            std::cout << "(";
            if (children[0]) {
                children[0]->print_as_tree();
            }
            std::cout << ",";
            if (children[1]) {
                children[1]->print_as_tree();
            }
            std::cout << ",";
            if (children[2]) {
                children[2]->print_as_tree();
            }
            std::cout << ")";
        }

        void print_in_order() {
            if (children[0]) {
                children[0]->print_in_order();
            }
            data[0].print();  std::cout << " ";
            if (children[1]) {
                children[1]->print_in_order();
            }
            if (size == 2) {
                data[1].print();  std::cout << " ";
                if (children[2]) {
                    children[2]->print_in_order();
                }
            }
        }


    };

    TreeNode* root;

    void find_elements_by_range_recursive(TreeNode* node, double min_modulus, double max_modulus, std::vector<Complex>& result) {
        if (!node) return;

        if (node->data[0].modulus() >= min_modulus && node->data[0].modulus() <= max_modulus) {
            result.push_back(node->data[0]);
        }
        if (node->size == 2 && node->data[1].modulus() >= min_modulus && node->data[1].modulus() <= max_modulus) {
            result.push_back(node->data[1]);
        }

        if (node->children[0] && node->data[0].modulus() >= min_modulus) {
            find_elements_by_range_recursive(node->children[0], min_modulus, max_modulus, result);
        }
        if (node->children[1] && node->data[0].modulus() <= max_modulus) {
            find_elements_by_range_recursive(node->children[1], min_modulus, max_modulus, result);
        }
        if (node->children[2] && node->size == 2 && node->data[1].modulus() <= max_modulus) {
            find_elements_by_range_recursive(node->children[2], min_modulus, max_modulus, result);
        }
    }
public:
    B23Tree() {
        root = nullptr;
    }

    void add(Complex data) {
        if (!root) {
            root = new TreeNode(data);
        }
        else {
            TreeNode* extra = root->add_and_split(data);
            if (extra) {
                root = extra;
            }
        }

    }

    bool remove(Complex data) {
        TreeNode::RemoveResult result = root->remove(data);
        if (result == TreeNode::NotFound) { return false; }
        if (result == TreeNode::Removed) { return true; }
        if (result == TreeNode::NeedParentRemove) {
            root = root->children[0];
            return true;
        }
    }

    void generate(int count, double min_modulus = -10.0, double max_modulus = 10.0) {
        for (int i = 0; i < count; ++i) {
            add(Complex(randomDouble(min_modulus, max_modulus), randomDouble(min_modulus, max_modulus)));
        }
    }

    void print_as_tree() {
        if (root) { root->print_as_tree(); }
        else { std::cout << "empty tree"; }
        std::cout << std::endl;
    }

    void print_in_order() {
        if (root) { root->print_in_order(); }
        else { std::cout << "empty tree"; }
        std::cout << std::endl;
    }

    void print_all() {
        print_as_tree();
        print_in_order();
    }

    bool find_element_by_val(Complex z) {
        TreeNode* current = root;
        while (current) {
            if (current->data[0] == z || (current->size == 2 && current->data[1] == z)) {
                return current;
            }
            else if (current->size == 1 && z < current->data[0]) {
                current = current->children[0];
            }
            else if (current->size == 2 && z < current->data[0]) {
                current = current->children[0];
            }
            else if (current->size == 2 && z < current->data[1]) {
                current = current->children[1];
            }
            else {
                current = current->children[2];
            }
        }
        return false;
    }


    std::vector<Complex> find_elements_by_range(double min_modulus, double max_modulus) {
        std::vector<Complex> result;
        find_elements_by_range_recursive(root, min_modulus, max_modulus, result);
        return result;
    }
};


void dem_mode();
void dem_mode_linked_list();
void dem_mode_array_list();
void dem_mode_binary_tree();
void dem_mode_AVL_tree();
void dem_mode_B23_tree();

void benchmark();
void benchmark_linked_list();
void benchmark_array_list();
void benchmark_binary_tree();
void benchmark_AVL_tree();
void benchmark_B23_tree();


enum DataStructureType {
    LINKED_LIST,
    ARRAY_LIST,
    BINARY_TREE,
    AVL_TREE,
    B23_TREE
};

struct interactive_struct
{
    linked_list ilink;
    array_list iarrl;
    binary_tree bin;
    AVL_tree avl;
    B23Tree b23;
};

void interactive_mode(DataStructureType type, interactive_struct& structs);

int main() {
    interactive_struct structs;
    char mode_choice;
    std::cout << "Choose a mode:" << std::endl;
    std::cout << "1. Demonstration Mode" << std::endl;
    std::cout << "2. Benchmark Mode" << std::endl;
    std::cout << "3. Interactive Mode" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> mode_choice;

    switch (mode_choice) {
    case '1':
        dem_mode();
        break;
    case '2':
        benchmark();
        break;
    case '3': {
        char data_structure_choice;
        std::cout << "Choose a data structure for interactive mode:" << std::endl;
        std::cout << "1. Linked List" << std::endl;
        std::cout << "2. Array List" << std::endl;
        std::cout << "3. Binary Tree" << std::endl;
        std::cout << "4. AVL Tree" << std::endl;
        std::cout << "5. B23 Tree" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> data_structure_choice;

        switch (data_structure_choice) {
        case '1':
            interactive_mode(LINKED_LIST, structs);
            break;
        case '2':
            interactive_mode(ARRAY_LIST, structs);
            break;
        case '3':
            interactive_mode(BINARY_TREE, structs);
            break;
        case '4':
            interactive_mode(AVL_TREE, structs);
            break;
        case '5':
            interactive_mode(B23_TREE, structs);
            break;
        default:
            std::cout << "Invalid choice. Exiting..." << std::endl;
            break;
        }
        break;
    }
    default:
        std::cout << "Invalid choice. Exiting..." << std::endl;
        break;
    }

    return 0;
}

void interactive_mode(DataStructureType type, interactive_struct& structs) {
    char choice;
    do {
        std::cout << "Choose an option:" << std::endl;
        std::cout << "1. Add a complex number" << std::endl;
        std::cout << "2. Remove a complex number" << std::endl;
        std::cout << "3. Generate random structure with complex numbers" << std::endl;
        std::cout << "4. Print the list" << std::endl;
        std::cout << "5. Find element by value" << std::endl;
        std::cout << "6. Find elements by range" << std::endl;
        std::cout << "7. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case '1': {
            double real, imag;
            std::cout << "Enter real part: ";
            std::cin >> real;
            std::cout << "Enter imaginary part: ";
            std::cin >> imag;
            if (type == LINKED_LIST)
                structs.ilink.add(Complex(real, imag));
            else if (type == ARRAY_LIST)
                structs.iarrl.add(Complex(real, imag));
            else if (type == BINARY_TREE)
                structs.bin.add(Complex(real, imag));
            else if (type == AVL_TREE)
                structs.avl.add(Complex(real, imag));
            else if (type == B23_TREE)
                structs.b23.add(Complex(real, imag));
            break;
        }
        case '2': {
            double real, imag;
            std::cout << "Enter real part: ";
            std::cin >> real;
            std::cout << "Enter imaginary part: ";
            std::cin >> imag;
            if (type == LINKED_LIST)
                structs.ilink.remove(Complex(real, imag));
            else if (type == ARRAY_LIST)
                structs.iarrl.remove(Complex(real, imag));
            else if (type == BINARY_TREE)
                structs.bin.remove(Complex(real, imag));
            else if (type == AVL_TREE)
                structs.avl.remove(Complex(real, imag));
            else if (type == B23_TREE)
                structs.b23.remove(Complex(real, imag));
            break;
        }
        case '3': {
            int count;
            double min_modulus, max_modulus;
            std::cout << "Enter count: ";
            std::cin >> count;
            std::cout << "Enter min real and img: ";
            std::cin >> min_modulus;
            std::cout << "Enter max real and img: ";
            std::cin >> max_modulus;
            if (type == LINKED_LIST)
                structs.ilink.generate(count, min_modulus, max_modulus);
            else if (type == ARRAY_LIST)
                structs.iarrl.generate(count, min_modulus, max_modulus);
            else if (type == BINARY_TREE)
                structs.bin.generate(count, min_modulus, max_modulus);
            else if (type == AVL_TREE)
                structs.avl.generate(count, min_modulus, max_modulus);
            else if (type == B23_TREE)
                structs.b23.generate(count, min_modulus, max_modulus);
            break;
        }
        case '4':
            if (type == LINKED_LIST)
                structs.ilink.print();
            else if (type == ARRAY_LIST)
                structs.iarrl.print();
            else if (type == BINARY_TREE)
                structs.bin.print();
            else if (type == AVL_TREE)
                structs.avl.print();
            else if (type == B23_TREE)
                structs.b23.print_all();
            break;
        case '5': {
            double real, imag;
            std::cout << "Enter real part: ";
            std::cin >> real;
            std::cout << "Enter imaginary part: ";
            std::cin >> imag;
            bool found;
            if (type == LINKED_LIST)
                found = structs.ilink.find_element_by_val(Complex(real, imag));
            else if (type == ARRAY_LIST)
                found = structs.iarrl.find_element_by_val(Complex(real, imag));
            else if (type == BINARY_TREE)
                found = structs.bin.find_element_by_val(Complex(real, imag));
            else if (type == AVL_TREE)
                found = structs.avl.find_element_by_val(Complex(real, imag));
            else if (type == B23_TREE)
                found = structs.b23.find_element_by_val(Complex(real, imag));

            if (found)
                std::cout << "Element is found!" << std::endl;
            else
                std::cout << "Element isn`t found!" << std::endl;
            break;
        }
        case '6': {
            double min_modulus, max_modulus;
            std::cout << "Enter min modulus: ";
            std::cin >> min_modulus;
            std::cout << "Enter max modulus: ";
            std::cin >> max_modulus;
            std::vector<Complex> allElem;
            if (type == LINKED_LIST)
                allElem = structs.ilink.find_elements_by_range(min_modulus, max_modulus);
            else if (type == ARRAY_LIST)
                allElem = structs.iarrl.find_elements_by_range(min_modulus, max_modulus);
            else if (type == BINARY_TREE)
                allElem = structs.bin.find_elements_by_range(min_modulus, max_modulus);
            else if (type == AVL_TREE)
                allElem = structs.avl.find_elements_by_range(min_modulus, max_modulus);
            else if (type == B23_TREE)
                allElem = structs.b23.find_elements_by_range(min_modulus, max_modulus);

            if (allElem.size() != 0) {
                for (auto elem : allElem) {
                    elem.print();
                    std::cout << std::endl;
                }
            }
            else {
                std::cout << "Elements wasn`t founded" << std::endl;
            }
            break;
        }
        case '7':
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != '7');
}

void dem_mode() {
    dem_mode_linked_list();
    std::cout << std::endl;
    dem_mode_array_list();
    std::cout << std::endl;
    dem_mode_binary_tree();
    std::cout << std::endl;
    dem_mode_AVL_tree();
    std::cout << std::endl;
    dem_mode_B23_tree();
    std::cout << std::endl;
}
void dem_mode_linked_list() {
    std::cout << "--- Linked List---\n\n";
    linked_list myList;

    std::cout << "Generate some random Complex numbers and add them to the list\n";
    myList.generate(5);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the list after generation:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myList.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex adding(2.5, 3.5);
    std::cout << "Adding Complex number: " << adding.real << " + " << adding.imag << "i" << std::endl;
    myList.add(adding);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the list after adding:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myList.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex toRemove(2.5, 3.5);
    std::cout << "Removing Complex number: " << toRemove.real << " + " << toRemove.imag << "i" << std::endl;
    myList.remove(toRemove);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the list after removing:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myList.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex toFind(1.2, -4.5);
    std::cout << "Checking if Complex number: " << toFind.real << " + " << toFind.imag << "i" << " exists in the list: ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (myList.find_element_by_val(toFind)) {
        std::cout << "Found!" << std::endl;
    }
    else {
        std::cout << "Not found!" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    double minMod = 1.0;
    double maxMod = 5.0;
    std::vector<Complex> withinRange = myList.find_elements_by_range(minMod, maxMod);

    std::cout << "Complex numbers within the range [" << minMod << ", " << maxMod << "]:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for (const auto& complexNum : withinRange) {
        std::cout << complexNum.real << " + " << complexNum.imag << "i" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
}
void dem_mode_array_list() {
    std::cout << "--- Array List ---\n\n";
    array_list myList;

    std::cout << "Generate some random Complex numbers and add them to the list\n";
    myList.generate(5);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the list after generation:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myList.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex adding(2.5, 3.5);
    std::cout << "Adding Complex number: " << adding.real << " + " << adding.imag << "i" << std::endl;
    myList.add(adding);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the list after adding:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myList.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex toRemove(2.5, 3.5);
    std::cout << "Removing Complex number: " << toRemove.real << " + " << toRemove.imag << "i" << std::endl;
    myList.remove(toRemove);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the list after removing:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myList.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex toFind(1.2, -4.5);
    std::cout << "Checking if Complex number: " << toFind.real << " + " << toFind.imag << "i" << " exists in the list: ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (myList.find_element_by_val(toFind)) {
        std::cout << "Found!" << std::endl;
    }
    else {
        std::cout << "Not found!" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    double minMod = 1.0;
    double maxMod = 5.0;
    std::vector<Complex> withinRange = myList.find_elements_by_range(minMod, maxMod);

    std::cout << "Complex numbers within the range [" << minMod << ", " << maxMod << "]:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for (const auto& complexNum : withinRange) {
        std::cout << complexNum.real << " + " << complexNum.imag << "i" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
}
void dem_mode_binary_tree() {
    std::cout << "--- Binary Tree ---\n\n";
    binary_tree myTree;

    std::cout << "Generate some random Complex numbers and add them to the tree\n";
    myTree.add(Complex(2.0, 3.0));
    myTree.add(Complex(-1.5, 4.5));
    myTree.add(Complex(0.0, -2.0));
    myTree.add(Complex(3.5, 1.2));
    myTree.add(Complex(-4.0, -3.0));
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the tree after addition:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myTree.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex adding(2.5, 3.5);
    std::cout << "Adding Complex number: " << adding.real << " + " << adding.imag << "i" << std::endl;
    myTree.add(adding);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the tree after adding:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myTree.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex toRemove(-1.5, 4.5);
    std::cout << "Removing Complex number: " << toRemove.real << " + " << toRemove.imag << "i" << std::endl;
    myTree.remove(toRemove);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the tree after removing:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myTree.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex toFind(3.5, 1.2);
    std::cout << "Checking if Complex number: " << toFind.real << " + " << toFind.imag << "i" << " exists in the tree: ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (myTree.find_element_by_val(toFind)) {
        std::cout << "Found!" << std::endl;
    }
    else {
        std::cout << "Not found!" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    double minMod = 2.0;
    double maxMod = 5.0;
    std::vector<Complex> withinRange = myTree.find_elements_by_range(minMod, maxMod);

    std::cout << "Complex numbers within the range [" << minMod << ", " << maxMod << "]:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for (const auto& complexNum : withinRange) {
        std::cout << complexNum.real << " + " << complexNum.imag << "i" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
}
void dem_mode_AVL_tree() {
    std::cout << "--- AVL Tree ---\n\n";
    AVL_tree myTree;

    std::cout << "Generate some random Complex numbers and add them to the tree\n";
    myTree.add(Complex(2.0, 3.0));
    myTree.add(Complex(-1.5, 4.5));
    myTree.add(Complex(0.0, -2.0));
    myTree.add(Complex(3.5, 1.2));
    myTree.add(Complex(-4.0, -3.0));
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the tree after addition:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myTree.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex adding(2.5, 3.5);
    std::cout << "Adding Complex number: " << adding.real << " + " << adding.imag << "i" << std::endl;
    myTree.add(adding);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the tree after adding:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myTree.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex toRemove(-1.5, 4.5);
    std::cout << "Removing Complex number: " << toRemove.real << " + " << toRemove.imag << "i" << std::endl;
    myTree.remove(toRemove);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the tree after removing:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myTree.print();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex toFind(3.5, 1.2);
    std::cout << "Checking if Complex number: " << toFind.real << " + " << toFind.imag << "i" << " exists in the tree: ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (myTree.find_element_by_val(toFind)) {
        std::cout << "Found!" << std::endl;
    }
    else {
        std::cout << "Not found!" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    double minMod = 2.0;
    double maxMod = 5.0;
    std::vector<Complex> withinRange = myTree.find_elements_by_range(minMod, maxMod);

    std::cout << "Complex numbers within the range [" << minMod << ", " << maxMod << "]:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for (const auto& complexNum : withinRange) {
        std::cout << complexNum.real << " + " << complexNum.imag << "i" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
}
void dem_mode_B23_tree() {
    std::cout << "--- 2-3 Tree ---\n\n";
    B23Tree myTree;

    std::cout << "Generate some random Complex numbers and add them to the tree\n";
    myTree.add(Complex(2.0, 3.0));
    myTree.add(Complex(-1.5, 4.5));
    myTree.add(Complex(0.0, -2.0));
    myTree.add(Complex(3.5, 1.2));
    myTree.add(Complex(-4.0, -3.0));
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the tree after addition:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myTree.print_all();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex adding(2.5, 3.5);
    std::cout << "Adding Complex number: " << adding.real << " + " << adding.imag << "i" << std::endl;
    myTree.add(adding);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the tree after adding:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myTree.print_all();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex toRemove(-1.5, 4.5);
    std::cout << "Removing Complex number: " << toRemove.real << " + " << toRemove.imag << "i" << std::endl;
    myTree.remove(toRemove);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print the contents of the tree after removing:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myTree.print_all();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Complex toFind(3.5, 1.2);
    std::cout << "Checking if Complex number: " << toFind.real << " + " << toFind.imag << "i" << " exists in the tree: ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (myTree.find_element_by_val(toFind)) {
        std::cout << "Found!" << std::endl;
    }
    else {
        std::cout << "Not found!" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    double minMod = 2.0;
    double maxMod = 5.0;
    std::vector<Complex> withinRange = myTree.find_elements_by_range(minMod, maxMod);

    std::cout << "Complex numbers within the range [" << minMod << ", " << maxMod << "]:" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for (const auto& complexNum : withinRange) {
        std::cout << complexNum.real << " + " << complexNum.imag << "i" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void benchmark() {
    benchmark_linked_list();
    std::cout << std::endl;
    benchmark_array_list();
    std::cout << std::endl;
    benchmark_binary_tree();
    std::cout << std::endl;
    benchmark_AVL_tree();
    std::cout << std::endl;
    benchmark_B23_tree();
    std::cout << std::endl;
}
void benchmark_linked_list() {

    linked_list list;
    auto start_gen = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Generate random linked list");
        list.generate(bm_elem, bm_min, bm_max);
    }
    auto end_gen = std::chrono::high_resolution_clock::now();
    auto duration_gen = std::chrono::duration_cast<std::chrono::milliseconds>(end_gen - start_gen);
    std::cout << "Time taken by generate function: " << duration_gen.count() << " milliseconds" << std::endl << std::endl;

    auto start_fe = std::chrono::high_resolution_clock::now();
    {
        Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
        BenchmarkMax memoryBenchmark("Find random elem in list: ");
        list.find_element_by_val(rand);
    }
    auto end_fe = std::chrono::high_resolution_clock::now();
    auto duration_fe = std::chrono::duration_cast<std::chrono::milliseconds>(end_fe - start_fe);
    std::cout << "Time taken by find_element__by_val function: " << duration_fe.count() << " milliseconds" << std::endl << std::endl;

    auto start_fer = std::chrono::high_resolution_clock::now();
    {
        double rand_min = randomDouble(bm_min, bm_max);
        double rand_max = randomDouble(bm_min, bm_max);
        if (rand_min > rand_max) std::swap(rand_min, rand_max);
        BenchmarkMax memoryBenchmark("Find random elem in range in list: ");
        list.find_elements_by_range(rand_min, rand_max);
    }
    auto end_fer = std::chrono::high_resolution_clock::now();
    auto duration_fer = std::chrono::duration_cast<std::chrono::milliseconds>(end_fer - start_fer);
    std::cout << "Time taken by find_elements_by_range function: " << duration_fer.count() << " milliseconds" << std::endl << std::endl;

    auto start_add = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Add random elem in list: ");
        for (int i = 0; i < bm_elem * 0.05; ++i) {
            Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
            list.add(rand);
        }
    }
    auto end_add = std::chrono::high_resolution_clock::now();
    auto duration_add = std::chrono::duration_cast<std::chrono::milliseconds>(end_add - start_add);
    std::cout << "Time taken by add function: " << duration_add.count() << " milliseconds" << std::endl << std::endl;

    auto start_del = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Remove random elem in list: ");
        for (int i = 0; i < bm_elem * 0.05; ++i) {
            Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
            list.remove(rand);
        }
    }
    auto end_del = std::chrono::high_resolution_clock::now();
    auto duration_del = std::chrono::duration_cast<std::chrono::milliseconds>(end_del - start_del);
    std::cout << "Time taken by remove function: " << duration_add.count() << " milliseconds" << std::endl << std::endl;


}
void benchmark_array_list() {
    std::cout << "Benchmark array list!" << std::endl;
    array_list list; 
    auto start_gen = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Generate random array list");
        list.generate(bm_elem, bm_min, bm_max);
    }
    auto end_gen = std::chrono::high_resolution_clock::now();
    auto duration_gen = std::chrono::duration_cast<std::chrono::milliseconds>(end_gen - start_gen);
    std::cout << "Time taken by generate function: " << duration_gen.count() << " milliseconds" << std::endl << std::endl;

    auto start_fe = std::chrono::high_resolution_clock::now();
    {
        Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
        BenchmarkMax memoryBenchmark("Find random elem in list: ");
        list.find_element_by_val(rand);
    }
    auto end_fe = std::chrono::high_resolution_clock::now();
    auto duration_fe = std::chrono::duration_cast<std::chrono::milliseconds>(end_fe - start_fe);
    std::cout << "Time taken by find_element__by_val function: " << duration_fe.count() << " milliseconds" << std::endl << std::endl;

    auto start_fer = std::chrono::high_resolution_clock::now();
    {
        double rand_min = randomDouble(bm_min, bm_max);
        double rand_max = randomDouble(bm_min, bm_max);
        if (rand_min > rand_max) std::swap(rand_min, rand_max);
        BenchmarkMax memoryBenchmark("Find random elem in range in list: ");
        list.find_elements_by_range(rand_min, rand_max);
    }
    auto end_fer = std::chrono::high_resolution_clock::now();
    auto duration_fer = std::chrono::duration_cast<std::chrono::milliseconds>(end_fer - start_fer);
    std::cout << "Time taken by find_elements_by_range function: " << duration_fer.count() << " milliseconds" << std::endl << std::endl;

    auto start_add = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Add random elem in list: ");
        for (int i = 0; i < bm_elem * 0.05; ++i) {
            Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
            list.add(rand);
        }
    }
    auto end_add = std::chrono::high_resolution_clock::now();
    auto duration_add = std::chrono::duration_cast<std::chrono::milliseconds>(end_add - start_add);
    std::cout << "Time taken by add function: " << duration_add.count() << " milliseconds" << std::endl << std::endl;

    auto start_del = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Remove random elem in list: ");
        for (int i = 0; i < bm_elem * 0.05; ++i) {
            Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
            list.remove(rand);
        }
    }
    auto end_del = std::chrono::high_resolution_clock::now();
    auto duration_del = std::chrono::duration_cast<std::chrono::milliseconds>(end_del - start_del);
    std::cout << "Time taken by remove function: " << duration_add.count() << " milliseconds" << std::endl << std::endl;

}
void benchmark_binary_tree() {
    std::cout << "Benchmark binary tree!" << std::endl;
    binary_tree tree;
    auto start_gen = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Generate random tree");
        tree.generate(bm_elem, bm_min, bm_max);
    }
    auto end_gen = std::chrono::high_resolution_clock::now();
    auto duration_gen = std::chrono::duration_cast<std::chrono::milliseconds>(end_gen - start_gen);
    std::cout << "Time taken by generate function: " << duration_gen.count() << " milliseconds" << std::endl << std::endl;

    auto start_fe = std::chrono::high_resolution_clock::now();
    {
        Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
        BenchmarkMax memoryBenchmark("Find random elem in tree: ");
        tree.find_element_by_val(rand);
    }
    auto end_fe = std::chrono::high_resolution_clock::now();
    auto duration_fe = std::chrono::duration_cast<std::chrono::milliseconds>(end_fe - start_fe);
    std::cout << "Time taken by find_element__by_val function: " << duration_fe.count() << " milliseconds" << std::endl << std::endl;

    auto start_fer = std::chrono::high_resolution_clock::now();
    {
        double rand_min = randomDouble(bm_min, bm_max);
        double rand_max = randomDouble(bm_min, bm_max);
        if (rand_min > rand_max) std::swap(rand_min, rand_max);
        BenchmarkMax memoryBenchmark("Find random elem in range in tree: ");
        tree.find_elements_by_range(rand_min, rand_max);
    }
    auto end_fer = std::chrono::high_resolution_clock::now();
    auto duration_fer = std::chrono::duration_cast<std::chrono::milliseconds>(end_fer - start_fer);
    std::cout << "Time taken by find_elements_by_range function: " << duration_fer.count() << " milliseconds" << std::endl << std::endl;

    auto start_add = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Add random elem in tree: ");
        for (int i = 0; i < bm_elem * 0.05; ++i) {
            Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
            tree.add(rand);
        }
    }
    auto end_add = std::chrono::high_resolution_clock::now();
    auto duration_add = std::chrono::duration_cast<std::chrono::milliseconds>(end_add - start_add);
    std::cout << "Time taken by add function: " << duration_add.count() << " milliseconds" << std::endl << std::endl;

    auto start_del = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Remove random elem in tree: ");
        for (int i = 0; i < bm_elem * 0.05; ++i) {
            Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
            tree.remove(rand);
        }
    }
    auto end_del = std::chrono::high_resolution_clock::now();
    auto duration_del = std::chrono::duration_cast<std::chrono::milliseconds>(end_del - start_del);
    std::cout << "Time taken by remove function: " << duration_add.count() << " milliseconds" << std::endl << std::endl;
}
void benchmark_AVL_tree() {
    std::cout << "Benchmark AVL tree!" << std::endl;
    AVL_tree tree;
    auto start_gen = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Generate random array tree");
        tree.generate(bm_elem, bm_min, bm_max);
    }
    auto end_gen = std::chrono::high_resolution_clock::now();
    auto duration_gen = std::chrono::duration_cast<std::chrono::milliseconds>(end_gen - start_gen);
    std::cout << "Time taken by generate function: " << duration_gen.count() << " milliseconds" << std::endl << std::endl;

    auto start_fe = std::chrono::high_resolution_clock::now();
    {
        Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
        BenchmarkMax memoryBenchmark("Find random elem in tree: ");
        tree.find_element_by_val(rand);
    }
    auto end_fe = std::chrono::high_resolution_clock::now();
    auto duration_fe = std::chrono::duration_cast<std::chrono::milliseconds>(end_fe - start_fe);
    std::cout << "Time taken by find_element__by_val function: " << duration_fe.count() << " milliseconds" << std::endl << std::endl;

    auto start_fer = std::chrono::high_resolution_clock::now();
    {
        double rand_min = randomDouble(bm_min, bm_max);
        double rand_max = randomDouble(bm_min, bm_max);
        if (rand_min > rand_max) std::swap(rand_min, rand_max);
        BenchmarkMax memoryBenchmark("Find random elem in range in tree: ");
        tree.find_elements_by_range(rand_min, rand_max);
    }
    auto end_fer = std::chrono::high_resolution_clock::now();
    auto duration_fer = std::chrono::duration_cast<std::chrono::milliseconds>(end_fer - start_fer);
    std::cout << "Time taken by find_elements_by_range function: " << duration_fer.count() << " milliseconds" << std::endl << std::endl;

    auto start_add = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Add random elem in tree: ");
        for (int i = 0; i < bm_elem * 0.05; ++i) {
            Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
            tree.add(rand);
        }
    }
    auto end_add = std::chrono::high_resolution_clock::now();
    auto duration_add = std::chrono::duration_cast<std::chrono::milliseconds>(end_add - start_add);
    std::cout << "Time taken by add function: " << duration_add.count() << " milliseconds" << std::endl << std::endl;

    auto start_del = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Remove random elem in tree: ");
        for (int i = 0; i < bm_elem * 0.05; ++i) {
            Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
            tree.remove(rand);
        }
    }
    auto end_del = std::chrono::high_resolution_clock::now();
    auto duration_del = std::chrono::duration_cast<std::chrono::milliseconds>(end_del - start_del);
    std::cout << "Time taken by remove function: " << duration_add.count() << " milliseconds" << std::endl << std::endl;
}
void benchmark_B23_tree() {
    std::cout << "Benchmark AVL tree!" << std::endl;
    B23Tree tree;
    auto start_gen = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Generate random array tree");
        tree.generate(bm_elem, bm_min, bm_max);
    }
    auto end_gen = std::chrono::high_resolution_clock::now();
    auto duration_gen = std::chrono::duration_cast<std::chrono::milliseconds>(end_gen - start_gen);
    std::cout << "Time taken by generate function: " << duration_gen.count() << " milliseconds" << std::endl << std::endl;

    auto start_fe = std::chrono::high_resolution_clock::now();
    {
        Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
        BenchmarkMax memoryBenchmark("Find random elem in tree: ");
        tree.find_element_by_val(rand);
    }
    auto end_fe = std::chrono::high_resolution_clock::now();
    auto duration_fe = std::chrono::duration_cast<std::chrono::milliseconds>(end_fe - start_fe);
    std::cout << "Time taken by find_element__by_val function: " << duration_fe.count() << " milliseconds" << std::endl << std::endl;

    auto start_fer = std::chrono::high_resolution_clock::now();
    {
        double rand_min = randomDouble(bm_min, bm_max);
        double rand_max = randomDouble(bm_min, bm_max);
        if (rand_min > rand_max) std::swap(rand_min, rand_max);
        BenchmarkMax memoryBenchmark("Find random elem in range in tree: ");
        tree.find_elements_by_range(rand_min, rand_max);
    }
    auto end_fer = std::chrono::high_resolution_clock::now();
    auto duration_fer = std::chrono::duration_cast<std::chrono::milliseconds>(end_fer - start_fer);
    std::cout << "Time taken by find_elements_by_range function: " << duration_fer.count() << " milliseconds" << std::endl << std::endl;

    auto start_add = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Add random elem in tree: ");
        for (int i = 0; i < bm_elem * 0.05; ++i) {
            Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
            tree.add(rand);
        }
    }
    auto end_add = std::chrono::high_resolution_clock::now();
    auto duration_add = std::chrono::duration_cast<std::chrono::milliseconds>(end_add - start_add);
    std::cout << "Time taken by add function: " << duration_add.count() << " milliseconds" << std::endl << std::endl;

    auto start_del = std::chrono::high_resolution_clock::now();
    {
        BenchmarkMax memoryBenchmark("Remove random elem in tree: ");
        for (int i = 0; i < bm_elem * 0.05; ++i) {
            Complex rand(randomDouble(bm_min, bm_max), randomDouble(bm_min, bm_max));
            tree.remove(rand);
        }
    }
    auto end_del = std::chrono::high_resolution_clock::now();
    auto duration_del = std::chrono::duration_cast<std::chrono::milliseconds>(end_del - start_del);
    std::cout << "Time taken by remove function: " << duration_add.count() << " milliseconds" << std::endl << std::endl;
}

