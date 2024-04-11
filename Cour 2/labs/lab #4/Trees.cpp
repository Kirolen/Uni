// Блок 1: 2, 3, 4,
// Блок 2: 9, 11, 12
// Блок 3: 13, 15 
// Блок 4: 18, 19
// Блок 5: 20
// Блок 6: 23

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <stack>
#include <string>
#include <cmath>
#include <chrono>
#include <thread>

#define M_Pi 3.141592
#define M_e 2.7182818

class N_ary_Tree
{
public:
    class tree_node
    {
    public:
        int value;
        std::vector<tree_node*> childrens;

        tree_node(int value) : value(value) {};
    };

    tree_node* root;

    //Block 0:
    N_ary_Tree() : root(nullptr) {}

    //Block 1:
    tree_node*& el_by_path(const std::vector<int>& _path) {
        tree_node* current_node = root;

        for (int index : _path) {
            if (index < 0 || index >= current_node->childrens.size())
                throw std::out_of_range("Invalid path");

            current_node = current_node->childrens[index];
        }
        return current_node;
    }

    void add(const int _value, const std::vector<int>& _path = {}) {
        if (root == nullptr && _path.empty()) {
            root = new tree_node(_value);
        }
        else if (root == nullptr && !_path.empty()) {
            std::cout << "Error path: Tree is empty\n";
        }
        else {
            tree_node* parent_node = el_by_path(_path);
            tree_node* new_child = new tree_node(_value);
            parent_node->childrens.push_back(new_child);
        }
    }

    void add_proc(const int _value, double p, tree_node* parent_node = nullptr) {
        if (!parent_node) {
            if (p == 0) return;
            if (!root && static_cast<double>(rand()) / RAND_MAX < p) {
                root = new tree_node(_value);
                return;
            }
            parent_node = root;
        }

        if (static_cast<double>(rand()) / RAND_MAX < p)
            parent_node->childrens.push_back(new tree_node(_value));
        else if (!parent_node->childrens.empty()) {
            add_proc(_value, 1-p, parent_node->childrens[rand() % parent_node->childrens.size()]);
        }
    }

    //Block 2:
    void print_tree(tree_node* node = nullptr, int retreats = 0) {
        if (!node) {
            if (!root) {
                std::cout << "Error 1: Tree is empty\n";
                return;
            }
            node = root;
        }

        for (int i = 0; i < retreats - 1; ++i)
            std::cout << "     ";

        if (retreats > 0)
            std::cout << "+--- ";

        std::cout << node->value << std::endl;

        for (size_t i = 0; i < node->childrens.size(); i++)
            print_tree(node->childrens[i], retreats + 1);
    }

    void print_tree_with_path(tree_node* node = nullptr, int retreats = 0, const std::vector<int> _path = {}) {
        if (!node) {
            if (!root) {
                std::cout << "Error 1: Tree is empty\n";
                return;
            }

            node = root;
        }

        std::cout << "Path: ";
        print_path(_path);
        std::cout << std::setw(40 - 4 * _path.size() - (_path.empty() ? 7 : 0));
        for (int i = 0; i < retreats - 1; ++i)
            std::cout << "     ";

        if (retreats > 0)
            std::cout << "+--- ";

        std::cout << node->value << std::endl;

        for (size_t i = 0; i < node->childrens.size(); i++)
        {
            std::vector<int> child_path = _path; 
            child_path.push_back(i);
            print_tree_with_path(node->childrens[i], retreats + 1, child_path);

        }
    }

