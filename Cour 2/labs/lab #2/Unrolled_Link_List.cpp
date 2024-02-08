#include <iostream>
#include <cmath>

using namespace std;

#define maxElements 4  
#define zeroIndex 0

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
            {
                foot->data[foot->numElements++] = value;
            }
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

        if (index == 0 && !head) insertBegin(value);
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
        while (current && currentIndex + current->numElements < index)
        {
            currentIndex += current->numElements;
            current = current->next;
        }

        current->data[currentIndex] = value;
    }

    int find(T value)
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

void demonstrationMode();

int main()
{
    demonstrationMode();
    return 0;
}

void demonstrationMode()
{
    UnLinList<int> myUnrolled;
    myUnrolled.insert(0, 6);
    myUnrolled.insert(0, 6);
    myUnrolled.insertBegin(6);
    myUnrolled.insert(0, 6);
    myUnrolled.insert(0, 6);
    myUnrolled.insertBegin(6);
    myUnrolled.print();
    cout << "================================\n";

    myUnrolled.insert(3, 1);
    myUnrolled.print();
    cout << "================================\n";

    myUnrolled.insert(3, 2);
    myUnrolled.print();
    cout << "================================\n";

    myUnrolled.insert(3, 3);
    myUnrolled.print();
    cout << "================================\n";

    myUnrolled.insert(3, 4);
    myUnrolled.print();
    cout << "================================\n";

    myUnrolled.insert(3, 5);
    myUnrolled.print();
    cout << "================================\n";
    myUnrolled.insert(11, 100);
    myUnrolled.print();
    cout << "================================\n";

    myUnrolled.remove(5);
    myUnrolled.remove(11);
    myUnrolled.remove(10);
    myUnrolled.print();
    cout << "================================\n";

    try { cout << "Get 0: " << myUnrolled.get(0) << endl; }
    catch (const char* errorMessage) { cerr << "Error: " << errorMessage << std::endl; }
    try { cout << "Get 5: " << myUnrolled.get(5) << endl; }
    catch (const char* errorMessage) { cerr << "Error: " << errorMessage << std::endl; }
    try { cout << "Get 6: " << myUnrolled.get(6) << endl; }
    catch (const char* errorMessage) { cerr << "Error: " << errorMessage << std::endl; }

    myUnrolled.set(0, 99);
    myUnrolled.print();
    cout << "================================\n";
        
    try { cout << "Find 99: " << myUnrolled.find(99) << endl; }
    catch (const char* errorMessage) { cerr << "Error: " << errorMessage << std::endl; };
    try { cout << "Find 1000: " << myUnrolled.find(1000) << endl; }
    catch (const char* errorMessage) { cerr << "Error: " << errorMessage << std::endl; }
    try { cout << "Find 3: " << myUnrolled.find(3) << endl; }
    catch (const char* errorMessage) { cerr << "Error: " << errorMessage << std::endl; };

    cout << "\n\n\n\n";
    myUnrolled.reverse_print();
}
