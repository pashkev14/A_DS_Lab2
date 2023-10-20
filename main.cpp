#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <chrono>

using namespace std::chrono;

enum SEQS {Shell, Hibbard, Sedgewick};

const int RUN = 32;

// instruments

int GetRandomNumber(int min, int max) {
    int num = min + rand() % (max - min + 1);
    return num;
}

int* GenerateArray(int length, int min, int max) {
    int* arr = new int[length];
    for (int i = 0; i < length; ++i)
        arr[i] = GetRandomNumber(min, max);
    return arr;
}

void PrintArray(int* arr, int length) {
    for (int i = 0; i < length; ++i)
        std::cout << arr[i] << " ";
    std::cout << '\n';
}

void Swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void Merge(int* arr, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* larr = new int[n1], * rarr = new int[n2];
    for (i = 0; i < n1; ++i)
        larr[i] = arr[left + i];
    for (j = 0; j < n2; ++j)
        rarr[j] = arr[mid + 1 + j];
    i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (larr[i] <= rarr[j]) {
            arr[k] = larr[i];
            ++i;
        }
        else {
            arr[k] = rarr[j];
            ++j;
        }
        ++k;
    }
    while (i < n1) {
        arr[k] = larr[i];
        ++i; ++k;
    }
    while (j < n2) {
        arr[k] = rarr[j];
        ++j; ++k;
    }
    delete[] larr;
    delete[] rarr;
}

int Partition(int* arr, int left, int right) {
    int pivot = arr[right];
    int i = left - 1;
    for (int j = left; j <= right - 1; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            Swap(arr[i], arr[j]);
        }
    }
    Swap(arr[i + 1], arr[right]);
    return i + 1;
}

void Heapify(int* arr, int length, int root) {
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;
    if (left < length && arr[left] > arr[largest])
        largest = left;
    if (right < length && arr[right] > arr[largest])
        largest = right;
    if (largest != root) {
        Swap(arr[root], arr[largest]);
        Heapify(arr, length, largest);
    }
}

int ChoosePivot(int* arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[mid] < arr[left])
        Swap(arr[left], arr[mid]);
    if (arr[right] < arr[left])
        Swap(arr[left], arr[right]);
    if (arr[right] < arr[mid])
        Swap(arr[mid], arr[right]);
    return mid;
}

int PartitionIntro(int* arr, int left, int right) {
    int pivotIndex = ChoosePivot(arr, left, right);
    int pivotValue = arr[pivotIndex];
    Swap(arr[pivotIndex], arr[right]);
    int storeIndex = left;
    for (int i = left; i < right; i++) {
        if (arr[i] < pivotValue) {
            Swap(arr[i], arr[storeIndex]);
            storeIndex++;
        }
    }
    Swap(arr[storeIndex], arr[right]);
    return storeIndex;
}

// sorting algorithms

void SelectionSort(int* arr, unsigned int length) {
    int index_of_min;
    for (int i = 0; i < length - 1; i++) {
        index_of_min = i;
        for (int j = i + 1; j < length; j++)
            if (arr[j] < arr[index_of_min]) 
                index_of_min = j;
        if (index_of_min != i) 
            Swap(arr[index_of_min], arr[i]);
    }
}