    void interactive_tree_view() {
        tree_node* current_node = root;
        std::vector<int> _path;

        if (!root) {
            std::cout << "Error 1: Tree is empty\n";
            return;
        }
         
        while (true) {
            std::cout << "\nCurrent Node: " << current_node->value << std::endl;
            std::cout << "Children: ";
            for (size_t i = 0; i < current_node->childrens.size(); i++) {
                std::cout << current_node->childrens[i]->value;
                if (i < current_node->childrens.size() - 1)
                    std::cout << ", ";
            }
            std::cout << std::endl;

            std::cout << "\nOptions:\n"
                "1. Go deeper (select a child)\n"
                "2. Go shallower (go back to parent)\n"
                "0. Exit\n"
                "Enter your choice: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
            case 0:
                std::cout << "Exiting interactive exploration.\n";
                return;
            case 1:
                if (!current_node->childrens.empty()) {
                    int childIndex;
                    std::cout << "Enter the index of the child to select: ";
                    std::cin >> childIndex;
                    if (childIndex >= 0 && childIndex < current_node->childrens.size()) {
                        _path.push_back(childIndex);
                        current_node = current_node->childrens[childIndex];
                    }
                    else
                        std::cout << "Invalid child index. Please try again.\n";
                }
                else
                    std::cout << "No children to go deeper. Please try again.\n";

                break;
            case 2:
                if (!_path.empty()) {
                    _path.pop_back();
                    current_node = (_path.empty()) ? root : el_by_path(_path);
                }
                else
                    std::cout << "Already at the root. Please try again.\n";

                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    //Block 3:
    N_ary_Tree del_by_path(const std::vector<int>& _path = {})
    {
        N_ary_Tree deleted_subtree;
        if (root == nullptr)
            return deleted_subtree;

        if (_path.empty()) {
            deleted_subtree.root = root;
            root = nullptr;
            return deleted_subtree;
        }

        int index_for_del = _path.back();      
        std::vector<int> parent_path = _path;
        parent_path.pop_back();
        tree_node*& parent_node = el_by_path(parent_path);
        deleted_subtree.root = parent_node->childrens[index_for_del];
        
        parent_node->childrens.erase(parent_node->childrens.begin() + index_for_del);
        return deleted_subtree;
    }

    void del_by_value(const int _value, tree_node* current_node = nullptr) {
        if (!current_node) {
            if (!root) {
                std::cout << "Tree is empty\n";
                return;
            }

            current_node = root;
        }

        for (int i = 0; i < current_node->childrens.size();) {
            if (current_node->childrens[i]->value == _value) {
                delete current_node->childrens[i];
                current_node->childrens.erase(current_node->childrens.begin() + i);
            }
            else {
                del_by_value(_value, current_node->childrens[i]);
                ++i;
            }
        }

        if (current_node->value == _value) {
            if (current_node == root) root = nullptr;
            delete current_node;
        }
    }

private:
    void print_path(const std::vector<int>& path) const {
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i];
            if (i < path.size() - 1) {
                std::cout << std::setw(2) << "|" << std::setw(2);
            }
        }
    }

};
class Bin_Tree
{
public:
    class tree_node 
    {
    public:
        int value;
        tree_node* left;
        tree_node* right;


        tree_node(int val) : value(val), left(nullptr), right(nullptr) {};
    };
    tree_node* root;

    Bin_Tree() : root(nullptr) {};
    ~Bin_Tree() {
        clear(root);
    };

    void print_tree(tree_node* node = nullptr, int retreats = 0) {
        if (!node) {
            if (!root) {
                std::cout << "Error 1: Tree is empty\n";
                return;
            }
            node = root;
        }

        for (int i = 0; i < retreats - 1; ++i)
            std::cout << "     ";

        if (retreats > 0)
            std::cout << "+--- ";

        std::cout << node->value << std::endl;

        if (node->left)
            print_tree(node->left, retreats + 1);

        if (node->right)
            print_tree(node->right, retreats + 1);
    }
    void print_levels_tree(tree_node* node = nullptr, int level = 0) const {
        if (!node) node = root;
        if (!node) {
            std::cout << "Error: Tree is empty\n";
            return;
        }

        if (node->right) print_levels_tree(node->right, level + 1);

        std::cout << std::string(level * 4, ' ') << node->value << "\n";

        if (node->left) print_levels_tree(node->left, level + 1);
    }

    //Block 4:
    void add(int _value, tree_node* current = nullptr) {
        if (!root) {
            root = new tree_node(_value);
            return;
        }

        if (!current) {
            current = root;
        }

        if (current == nullptr) {
            current = new tree_node(_value);
            return;
        }

        if (current->value > _value) {
            if (current->left)
                add(_value, current->left);
            else
                current->left = new tree_node(_value);
        }
        else {
            if (current->right)
                add(_value, current->right);
            else
                current->right = new tree_node(_value);
        }
    }

    void convert_N_ary_To_Binary(N_ary_Tree::tree_node* n_ary_node, Bin_Tree::tree_node*& bin_node) {
        if (!n_ary_node) return;

        bin_node = new tree_node(n_ary_node->value);

        for (size_t i = 0; i < n_ary_node->childrens.size(); ++i) {
            tree_node* binary_child = nullptr;
            convert_N_ary_To_Binary(n_ary_node->childrens[i], binary_child);

            if (i == 0) {
                bin_node->left = binary_child;
            }
            else {
                tree_node* temp = bin_node->left;
                while (temp->right) {
                    temp = temp->right;
                }
                temp->right = binary_child;
            }
        }
    }

private:
    void clear(tree_node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
};
class Threaded_Bin_Tree
{
public:
    class tree_node
    {
    public:
        int value;
        tree_node* left;
        tree_node* right;

        bool left_thread;
        bool right_thread;

        tree_node(int val) : value(val), left(nullptr), right(nullptr), left_thread(false), right_thread(false) {};
    };
    tree_node* root;

    Threaded_Bin_Tree() : root(nullptr) {};

    ~Threaded_Bin_Tree() {
        clear(root);
    }

    void buildh(Bin_Tree::tree_node* bin_node)
    {
        root = (bin_node ? buildo(bin_node, nullptr, nullptr) : nullptr);
    }

