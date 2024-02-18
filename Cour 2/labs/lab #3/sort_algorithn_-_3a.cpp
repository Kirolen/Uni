#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <algorithm>

#define increasingValues true
#define show true
#define changeStep 25

using namespace std;

struct Point
{
    double x, y, z;

    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    bool operator>(const Point& other) const
    {
        return x > other.x || (x == other.x && (y > other.y || (y == other.y && z > other.z)));
    }
    bool operator<(const Point& other) const
    {
        return x < other.x || (x == other.x && (y < other.y || (y == other.y && z < other.z)));
    }

    friend ostream& operator<<(ostream& os, const Point& p)
    {
        os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
        return os;
    }

    double distanceFromZero() const
    {
        return sqrt(x * x + y * y + z * z);
    }
};

void demonstrationMode();
void benchmarkMode();

vector<Point> generateRandomPoints(int size);
vector<Point> generateIncreasingPoints(int size, bool increasing);
template <typename T>
void printArray(const vector<T>& arr);
template <typename T>
int hoarePartition(vector<T>& arr, int left, int right, bool showStep);
template <typename T>
void merge(vector<T>& left, vector<T>& right, vector<T>& arr);
template <typename T>
T getValidInput(const string& prompt);

template <typename T>
void insertionSort(vector<T>& arr, int left, int right, bool showStep);
template <typename T>
void quickSort(vector<T>& arr, int left, int right, bool showStep);
template <typename T>
void mergeSort(vector<T>& arr);
template <typename T>
void combinateSort(vector<T>& arr, int left, int right, int n, bool showStep);


int main()
{
    srand(time(NULL));
    //demonstrationMode();
    benchmarkMode();
}

void demonstrationMode()
{
    cout << "\n=== DEMONSTRATION MODE ===\n";
    int size = 5;
    vector<Point> randomArray = generateRandomPoints(size);
    vector<Point> increasingArray = generateIncreasingPoints(size, increasingValues);
    vector<Point> decreasingArray = generateIncreasingPoints(size, !increasingValues);

    for (int mode = 0; mode < 5; mode++)
    {
        vector<Point> temp = randomArray, increasingTemp = increasingArray, decreasingTemp = decreasingArray;
        if (mode == 0) cout << "\n=== DEMONSTRATION INSERTION ===\n";
        else if (mode == 1) cout << "\n\n=== DEMONSTRATION QUICK ===\n";
        else if (mode == 2) cout << "\n\n=== DEMONSTRATION MERGE ===\n";
        else if (mode == 3) cout << "\n\n=== DEMONSTRATION COMBINATE SORT ===\n";
        else if (mode == 4) cout << "\n\n=== DEMONSTRATION SORT C++ FUNCTION ===\n";

        cout << "Sorted (random array):\n";
        if (mode == 0) insertionSort(temp, 0, size, !show);
        else if (mode == 1) quickSort(temp, 0, size - 1, !show);
        else if (mode == 2) mergeSort(temp);
        else if (mode == 3) combinateSort(temp, 0, size - 1, changeStep, !show);
        else if (mode == 4) sort(temp.begin(), temp.end());
        printArray(temp);

        cout << "Sorted (increasing array):\n";
        if (mode == 0) insertionSort(increasingTemp, 0, size, !show);
        else if (mode == 1) quickSort(increasingTemp, 0, size - 1, !show);
        else if (mode == 2) mergeSort(increasingTemp);
        else if (mode == 3) combinateSort(increasingTemp, 0, size - 1, changeStep, !show);
        else if (mode == 4) sort(increasingTemp.begin(), increasingTemp.end());
        printArray(increasingTemp);

        cout << "Sorted (decreasing array):\n";
        if (mode == 0) insertionSort(decreasingTemp, 0, size, !show);
        else if (mode == 1) quickSort(decreasingTemp, 0, size - 1, !show);
        else if (mode == 2) mergeSort(decreasingTemp);
        else if (mode == 3) combinateSort(decreasingTemp, 0, size - 1, changeStep, !show);
        else if (mode == 4) sort(decreasingTemp.begin(), decreasingTemp.end());
        printArray(decreasingTemp);
    }
}
void benchmarkMode()
{
    cout << "\n=== BENCHMARK MODE ===\n";

    int size;
    do { size = getValidInput<int>("Enter a size: "); } while (size < 1);

    vector<Point> test = generateRandomPoints(size);
    vector<Point> increasingTest = generateIncreasingPoints(size, increasingValues);
    vector<Point> decreasingTest = generateIncreasingPoints(size, !increasingValues);
    for (int mode = 0; mode < 5; mode++)
    {
        vector<Point> temp = test, increasingTemp = increasingTest, decreasingTemp = decreasingTest;
        if (mode == 0) cout << "\n=== BENCHMARK INSERTION ===\n";
        else if (mode == 1) cout << "\n\n=== BENCHMARK QUICK ===\n";
        else if (mode == 2) cout << "\n\n=== BENCHMARK MERGE ===\n";
        else if (mode == 3) cout << "\n\n=== BENCHMARK COMBINATE SORT ===\n";
        else if (mode == 4) cout << "\n\n=== BENCHMARK SORT C++ FUNCTION ===\n";

        auto startTime = chrono::high_resolution_clock::now();
        auto startRandomTime = chrono::high_resolution_clock::now();
        if (mode == 0)int k; //insertionSort(temp, 0, size, !show);
        else if (mode == 1) quickSort(temp, 0, size - 1, !show);
        else if (mode == 2) mergeSort(temp);
        else if (mode == 3) combinateSort(temp, 0, size-1, changeStep, !show);
        else if (mode == 4) sort(temp.begin(), temp.end());
        auto endRandomTime = chrono::high_resolution_clock::now();
        //cout << "Is sorted: " << is_sorted(temp.begin(), temp.end()) << endl;

        auto startIncreasingTime = chrono::high_resolution_clock::now();
        if (mode == 0)int k;// insertionSort(increasingTemp, 0, size, !show);
        else if (mode == 1) quickSort(increasingTemp, 0, size - 1, !show);
        else if (mode == 2) mergeSort(increasingTemp);
        else if (mode == 3) combinateSort(increasingTemp, 0, size - 1, changeStep, !show);
        else if (mode == 4) sort(increasingTemp.begin(), increasingTemp.end());
        auto endIncreasingTime = chrono::high_resolution_clock::now();

        auto startDeincreasingTime = chrono::high_resolution_clock::now();
        if (mode == 0) int k;// insertionSort(decreasingTemp, 0, size, !show);
        else if (mode == 1) quickSort(decreasingTemp, 0, size - 1, !show);
        else if (mode == 2) mergeSort(decreasingTemp);
        else if (mode == 3) combinateSort(decreasingTemp, 0, size - 1, changeStep, !show);
        else if (mode == 4) sort(decreasingTemp.begin(), decreasingTemp.end());
        auto endDeincreasingTime = chrono::high_resolution_clock::now();

        auto endTime = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
        auto durationRandom = chrono::duration_cast<chrono::milliseconds>(endRandomTime - startRandomTime);
        auto durationIncreasing = chrono::duration_cast<chrono::milliseconds>(endIncreasingTime - startIncreasingTime);
        auto durationDeincreasing = chrono::duration_cast<chrono::milliseconds>(endDeincreasingTime - startDeincreasingTime);

        cout << "Running Time Randomly Array: " << durationRandom.count() / 1000.0 << " seconds\n";
        cout << "Running Time Increasing Array: " << durationIncreasing.count() / 1000.0 << " seconds\n";
        cout << "Running Time Deincreasing Array: " << durationDeincreasing.count() / 1000.0 << " seconds\n";
        cout << "Full Running Time: " << duration.count() / 1000.0 << " seconds\n";
    }
    cout << "\n=== BENCHMARK MODE END ===\n";
}

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
vector<Point> generateRandomPoints(int size)
{
    vector<Point> arr;
    Point a;

    for (int i = 0; i < size; ++i)
    {
        a.x = rand() % 100 / 10.0;
        a.y = rand() % 100 / 10.0;;
        a.z = rand() % 100 / 10.0;;
        arr.push_back(a);
    }

    return arr;
}
vector<Point> generateIncreasingPoints(int size, bool increasing)
{
    vector<Point> arr;
    Point a;

    for (int i = 0; i < size; ++i)
    {
        increasing ? a.x = i : a.x = size - i;
        a.y = 0;
        a.z = 0;;
        arr.push_back(a);
    }

    return arr;
}
template <typename T>
void printArray(const vector<T>& arr)
{
    cout << "[ ";
    for (const T& p : arr)
        cout << p << " ";
    cout << "]\n";
}

