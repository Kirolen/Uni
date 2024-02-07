
#include <iostream>
#include <cmath>
using namespace std;
#define maxElements 4  

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

        Node(int numElements = 0, Node* next = nullptr)
        {
            this->numElements = numElements;
            this->next = next;
        }
    };

    Node* head;
    int size;

public:
    UnLinList()
    {
        head = nullptr;
        size = 0;
    }

    void push_back(const T& value)
    {
        if (!head)
        {
            head = new Node();
            head->data[head->numElements++] = value;
        }
        else 
        {
            Node* current = head;
            while (current->next) { current = current->next; }

            if (current->numElements < ceil(maxElements/2)) { current->data[current->numElements++] = value; }
            else
            {
                current->next = new Node;
                current->next->data[current->next->numElements++] = value;
            }
        }
        size++;
    }
    void insert(int index, const T& value)
    {
        if (index < 0 || index > size)
        {
            cout << "\n### Error: incorrect index! ###\n";
            return;
        }

        if (index == 0) insertBegin(value);
        else
        {
            int currentIndex = 0;
            Node* current = head;
            while (current && currentIndex + current->numElements <= index)
            {
                currentIndex += current->numElements;
                current = current->next;
            }

            if (!current)
            {
                cout << "\n### Error: unable to find the node before the given index! ###\n";
                return;
            }

            index -= currentIndex;
            if (current->numElements < maxElements)
            {
                for (int i = current->numElements; i > index; i--)   current->data[i] = current->data[i - 1];
                current->data[index] = value;
                current->numElements++;
                size++;
            }
            else if (current->next && current->next->numElements < maxElements)
            {
                Node* nextNode = current->next;
                for (int i = nextNode->numElements; i >= 0; i--)     nextNode->data[i] = nextNode->data[i-1];
                nextNode->data[0] = current->data[maxElements - 1];
                nextNode->numElements++;
                current->data[maxElements - 1] = value;
                size++;
            }
            else
            {
                Node* newNode = new Node();
                int mid = maxElements / 2;
                for (int i = mid; i < maxElements; ++i)
                    newNode->data[i - mid] = current->data[i];
                newNode->numElements = maxElements - mid;
                current->numElements = mid;

                newNode->next = current->next;
                current->next = newNode;

                insert(index, value);
            }
        }
    }
    void insertBegin(const T& value)
    {
        if (!head || head->numElements < maxElements)
        {
            if (!head) head = new Node();
            for (int i = head->numElements; i > 0; i--) head->data[i] = head->data[i - 1];
            head->data[0] = value;
            head->numElements++;
            size++;
        }
        else if (head->next&& head->next->numElements < maxElements)
        {
            Node* nextNode = head->next;
            for (int i = nextNode->numElements; i > 0; i--)   nextNode->data[i] = nextNode->data[i - 1];
            nextNode->data[0] = head->data[maxElements - 1];
            nextNode->numElements++;
            head->data[maxElements - 1] = value;
            size++;
        }
        else
        {
            Node* newHead = new Node();
            int mid = head->numElements / 2;
            for (int i = mid; i < head->numElements; ++i)   newHead->data[i - mid] = head->data[i];
            newHead->numElements = head->numElements - mid;
            head->numElements = mid;
            newHead->next = head->next;
            head->next = newHead;

            insertBegin(value);
        }
    }

    void print()
    {
        Node* current = head;

        while (current)
        {
            cout << "[ ";
            for (int i = 0; i < current->numElements; i++)
            {
                cout << current->data[i] << ", ";
            }
            cout << "] --> ";
            current = current->next;
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

    myUnrolled.push_back(1);
    myUnrolled.push_back(2);
    myUnrolled.push_back(3);
    myUnrolled.push_back(4);

    //myUnrolled.print();

    myUnrolled.push_back(5);
   // myUnrolled.print();

    //cout << "ADd 6 on 0:\n";
    myUnrolled.insert(0, 6);
    myUnrolled.insert(0, 6);
    myUnrolled.print();


    myUnrolled.insert(6, 14);
    myUnrolled.print();
}
