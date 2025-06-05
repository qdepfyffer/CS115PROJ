#ifndef QUEUE_H
#define QUEUE_H

class Queue {
public:

    explicit Queue(int capacity);
    Queue();
    ~Queue();

    [[nodiscard]] bool isFull() const;
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int getSize() const;

    void enqueue(int element);
    int dequeue();
    [[nodiscard]] int peek() const;

    void displayQueue() const ;

private:
    int capacity;
    int size;
    int front;
    int rear;
    int* queue; // This can point to whatever type you're trying to enqueue. Just change the type declaration

};

#endif //QUEUE_H