template <typename T>
void insertionSort(vector<T>& arr, int left, int right,  bool showStep)
{
    int size = right, j;
    if (size <= 1) return;

    for (int i = left + 1; i < size; i++)
    {
        T key = arr[i];
        for (j = i - 1; j >= 0 && arr[j] > key; j--)
            arr[j + 1] = arr[j];
        arr[j + 1] = key;
        if (showStep) printArray(arr);
    }
}

template <typename T>
int hoarePartition(vector<T>& arr, int left, int right, bool showStep) {
    T pivot = arr[(left + right) / 2]; 
    while (true) {
        while (pivot > arr[left]) left++;
        while (arr[right] > pivot) right--;
        if (left >= right)
            return right;
        swap(arr[left], arr[right]);
        left++;
        right--;
        if (showStep) printArray(arr);
    }
}
template <typename T>
void quickSort(vector<T>& arr, int left, int right, bool showStep) {
    if (left < right) 
    {
        int partition_index = hoarePartition(arr, left, right, showStep);
        quickSort(arr, left, partition_index, showStep);
        quickSort(arr, partition_index + 1, right, showStep);
    }
}

template <typename T>
void mergeSort(vector<T>& arr)
{
    size_t size = arr.size();
    if (size <= 1) return;

    size_t middle = size / 2;
    vector<T> left(arr.begin(), arr.begin() + middle);
    vector<T> right(arr.begin() + middle, arr.end());

    mergeSort(left);
    mergeSort(right);
    merge(left, right, arr);
}
template <typename T>
void merge(vector<T>& left, vector<T>& right, vector<T>& arr)
{
    size_t leftSize = left.size();
    size_t rightSize = right.size();
    size_t i = 0, l = 0, r = 0;

    while (l < leftSize && r < rightSize)
    {
        if (left[l] < right[r])
            arr[i++] = left[l++];
        else
            arr[i++] = right[r++];
    }

    while (l < leftSize)
        arr[i++] = left[l++];

    while (r < rightSize)
        arr[i++] = right[r++];
}

template <typename T>
void combinateSort(vector<T>& arr, int left, int right, int n, bool showStep)
{
    int size = right - left + 1;
    if (size <= 1)
        return;

    if (size <= n)
    {
        insertionSort(arr, left, ++right, showStep);
    }
    else
    {
        int partition_index = hoarePartition(arr, left, right, showStep);
        combinateSort(arr, left, partition_index, n, showStep);
        combinateSort(arr, partition_index + 1, right, n, showStep);
    }
}
