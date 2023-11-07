#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <chrono>

using namespace std::chrono;

enum SEQS { Shell, Hibbard, Sedgewick };

const int MINRUN = 64;

std::vector<int> GAPS;

int MAX_DEPTH;

// utilities

void PrintArray(int* arr, int length) {
	int i;
	for (i = 0; i < length; ++i)
		std::cout << arr[i] << " ";
	std::cout << '\n';
}

void Merge(int* arr, int left, int mid, int right) {
	int i, j, k, n1, n2;
	int* left_arr, * right_arr;
	n1 = mid - left + 1;
	n2 = right - mid;
	left_arr = new int[n1];
	right_arr = new int[n2];
	for (i = 0; i < n1; ++i)
		left_arr[i] = arr[left + i];
	for (j = 0; j < n2; ++j)
		right_arr[j] = arr[mid + 1 + j];
	i = 0; j = 0; k = left;
	while (i < n1 && j < n2) {
		if (left_arr[i] <= right_arr[j]) {
			arr[k] = left_arr[i];
			++i;
		}
		else {
			arr[k] = right_arr[j];
			++j;
		}
		++k;
	}
	while (i < n1) {
		arr[k] = left_arr[i];
		++i; ++k;
	}
	while (j < n2) {
		arr[k] = right_arr[j];
		++j; ++k;
	}
	delete[] left_arr;
	delete[] right_arr;
}

int Partition(int* arr, int left, int right) {
	int i, j, temp, pivot;
	pivot = arr[left];
	i = left - 1;
	j = right + 1;
	while (true) {
		while (++i < right && arr[i] < pivot);
		while (--j > left && arr[j] > pivot);
		if (i < j) {
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
		else {
			return j;
		}
	}
}

void MakeSequence(int length, int seq) {
	int i, temp;
	switch (seq) {
	case Shell: // Shell gap sequence
		temp = length / 2;
		while (temp > 0) {
			GAPS.push_back(temp);
			temp /= 2;
		}
		break;
	case Hibbard: // Hibbard gap sequence (OEIS A000225)
		i = 1;
		temp = pow(2, i) - 1;
		while (temp < length) {
			GAPS.push_back(temp);
			++i; temp = pow(2, i) - 1;
		}
		std::reverse(GAPS.begin(), GAPS.end());
		break;
	case Sedgewick: // Sedgewick gap sequence (OEIS A036562)
		i = 1;
		GAPS.push_back(1);
		temp = pow(4, i) + 3 * pow(2, i - 1) + 1;
		while (temp < length) {
			GAPS.push_back(temp);
			++i; temp = pow(4, i) + 3 * pow(2, i - 1) + 1;
		}
		std::reverse(GAPS.begin(), GAPS.end());
		break;
	default: // default is Shell sequence
		temp = length / 2;
		while (temp > 0) {
			GAPS.push_back(temp);
			temp /= 2;
		}
		break;
	}
}

void Heapify(int* arr, int length, int root) {
	int left, right, largest, temp;
	largest = root;
	left = 2 * root + 1;
	right = 2 * root + 2;
	if (left < length && arr[left] > arr[largest])
		largest = left;
	if (right < length && arr[right] > arr[largest])
		largest = right;
	if (largest != root) {
		temp = arr[root];
		arr[root] = arr[largest];
		arr[largest] = temp;
		Heapify(arr, length, largest);
	}
}

int CalculateMinrun(int length) {
	int flag;
	flag = 0;
	while (length >= MINRUN) {
		flag |= length & 1;
		length >>= 1;
	}
	return length + flag;
}

void CalculateMaxDepth(int length) {
	MAX_DEPTH = 2 * (log(length) / log(2));
}

// sorting algorithms

void SelectionSort(int* arr, int length) {
	int i, j, min_index, temp;
	for (i = 0; i < length - 1; ++i) {
		min_index = i;
		for (j = i + 1; j < length; ++j) {
			if (arr[j] < arr[min_index]) {
				min_index = j;
			}
		}
		if (min_index != i) {
			temp = arr[min_index];
			arr[min_index] = arr[i];
			arr[i] = temp;
		}
	}
}

void InsertionSort(int* arr, int begin, int end) {
	int i, j, key;
	for (i = begin + 1; i <= end; ++i) {
		key = arr[i];
		j = i - 1;
		while (j >= begin && arr[j] > key) {
			arr[j + 1] = arr[j];
			--j;
		}
		arr[j + 1] = key;
	}
}

void BubbleSort(int* arr, int length) {
	int i, j, temp;
	bool swapped;
	for (i = 0; i < length - 1; ++i) {
		swapped = false;
		for (j = 0; j < length - i - 1; ++j) {
			if (arr[j] > arr[j + 1]) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				swapped = true;
			}
		}
		if (swapped == false) {
			return;
		}
	}
}

