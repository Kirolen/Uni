#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>

#define RUN 32

void create_random_array(std::vector<int>& arr);
void print_arr(std::vector<int>& arr);
void print_first_last(std::vector<int>& arr);

void insertion_sort(std::vector<int>& arr, int left, int right);
void merge(std::vector<int>& arr, int left, int middle, int right);
void timsort(std::vector<int>& arr);

void demonstartion_mode();
void benchmark_mode();

int main()
{
    srand(time(NULL));
    demonstartion_mode();
    //benchmark_mode();
}

void demonstartion_mode()
{
    std::vector<int> random_arr;
    create_random_array(random_arr);

    std::cout << "Start array: \n";
    print_first_last(random_arr);
    timsort(random_arr);

    std::cout << "Timsorted array: \n";
    print_first_last(random_arr);
}

void benchmark_mode()
{
    std::cout << "\n=== BENCHMARK MODE ===\n";

    std::vector<int> random_arr, temp;
    create_random_array(random_arr);
    temp = random_arr;

    std::cout << "\n=== BENCHMARK TIMSORT ===\n";

    auto startTime = std::chrono::high_resolution_clock::now();
    timsort(random_arr);
    std::cout << "Is_sorted: " << std::is_sorted(random_arr.begin(), random_arr.end()) << std::endl;
    auto endTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);


    std::cout << "Time Sorting Array: " << duration.count() / 1000.0 << " seconds\n";
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

void print_first_last(std::vector<int>& arr) {
    int size = arr.size();

    if (size <= 20) {
        for (int num : arr) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "First five elements: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Last five elements: ";
        for (int i = size - 5; i < size; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
}

void insertion_sort(std::vector<int>& arr, int left, int right)
{
    if (left >= right) return;

    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(std::vector<int>& arr, int left, int middle, int right) {
    int len1 = middle - left + 1;
    int len2 = right - middle;

    std::vector<int> left_arr(len1), right_arr(len2);

    for (int i = 0; i < len1; ++i)
        left_arr[i] = arr[left + i];
    for (int j = 0; j < len2; ++j)
        right_arr[j] = arr[middle + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < len1 && j < len2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        }
        else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < len1) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < len2) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}

void timsort(std::vector<int>& arr) {
    const int n = arr.size();

    for (int i = 0; i < n; i += RUN) {
        insertion_sort(arr, i, std::min((i + RUN - 1), (n - 1)));
    }

    for (int size = RUN; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}
