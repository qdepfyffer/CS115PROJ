#include "PriorityQueue.h"

#include <iostream>

// Ensures a heap maintains the heap property.
void PriorityQueue::MinHeapify(int i, int size) {

    // Find left and right children of i, set the original smallest element to i
    int left = 2 * i + 1; // Left child of i
    int right = 2 * i + 2; // Right child of i
    int smallest = i; // Assume that this is the original smallest element

    // If the left child is within range and is smaller than the current smallest element:
    if (left < size && queue[left].value < queue[smallest].value) {
        smallest = left;
    }

    // If the right child is within range and is smaller than the current smallest element:
    if (right < size && queue[right].value < queue[smallest].value) {
        smallest = right;
    }

    // If smallest has changed, swap values and recursively call minheap at the location of the swapped value
    if (smallest != i) {
        queueElement tmp = queue[i];
        queue[i] = queue[smallest];
        queue[smallest] = tmp;

        // Recursive call:
        MinHeapify(smallest, size);
    }
}

PriorityQueue::PriorityQueue(int capacity) : capacity(capacity), elements(0), queue(new queueElement[capacity]) {}

PriorityQueue::PriorityQueue() : PriorityQueue(10) {}

PriorityQueue::~PriorityQueue() {
    delete[] queue;
}

// Returns true if the priority queue is full. Otherwise, returns false.
bool PriorityQueue::isFull() const {
    return elements == capacity;
}


// Returns true if the priority queue is empty. Otherwise, returns false.
bool PriorityQueue::isEmpty() const {
    return elements == 0;
}

// Returns the index of the parent for the given child.
int PriorityQueue::getParent(int child) const {
    return (child - 1) / 2;
}

// Returns the index of the left child for the given parent.
int PriorityQueue::leftChild(int parent) const {
    return 2 * parent + 1;
}

// Returns the index of the right child for the given parent.
int PriorityQueue::rightChild(int parent) const {
    return 2 * parent + 2;
}

// Builds the min heap from the array.
void PriorityQueue::buildMinHeap() {
    // Iterate backward through all interior nodes.
    for (int i = elements / 2 - 1; i >= 0; i--) {
        MinHeapify(i, elements);
    }
}

// Push given element to the priority queue.
void PriorityQueue::push(int index, double value) {
    // Check if the priority queue is full. If so, inform the user and return.
    if (isFull()) {
        std::cout << "Queue is full. Cannot enqueue element " << index << " with value " << value << ".\n";
    }

    // Place the new element at the end of the priority queue.
    queueElement tmp(index, value);
    int i = elements;
    queue[i] = tmp;
    elements++;

    // Bubble new element up (while the value of a node is less than that of its child).
    while (i > 0 && queue[i].value < queue[(i - 1) / 2].value) {
        // Swap child and parent
        queueElement tmp = queue[i];
        queue[i] = queue[(i - 1) / 2];
        queue[(i - 1) / 2] = tmp;

        // Move to parent's new index
        i = (i - 1) / 2;
    }
}

// Pop the first element from priority queue and return it.
queueElement PriorityQueue::pop() {
    // Check if the priority queue is empty. If so, inform the user and return a default element.
    if (isEmpty()) {
        std::cout << "Queue is empty. Nothing to pop.";
        return queueElement(0, -1);
    }

    // If the priority queue is not empty, store the first element, replace the root with the last element, and heapify.
    queueElement tmp = queue[0];
    elements--;
    queue[0] = queue[elements];
    MinHeapify(0, elements);

    // Return stored value.
    return tmp;
}

// Decreases key value of a given element.
void PriorityQueue::decreaseKey(int index, double value) {
    // First, find the desired element.
    int position = -1;
    for (int i = 0; i < elements && position < 0; i++) {
        if (queue[i].index == index) {
            // Update element's value, set position to break out of loop and continue
            queue[i].value = value;
            position = i;
        }
    }

    // If the element is not found, inform the user and return.
    if (position == -1) {
        std::cout << "Element " << index << " could not be found.\n";
        return;
    }

    // If the element is found, bubble it up to maintain heap property (while the value of a node is less than that of its child).
    while (position > 0 && queue[position].value < queue[(position - 1) / 2].value) {
        // Swap child and parent.
        queueElement tmp = queue[position];
        queue[position] = queue[(position - 1) / 2];
        queue[(position - 1) / 2] = tmp;
        position = (position - 1) / 2;
    }
}

// Displays information about the queue and its elements in the console.
void PriorityQueue::display() const {
    // Display information about queue.
    std::cout << "Priority Queue" << std::endl
              << "Capacity: " << capacity << std::endl
              << "Elements: " << elements << std::endl;

    // Display elements in the queue.
    for (int i = 0; i < elements; i++) {
        std::cout << i << ": (" << queue[i].index << ", " << queue[i].value << ")\n";
    }
    std::cout << std::endl;
}