void MergeSort(int* arr, int begin, int end) {
	if (begin < end) {
		int mid;
		mid = begin + (end - begin) / 2;
		MergeSort(arr, begin, mid);
		MergeSort(arr, mid + 1, end);
		Merge(arr, begin, mid, end);
	}
}

void QuickSort(int* arr, int begin, int end) {
	int mid;
	while (begin < end) {
		mid = Partition(arr, begin, end);
		if (mid - begin <= end - mid + 1) {
			QuickSort(arr, begin, mid);
			begin = mid + 1;
		}
		else {
			QuickSort(arr, mid + 1, end);
			end = mid;
		}
	}
}

void ShellSort(int* arr, int length) {
	int i, j, k, gap, temp;
	for (i = 0; i < GAPS.size(); ++i) {
		gap = GAPS[i];
		for (j = gap; j < length; ++j) {
			temp = arr[j];
			for (k = j; k >= gap && arr[k - gap] > temp; k -= gap)
				arr[k] = arr[k - gap];
			arr[k] = temp;
		}
	}
}

void HeapSort(int* arr, int begin, int end) {
	int i, temp;
	for (i = end / 2 - 1; i >= 0; i--)
		Heapify(arr, end, i);
	for (i = end - 1; i >= 0; i--) {
		temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		Heapify(arr, i, 0);
	}
}

void TimSort(int* arr, int length) {
	int i, size, left, mid, right, min_run;
	min_run = CalculateMinrun(length);
	for (i = 0; i < length; i += min_run)
		InsertionSort(arr, i, std::min((i + min_run - 1), (length - 1)));
	for (size = min_run; size < length; size *= 2) {
		for (left = 0; left < length; left += 2 * size) {
			mid = left + size - 1;
			right = std::min((left + 2 * size - 1), (length - 1));
			if (mid < right)
				Merge(arr, left, mid, right);
		}
	}
}

void IntroSort(int* arr, int begin, int end, int max_depth) {
	int length;
	length = end - begin + 1;
	if (length < 16) {
		InsertionSort(arr, begin, end);
	}
	else if (max_depth == 0) {
		HeapSort(arr, begin, end);
	}
	else {
		int mid;
		mid = Partition(arr, begin, end);
		IntroSort(arr, begin, mid, max_depth - 1);
		IntroSort(arr, mid + 1, end, max_depth - 1);

	}
}

int main() {
	srand(time(0));

	int i, j, min, max, length;
	
	int* arr;

	std::cout << "Input array length: ";
	std::cin >> length;

	std::cout << "Input minimum value in range: ";
	std::cin >> min;

	std::cout << "Input maximum value in range: ";
	std::cin >> max;

	arr = new int[length];
	for (i = 0; i < length; ++i)
		arr[i] = (min + rand() % (max - min + 1));

	std::cout << "Your array: ";
	PrintArray(arr, length);

	auto start = high_resolution_clock::now();
	BubbleSort(arr, length);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	std::cout << "Your sorted array: ";
	PrintArray(arr, length);

	std::cout << "Algorithm execution time (in microseconds): " << duration.count();

	delete[] arr;

	return 0;
}