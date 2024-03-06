import random
import time

def swap(a, b):
    t = a
    a = b
    b = a

def insertion_sort(arr):
    size = len(arr)
    if size <= 1: return 

    for i in range(1, len(arr)):
        
        key = arr[i]
        j = i - 1
        while j >= 0 and arr[j] > key:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key

def merge_sort(arr):
    size = len(arr)
    if size <= 1: return

    middle = size // 2
    left = arr[:middle]
    right = arr[middle:]

    merge_sort(left)
    merge_sort(right)
    merge(left, right, arr)
    return

def merge(left, right, arr):
    left_size, right_size = len(left), len(right)
    i, l, r = 0, 0, 0

    while l < left_size and r < right_size:
        if left[l] < right[r]:
            arr[i] = left[l]
            l += 1
        else:
            arr[i] = right[r]
            r += 1
        i += 1
     
    while l < left_size:
        arr[i] = left[l]
        l +=1
        i +=1

    while r < right_size:
        arr[i] = right[r]
        i += 1
        r += 1
    return

def hoare_partition(arr, left, right):
    pivot = arr[(left + right) // 2]
    while True:
        while pivot > arr[left]: left += 1
        while arr[right] > pivot: right -= 1
        if left >= right: return right
        arr[left], arr[right] = arr[right], arr[left]
        left += 1
        right -= 1

def quick_sort(arr, left, right):
    if left < right:
        partition_index = hoare_partition(arr, left, right)
        quick_sort(arr, left, partition_index)
        quick_sort(arr, partition_index + 1, right)
        
def demonstartin_mode():
    num = int(input("Enter a number of elements: "))
    random_list = [random.randint(0, 10) for _ in range(num)]
    temp = list(random_list)

    print("Original list:", temp)

    insertion_sort(temp)
    print("Inserion sorted list:", temp)
    temp = list(random_list)

    merge_sort(temp)
    print("Merge sorted list:", temp)
    temp = list(random_list)

    quick_sort(temp, 0, len(temp)-1)
    print("Quick sorted list:", temp)
    return;
    

def benchmark_mode():
     num = int(input("Enter a number of elements: "))
     diapasone = int(input("Enter a diaposone of elements: "))
     random_list = [random.randint(0, diapasone) for _ in range(num)]
     
     temp = list(random_list)
     start_time = time.time()
     insertion_sort(temp)
     end_time = time.time()
     elapsed_insertion_time = end_time - start_time
     
     temp = list(random_list)
     start_time = time.time()
     merge_sort(temp)
     end_time = time.time()
     elapsed_merge_time = end_time - start_time

     temp = list(random_list)
     start_time = time.time()
     quick_sort(temp, 0, len(temp) - 1)
     end_time = time.time()
     elapsed_quick_time = end_time - start_time

     print("Time insertion sort: " + str(round(elapsed_insertion_time, 2)) + "seconds")
     print("Time merge sort: " +  str(round(elapsed_merge_time, 2)) + "seconds")
     print("Time quick sort: " + str(round(elapsed_quick_time, 2)) + "seconds")


#demonstartin_mode()
benchmark_mode()