void InsertionSort(int* arr, int begin, int end) {
    int key, j;
    for (int i = begin + 1; i <= end; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= begin && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void BubbleSort(int* arr, unsigned int length) {
    for (int i = 0; i < length - 1; i++) 
        for (int j = 0; j < length - i - 1; j++) 
            if (arr[j] > arr[j + 1]) 
                Swap(arr[j], arr[j + 1]);
}

void MergeSort(int* arr, int begin, int end) {
    if (begin < end) {
        int mid = begin + (end - begin) / 2;
        MergeSort(arr, begin, mid);
        MergeSort(arr, mid + 1, end);
        Merge(arr, begin, mid, end);
    }
}

void QuickSort(int* arr, int begin, int end) {
    if (begin < end) {
        int pivot_index = Partition(arr, begin, end);
        QuickSort(arr, begin, pivot_index - 1);
        QuickSort(arr, pivot_index + 1, end);
    }
}

void ShellSort(int* arr, int length, int seq) {
    int i, j, k, temp, gap;
    std::vector<int> gaps;
    switch (seq) {
    case 0: // Shell gap sequence
        temp = length / 2;
        while (temp > 0) {
            gaps.push_back(temp);
            temp /= 2;
        }
        break;
    case 1: // Hibbard gap sequence (OEIS A000225)
        i = 1;
        temp = pow(2, i) - 1;
        while (temp < length) {
            gaps.push_back(temp);
            ++i; temp = pow(2, i) - 1;
        }
        std::reverse(gaps.begin(), gaps.end());
        break;
    case 2: // Sedgewick gap sequence (OEIS A036562)
        i = 1;
        gaps.push_back(1);
        temp = pow(4, i) + 3 * pow(2, i - 1) + 1;
        while (temp < length) {
            gaps.push_back(temp);
            ++i; temp = pow(4, i) + 3 * pow(2, i - 1) + 1;
        }
        std::reverse(gaps.begin(), gaps.end());
        break;
    default: // default is Shell gap sequence
        temp = length / 2;
        while (temp > 0) {
            gaps.push_back(temp);
            temp /= 2;
        }
        break;
    }
    for (i = 0; i < gaps.size(); ++i) {
        gap = gaps[i];
        for (j = gap; j < length; ++j) {
            temp = arr[j];
            for (k = j; k >= gap && arr[k - gap] > temp; k -= gap)
                arr[k] = arr[k - gap];
            arr[k] = temp;
        }
    }
}

void HeapSort(int* arr, int length) {
    for (int i = length / 2 - 1; i >= 0; i--)
        Heapify(arr, length, i);
    for (int i = length - 1; i >= 0; i--) {
        Swap(arr[0], arr[i]);
        Heapify(arr, i, 0);
    }
}

void TimSort(int* arr, int length) {
    for (int i = 0; i < length; i += RUN)
        InsertionSort(arr, i, std::min((i + RUN - 1), (length - 1)));
    for (int size = RUN; size < length; size *= 2) {
        for (int left = 0; left < length; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min((left + 2 * size - 1), (length - 1));
            if (mid < right)
                Merge(arr, left, mid, right);
        }
    }
}

void QuickSortIntro(int* arr, int begin, int end, int depthLimit) {
    while (end - begin > 16) {
        if (depthLimit == 0) {
            // If depth limit is reached, switch to heapsort
            std::make_heap(arr + begin, arr + end + 1);
            std::sort_heap(arr + begin, arr + end + 1);
            return;
        }
        --depthLimit;
        int pivotIndex = PartitionIntro(arr, begin, end);
        QuickSortIntro(arr, pivotIndex + 1, end, depthLimit);
        end = pivotIndex - 1;
    }
    InsertionSort(arr, begin, end);
}

void IntroSort(int* arr, int length) {
    int depthLimit = 2 * log(length);
    QuickSortIntro(arr, 0, length - 1, depthLimit);
}

int main() {
    srand(time(0));

    int length, min, max;
    
    std::cout << "Input array length: ";
    std::cin >> length;
    
    std::cout << "Input minimum value in range: ";
    std::cin >> min;
    
    std::cout << "Input maximum value in range: ";
    std::cin >> max;
 
    int* arr = GenerateArray(length, min, max);
    
    std::cout << "Your array: ";
    PrintArray(arr, length);
    
    auto start = high_resolution_clock::now();
    QuickSort(arr, 0, length - 1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    std::cout << "Your sorted array: ";
    PrintArray(arr, length);

    std::cout << "Algorithm execution time (in microseconds): " << duration.count();
    
    delete[] arr;

	return 0;
}