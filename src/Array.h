#ifndef ARRAY_H
#define ARRAY_H

class Array {
public:

    explicit Array(int capacity);
    Array();
    ~Array();

    [[nodiscard]] int getCapacity() const;
    [[nodiscard]] int getSize() const;
    [[nodiscard]] bool isFull() const;
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] bool isSorted() const;

    int& operator[](int index); // Overload for subscript operator

    void insertionSort();
    void selectionSort();
    void bubbleSort();
    void quickSort();
    void mergeSort();

    [[nodiscard]] int search(int element) const;
    [[nodiscard]] int min() const;
    [[nodiscard]] int max() const;

    void insert(int element, int index);
    void push_back(int element);
    void push_front(int element);
    int remove(int index);
    int pop_back();
    int pop_front();

    void display() const;

private:
    int capacity; // Maximum number of stored elements.
    int size; // Current number of stored elements.
    int* arr; // Store of values currently present in the array

    void resize();
    [[nodiscard]] int partition(int low, int high);
    void merge(int low, int mid, int high);
    void quickSortHelper(int low, int high);
    void mergeSortHelper(int low, int high);
    [[nodiscard]] int search_binaryHelper(int element, int left, int right) const;

};

#endif //ARRAY_H