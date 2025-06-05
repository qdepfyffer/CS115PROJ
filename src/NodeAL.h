#ifndef NODEAL_H
#define NODEAL_H

struct NodeAL {
    int v; // Vertex.
    double w; // Weight.
    NodeAL* next; // Pointer to the next node.
    NodeAL(int v, double w);
    NodeAL();
    ~NodeAL();
};

#endif //NODEAL_H
