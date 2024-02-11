/*Реалізувати структуру даних Random Shortcut List. 
Ця структура будується на основі однозв’язного циклічного списку фіксованого розміру N, елементами якого є числа від 1 до N.
Проте задається ймовірність p_add, що кожен елемент списку буде мати додаткове посилання (shortcut) на ще один елемент списку, який вибирається випадковим чином. 
Чи буде в заданому елементі shortcut, та якщо буде – на який елемент він вказує, визначається на етапі створення структури даних. 
Реалізувати обхід списку за наступними правилами:починаючи з певного елемента, якщо в даному елементі є додаткове посилання – переходимо по ньому з ймовірністю p_follow; 
інакше переходимо до наступного елемента списку. 
Реалізувати обхід для пошуку заданого елементу.
Реалізувати обхід списку, доки не будуть відвідані всі елементи. 
В обох випадках зберегти та вивести кількість кроків. 
Повторити обходи кілька разів для фіксованої структури списку (результати можуть бути різними через випадковий характер руху), 
вивести середні, мінімальні та максимальні значення кількості кроків.
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>

#define printList true

using namespace std;

class RandomShortcutList {
private:
    class Node 
    {
        public:
            int value;
            Node* next;
            Node* shortcut; 

            Node(int value) 
            {
                this->value = value; 
                this->next = nullptr;
                this->shortcut = nullptr;
            }
    };
    Node* head;
    int size;
    double p_add;
    double p_follow;
    std::vector<Node*> nodes; // Вектор для зберігання всіх вузлів

public:
    RandomShortcutList(int N, double p_add, double p_follow)
    {
        this->size = N;
        this->p_add = p_add;
        this->p_follow = p_follow;
        head = nullptr;
        nodes.reserve(N);
        for (int i = 0; i < N; ++i) {
            Node* newNode = new Node(i + 1); 
            nodes.push_back(newNode);
            if (head == nullptr) 
                head = newNode;
            else
                nodes[i - 1]->next = newNode;
        }

        for (int i = 0; i < N; ++i)
        {
            if (rand() / static_cast<double>(RAND_MAX) < p_add)
            {
                int link;
                do
                {
                    link = rand() % N;
                } while (link == i);
                nodes[i]->shortcut = nodes[link];
            }
        }
            

        nodes[N - 1]->next = head;
    }

    int traverse(int index, bool printTraverse) 
    {
        if (index + 1 > size) index = 0;

        int steps = 0;
        Node* currentNode = nodes[index];
        while (true) 
        {
            if (printTraverse) cout << currentNode->value << " ";
            steps++;
            if (rand() / static_cast<double>(RAND_MAX) < p_follow && currentNode->shortcut != nullptr)
            {
                if (printTraverse) cout << "\n" << currentNode->value << " ";
                currentNode = currentNode->shortcut;
            }
            else 
                currentNode = currentNode->next;
            if (currentNode == nodes[index])
                break; 
        }
        return steps;
    }

    int traverseAllElement(int index, bool printTraverse)
    {
        if (index + 1 > size) index = 0;

        vector<bool> visited(size, false); 
        int visitedCount = 0;
        int steps = 0;
        Node* currentNode = nodes[index];
        while (true)
        {
            if (visited[currentNode->value - 1] == false)
            {
                visited[currentNode->value - 1] = true;
                visitedCount++;
            }

            if (printTraverse) cout << currentNode->value << " ";
            if (visitedCount == size) break;

            steps++;
            if (rand() / static_cast<double>(RAND_MAX) < p_follow && currentNode->shortcut != nullptr)
            {
                if (printTraverse) cout << "\n" << currentNode->value << " ";
                currentNode = currentNode->shortcut;
            }
            else
                currentNode = currentNode->next;

        }
        return steps;
    }

    // Пошук заданого елементу та обхід списку
    int search(int target) 
    {
        if (target > size)
            throw("\n### Incorrect index! ###\n");

        int steps = 0;
        Node* currentNode = head;
        while (true)
        {
            if (currentNode->value == target)
                break;
            steps++;
            if (rand() / static_cast<double>(RAND_MAX) < p_follow && currentNode->shortcut != nullptr)
                currentNode = currentNode->shortcut;
            else
                currentNode = currentNode->next;
        }
        return steps;
    }

    ~RandomShortcutList() {
        // Звільнення пам'яті
        for (Node* node : nodes) {
            delete node;
        }
    }
};

void interactiveMode();
void demonstrationMode();
void benchmarkMode();

template <typename T>
T getValidInput(const string& prompt);

int main() 
{
    srand(time(NULL));
    int modeChoice;
    cout << "Select mode:\n"
        "1. Interactive Mode\n"
        "2. Demonstration Mode\n"
        "3. Benchmark Mode\n"
        "0. Exit\n";
    modeChoice = getValidInput<int>("Your choice: ");

    switch (modeChoice) {
    case 0:
        break;
    case 1:
        interactiveMode();
        break;
    case 2:
        demonstrationMode();
        break;
    case 3:
        benchmarkMode();
        break;
    default:
        cout << "\n### Invalid mode selected! ###\n";
    }
    return 0;
}

void interactiveMode()
{
    int N, operation;
    double p_add, p_follow;
    RandomShortcutList* list = nullptr;
    cout << "\n=== INTERACTIVE MODE === \n";

    do
    {
        cout << "\nInttecive mode:\n"
                "1. Create Random Shortcat List\n"
                "2. Traverse list\n"
                "3. Traverse all element in list\n"
                "4. Find element in list\n"
                "0. Exit\n";
        operation = getValidInput<int>("Your choice: ");

        switch (operation)
        {
        case 1:
            if (list != nullptr) {
                delete list;
                list = nullptr;
            }
            N = getValidInput<int>("Enter the number of elements: ");
            p_add = getValidInput<double>("Enter the probability of creating a new link: ");
            p_follow = getValidInput<double>("Enter the probability of following a link: ");
            list = new RandomShortcutList(N, p_add, p_follow);
            cout << "\n=== Random Shortcut List created! ===\n";
            break;
        case 2:
            if (list == nullptr) {
                cout << "Please create the Random Shortcut List first.\n";
                break;
            }
            int index;
            index = getValidInput<int>("Enter the starting index: ");
            list->traverse(index, printList);
            cout << endl;
            break;
        case 3:
            if (list == nullptr) {
                cout << "Please create the Random Shortcut List first.\n";
                break;
            }
            int indexAll;
            indexAll = getValidInput<int>("Enter the starting index: ");
            list->traverseAllElement(indexAll, printList);
            cout << endl;
            break;
        case 4:
            if (list == nullptr) {
                cout << "Please create the Random Shortcut List first.\n";
                break;
            }
            int target;
            target = getValidInput<int>("Enter the target value to search: ");
            try {
                int steps = list->search(target);
                cout << "Steps to find " << target << ": " << steps << endl;
            }
            catch (const exception& e) {
                cerr << e.what() << endl;
            }
            break;
        case 0:
            cout << "=== EXIT ===\n";
            break;
        default:
            cout << "\n### Invalid operation. Please try again! ###\n";
            break;
        }
    } while (operation != 0);

}

void demonstrationMode()
{
    cout << "\n=== DEMONSTRATION MODE RANDOM SHORTCUT LIST ===\n\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Start parameters:\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Numbers element: 20\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Probability of creating a new link: 50%\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Probability of going to this link: 20%\n";
    this_thread::sleep_for(chrono::seconds(1));

    int N = 20;
    double p_add = 0.5;
    double p_follow = 0.2;

    cout << "\n=== CREATE RANDOM SHORTCUT LIST ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    RandomShortcutList list(N, p_add, p_follow);


    cout << "\n=== TRAVERSING AND PRINTING ELEMENT OF `RANDOM SHORTCUT LIST` STARTINT AT 8 INDEX ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    list.traverse(8, printList);
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== TRAVERSING AND PRINTING ELEMENT OF `RANDOM SHORTCUT LIST` STARTING AT 3 INDEX ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    list.traverse(3, printList);
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== TRAVERSING AND PRINTING ALL ELEMENTS OF `RANDOM SHORTCUT LIST` STARTING AT 5 INDEX ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    list.traverseAllElement(5, printList);
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== TRAVERSING AND PRINTING ALL ELEMENTS OF `RANDOM SHORTCUT LIST` STARTING AT 9 INDEX ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    list.traverseAllElement(9, printList);
    this_thread::sleep_for(chrono::seconds(1));


    cout << "\n=== FIND 4 IN RANDOM SHORTCUT LIST ===\n";
    try { cout << "Steps for find 4 in list: " << list.search(4) << endl; }
    catch (const std::exception& e) { std::cerr << e.what() << endl; }
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== FIND 19 IN RANDOM SHORTCUT LIST ===\n";
    try { cout << "Steps for find 4 in list: " << list.search(19) << endl; }
    catch (const std::exception& e) { std::cerr << e.what() << endl; }
    this_thread::sleep_for(chrono::seconds(1));
}

void benchmarkMode()
{
    cout << "\n=== BENCHMARK MODE RANDOM SHORTCUT LIST ===\n\n";

    int N;
    cout << "Enter a number of elements: ";
    do { cin >> N; } while (N < 0);

    int iterations;
    cout << "Enter a number of itertions: ";
    do { cin >> iterations; } while (iterations < 0);

    int totalFindSteps = 0, totalTraverseSteps = 0;
    int minFindSteps = INT_MAX, minTraverseSteps = INT_MAX;
    int maxFindSteps = 0, maxTraverseSteps = 0;
    double p_add = rand() / static_cast<double>(RAND_MAX);
    double p_follow = rand() / static_cast<double>(RAND_MAX);

    cout << "\n=== START BENCHMARK MODE RANDOM SHORTCUT LIST\n";

    auto startTime = chrono::high_resolution_clock::now();
    auto startCreateListTime = chrono::high_resolution_clock::now();
    RandomShortcutList list(N, p_add, p_follow);
    auto endCreateListTime = chrono::high_resolution_clock::now();

    auto startFindTime = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        int steps = list.search(rand() % N + 1);
        totalFindSteps += steps;
        if (steps < minFindSteps) minFindSteps = steps;
        if (steps > maxFindSteps) maxFindSteps = steps;
    }
    auto endFindTime = chrono::high_resolution_clock::now();

    auto startTreverseTime = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        int steps = list.traverseAllElement(rand() % N,!printList);
        totalTraverseSteps += steps;
        if (steps < minTraverseSteps) minTraverseSteps = steps;
        if (steps > maxTraverseSteps) maxTraverseSteps = steps;
    }
    auto endTreverseTime = chrono::high_resolution_clock::now();

    auto endTime = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    auto durationCreate = chrono::duration_cast<chrono::milliseconds>(endCreateListTime - startCreateListTime);
    auto durationFind = chrono::duration_cast<chrono::milliseconds>(endFindTime - startFindTime);
    auto durationTraverse = chrono::duration_cast<chrono::milliseconds>(endTreverseTime - startTreverseTime);

    cout << "THE VALUES BEFORE BENCHMARK START:\n";
    cout << "Probability of a new link: " << p_add << endl;
    cout << "Probability of going to this link: " << p_follow << endl << endl;

    cout << "THE VALUES OF THE STEPS:\n";
    cout << "Average find steps: " << static_cast<double>(totalFindSteps) / iterations << endl;
    cout << "Min find steps: " << minFindSteps << endl;
    cout << "Max find steps: " << maxFindSteps << endl;
    cout << "Average treverse all elements steps: " << static_cast<double>(totalTraverseSteps) / iterations << endl;
    cout << "Min treverse all elements steps: " << minTraverseSteps << endl;
    cout << "Max treverse all elements steps: " << maxTraverseSteps << endl << endl;

    cout << "THE VALUE OF THE TIME:\n";
    cout << "Time to create the list: " << durationCreate.count() << " milliseconds\n";
    cout << "Time to add to the lisе: " << durationFind.count() << " milliseconds\n";
    cout << "Time to traverse the list: " << durationTraverse.count() << " milliseconds\n";
    cout << "Total working time: " << duration.count() / 1000.0 << " seconds\n";
    cout << "\n=== BENCHMARK MODE END ===";
}

template <typename T>
T getValidInput(const string& prompt) {
    T value;
    bool cinFail;
    do
    {
        cinFail = false;
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cerr << "\n### Invalid input. Please enter a valid number. ###\n\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cinFail = true;
        };
    } while (cinFail);
    return value;
}
