#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>

void create_random_array(std::vector<int>& arr);
void print_arr(std::vector<int>& arr);

void insertion_sort(std::vector<int>& arr);

void merge_sort(std::vector<int>& arr);
void merge(std::vector<int>& left, std::vector<int>& right, std::vector<int>& arr);

int hoare_partition(std::vector<int>& arr, int left, int right);
void quick_sort(std::vector<int>& arr, int left, int right);

void demonstartion_mode();
void benchmark_mode();

int main()
{
    srand(time(NULL));
   // demonstartion_mode();
    benchmark_mode();
}

void demonstartion_mode() 
{
    std::vector<int> random_arr, temp;
    create_random_array(random_arr);
    temp = random_arr;

    std::cout << "Start array: \n";
    print_arr(temp);

    std::cout << "Insertion sorted array: \n";
    insertion_sort(temp);
    print_arr(temp);

    std::cout << "Merge sorted array: \n";
    temp = random_arr;
    merge_sort(temp);
    print_arr(temp);

    std::cout << "Merge sorted array: \n";
    temp = random_arr;
    merge_sort(temp);
    print_arr(temp);
}

void benchmark_mode()
{
    std::cout << "\n=== BENCHMARK MODE ===\n";

    std::vector<int> random_arr, temp;
    create_random_array(random_arr);
    temp = random_arr;
    for (int mode = 0; mode < 3; mode++)
    {
        temp = random_arr;
        if (mode == 0) std::cout << "\n=== BENCHMARK INSERTION ===\n";
        else if (mode == 1) std::cout << "\n\n=== BENCHMARK MERGE ===\n";
        else if (mode == 2) std::cout << "\n\n=== BENCHMARK QUICK ===\n";
      

        auto startTime = std::chrono::high_resolution_clock::now();
        if (mode == 0) insertion_sort(temp);
        else if (mode == 1) merge_sort(temp);
        else if (mode == 2) quick_sort(temp, 0, temp.size() - 1);
        auto endTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);


        std::cout << "Time Sorting Array: " << duration.count() / 1000.0 << " seconds\n";
    }
    std::cout << "\n=== BENCHMARK MODE END ===\n";
}

void create_random_array(std::vector<int>& arr) 
{
    int n, diapasone;
    std::cout << "Entter numbers of element: ";
    std::cin >> n;
    std::cout << "Entter diapasone of element: ";
    std::cin >> diapasone;
    for (int i = 0; i < n; i++)
        arr.push_back(rand() % diapasone);
}

void print_arr(std::vector<int>& arr)
{
    int size = arr.size();
    for (int i = 0; i < size; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

void insertion_sort(std::vector<int>& arr) 
{
    int size = arr.size();
    if (size <= 1) return;
    
    for (int i = 1; i < size; i++)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }

}

void merge_sort(std::vector<int>& arr)
{
    int size = arr.size();
    if (size <= 1) return;

    int middle = size / 2;
    std::vector<int> left(arr.begin(), arr.begin() + middle);
    std::vector<int> right(arr.begin() + middle, arr.end());

    merge_sort(left);
    merge_sort(right);
    merge(left, right, arr);
}
void merge(std::vector<int>& left, std::vector<int>& right, std::vector<int>& arr)
{
    int leftSize = left.size();
    int rightSize = right.size();
    int i = 0, l = 0, r = 0;

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

int hoare_partition(std::vector<int>& arr, int left, int right) {
    int pivot_el = arr[(left + right) / 2];
    while (true) {
        while (pivot_el > arr[left]) left++;
        while (arr[right] > pivot_el) right--;
        if (left >= right)
            return right;
        std::swap(arr[left], arr[right]);
        left++;
        right--;
    }
}
void quick_sort(std::vector<int>& arr, int left, int right) {
    if (left < right)
    {
        int partition_index = hoare_partition(arr, left, right);
        quick_sort(arr, left, partition_index);
        quick_sort(arr, partition_index + 1, right);
    }
}
