#ifndef GRAPHAM_H
#define GRAPHAM_H

#include "Matrix.h"

#include <iostream>

class GraphAM {
public:

    explicit GraphAM(int vertices);
    GraphAM();
    ~GraphAM();

    [[nodiscard]] int getVertices() const;
    [[nodiscard]] int getDegree(int u) const;
    [[nodiscard]] int getIndex(int u, int v) const;

    [[nodiscard]] double getEdgeWeight(int u, int v) const;
    [[nodiscard]] bool hasEdge(int u, int v) const;

    void addDirectedEdge(int u, int v, double w);
    void addDirectedEdge(int u, int v);
    void addUndirectedEdge(int u, int v, double w);
    void addUndirectedEdge(int u, int v);
    void removeDirectedEdge(int u, int v);
    void removeUndirectedEdge(int u, int v);

    [[nodiscard]] bool hasLoops() const ;
    [[nodiscard]] bool isUndirected() const;

    void setCurrentVertex(int u);
    bool getNextAdjacent(int u, int &vOut);

    void BFS(int s);
    void DFS();

    static void displayShortestPath(double *distance, int *predecessor, int n);
    void initializeSingleSource(double *distance, int *predecessor) const;
    void relax(int u, int v, double *distance, int *predecessor) const;
    bool bellmanFord(int src, double *&distance, int *&predecessor);
    void dijkstra(int src, double *&distance, int *&predecessor);

    static void printAllPairs(Matrix *predecessor, int i, int j);
    void floydWarshall(Matrix *&distanceOut, Matrix *&predecessorOut);
    void dijkstraMultiSource(Matrix *&distanceOut, Matrix *&predecessorOut);

    void transitiveClosure(Matrix *&tcOut);

    int minKey(int *key, bool *mstSet);
    static bool isAcyclic(int u, GraphAM &graph);
    void MSTKruskal();
    void MSTPrim();
    void displayMST(int *parent);

    void makeRandomGraph(int n, double p);

    void display() const;
    void displayDirected() const;
    void displayUndirected() const;
private:

    int vertices; // Number of vertices.
    double* matrix; // Adjacency matrix.
    int* current; // For keeping track of current adjacent vertex.

};

#endif //GRAPHAM_H
