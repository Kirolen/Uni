/*
2b. 26. Реалізувати структуру даних Unrolled link list (див. наприклад https://en.wikipedia.org/wiki/Unrolled_linked_list ). 
Операції стандартні для списків – обхід (в обох напрямках), додавання елементу (в кінець, початок або довільну позицію), 
видалення елементу, доступ за індексом, пошук за значенням.
*/

#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

#define maxElements 4  
#define zeroIndex 0
#define randIndex 10000
#define maxElem 4000000
#define isBenchmark true
template <typename T>
T getValidInput(const string& prompt) 
{
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


template <typename T>
class UnLinList
{
private:

    class Node
    {
    public:
        int numElements;
        T data[maxElements];
        Node* next;
        Node* prev;

        Node(int numElements = 0, Node* next = nullptr, Node* prev = nullptr)
        {
            this->numElements = numElements;
            this->next = next;
            this->prev = prev;
        }
    };

    Node* foot;
    Node* head;
    int size;

public:
    UnLinList()
    {
        foot = nullptr;
        head = nullptr;
        size = 0;
    }

    void push_back(const T& value)
    {
        if (!head)
        {
            head = new Node();
            head->data[head->numElements++] = value;
            foot = head;
        }
        else 
        {
            if (foot->numElements < ceil(maxElements/2)) 
                foot->data[foot->numElements++] = value;
            else
            {
                foot->next = new Node;
                foot->next->data[foot->next->numElements++] = value;
                foot->next->prev = foot;
                foot = foot->next;
            }
        }
        size++;
    }
    void insert(int index, const T& value)
    {
        const int startIndex = index;
        if (index < 0 || index > size)
        {
            cerr << "\n### Error: incorrect index! ###\n";
            return;
        }

        if (index == 0 && !head) 
            insertBegin(value);
        else
        {
            int currentIndex = 0;
            Node* current = head;
            while (current && currentIndex + current->numElements < index)
            {
                currentIndex += current->numElements;
                current = current->next;
            }

            if (!current)
            {
                cerr << "\n### Error: unable to find the node before the given index! ###\n";
                return;
            }

            index -= currentIndex;
            if (current->numElements < maxElements)
            {
                for (int i = current->numElements; i > index; i--) 
                    current->data[i] = current->data[i - 1];

                current->data[index] = value;
                current->numElements++;
                size++;
            }
            else if (current->next && current->next->numElements < maxElements)
            {
                Node* nextNode = current->next;
                for (int i = nextNode->numElements; i > 0; i--)  
                    nextNode->data[i] = nextNode->data[i-1];

                nextNode->data[zeroIndex] = current->data[maxElements - 1];
                nextNode->numElements++;
                current->numElements--;

                insert(startIndex, value);
            }
            else
            {
                Node* newNode = new Node();
                int mid = maxElements / 2;

                for (int i = 0; i < mid; i++)
                {
                    newNode->data[i] = current->data[mid + i];
                    newNode->numElements++;
                    current->numElements--;
                }
                
                newNode->prev = current;
                newNode->next = current->next;
                if (current->next)
                    current->next->prev = newNode;
                else
                    foot = newNode;
                current->next = newNode;

                insert(startIndex, value);
            }
        }
    }
    void insertBegin(const T& value)
    {
        if (!head || head->numElements < maxElements)
        {
            if (!head) 
            {
                head = new Node();
                foot = head;
            }

            for (int i = head->numElements; i > 0; i--) 
                head->data[i] = head->data[i - 1];

            head->data[zeroIndex] = value;
            head->numElements++;
            size++;
        }
        else
            insert(zeroIndex, value);
    }

    void remove(int index)
    {
        if (index < 0 || index > size)
        {
            cerr << "\n### Error: incorrect index! ###\n";
            return;
        }

        int currentIndex = 0;
        Node* current = head;
        while (current && currentIndex + current->numElements < index)
        {
            currentIndex += current->numElements;
            current = current->next;
        }

        if (!current)
        {
            cerr << "\n### Error: unable to find the node before the given index! ###\n";
            return;
        }

        index -= currentIndex - 1;
        for (int i = index; i < current->numElements - 1; i++)
        {
            current->data[i] = current->data[i + 1];
        }
        current->numElements--;

        if (current->numElements == 0)
        {
            if (current == head)
            {
                head = current->next;
                if (head)
                    head->prev = nullptr;
            }
            else if (current == foot)
            {
                Node* prev = foot->prev;
                foot = prev;
                prev->next = nullptr;
            }
            else
            {
                current->prev->next = current->next;
                if (current->next)
                    current->next->prev = current->prev;
            }

            delete current;
        }
        size--;
    }

    T get(int index)
    {
        if (index < 0 || index >= size)
        {
            throw ("\n### Error: incorrect index! ###\n");
        }

        int currentIndex = 0;
        Node* current = head;
        while (current && currentIndex + current->numElements <= index)
        {
            currentIndex += current->numElements;
            current = current->next;
        }
    
        return current->data[index -= currentIndex];
    }
    void set(int index, T value)
    {
        if (index < 0 || index > size)
        {
            cerr << "\n### Error: incorrect index! ###\n";
        }

        int currentIndex = 0;
        Node* current = head;
        while (current && currentIndex + current->numElements <= index)
        {
            currentIndex += current->numElements;
            current = current->next;
        }

        current->data[index -= currentIndex] = value;
    }

    int find(T value, bool benchmarkMode)
    {
        Node* current = head;
        int index = 0;
        while (current)
        {
            for(int i = 0; i < current->numElements; i++)
            {
                if (current->data[i] == value) return index;
                index++;
            }
            current = current->next;
        }
        if (benchmarkMode) return -1;
        throw ("\n### Element didn`t found ###\n");
    }

    void print()
    {
        Node* current = head;

        while (current)
        {
            cout << "[ ";
            for (int i = 0; i < current->numElements; i++)
            {
                cout << current->data[i];
                if (i != current->numElements - 1) cout << ", ";
            }
            cout << " ] --> ";
            current = current->next;
        }
        cout << "nullptr\n";
    }
    void reverse_print()
    {
        Node* current = foot;

        while (current)
        {
            cout << "[ ";
            for (int i = current->numElements - 1; i >= 0 ; i--)
            {
                cout << current->data[i];
                if (i != 0) cout << ", ";
            }
            cout << " ] --> ";
            current = current->prev;
        }
        cout << "nullptr\n";
    }

    int lenght() 
    {
        return this->size;
    }

    ~UnLinList()
    {
        while (head != nullptr)
        {
            Node* temp = head->next;
            delete head;
            head = temp;
        }
    }
};

void selectMode();
void interactiveMode();
void demonstrationMode();
void benchmarkMode();

void interactiveMenu();
void selectMenu();

int main()
{
    selectMode();
    return 0;
}

void selectMode()
{
    int choice;
    do
    {
        selectMenu();
        choice = getValidInput<int>("Enter your choice: ");
        switch (choice)
        {
        case 1:
            demonstrationMode();
            break;
        case 2:
            benchmarkMode();
            break;
        case 3:
            interactiveMode();
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
            break;
        }
    } while (choice != 0);
}
void interactiveMode()
{
    UnLinList<int> mylist;
    cout << "\n\n=== INTERACTIVE MODE ===\n";

    int choice;
    do
    {
        interactiveMenu();
        choice = getValidInput<int>("Enter your choice: ");

        switch (choice)
        {
        case 1:
        {
            int value = getValidInput<int>("Enter the value to add: ");
            mylist.push_back(value);
            cout << "Element added successfully!\n";
            break;
        }
        case 2:
        {
            int index = getValidInput<int>("Enter the index where you want to insert: ");
            int value = getValidInput<int>("Enter the value to insert: ");
            mylist.insert(index, value);
            cout << "Element inserted successfully!\n";
            break;
        }
        case 3:
        {
            int index = getValidInput<int>("Enter the index of the element to remove: ");
            mylist.remove(index);
            cout << "Element removed successfully!\n";
            break;
        }
        case 4:
        {
            int index = getValidInput<int>("Enter the index of the element to get: ");
            try { cout << "Element at index " << index << ": " << mylist.get(index) << endl; }
            catch (const char* errorMessage) { cerr << errorMessage << endl; }
            break;
        }
        case 5:
        {
            int index = getValidInput<int>("Enter the index of the element to set: ");
            int value = getValidInput<int>("Enter the new value: ");
            mylist.set(index, value);
            cout << "Element set successfully!\n";
            break;
        }
        case 6:
        {
            int value = getValidInput<int>("Enter the value to find: ");
            try
            {
                int index = mylist.find(value, !isBenchmark);
                cout << "Element found at index: " << index << endl;
            }
            catch (const char* errorMessage) { cerr << errorMessage << endl; }
            break;
        }
        case 7:
        {
            cout << "List contents:\n";
            mylist.print();
            break;
        }
        case 8:
        {
            cout << "List contents in reverse:\n";
            mylist.reverse_print();
            break;
        }
        case 9:
        {
            cout << "Size of the list: " << mylist.lenght() << endl;
            break;
        }
        case 0:
            cout << "Exiting interactive mode...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
            break;
        }
    } 
    while (choice != 0);
}
void demonstrationMode()
{
    UnLinList<int> myUnrolled;

    this_thread::sleep_for(chrono::seconds(1));
    cout << "\n\n=== DEMONSTATION UNROLLED LINKED LIST ===\n";

    this_thread::sleep_for(chrono::seconds(1));
    cout << "\n=== ADD 10 ELEMENTS TO THE END ===\n";
    for (int i = 0; i < 10; i++)
        myUnrolled.push_back(i);

    this_thread::sleep_for(chrono::seconds(1));
    cout << "\n=== PRINT THE LIST ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    myUnrolled.print();
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== REVETSE THE LIST IN REVERSE ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    myUnrolled.reverse_print();
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== REMOVE AN ELEMENT AT 3 INDEX ===\n";
    myUnrolled.remove(3);
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== INSERT -2 AT 8 INDEX ===\n";
    myUnrolled.insert(8, -2);
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== INSERT 15 AT 15 INDEX ===\n";
    myUnrolled.insert(15, 15);
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== SET -20 AT 4 INDEX ===\n";
    myUnrolled.set(4, -20);
    this_thread::sleep_for(chrono::seconds(1));

    this_thread::sleep_for(chrono::seconds(1));
    cout << "\n=== PRINT THE LIST ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    myUnrolled.print();
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== PRINT THE LIST IN REVERSE ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    myUnrolled.reverse_print();
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== GET AN ELEMENT AT 5 INDEX ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    try { cout << "Get: " << myUnrolled.get(5) << endl; }
    catch (const char* errorMessage) { cerr << errorMessage << endl; }
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== GET AN ELEMENT AT 14 INDEX ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    try { cout << "Get: " << myUnrolled.get(14) << endl; }
    catch (const char* errorMessage) { cerr << errorMessage << endl; }
    this_thread::sleep_for(chrono::seconds(1));
       
    cout << "\n=== GET THE SIZE OF THE LIST ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Size of the list: " << myUnrolled.lenght() << endl;
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== FIND THE INDEX OF -2 ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    try { cout << "Element founded at index: " << myUnrolled.find(-2, !isBenchmark) << endl; }
    catch (const char* errorMessage) { cerr << errorMessage << endl; };
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== FIND THE INDEX OF -1 ===\n";
    this_thread::sleep_for(chrono::seconds(1));
    try { cout << "Element founded at index: " << myUnrolled.find(-1, !isBenchmark) << endl; }
    catch (const char* errorMessage) { cerr << errorMessage << endl; };
    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n=== DEMONSTRATION END ===";
}
void benchmarkMode()
{
    cout << "\n=== BENCHMARK MODE ===\n";
    UnLinList<int> mylist;

    int numOperation;
    do { numOperation = getValidInput<int>("Enter a num of operation: "); } while (numOperation < 1 || numOperation > maxElem);
    int tenPercentOperation = ceil(numOperation * 0.1);

    this_thread::sleep_for(chrono::seconds(1));
    cout << "\n\n=== BENCHMARK UNROLLED LINKED LIST ===\n";

    auto startTime = chrono::high_resolution_clock::now();

    // ADD N ELEMENTS
    auto startTimeAdd = chrono::high_resolution_clock::now();
    for (int i = 0; i < numOperation; i++)
        mylist.push_back(rand() % randIndex);

   auto endTimeAdd = chrono::high_resolution_clock::now();

    // REMOVE 10% ELEMENTS FROM START NUMBERS ELEMENT ON RANDOM PLACE
    auto startTimeRemove = chrono::high_resolution_clock::now();
    for (int i = 0; i < tenPercentOperation; i++)
        mylist.remove(rand() % mylist.lenght());
    auto endTimeRemove = chrono::high_resolution_clock::now();

    // INSERT 10% RANDOM ELEMENTS FROM START NUMBERS ELEMENT BEFORE RANDOM INDEX
    auto startTimeInsert = chrono::high_resolution_clock::now();
    for (int i = 0; i < tenPercentOperation; i++)
        mylist.insert(rand() % mylist.lenght(), rand() % randIndex);
    auto endTimeInsert = chrono::high_resolution_clock::now();

    // SET 10 % RANDOM ELEMENTS FROM START NUMBERS ELEMENT ON RANDOM PLACE
    auto startTimeSet = chrono::high_resolution_clock::now();
    for (int i = 0; i < tenPercentOperation; i++)  
        mylist.set(rand() % mylist.lenght(), rand() % randIndex);
    auto endTimeSet = chrono::high_resolution_clock::now();

    // GET 10% RANDOM ELEMENTS FROM START NUMBERS ELEMENT ON RANDOM PLACE
    auto startTimeGet = chrono::high_resolution_clock::now();
    for (int i = 0; i < tenPercentOperation; i++)
        mylist.get(rand() % mylist.lenght());
    auto endTimeGet = chrono::high_resolution_clock::now();

    // FIND 10% RANDOM ELEMENTS FROM START NUMBERS ELEMENT ON RANDOM PLACE
    auto startTimeFind = chrono::high_resolution_clock::now();
    for (int i = 0; i < tenPercentOperation; i++)
        mylist.find(rand() % randIndex, isBenchmark);
    auto endTimeFind = chrono::high_resolution_clock::now();

    auto endTime = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    auto durationAdd = chrono::duration_cast<chrono::milliseconds>(endTimeAdd - startTimeAdd);
    auto durationRemove = chrono::duration_cast<chrono::milliseconds>(endTimeRemove - startTimeRemove);
    auto durationInsert = chrono::duration_cast<chrono::milliseconds>(endTimeInsert - startTimeInsert);
    auto durationSet = chrono::duration_cast<chrono::milliseconds>(endTimeSet - startTimeSet);
    auto durationGet = chrono::duration_cast<chrono::milliseconds>(endTimeGet - startTimeGet);
    auto durationFind = chrono::duration_cast<chrono::milliseconds>(endTimeFind - startTimeFind);

    cout << "Duration Add: " << durationAdd.count() << " milliseconds\n";
    cout << "Duration Remove: " << durationRemove.count() << " milliseconds\n";
    cout << "Duration Insert: " << durationInsert.count() << " milliseconds\n";
    cout << "Duration Set: " << durationSet.count() << " milliseconds\n";
    cout << "Duration Get: " << durationGet.count() << " milliseconds\n";
    cout << "Duration Find: " << durationFind.count() << " milliseconds\n";
    cout << "Duration: " << duration.count()/ 1000.0 << " seconds\n";
    cout << "\n=== BENCHMARK MODE END ===";
}

void interactiveMenu()
{
    cout << "\nMENU:\n"
            "1. Add an element to the end\n"
            "2. Insert an element at a specific index\n"
            "3. Remove an element at a specific index\n"
            "4. Get an element at a specific index\n"
            "5. Set an element at a specific index\n"
            "6. Find the index of an element\n"
            "7. Print the list\n"
            "8. Print the list in reverse\n"
            "9. Get the size of the list\n"
            "0. Exit\n";

}
void selectMenu()
{
    cout << "\n=== SELECT MODE ===\n"
            "1. Demonstration Mode\n"
            "2. Benchmark Mode\n"
            "3. Interactive Mode\n"
            "0. Exit\n";

}
