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

            if (visitedCount == size) break;

            if (printTraverse) cout << currentNode->value << " ";
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

void demonstrationMode();
void benchmarkMode();

int main() 
{
    srand(time(NULL));
    //demonstrationMode();
    benchmarkMode();
    return 0;
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