    void symn(tree_node* p) {
        while (p->left_thread) p = p->left;
        while (p) {
            std::cout << p->value << ", ";
            p = suc(p);
        }
    }

private:
    tree_node* suc(tree_node* p) {
        tree_node* q = p->right;
        if (p->right_thread) {
            while (q->left_thread) q = q->left;
        }
        return q;
    }

    Threaded_Bin_Tree::tree_node* buildo(Bin_Tree::tree_node* bin_node, Threaded_Bin_Tree::tree_node* sl, Threaded_Bin_Tree::tree_node* sr)
    {
        if (!bin_node)
            return nullptr;

        tree_node* t = new tree_node(bin_node->value);

        if ((t->left_thread = (bin_node->left != nullptr))) {
            t->left = buildo(bin_node->left, sl, t);
        }
        else t->left = sl;

        if ((t->right_thread = (bin_node->right != nullptr))) {
            t->right = buildo(bin_node->right, t, sr);
        }
        else t->right = sr;

        return t;
    }

    void clear(tree_node* node) {
        if (!node) return;

        if (node->left_thread) clear(node->left);
        if (node->right_thread) clear(node->right);
        delete node;
    }
};
class Expression_Tree {
public:
    struct tree_node {
        std::string value;
        tree_node* left;
        tree_node* right;

        tree_node(const std::string& val = "", tree_node* left = nullptr, tree_node* right = nullptr) : value(val), left(left), right(right) {}
    };

    tree_node* root;

    Expression_Tree() : root(nullptr) {}

    ~Expression_Tree() {}

    void create_tree() {
        std::cout << "< Allowed values: numbers, -munbers, e, Pi, +, -, *, /, ^, sin, cos, tg, ctg, ln, log >\n"
            "<log(a, b) or a^b: a - is left value, b - is right value\n"
            "Input start: ";
        root = add_to_tree();
    }

    void simplify() {
        if (root) s1mple(root);
    }

    void differentiate(std::string var) {
        if (!root) return;
        simplify();
        tree_node* diff_exp_tree = diff(root, var);
        clear(root);
        root = diff_exp_tree;
    }

    void count_sub_exp() {
        if (root) counting(root);
    }

    std::pair<std::string, double> calculate(std::vector<std::pair<std::string, std::string>> vars) {
        if (!root) return { "Tree is empty.", 0 };
        tree_node* copy_for_calc = copytr(root);
        change_var_to_num(copy_for_calc, vars);
        std::string error = check_math_errors(copy_for_calc);
        if (error != "No errors found.") {
            return { error, 0};
        }

        counting(copy_for_calc);
        double result = stod(copy_for_calc->value);
        clear(copy_for_calc);
        return {error, result};
    }

    std::string check_math_errors(tree_node* node) {
        if (!node)
            return "No errors found.";

        if (is_operation(node->value) || is_function(node->value)) {
            if (node->value == "/") {
                tree_node* check_result = copytr(node->right);
                counting(check_result);
                std::string check_value = check_result->value;
                clear(check_result);
                if (check_value == "0") {

                    return "Error: Division by zero.";
                }
            }
            else if (node->value == "^") {
                tree_node* check_result_right = copytr(node->right);
                tree_node* check_result_left = copytr(node->left);
                counting(check_result_right);
                counting(check_result_left);

                std::string check_value_right = check_result_right->value;
                std::string check_value_left = check_result_left->value;
                clear(check_result_right); clear(check_result_left);

                if (check_value_right == "0" && check_value_left == "0") {
                    return "Error: Zero to the zero power.";
                }
                if (stod(check_value_right) < 0) {
                    return "Error: Negative power.";
                }
            }
            else if (node->value == "log") {
                tree_node* check_result_right = copytr(node->right);
                tree_node* check_result_left = copytr(node->left);
                counting(check_result_right);
                counting(check_result_left);

                std::string check_value_right = check_result_right->value;
                std::string check_value_left = check_result_left->value;
                clear(check_result_right); clear(check_result_left);

                if (stod(check_value_right) < 0 || stod(check_value_left) < 0) {
                    return "Error: Negative variables in log.";
                }
            }
            else if (node->value == "ln") {
                tree_node* check_result_right = copytr(node->right);
                counting(check_result_right);

                std::string check_value_right = check_result_right->value;
                clear(check_result_right);

                if (is_num(check_value_right) && stod(check_value_right) < 0) {
                    return "Error: Negative variables in ln.";
                }
            }

        }

        std::string left_errors = check_math_errors(node->left);
        if (left_errors != "No errors found.")
            return left_errors;

        std::string right_errors = check_math_errors(node->right);
        if (right_errors != "No errors found.")
            return right_errors;

        return "No errors found.";
    }

    void print_tree(tree_node* node, int level = 0) const {
        if (!root) {
            std::cout << "Error: Tree is empty\n";
            return;
        }

        if (node->right) print_tree(node->right, level + 1);

        std::cout << std::string(level * 4, ' ') << node->value << "\n";

        if (node->left) print_tree(node->left, level + 1);
    }

