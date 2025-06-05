#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

struct queueElement {

    int index;
    double value;
    queueElement(int index, double value) : index(index), value(value) {}
    queueElement() : queueElement(0, 0) {}

};

class PriorityQueue {
public:

    explicit PriorityQueue(int capacity);
    PriorityQueue();
    ~PriorityQueue();

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] bool isFull() const;

    [[nodiscard]] int getParent(int child) const;
    [[nodiscard]] int leftChild(int parent) const;
    [[nodiscard]] int rightChild(int parent) const;

    void push(int index, double value);
    queueElement pop();

    void buildMinHeap();
    void decreaseKey(int index, double value);

    void display() const;

private:
    int capacity;
    int elements;
    queueElement* queue;

    void MinHeapify(int i, int size);

};

#endif //PRIORITYQUEUE_H