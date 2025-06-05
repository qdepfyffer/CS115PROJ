#include <iostream>

#include "Array.h"

// Resizes the array to increase capacity.
void Array::resize() {
    // Store the location of the old array, double the capacity, and create a new array.
    int* tmp = arr;
    capacity *= 2;
    arr = new int[capacity];

    // Iterate through the old array and transfer values to the new array. Then, delete the old values.
    for (int i = 0; i < size; i++) {
        arr[i] = tmp[i];
    }
    delete[] tmp;
}

// Partition function for use in the quick sort function.
int Array::partition(int low, int high) {
    // Swap the final value in the array with the value in the middle to attain better performance (on average).
    int mid = low + (high - low) / 2;
    int tmp = arr[high];
    arr[high] = arr[mid];
    arr[mid] = tmp;

    // Select pivot element and index used for swapping
    int pivot = arr[high];
    int i = (low - 1);

    // Iterate through the array comparing with the pivot value, swapping with the greater element at index i.
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            tmp = arr[j];
            arr[j] = arr[i];
            arr[i] = tmp;
        }
    }

    // Swap pivot with the greater element.
    tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;

    // Return position of partition.
    return (i + 1);
}

void Array::merge(int low, int mid, int high) {
    // Calculate the necessary length of the subarrays, then create and fill subarrays.
    int lLength = mid - low + 1;
    int rLength = high - mid;
    int *lSub = new int[lLength];
    int *rSub = new int[rLength];

    for (int l = 0; l < lLength; l++) {
        lSub[l] = arr[low + l];
    }

    for (int l = 0; l < rLength; l++) {
        rSub[l] = arr[mid + 1 + l];
    }

    // Merge the subarrays by adding the lowest value from either subarray, continuing until the end of either subarray is reached.
    int i = 0;
    int j = 0;
    int k = low;
    while (i < lLength && j < rLength) {
        if (lSub[i] <= rSub[j]) {
            arr[k] = lSub[i];
            i++;
        }
        else {
            arr[k] = rSub[j];
            j++;
        }
        k++;
    }

    // Once the end of either subarray is reached, add the remaining elements from the other subarray to the merged array.
    while (i < lLength) {
        arr[k] = lSub[i];
        i++;
        k++;
    }

    while (j < rLength) {
        arr[k] = rSub[j];
        j++;
        k++;
    }

    delete[] lSub;
    delete[] rSub;
}

// Helper function to sort the array using quick sort.
void Array::quickSortHelper(int low, int high) {
    // Ensure bounds are valid.
    if (low < high) {
        // Employ quick sort algorithm.
        int partitionIndex = partition(low, high);
        quickSortHelper(low, partitionIndex - 1);
        quickSortHelper(partitionIndex + 1, high);
    }
}

// Helper function to sort the array using merge sort.
void Array::mergeSortHelper(int low, int high) {
    // Ensure bounds are valid.
    if (low < high) {
        // Employ merge sort algorithm.
        int mid = low + (high - low) /2;
        mergeSortHelper(low, mid);
        mergeSortHelper(mid + 1, high);
        merge(low, mid, high);
    }
}

// Performs a binary search of the array for the given element. Only works when the array is sorted.
int Array::search_binaryHelper(int element, int left, int right) const {
    if (left <= right) {
        // Find the middle of the array, check if it holds the desired element.
        int mid = left + (right - left) / 2;
        if (arr[mid] == element) {
            return mid;
        }

        // If the middle doesn't hold the desired element, recursively call the function on both sides of the array.
        if (arr[mid] > element) {
            return search_binaryHelper(element, left, mid - 1);
        }

        return search_binaryHelper(element, mid + 1, right);
    }

    // If the function hasn't returned yet, the element must not be present. Return -1 (flagging value, not found).
    return -1;
}

Array::Array(int capacity) : capacity(capacity), size(0), arr(new int[capacity]) {}

Array::Array() : Array(10) {}

Array::~Array() {
    delete[] arr;
}

// Returns the current capacity of the array.
int Array::getCapacity() const {
    return capacity;
}

// Returns the current size of the array.
int Array::getSize() const {
    return size;
}

// Allows for direct access to elements of the array using their index.
int& Array::operator[](int index) {
    return arr[index];
}

// Returns true if the array is full. Otherwise, returns false.
bool Array::isFull() const {
    return size == capacity;
}

// Returns true if the array is empty. Otherwise, returns false.
bool Array::isEmpty() const {
    return size == 0;
}

// Returns true if the array is sorted. Otherwise, returns false.
bool Array::isSorted() const {
    // Iterate through the list, check each element against its following element, return false if the following element is lesser.
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }

    // If the function hasn't yet returned, the array is sorted. Return true.
    return true;
}