    void dem_mode() {
        std::pair<std::string, double> res;
        std::vector<std::pair<std::string, std::string>> var_val = { {"x", "0"}, {"y", "0"}, {"z", "0"} };
        std::cout << "Let`s create expression tree and print it\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        /*              +
                /               +
            1       x       ln      z
                                y

        */
        root = new tree_node("+");
        root->left = new tree_node("/");
        root->left->left = new tree_node("1");
        root->left->right = new tree_node("x");
        root->right = new tree_node("+");
        root->right->left = new tree_node("ln");
        root->right->left->right = new tree_node("y");
        root->right->right = new tree_node("z");

        print_tree(root);
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Let`s check math errors in tree: " << check_math_errors(root) << std::endl;
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        std::cout << "Let`s count for x = 0, y = e z = 2\n";
        var_val = { {"x", "0"}, {"y", "e"}, {"z", "2"} };
        res = calculate(var_val);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (res.first != "No errors found.")
            std::cout << "Tree expression have error!" << std::endl << res.first << std::endl;
        else
            std::cout << "Result: " << res.second << std::endl;
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Let`s count for x = 10, y = -e z = 2\n";
        var_val = { {"x", "10"}, {"y", "-e"}, {"z", "2"} };
        res = calculate(var_val);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (res.first != "No errors found.")
            std::cout << "Tree expression have error!" << std::endl << res.first << std::endl;
        else
            std::cout << "Result: " << res.second << std::endl;
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Let`s count for x = 1, y = e z = 2\n";
        var_val = { {"x", "1"}, {"y", "e"}, {"z", "2"} };
        res = calculate(var_val);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (res.first != "No errors found.")
            std::cout << "Tree expression have error!" << std::endl << res.first << std::endl;
        else
            std::cout << "Result: " << res.second << std::endl;
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Let`s differentiate for y and print differentiation tree\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        differentiate("y");
        print_tree(root);
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Let`s count subtree,simplify it and print\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        count_sub_exp();
        simplify();
        print_tree(root);
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Let`s count for x = 10000, y = 12, z = 134\n";
        var_val = { {"x", "10000"}, {"y", "12"}, {"z", "134"} };
        res = calculate(var_val);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (res.first != "No errors found.")
            std::cout << "Tree expression have error!" << std::endl << res.first << std::endl;
        else
            std::cout << "Result: " << res.second << std::endl;
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
private:
    tree_node* add_to_tree() {
        std::string value;
        value = input_math_symbols();
        tree_node* node = new tree_node(value);
        if (is_operation(value) || value == "log") {
            std::cout << "Input left allowed value (" << value << "): ";
            node->left = add_to_tree();
            std::cout << "Input right allowed value (" << value << "): ";
            node->right = add_to_tree();
        }
        else if (is_function(value) && value != "log") {
            std::cout << "Input right allowed value (" << value << "): ";
            node->right = add_to_tree();
        }
        return node;
    }

    bool is_operation(const std::string& p_op) {
        if (p_op.length() != 1) return false;
        return std::string("+-*/^").find(p_op) != std::string::npos;
    }

    bool is_function(const std::string& p_func) {
        if (p_func.length() != 2 && p_func.length() != 3) return false;
        return std::string("cossintgctglnlog").find(p_func) != std::string::npos;
    }

    bool is_num(const std::string& p_num) {
        if (p_num.empty())
            return false;

        bool has_decimal_point = false;
        bool has_minus_sign = false;

        for (size_t i = 0; i < p_num.length(); ++i) {
            char c = p_num[i];

            if (!isdigit(c)) {
                if (c == '.' && p_num.length() > 1) {
                    if (has_decimal_point)
                        return false;
                    has_decimal_point = true;
                }
                else if (c == '-' && p_num.length() > 1) {
                    if (i != 0 || has_minus_sign)
                        return false;
                    has_minus_sign = true;
                }
                else if (p_num == "Pi" || p_num == "e" || p_num == "-Pi" || p_num == "-e")
                    return true;
                else {
                    return false;
                }
            }
        }

        return true;
    }

    bool is_variable(const std::string& p_var) {
        if (p_var.length() != 1) return false;
        return (p_var.find_first_not_of("xyz") == std::string::npos);
    }

    tree_node* copy_with_new_node(std::string op, tree_node* pl, tree_node* pr) {
        return new tree_node(op, pl, pr);
    }

    tree_node* copytr(tree_node* p) {
        if (p) return new tree_node(p->value, copytr(p->left), copytr(p->right));
        else return nullptr;
    }

    std::string input_math_symbols() {
        std::string math_symbols; bool correct_input = true;
        do {
            if (!correct_input) std::cout << "Wrong value. Input one more time: ";
            std::cin >> math_symbols; correct_input = false;
        } while (!is_function(math_symbols) && !is_num(math_symbols) && !is_operation(math_symbols) && !is_variable(math_symbols));

        return math_symbols;
    }

    void s1mple(tree_node*& node) {
        if (!node) {
            return;
        }

        s1mple(node->left);
        s1mple(node->right);

        if (node->value == "+" && node->left && node->right) {
            if (node->left->value == "0") {
                node = node->right;
            }
            else if (node->right->value == "0") {
                node = node->left;
            }
        }
        else if (node->value == "-" && node->right && node->right) {
            if (node->right->value == "0" && is_num(node->left->value)) {
                node = node->left;
            }
            else if (node->left->value == "0" && is_num(node->right->value)) {
                node = node->right;
                node->value = std::to_string(std::stod(node->value) * -1);
            }
        }

        else if (node->value == "*") {
            if (node->left && node->right) {
                if (node->left->value == "0" || node->right->value == "0") {
                    node = new tree_node("0");
                }
                else if (node->left->value == "1") {
                    node = node->right;
                }
                else if (node->right->value == "1") {
                    node = node->left;
                }
            }
        }
        else if (node->value == "/" && node->right) {
            if (node->right->value == "1") {
                node = node->left;
            }
        }
        else if (node->value == "^") {
            if (node->right->value == "0") {
                node = new tree_node("1");
            }
            else if (node->right->value == "1") {
                node = node->left;
            }
            else if (node->left->value == "0") {
                node = new tree_node("0");
            }
            else if (node->left->value == "1") {
                node = new tree_node("1");
            }
        }

        if (node->left && node->right && node->left->value == "0" && node->right->value == "0" && node->value != "/") {
            node = new tree_node("0");
        }
    }

    tree_node* diff(tree_node* node, const std::string& var) {
        if (!node)
            return nullptr;

        if (node->value == "+")
            return copy_with_new_node("+", diff(node->left, var), diff(node->right, var));
        else if (node->value == "-")
            return copy_with_new_node("-", diff(node->left, var), diff(node->right, var));
        else if (node->value == "*")
            return copy_with_new_node("+",
                copy_with_new_node("*", diff(node->left, var), copytr(node->right)),
                copy_with_new_node("*", copytr(node->left), diff(node->right, var)));
        else if (node->value == "/")
            return copy_with_new_node("/",
                copy_with_new_node("-",
                    copy_with_new_node("*", diff(node->left, var), copytr(node->right)),
                    copy_with_new_node("*", copytr(node->left), diff(node->right, var))),
                copy_with_new_node("*", copytr(node->right), copytr(node->right)));
        else if (node->value == "^") {
            tree_node* a = node->left;
            tree_node* b = node->right;

            tree_node* da_dx = diff(a, var);
            tree_node* db_dx = diff(b, var);

            tree_node* term1 = copy_with_new_node("*", copytr(b), copy_with_new_node("*", copy_with_new_node("^", copytr(a), copy_with_new_node("-", copytr(b), new tree_node("1"))), da_dx));
            tree_node* term2 = copy_with_new_node("*", copy_with_new_node("^", copytr(a), copytr(b)), copy_with_new_node("*", copy_with_new_node("ln", nullptr, copytr(a)), db_dx));
            return copy_with_new_node("+", term1, term2);
        }
        else if (node->value == "sin") {
            tree_node* du_sinu = copy_with_new_node("cos", nullptr, copytr(node->right));
            return copy_with_new_node("*", du_sinu, diff(node->right, var));
        }
        else if (node->value == "cos") {
            tree_node* du_cosu = copy_with_new_node("-", new tree_node("sin", nullptr, copytr(node->right)), new tree_node("0"));
            return copy_with_new_node("*", du_cosu, diff(node->right, var));
        }
        else if (node->value == "tg") {
            tree_node* du_tgu = copy_with_new_node("/", new tree_node("1"), copy_with_new_node("^", copy_with_new_node("cos", nullptr, copytr(node->right)), new tree_node("2")));
            return copy_with_new_node("*", du_tgu, diff(node->right, var));
        }
        else if (node->value == "ctg") {
            tree_node* minus_one = copy_with_new_node("-", new tree_node("0"), new tree_node("1"));
            tree_node* du_ctgu = copy_with_new_node("/", minus_one, copy_with_new_node("^", copy_with_new_node("sin", nullptr, copytr(node->right)), new tree_node("2")));
            return copy_with_new_node("*", du_ctgu, diff(node->right, var));
        }
        else if (node->value == "log") {
            tree_node* base = new tree_node("ln", nullptr, node->left);
            tree_node* argument = new tree_node("ln", nullptr, node->right);
            tree_node* diff_log = diff(copy_with_new_node("/", argument, base), var);

            return diff_log;
        }
        else if (node->value == "ln") {
            return copy_with_new_node("*", copy_with_new_node("/", new tree_node("1"), copytr(node->right)), diff(node->right, var));
        }
        else if (node->value == var) {
            return new tree_node("1");
        }
        else if (is_num(node->value) || (is_variable(var) && node->value != var)) {
            return new tree_node("0");
        }

        return nullptr;
    }

    void counting(tree_node*& node) {
        if (!node) {
            return;
        }

        counting(node->left); counting(node->right);
        double result = 0, left_value = 0, right_value = 0;
        bool calc = false;

        if (node->left && node->right && is_num(node->left->value) && is_num(node->right->value)) {
            node->left->value == "Pi" ? left_value = M_Pi :
                (node->left->value == "e") ? left_value = M_e :
                (node->left->value == "-e") ? left_value = -1 * M_e :
                (node->left->value == "-Pi") ? left_value = -1 * M_Pi : left_value = stod(node->left->value);
            node->right->value == "Pi" ? right_value = M_Pi :
                (node->right->value == "e") ? right_value = M_e :
                (node->right->value == "-e") ? right_value = -1 * M_e :
                node->right->value == "-Pi" ? right_value = -1 * M_Pi : right_value = stod(node->right->value);

            if (node->value == "+") {
                result = left_value + right_value;
            }
            else if (node->value == "-") {
                result = left_value - right_value;
            }
            else if (node->value == "*") {
                result = left_value * right_value;
            }
            else if (node->value == "/") {
                if (right_value != 0) {
                    result = left_value / right_value;
                }
                else {
                    result = std::numeric_limits<double>::quiet_NaN();
                }
            }
            else if (node->value == "^") {
                std::cout << "r: " << right_value << std::endl;
                if (right_value >= 0.0) {
                    result = std::pow(left_value, right_value);
                }
                else if (right_value < 0.0) {
                    result = 1 / std::pow(left_value, -1 * right_value);
                }
                else {
                    result = std::numeric_limits<double>::quiet_NaN();
                    std::cout << "==";
                }
                std::cout << "result: " << result << std::endl;
            }
            else if (node->value == "log") {

                result = std::log(right_value) / std::log(left_value);
            }
            calc = true;
        }
        else if (node->right && is_num(node->right->value) && is_function(node->value)) {
            node->right->value == "Pi" ? right_value = M_Pi :
                (node->right->value == "e") ? right_value = M_e :
                (node->right->value == "-e") ? right_value = -1 * M_e :
                node->right->value == "-Pi" ? right_value = -1 * M_Pi : right_value = stod(node->right->value);
            if (node->value == "ln") {
                result = std::log(right_value);
            }
            else if (node->value == "sin") {
                result = std::sin(right_value);
            }
            else if (node->value == "cos") {
                result = std::cos(right_value);
            }
            else if (node->value == "tg") {
                result = std::tan(right_value);
            }
            else if (node->value == "ctg") {
                result = 1.0 / std::tan(right_value);
            }
            calc = true;
        }
        else if (is_num(node->value)) {
            node->value == "Pi" ? result = M_Pi :
                (node->value == "e") ? result = M_e :
                (node->value == "-e") ? result = -1 * M_e :
                (node->value == "-Pi") ? result = -1 * M_Pi : result = stod(node->value);
            calc = true;
        }
        if (calc) {
            result = round(result * 10000) / 10000.0;

            if (std::fabs(result - std::round(result)) < std::numeric_limits<double>::epsilon())
                node->value = std::to_string(static_cast<int>(result));
            else
                node->value = std::to_string(result);

            delete node->left;
            delete node->right;
            node->left = nullptr;
            node->right = nullptr;
        }
    }

    void change_var_to_num(tree_node*& node, std::vector<std::pair<std::string, std::string>> vars) {
        if (!node) {
            return;
        }

        if (is_variable(node->value)) {
            for (const auto& var : vars) {
                if (var.first == node->value) {
                    node->value = var.second;
                    return;
                }
            }
        }

        change_var_to_num(node->left, vars);
        change_var_to_num(node->right, vars);
    }

    void clear(tree_node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
            node = nullptr;
        }
    }
};

template <typename T>
T getValidInput(const std::string& prompt);
void input_path(std::vector<int>& path);
void demonstration_mode();
void interactive_mode();
int main()
{
    srand(time(NULL));
    int mode;
    do {
        std::cout << "Modes: \n"
            "1. Interactive mode\n"
            "2. Demonstartion mode\n"
            "0. Exit\n";
        mode = getValidInput<int>("Choose mode: ");
        system("cls");
        switch (mode) {
        case 0:
            break;
        case 1:
            interactive_mode();
            break;
        case 2:
            demonstration_mode();
            break;
        default:
            std::cout << "Wrong mode.\n";
        }
    } while (mode != 0);

    return 0;
}

void demonstration_mode()
{
    std::cout << "=== Demonstration mode ===\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Create Tree like:\n"
        "0\n"
        "+--- 1\n"
        "     +--- 4\n"
        "     +--- 5\n"
        "+--- 2\n"
        "+--- 3\n"
        "     +--- 6\n"
        "     +--- 7\n"
        "          +--- 9\n"
        "     +--- 8\n";

    std::cout << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    N_ary_Tree tree;
    tree.add(0);
    tree.add(1);
    tree.add(2);
    tree.add(3);
    tree.add(4, { 0 });
    tree.add(5, { 0 });
    tree.add(6, { 2 });
    tree.add(7, { 2 });
    tree.add(8, { 2 });
    tree.add(9, { 2, 1 });

    std::cout << "Let`s print our Tree:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tree.print_tree();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let`s delete subtree which have path {}\n";
    N_ary_Tree deleted_subtree = tree.del_by_path({});
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Update tree:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tree.print_tree();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Deleted subtree:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    deleted_subtree.print_tree();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Add some elements for start tree:\n";
    tree.add(0);
    tree.add(1);
    tree.add(2);
    tree.add(3);
    tree.add(4, { 0 });
    tree.add(5, { 0 });
    tree.add(1);
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Print new start tree:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tree.print_tree();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << std::endl;

    std::cout << "Let`s use add_proc:\n"
        "1) 10 with start chance 10%\n"
        "2) 20 with start chance 20%\n"
        "3) 30 with start chance 30%\n"
        "4) 40 with start chance 40%\n"
        "5) 50 with start chance 50%\n"
        "6) 60 with start chance 60%\n";
    tree.add_proc(10, 0.1);
    tree.add_proc(20, 0.2);
    tree.add_proc(30, 0.3);
    tree.add_proc(40, 0.4);
    tree.add_proc(50, 0.5);
    tree.add_proc(60, 0.6);
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "Print new start tree with add_proc elements:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tree.print_tree();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << "Let`s delete subtree which have path {1}\n";
    deleted_subtree = tree.del_by_path({ 1 });
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Update tree:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tree.print_tree();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Deleted subtree:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    deleted_subtree.print_tree();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let`s delete subtrees which start with 1\n";
    tree.del_by_value(1);
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Update tree:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tree.print_tree();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let`s print with path every element:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tree.print_tree_with_path();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let`s open interactive view n-ary tree:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tree.interactive_tree_view();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let`s create binary tree\n";
    Bin_Tree bin;
    bin.add(30);
    bin.add(10);
    bin.add(40);
    bin.add(15);
    bin.add(5);
    bin.add(35);
    bin.add(60);
    bin.add(80);
    bin.add(50);
    bin.add(0);
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let`s print binary tree\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    bin.print_tree();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let`s create new N-ary tree and converted it to binary\n";
    N_ary_Tree tree_for_converted;
    tree_for_converted.add(0);
    tree_for_converted.add(1);
    tree_for_converted.add(2);
    tree_for_converted.add(3);
    tree_for_converted.add(4, { 0 });
    tree_for_converted.add(5, { 0 });
    tree_for_converted.add(6, { 2 });
    tree_for_converted.add(7, { 2 });
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let`s print new N-ary tree\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tree_for_converted.print_tree();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Bin_Tree converted_bin_tree;
    converted_bin_tree.convert_N_ary_To_Binary(tree_for_converted.root, converted_bin_tree.root);

    std::cout << "Let`s print converted binary tree\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    converted_bin_tree.print_tree();
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let`s do threaded binary tree with converted binary tree\n";
    Threaded_Bin_Tree t;
    t.buildh(converted_bin_tree.root);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Let`s do symmetric threaded binary tree traversal\n";
    t.symn(t.root);
    std::cout << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Expression_Tree exp;
    exp.dem_mode();
}
void interactive_mode() {
    N_ary_Tree n_ary;
    N_ary_Tree del;
    Bin_Tree bin;
    Threaded_Bin_Tree thread;
    Expression_Tree expression;
    std::string exp, var;
    std::vector<int> path;
    int val;
    std::pair<std::string, double> res;
    std::vector<std::pair<std::string, std::string>> var_val = { {"x", "0"}, {"y", "0"}, {"z", "0"}};
 

    int tree_num;

    do {
        system("cls");

       std::cout << "=== Interactive mode ===\n";
        std::cout << "Trees:\n"
            "1. N-ary tree\n"
            "2. Binary tree\n"
            "3. Expression tree\n";

        tree_num = getValidInput<int>("Choose tree: ");
        int mode;
        switch (tree_num) {
        case 1:
            do {
                system("cls");
                std::cout << "Operattion for n-ary tree:\n"
                    "1. Add by path\n"
                    "2. Add by proc\n"
                    "3. Del by path\n"
                    "4. Del by value\n"
                    "5. Get value by path\n"
                    "6. Print tree\n"
                    "7. Print tree with path\n"
                    "0. Exit\n";
                mode = getValidInput<int>("Choose opeartion: ");
                system("cls");
                switch (mode)
                {
                case 0:
                    break;
                case 1:
                    input_path(path);
                    val = getValidInput<int>("Input value: ");
                    n_ary.add(val, path);
                    break;
                case 2:
                    val = getValidInput<int>("Input value: ");
                    double p; p = getValidInput<double>("Input chance (from 0 to 1): ");
                    n_ary.add_proc(val, p);
                    break;
                case 3:
                    input_path(path);
                    del = n_ary.del_by_path(path);
                    std::cout << "Deleted subtree: \n";
                    del.print_tree();
                    std::cout << std::endl;
                    system("pause");
                    break;
                case 4:
                    val = getValidInput<int>("Input value by delete: ");
                    n_ary.del_by_value(val);
                    break;
                case 5:
                    input_path(path);
                    system("pause");
                    break;
                case 6:
                    n_ary.print_tree();
                    std::cout << std::endl;
                    system("pause");
                    break;
                case 7:
                    n_ary.print_tree_with_path();
                    std::cout << std::endl;
                    system("pause");
                    break;
                case 8:
                    n_ary.interactive_tree_view();
                    break;
                default:
                    std::cout << "Wrong operation.\n";
                    break;
                }
                
            } while (mode != 0);
            break;
        case 2:
            do {
                system("cls");
                std::cout << "Operattion for binary tree:\n"
                    "1. Add value \n"
                    "2. Convert n-ary tree to binary\n"
                    "3. Print tree by levels\n"
                    "4. Print tree\n"
                    "5. Build thread binary tree\n"
                    "6. Print symetric thread binary tree\n"
                    "0. Exit\n";
                mode = getValidInput<int>("Choose opeartion: ");
                system("cls");
                switch (mode)
                {
                case 0:
                    break;
                case 1:
                    val = getValidInput<int>("Input value: ");
                    bin.add(val);
                    break;
                case 2:
                    bin.convert_N_ary_To_Binary(n_ary.root, bin.root);
                    break;
                case 3:
                    bin.print_levels_tree();
                    system("pause");
                    break;
                case 4:
                    bin.print_tree();
                    system("pause");
                    break;
                case 5:
                    thread.buildh(bin.root);
                    break;
                case 6:
                    if (!thread.root) {
                        std::cout << "Binary tree isn`t thread now\n";
                        system("pause");
                        break;
                    }
                    thread.symn(thread.root);
                    system("pause");
                    break;
                default:
                    std::cout << "Wrong operation.\n";
                    break;
                }


            } while (mode != 0);
            break;
        case 3:
            do {
                system("cls");
                std::cout << "Operattion for expression tree:\n"
                    "1. Create tree\n"
                    "2. Check errors in tree\n"
                    "3. Differentiation tree\n"
                    "4. Count sub-tree\n"
                    "5. Simplify tree\n"
                    "6. Calculate tree\n"
                    "7. Print tree\n"
                    "0. Exit\n";
                mode = getValidInput<int>("Choose opeartion: ");
                system("cls");
                switch (mode)
                {
                case 0:
                    break;
                case 1:
                    expression.create_tree();
                    break;
                case 2:
                    std::cout << expression.check_math_errors(expression.root) << std::endl;
                    system("pause");
                    break;
                case 3:
                    var = getValidInput<std::string>("Input var for differentiate: ");
                    if (var.length() != 1) break;
                    expression.differentiate(var);
                    break;
                case 4:
                    expression.count_sub_exp();
                    break;
                case 5:
                    expression.simplify();
                    break;
                case 6:
                    for (int i = 0; i < var_val.size(); i++) {
                        std::cout << "Input value for " << var_val[i].first << ": ";
                        double val; val = getValidInput<double>("");
                        var_val[i].second = std::to_string(val);
                    }
                    res = expression.calculate(var_val);
                    if (res.first != "No errors found.")
                        std::cout << "Tree expression have error!" << std::endl << res.first << std::endl;
                    else
                        std::cout << "Result: " << res.second << std::endl;
                    system("pause");
                    break;
                case 7:
                    expression.print_tree(expression.root);
                    system("pause");
                default:
                    std::cout << "Wrong operation.\n";
                    break;
                }


            } while (mode != 0);
            break;
        } 

    } while (tree_num != 0);
}

template <typename T>
T getValidInput(const std::string& prompt) {
    T value;
    bool cinFail;
    do {
        cinFail = false;
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cerr << "\n### Invalid input. Please enter a valid value. ###\n\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cinFail = true;
        }
        else {
            char extra;
            if (std::cin.get(extra) && extra != '\n') {
                std::cerr << "\n### Invalid input. Extra characters detected. ###\n\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cinFail = true;
            }
        }
    } while (cinFail);
    return value;
}

void input_path(std::vector<int>& path) {
    path.clear();
    int len, k;
    std::cout << "If n-ary tree empty or have only one element, then must be len = 0\n";
    std::cout << "Path example: {0, 3, 1}\n";
    len = getValidInput<int>("Enter len path: ");
    for (int i = 0; i < len; i++) {
        std::cout << "Print " << i << ": ";
        k = getValidInput<int>("");
        path.push_back(k);
    }
}
