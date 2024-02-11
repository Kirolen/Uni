/*
Реалізувати структуру даних Unrolled link list (див. наприклад https://en.wikipedia.org/wiki/Unrolled_linked_list ). 
Операції стандартні для списків – обхід (в обох напрямках), додавання елементу (в кінець, початок або довільну позицію), 
видалення елементу, доступ за індексом, пошук за значенням.
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <cmath>

const int SIZE = 40000;

using namespace std;

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

struct point
{
	double x, y, z;

	point(int x = int(), int y = int(), int z = int())
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	friend ostream& operator<<(ostream& os, const point& object)
	{
		os << "x: " << object.x << " y: " << object.y << " z: " << object.z;
		return os;
	}
	bool operator ==(const point& object)
	{
		return (object.x == this->x) && (object.y == this->y) && (object.z == this->z);
	}

	point get()
	{
		this->x = getValidInput<double>("Enter x value for the point: ");
		this->y = getValidInput<double>("Enter y value for the point: ");
		this->z = getValidInput<double>("Enter z value for the point: ");
		return *this;
	}
};

enum DataStructure 
{
	LIST_ARRAY,
	DYNAMIC_ARRAY,
	LIST
};

template <typename T>
class ListArr
{
private:
	T arrData[SIZE];
	int arrSize;

public:
	ListArr() 
	{
		arrSize = 0;
	}

	void append(T data) 
	{
		if (arrSize == SIZE) 
		{
			cerr << "\n\n### Error: List is full! ###\n\n";
			return;
		}
		arrData[arrSize++] = data;
	}

	void insert(T data, int index)
	{
		if (index < 0 || index >= SIZE)
		{
			cerr << "\n### Error: incorrect index! ###\n";
			return;
		}
		else if (arrSize == SIZE) 
		{
			cerr << "\n### Error: List is full!###\n";
			return;
		}

		for (int i = arrSize; i > index; i--)
			arrData[i] = arrData[i - 1];

		arrData[index] = data;
		arrSize++;
	}

	void remove(int index)
	{
		if (index < 0 || index >= arrSize)
		{
			cerr << "\n### Error: incorrect index! ###\n";
			return;
		}
		for (int i = index; i < arrSize - 1; i++) 
			arrData[i] = arrData[i + 1]; 
		arrSize--;
	}

	T get(int index)
	{
		if (index < 0 || index >= arrSize) 
			throw out_of_range("\n### Error: incorrect index! ###\n"); 
		return arrData[index];
	}

	void set(T data, int index) 
	{
		if (index < 0 || index >= arrSize) {
			cout << "\n### Error: incorrect index! ###\n";
			return;
		}
		arrData[index] = data;
	}
	
	int lenght()
	{
		return arrSize;
	}

	int find(T data) 
	{
		for (int i = 0; i < arrSize; i++) 
			if (arrData[i] == data) return i;
		return -1;
	}

	bool isEmpty()
	{
		return arrSize == 0;
	}

	void print() 
	{
		for (int i = 0; i < arrSize; i++)
			cout << arrData[i] << "\n";
	}
};

template <typename T>
class dynamicArr
{
private:
	T* dataArr;
	int size;

public:
	dynamicArr()
	{
		dataArr = nullptr;
		size = 0;
	}

	void append (T data)
	{
		int newSize = size + 1;
		T* newDataArr = new T[newSize];
		for (int i = 0; i < size; i++) 
			newDataArr[i] = dataArr[i];
		newDataArr[size] = data;
		delete[] dataArr;
		size++;
		dataArr = newDataArr;
	}

	void insert(T data, int index)
	{
		if (index < 0 || index >= size)
		{
			cout << "\n### Error: incotrect index! ###\n";
			return;
		}

		int newSize = size + 1;
		T* newDataArr = new T[newSize];
		for (int i = 0, j = 0; i < newSize; i++)
		{
			if (i == index) continue;
			newDataArr[i] = dataArr[j++];
		}
		newDataArr[index] = data;
		delete[] dataArr;
		size++;
		dataArr = newDataArr;
	}

	void remove(int index)
	{
		if (index < 0 || index >= size)
		{
			cout << "\n### Error: incotrect index! ###\n";
			return;
		}

		int newSize = size - 1;
		T* newDataArr = new T[newSize];
		for (int i = 0, j = 0; i < size; i++)
		{
			if (i == index) continue;
			newDataArr[j++] = dataArr[i];
		}
		delete[] dataArr;
		size--;
		dataArr = newDataArr;
	}

	T get(int index)
	{
		if (index < 0 || index >= size) 
			throw out_of_range("\n### Error: incorrect index! ###\n"); 
		return dataArr[index];
	}

	void set(T data, int index) {
		if (index < 0 || index >= size) 
		{
			cout << "\n### Error: incotrect index! ###\n";
			return;
		}
		dataArr[index] = data;
	}

	int lenght()
	{
		return size;
	}

	int find(T data) 
	{
		for (int i = 0; i < size; i++) 
			if (dataArr[i] == data) return i; 
		return -1;
	}

	void print()
	{
		for (int i = 0; i < size; i++)
			cout << dataArr[i] << "\n";
	}

	bool isEmpty()
	{
		return size == 0;
	}

	~dynamicArr()
	{
		delete[] dataArr;
	}
};

template <typename T>
class List
{
private:
	class Node
	{
	public:
		T data;
		Node* next;
		Node(T data = T(),Node *next = nullptr)
		{
			this->data = data;
			this->next = next;
		}
	};

	Node* head;
	int size;
public:
	List()
	{
		size = 0;
		head = nullptr;
	}

	void append(T data)
	{
		if (head == nullptr)
		{
			head = new Node(data);
			this->size++;
		}
		else
		{
			Node* current = this->head;
			while (current->next != nullptr) current = current->next;
			current->next = new Node(data);
			this->size++;
		}
	}

	void insert(T data, int index)
	{
		if (index < 0 || index > size)
		{
			cout << "\n### Error: incotrect index! ###\n";
			return;
		}

		if (index == 0)
		{
			head = new Node(data, head);
			size++;
			return;
		}

		Node* current = head;
		for (int i = 0; i < index - 1; i++) 
			current = current->next; 
		current->next = new Node(data, current->next);
		size++;
	}

	void remove(int index)
	{
		if (index < 0 || index >= size)
		{
			cout << "\n### Error: incotrect index! ###\n";
			return;
		}

		if (index == 0)
		{
			Node* headTemp = head->next;
			delete head;
			head = headTemp;
			size--;
			return;
		}

		int i = 0;
		Node* current = head;
		while (i != index - 1)
		{
			current = current->next;
			i++;
		}

		Node* temp = current->next;
		current->next = temp->next;
		delete temp;
		size--;
	}

	T get(int index)
	{
		if (index < 0 || index >= size) 
			throw out_of_range("\n### Error: incorrect index! ###\n"); 

		Node* current = head;
		for (int i = 0; i < index-1; i++) 
			current = current->next; 

		return current->data;
	}

	void set(T data, int index) 
	{
		if (index < 0 || index >= size) {
			cout << "\n### Error: incorrect index! ###\n";
			return;
		}

		Node* current = head;
		for (int i = 0; i < index; i++) 
			current = current->next; 
		current->data = data;
	}

	int lenght()
	{
		return size;
	}

	int find(T data) 
	{
		Node* current = head;
		for (int i = 0; i < size; i++) 
		{
			if (current->data == data) return i;
			current = current->next;
		}
		return -1;
	}

	bool isEmpty()
	{
		return size == 0;
	}

	void print()
	{
		Node* current = this->head;
		while (current != nullptr)
		{
			cout << current->data << "\n";
			current = current->next;
		}
	}

	~List()
	{
		while (head != nullptr)
		{
			Node* temp = head->next;
			delete head;
			head = temp;
		}

	}
};

void interactiveMode();
void demonstrationMode();
void benchmarkMode();

void menu();
DataStructure selectDataStructure();

point randomPoint();

int main()
{
	srand(time(NULL));
	cout << "Program Mode\n"
		"1. Interactive Mode\n"
		"2. Demonstartion Mode\n"
		"3. Benchmark Mode\n";
	int choice;
	choice = getValidInput<int>("Enter a program mode: ");
	switch(choice)
	{
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
			cout << "\n### Invalid number! ###\n";
			break;
	}
}

void menu()
{
	cout << "\nMenu: \n"
			"1. Append\n"
			"2. Insert\n"
			"3. Remove\n"
			"4. Get\n"
			"5. Set\n"
			"6. Print\n"
			"7. Find\n"
			"8. Length\n"
			"9. isEmpty\n"
			"0. Exit\n";

}
DataStructure selectDataStructure() {
	int choice;

	while (true)
	{
		cout << "\n=== Data Structure Selection ===\n";
		cout << "1. Fixed Size Array (ListArr)\n";
		cout << "2. Dynamic Array (DynamicArr)\n";
		cout << "3. Linked List (List)\n";

		choice = getValidInput<int>("Enter your choice: ");

		if (choice < 1 || choice > 3) {
			cout << "Invalid choice. Please try again.\n";
			continue;
		}

		return static_cast<DataStructure>(choice - 1);
	}
}
point randomPoint()
{
	point random(rand() / 10000.0, rand() / 10000.0, rand() / 10000.0);
	return random;
}

void demonstrationMode()
{
	cout << "\n=== DEMONSTRATION MODE ===\n";
	ListArr<point> myArray;
	dynamicArr<point> myDynamicArray;
	List<point> myList;

	for (int mode = 0; mode < 3; mode++)
	{
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) cout << "\n\n=== DEMONSTATION FIXED SIZE ARRAY ===\n";
		else if (mode == 1) cout << "\n\n=== DEMONSTRATION DYNAMIC SIZE ARRAY ===\n";
		else if (mode == 2) cout << "\n\n=== DEMONSTRATION LIST ===\n";

		this_thread::sleep_for(chrono::seconds(1));
		cout << "\n=== ADD 5 ELEMENTS ===\n";
		for (int i = 1; i < 6; i++)
		{
			point temp(i, i, i);
			if (mode == 0) myArray.append(temp);
			else if (mode == 1) myDynamicArray.append(temp);
			else if (mode == 2) myList.append(temp);
		}

		this_thread::sleep_for(chrono::seconds(1));
		cout << "\n=== PRINT ALL ELEMENT ===\n";
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) myArray.print();
		else if (mode == 1) myDynamicArray.print();
		else if (mode == 2) myList.print();

		this_thread::sleep_for(chrono::seconds(1));
		cout << "\n=== ADD 1 MORE ELEMENT ===\n";
		point addTemp(0, 0, 0);
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) myArray.append(addTemp);
		else if (mode == 1) myDynamicArray.append(addTemp);
		else if (mode == 2) myList.append(addTemp);

		cout << "\n=== REMOVE 0 INDEX ELEMENT ===\n";
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) myArray.remove(0);
		else if (mode == 1) myDynamicArray.remove(0);
		else if (mode == 2) myList.remove(0);

		cout << "\n=== INSERT {10, 10, 10} BEFORE 1 INDEX ELEMENT ===\n";
		point insertTemp(10, 10, 10);
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) myArray.insert(insertTemp, 0);
		else if (mode == 1) myDynamicArray.insert(insertTemp, 0);
		else if (mode == 2) myList.insert(insertTemp, 0);

		cout << "\n=== SET {8, 0, 0} ON 4 INDEX ELEMENT ===\n";
		point setTemp(8, 10, 10);
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) 	myArray.set(setTemp, 4);
		else if (mode == 1) myDynamicArray.set(setTemp, 4);
		else if (mode == 2) myList.set(setTemp, 4);

		this_thread::sleep_for(chrono::seconds(1));
		cout << "\n=== PRINT ARRAY ===\n";
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) myArray.print();
		else if (mode == 1) myDynamicArray.print();
		else if (mode == 2) myList.print();

		this_thread::sleep_for(chrono::seconds(1));
		cout << "\n=== GET 4 INDEX ELEMENT ===\n";
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) 	cout << "Get: " << myArray.get(4) << "\n";
		else if (mode == 1) cout << "Get: " << myDynamicArray.get(4) << "\n";
		else if (mode == 2) cout << "Get: " << myList.get(4) << "\n";

		this_thread::sleep_for(chrono::seconds(1));
		cout << "\n=== GET SIZE ARRAY ===\n";
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) 	cout << "Size: " << myArray.lenght() << "\n";
		else if (mode == 1) cout << "Size: " << myDynamicArray.lenght() << "\n";
		else if (mode == 2) cout << "Size: " << myList.lenght() << "\n";

		this_thread::sleep_for(chrono::seconds(1));
		cout << "\n=== FIND {8, 0, 0} ELEMENT INDEX ===\n";
		point findTemp(8, 0, 0);
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) 	cout << "Index: " << myArray.find(findTemp) << "\n";
		else if (mode == 1) cout << "Index: " << myDynamicArray.find(findTemp) << "\n";
		else if (mode == 2) cout << "Size: " << myList.find(findTemp) << "\n";
	}
	cout << "\n=== DEMONSTRATION END ===";
}
void interactiveMode()
{
	cout << "\n=== Interactive Mode ===\n";
	DataStructure ds = selectDataStructure();

	int choice = -1;
	int index;
	point data;

	ListArr<point> listArr;
	dynamicArr<point> dynamic;
	List<point> myList;
	while (choice != 0) 
	{
		menu();
		choice = getValidInput<int>("Enter your choice: ");

		switch (choice) {
		case 0:
			cout << "\n=== EXIT ===\n";
			break;
		case 1:
			data = data.get();
			if (ds == LIST_ARRAY) listArr.append(data);
			else if (ds == DYNAMIC_ARRAY) dynamic.append(data);
			else myList.append(data);

			break;
		case 2:
			index = getValidInput<int>("Enter index to insert: ");
			data = data.get();
			if (ds == LIST_ARRAY) listArr.insert(data, index);
			else if (ds == DYNAMIC_ARRAY) dynamic.insert(data, index);
			else myList.insert(data, index);

			break;
		case 3:
			index = getValidInput<int>("Enter index to remove: ");
			if (ds == LIST_ARRAY) listArr.remove(index);
			else if (ds == DYNAMIC_ARRAY) dynamic.remove(index);
			else myList.remove(index);

			break;
		case 4:
			index = getValidInput<int>("Enter index to get: ");
			try 
			{
				if (ds == LIST_ARRAY) cout << listArr.get(index) << "\n";
				else if (ds == DYNAMIC_ARRAY) cout << dynamic.get(index) << "\n";
				else cout << myList.get(index) << "\n";
			}
			catch (const out_of_range& e) { cerr << e.what() << endl; }
			break;
		case 5:
			index = getValidInput<int>("Enter index to set: ");
			data = data.get();
			if (ds == LIST_ARRAY) listArr.set(data, index);
			else if (ds == DYNAMIC_ARRAY) dynamic.set(data, index);
			else myList.set(data, index);

			break;
		case 6:
			if (ds == LIST_ARRAY) listArr.print();
			else if (ds == DYNAMIC_ARRAY) dynamic.print();
			else myList.print();

			break;
		case 7:
			cout << "Enter data to find: \n";
			data = data.get();
			if (ds == LIST_ARRAY) cout << "Index: " << listArr.find(data) << "\n";
			else if (ds == DYNAMIC_ARRAY) cout << "Index: " << dynamic.find(data) << "\n";
			else cout << "Index: " << myList.find(data) << "\n";

			break;
		case 8:
			if (ds == LIST_ARRAY) cout << "Lenght: " << listArr.lenght() << "\n";
			else if (ds == DYNAMIC_ARRAY) cout << "Lenght: " << dynamic.lenght() << "\n";
			else cout << "Lenght: " << myList.lenght() << "\n";
			break;
		case 9:
			if (ds == LIST_ARRAY) listArr.isEmpty() ? cout << "Fixed array is empty\n" : cout << "Fixed array isn`t empty\n";
			else if (ds == DYNAMIC_ARRAY) dynamic.isEmpty() ? cout << "Dynamic array is empty\n" : cout << "Dynamic array isn`t empty\n";
			else myList.isEmpty() ? cout << "List is empty\n" : cout << "List isn`t empty\n";

			break;
		}

	}
}
void benchmarkMode()
{
	cout << "\n=== BENCHMARK MODE ===\n";
	ListArr<point> myArray;
	dynamicArr<point> myDynamicArray;
	List<point> myList;

	int numOperation;
	do { numOperation = getValidInput<int>("Enter a num of operation: "); } while (numOperation < 1 || numOperation > SIZE);
	int tenPercentOperation = ceil(numOperation * 0.1);

	for (int mode = 0; mode < 3; mode++)
	{
		this_thread::sleep_for(chrono::seconds(1));
		if (mode == 0) cout << "\n\n=== BENCHMARK FIXED SIZE ARRAY ===\n";
		else if (mode == 1) cout << "\n\n=== BENCHMARK DYNAMIC SIZE ARRAY ===\n";
		else if (mode == 2) cout << "\n\n=== BENCHMARK LIST ===\n";

		auto startTime = chrono::high_resolution_clock::now();

		// ADD N ELEMENTS
		auto startTimeAdd = chrono::high_resolution_clock::now();
		for (int i = 1; i < numOperation; i++)
		{
			point temp(i, i, i);
			if (mode == 0) myArray.append(temp);
			else if (mode == 1) myDynamicArray.append(temp);
			else if (mode == 2) myList.append(temp);
		}
		auto endTimeAdd = chrono::high_resolution_clock::now();

		// REMOVE 10% ELEMENT FROM START NUMBERS ELEMENT ON RANDOM PLACE
		auto startTimeRemove = chrono::high_resolution_clock::now();
		for (int i = 0; i < tenPercentOperation; i++)
		{
			if (mode == 0) myArray.remove(rand() % myArray.lenght());
			else if (mode == 1) myDynamicArray.remove(rand() % myDynamicArray.lenght());
			else if (mode == 2) myList.remove(rand() % myList.lenght());
		}
		auto endTimeRemove = chrono::high_resolution_clock::now();


		// INSERT 10% RANDOM POINT FROM START NUMBERS ELEMENT BEFORE RANDOM INDEX
		auto startTimeInsert = chrono::high_resolution_clock::now();
		for (int i = 0; i < tenPercentOperation; i++)
		{
			if (mode == 0) myArray.insert(randomPoint(), rand() % myArray.lenght());
			else if (mode == 1) myDynamicArray.insert(randomPoint(), rand() % myDynamicArray.lenght());
			else if (mode == 2) myList.insert(randomPoint(), rand() % myList.lenght());
		}
		auto endTimeInsert = chrono::high_resolution_clock::now();

		// SET 10 % RANDOM POINT FROM START NUMBERS ELEMENT ON RANDOM PLACE
		auto startTimeSet = chrono::high_resolution_clock::now();
		for (int i = 0; i < tenPercentOperation; i++)
		{
			if (mode == 0) 	myArray.set(randomPoint(), rand() % myArray.lenght());
			else if (mode == 1) myDynamicArray.set(randomPoint(), rand() % myDynamicArray.lenght());
			else if (mode == 2) myList.set(randomPoint(), rand() % myList.lenght());
		}
		auto endTimeSet = chrono::high_resolution_clock::now();

		// GET 10% RANDOM POINT FROM START NUMBERS ELEMENT ON RANDOM PLACE
		auto startTimeGet = chrono::high_resolution_clock::now();
		for (int i = 0; i < tenPercentOperation; i++)
		{
			if (mode == 0) myArray.get(rand() % myArray.lenght());
			else if (mode == 1) myDynamicArray.get(rand() % myDynamicArray.lenght());
			else if (mode == 2) myList.get(rand() % myList.lenght());
		}
		auto endTimeGet = chrono::high_resolution_clock::now();

		auto endTime = chrono::high_resolution_clock::now();

		auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
		auto durationAdd = chrono::duration_cast<chrono::milliseconds>(endTimeAdd - startTimeAdd);
		auto durationRemove = chrono::duration_cast<chrono::milliseconds>(endTimeRemove - startTimeRemove);
		auto durationInsert = chrono::duration_cast<chrono::milliseconds>(endTimeInsert - startTimeInsert);
		auto durationSet = chrono::duration_cast<chrono::milliseconds>(endTimeSet - startTimeSet);
		auto durationGet = chrono::duration_cast<chrono::milliseconds>(endTimeGet - startTimeGet);

		cout << "Duration Add: " << durationAdd.count() << " milliseconds\n";
		cout << "Duration Remove: " << durationRemove.count() << " milliseconds\n";
		cout << "Duration Insert: " << durationInsert.count() << " milliseconds\n";
		cout << "Duration Set: " << durationSet.count() << " milliseconds\n";
		cout << "Duration Get: " << durationGet.count() << " milliseconds\n";
		cout << "Duration: " << duration.count() / 1000.0<< " seconds\n";
	}
	cout << "\n=== BENCHMARK MODE END ===\n";
}
