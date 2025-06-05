#include <iostream>

#include "Queue.h"

Queue::Queue(int capacity) : capacity(capacity), size(0), front(0), rear(0), queue(new int[capacity]) {}

Queue::Queue() : Queue(10) {}

Queue::~Queue() {
    delete[] queue;
}

// Returns true if the Queue is full. Otherwise, returns false.
bool Queue::isFull() const {
    return size == capacity;
}

// Returns true if the Queue is empty. Otherwise, returns false.
bool Queue::isEmpty() const {
    return size == 0;
}

// Returns the current size of the queue.
int Queue::getSize() const {
    return size;
}

// Enqueues an element.
void Queue::enqueue(int element) {
    // Check if the queue is full. If so, we inform the user and return early.
    if (isFull()) {
        std::cout << "Queue is full. Cannot enqueue element " << element << ".\n";
        return;
    }

    // If the queue is not full, append element to "end" of queue, update position of rear, and increment size.
    queue[rear] = element;
    rear = (rear + 1) % capacity;
    size++;
}

// Dequeues and returns the first element in the queue
int Queue::dequeue() {
    // Check if the queue is empty. If so, inform the user and return a default value.
    if (isEmpty()) {
        std::cout << "Queue is empty. Nothing to dequeue.\n";
        return INT_MIN;
    }

    // If the queue is not empty, store the front element, update position of front, decrement size, and return tmp.
    int tmp = queue[front];
    front = (front + 1) % capacity;
    size--;
    return tmp;
}

// Returns the first element in the queue without dequeue-ing it.
int Queue::peek() const {
    // Check if the queue is empty. If so, inform the user and return a default value.
    if (isEmpty()) {
        std::cout << "Queue is empty. Nothing to peek.\n";
        return INT_MIN;
    }

    // If the queue is not empty, return the element at the front of the queue.
    return queue[front];
}

// Displays the queue in the console.
void Queue::displayQueue() const {
    // Display some information about the queue first.
    std::cout << "Capacity: " << capacity << ". Size: " << size << ".\n";

    // Display the elements in the queue (from the "front" onward).
    for (int i = 0; i < size; i++) {
        std::cout << i << ": " << queue[(front + i) % capacity] << std::endl;
    }
}