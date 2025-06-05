#ifndef GRAPHAL_H
#define GRAPHAL_H

#include "NodeAL.h"
#include "Queue.h"

enum class State {
    Unvisited,
    Visited,
    Finished,
};

class GraphAL {
public:
    explicit GraphAL(int n);
    GraphAL();
    ~GraphAL();

    [[nodiscard]] int getVertices() const;
    [[nodiscard]] int getDegree(int u) const;

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

    [[nodiscard]] bool hasAdjacent(int u) const;
    void setCurrentVertex(int u);
    int getNextAdjacent(int u);

    static void BFS_MAIN(int src, GraphAL *graph, State *&state, double *&distance, int*& predecessor);
    void BFS(int s);
    void DFS();

    static void displayShortestPath(double *distance, int *predecessor, int n);
    void initializeSingleSource(double *distance, int *predecessor) const;
    void relax(int u, int v, double *distance, int *predecessor) const;
    bool bellmanFord(int src, double *&distance, int *&predecessor);
    void dijkstra(int src, double *&distance, int *&predecessor);
    void extractPath(int dest, int* predecessor, Queue* path);

    void makeRandomGraph(int n, double p);

    void display() const;

private:
    NodeAL** lists; // Array of node lists.
    NodeAL** current; // Iterators for tracking current node.
    int* degree; // Degree of each vertex.
    int n; // Number of vertices;

};

#endif //GRAPHAL_H