// Sorts the array using insertion sort.
void Array::insertionSort() {
    // Start at the second element, assume the first element is sorted.
    for (int i = 1; i < size; i++) {

        // Store current value, start j at i - 1, iterate backward and compare to the current value.
        int current = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > current) {
            // If the value of vec[j] is greater than the current value, move it to the right.
            arr[j + 1] = arr[j];
            j--;
        }

        // Swap current with its proper place in the array.
        arr[j + 1] = current;
    }
}

// Sorts the array using selection sort.
void Array::selectionSort() {
    // Start at the beginning of the vector, iterate through to the end.
    for (int sorted = 0; sorted < size; sorted++) {
        // Assume the minimum element is at the position of sorted.
        int min = sorted;

        // Iterate through all subsequent values, keep track of the index of the minimum value.
        for (int i = sorted + 1; i < size; i++) {
            if (arr[i] < arr[min]) {
                min = i;
            }
        }

        // Swap the smallest element with the element at index sorted, where it belongs.
        int tmp = arr[sorted];
        arr[sorted] = arr[min];
        arr[min] = tmp;
    }
}

// Sorts the array using bubble sort.
void Array::bubbleSort() {
    // Iterate through the array, set swap to false.
    for (int i = 0; i < size; i++) {
        bool swap = false;
        // Iterate through the array again, comparing each element with the one following it.
        for (int j = 0; j < size - 1; j++) {
            // If an element is greater than the one following it, swap them, and indicate that with swap.
            int tmp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = tmp;
            swap = true;
        }
        // If a swap has not occurred, the array is sorted. Return to avoid unnecessary work.
        if (!swap) {
            return;
        }
    }
}

// Sorts the array using quick sort.
void Array::quickSort() {
    quickSortHelper(0, size);
}

// Sorts the array using merge sort.
void Array::mergeSort() {
    mergeSortHelper(0, size);
}

// Performs a linear search of the array for the given element.
int Array::search(int element) const {
    // Iterate through the array from 0 to size - 1, return i if the element is found.
    for (int i = 0; i < size; i++) {
        if (arr[i] == element) {
            return i;
        }
    }
    return -1;
}

// Returns the minimum value in the array.
int Array::min() const {
    // Assume the first element is the smallest.
    int smallest = arr[0];

    // Iterate through the array, replace smallest if a smaller value is found.
    for (int i = 0; i < size; i++) {
        if (arr[i] < smallest) {
            smallest = arr[i];
        }
    }

    // Smallest now holds the minimum value present in the array. Return smallest.
    return smallest;
}

// Returns the maximum value in the array.
int Array::max() const {
    // Assume the first element is the largest.
    int largest = arr[0];

    // Iterate through the array, replace largest if a larger value is found.
    for (int i = 0; i < size; i++) {
        if (arr[i] > largest) {
            largest = arr[i];
        }
    }

    // Largest now holds the maximum value present in the array. Return largest.
    return largest;
}

// Inserts an element at the specified index.
void Array::insert(int element, int index) {
    // First, check if the array is full. If so, resize the array.
    if (isFull()) {
        resize();
    }

    // If the user passes an index greater than or equal to the current final open index, insert at the end and increment size.
    if (index >= size) {
        arr[size] = element;
        size++;
    }

    // Otherwise, move all the elements from the specified index onward to the right and insert in the open space.
    else {
        for (int i = size; i >= index; i--) {
            arr[i] = arr[i - 1];
        }
        arr[index] = element;
        size++;
    }
}

// Appends an element to the end of the array.
void Array::push_back(int element) {
    insert(element, size);
}

// Inserts an element at the front of the array.
void Array::push_front(int element) {
    insert(element, 0);
}

// Removes the element at the specified index in the array
int Array::remove(int index) {
    // First, ensure that the index is in bounds. If not, return a default value.
    if (index >= size || index < 0) {
        return INT_MIN;
    }

    // Store the element at the specified index, then shift all subsequent elements to the left, decrement size, and return tmp.
    int tmp = arr[index];
    for (int i = index; i < size; i++) {
        arr[i] = arr[i + 1];
    }
    size--;
    return tmp;
}

// Removes and returns the element at the end of the array.
int Array::pop_back() {
    // Nothing really needs to be done here besides decrementing the size of the array. That renders the final element
    // inaccessible and primed to be overwritten next time the array is inserted into.
    int tmp = arr[size - 1]; // size corresponds to the next index for insertion. size - 1 is the last element.
    size--;
    return tmp;
}

// Removes and returns the element at the front of the array.
int Array::pop_front() {
    return remove(0);
}

// Displays the array in the console.
void Array::display() const {
    std::cout << "[";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i];
        if (i < size - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}