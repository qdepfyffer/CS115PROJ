#include <iostream>
#include <cfloat>

#include "GraphAL.h"
#include "PriorityQueue.h"
#include "Queue.h"



// Constructs a new graph with n vertices.
GraphAL::GraphAL(int n) : lists(new NodeAL*[n]), current(new NodeAL*[n]), degree(new int[n]), n(n) {
    for (int i = 0; i < n; i++) {
        lists[i] = nullptr;
        current[i] = nullptr;
        degree[i] = 0;
    }
}

// Default constructor for GraphAL. Constructs a new graph with 10 vertices.
GraphAL::GraphAL() : GraphAL(10) {}

// Destructor for GraphAL.
GraphAL::~GraphAL() {
    for (int i = 0; i < n; i++) {
        NodeAL *current = lists[i];
        while (current) {
            NodeAL *next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] lists;
    delete[] current;
    delete[] degree;
}

// Returns the number of vertices in the graph.
int GraphAL::getVertices() const {
    return n;
}

// Returns the degree of vertex u.
int GraphAL::getDegree(int u) const {
    return degree[u];
}

// Returns the weight of the edge between nodes u and v.
double GraphAL::getEdgeWeight(int u, int v) const {
    // Ensure u is an actual node in the graph.
    if (u < n) {
        // Iterate through the adjacency list. If you encounter v, return the w (weight) value. Otherwise, check the next node.
        NodeAL *current = lists[u];
        while (current) {
            if (current->v == v) {
                return current->w;
            }
            current = current->next;
        }
    }
    // If we haven't encountered a match for v, there is no edge between u and v. Return 0.
    return 0;
}

// Returns true if an edge exists between u and v. Otherwise, returns false
bool GraphAL::hasEdge(int u, int v) const {
    // Basically does the same thing as above. Returns a boolean rather than an edge weight.
    // We could theoretically simply return getEdgeWeight(u, v) != 0 and be done though.
    // Ensure u is an actual node in the graph.
    if (u < n) {
        // Iterate through the adjacency list. If you encounter v, return true. Otherwise, check the next node.
        NodeAL *current = lists[u];
        while (current) {
            if (current->v == v) {
                return true;
            }
            current = current->next;
        }
    }
    // If we haven't encountered a match for v, there is no edge between u and v. Return false.
    return false;
}

// Adds a directed edge between nodes u and v with weight w.
void GraphAL::addDirectedEdge(int u, int v, double w) {
    // Make sure no edge already exists.
    if (!hasEdge(u, v)) {
        NodeAL *tmp = new NodeAL(v, w);
        tmp->next = lists[u];
        lists[u] = tmp;
        degree[u]++;
    }
}

// Adds an unweighted directed edge between u and v.
void GraphAL::addDirectedEdge(int u, int v) {
    addDirectedEdge(u, v, 1);
}

// Adds an undirected edge between nodes u and v with weight w.
void GraphAL::addUndirectedEdge(int u, int v, double w) {
    addDirectedEdge(u, v, w);
    addDirectedEdge(v, u, w);
}

// Adds an unweighted undirected edge between nodes u and v.
void GraphAL::addUndirectedEdge(int u, int v) {
    addDirectedEdge(u, v, 1);
    addDirectedEdge(v, u, 1);
}

// Removes a directed edge between nodes u and v.
void GraphAL::removeDirectedEdge(int u, int v) {
    NodeAL *current = lists[u];
    if (current) {
        if (current->v == v) {
            lists[u] = current->next;
            delete current;
        }
        else {
            if (current->next) {
                NodeAL *prev = current;
                current = current->next;
                bool found = false;
                while (current && !found) {
                    if (current->v == v) {
                        prev->next = current->next;
                        delete current;
                        found = true;
                    }
                    else {
                        prev = current;
                        current = current->next;
                    }
                }
            }
        }
    }
    degree[u]--;
}

// Removes an undirected edge between nodes u and v.
void GraphAL::removeUndirectedEdge(int u, int v) {
    removeDirectedEdge(u, v);
    removeDirectedEdge(v, u);
}

// Determines whether there are loops present in the graph.
bool GraphAL::hasLoops() const  {
    // Iterate through every node in the graph and check its adjacency list for itself. If a match is found, return true.
    for (int u = 0; u < n; u++) {
        if (hasEdge(u, u)) {
            return true;
        }
    }
    return false;
}

// Returns true if the graph is undirected. Otherwise, returns false.
bool GraphAL::isUndirected() const {
    // Make sure edge (u, v) has the same weight as (v, u).
    for (int u = 0; u < n; u++) {
        for (int v = u; v < n; v++) {
            if (getEdgeWeight(u, v) != getEdgeWeight(v, u)) {
                return false;
            }
        }
    }
    return !hasLoops();
}

// Returns true if a given vertex has an adjacent vertex.
bool GraphAL::hasAdjacent(int u) const {
    return current[u] != nullptr;
}

// Sets the current vertex for the given vertex.
void GraphAL::setCurrentVertex(int u) {
    current[u] = lists[u];
}

// Gets the next adjacent vertex for the given vertex.
int GraphAL::getNextAdjacent(int u) {
    int v = current[u]->v;
    current[u] = current[u]->next;
    return v;
}

// Main breadth-first search function for GraphAL.
void GraphAL::BFS_MAIN(int src, GraphAL *graph, State *&state, double *&distance, int*& predecessor) {
    int vertices = graph->getVertices();
    if (src < 0 || src > vertices) {
        std::cout << "Bad source.\n";
    }
    else {
        Queue *queue = new Queue(vertices);
        state = new State[vertices];
        distance = new double[vertices];
        predecessor = new int[vertices];
        for (int i = 0; i < vertices; i++) {
            state[i] = State::Unvisited;
            distance[i] = 0;
            predecessor[i] = -1;
        }
        state[src] = State::Visited;
        distance[src] = 0;
        predecessor[src] = -1;
        queue->enqueue(src);
        while (!queue->isEmpty()) {
            int u = queue->dequeue();
            for (int v = 0; v < vertices; v++) {
                if (graph->hasEdge(u, v)) {
                    if (state[v] == State::Unvisited) {
                        state[v] = State::Visited;
                        distance[v] = distance[u] + 1; // Or getEdgeWeight(u, v)?
                        predecessor[v] = u;
                        queue->enqueue(v);
                    }
                }
            }
            state[u] = State::Finished;
        }
        delete queue;
    }
}

// Performs a breadth-first search of the graph from source node src.
void GraphAL::BFS(int src) {
    std::cout << "Breadth-First Search:\n";
    State *state;
    double *distance;
    int *predecessor;
    BFS_MAIN(src, this, state, distance, predecessor);
    for (int u = 0; u < n; u++) {
        std::cout << "From: " << src << " to " << u << ": Distance = " << distance[u] << " | " << predecessor[u] << std::endl;
    }
    delete[] state;
    delete[] distance;
    delete[] predecessor;
}

// Depth-first visitation function for GraphAL.
void DFS_VISIT(int u, GraphAL *graph, State *state, int *discovered, int *finished, int *predecessor, int &t) {
    state[u] = State::Visited;
    t++;
    discovered[u] = t;
    graph->setCurrentVertex(u);
    while (graph->hasAdjacent(u)) {
        int v = graph->getNextAdjacent(u);
        if (state[v] == State::Unvisited) {
            predecessor[v] = u;
            DFS_VISIT(v, graph, state, discovered, finished, predecessor, t);
        }
    }
    state[u] = State::Finished;
    t++;
    finished[u] = t;
}

// Main depth-first search function for GraphAL.
void DFS_MAIN(GraphAL *graph, State *&state, int *&discovered, int *&finished, int *&predecessor) {
    int vertices = graph->getVertices();
    state = new State[vertices];
    discovered = new int[vertices];
    finished = new int[vertices];
    predecessor = new int[vertices];
    for (int i = 0; i < vertices; i++) {
        state[i] = State::Unvisited;
        discovered[i] = 0;
        finished[i] = 0;
        predecessor[i] = 0;
    }
    int t = 0;
    for (int u = 0; u < vertices; u++) {
        if (state[u] == State::Unvisited) {
            DFS_VISIT(u, graph, state, discovered, finished, predecessor, t);
        }
    }
}

// Performs a depth-first search of GraphAL.
void GraphAL::DFS() {
    std::cout << "Depth-First Search:\n";
    State *state;
    int *discovered;
    int *finished;
    int *predecessor;
    DFS_MAIN(this, state, discovered, finished, predecessor);
    for (int u = 0; u < n; u++) {
        std::cout << u << ": (" << discovered[u] << ", " << finished[u] << ") Predecessor: " << predecessor[u] << std::endl;;
    }
    delete[] state;
    delete[] discovered;
    delete[] finished;
    delete[] predecessor;
}

// Displays the shortest path.
void GraphAL::displayShortestPath(double *distance, int *predecessor, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << "Vertex: " << i << " : Distance = " << distance[i] << " : Predecessor = " << predecessor[i] << std::endl;
    }
    std::cout << std::endl;
}

// Initializes distance and predecessor arrays for use in Bellman Ford / Dijkstra shortest path algorithms.
void GraphAL::initializeSingleSource(double *distance, int *predecessor) const {
    for (int i = 0; i < n; i++) {
        distance[i] = DBL_MAX;
        predecessor[i] = -1;
    }
}

// Relaxes an edge beween vertices u and v, used in Bellman Ford / Dijkstra shortest path algorithms.
void GraphAL::relax(int u, int v, double *distance, int *predecessor) const {
    double w = getEdgeWeight(u, v);
    if (distance[v] > distance[u] + w) {
        distance[v] = distance[u] + w;
        predecessor[v] = u;
    }
}

// Finds the shortest path from a source node to all other nodes using the Bellman Ford algorithm.
bool GraphAL::bellmanFord(int src, double *&distance, int *&predecessor) {
    distance = new double[n];
    predecessor = new int[n];
    initializeSingleSource(distance, predecessor);
    distance[src] = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int u = 0; u < n; u++) {
            setCurrentVertex(u);
            while (hasAdjacent(u)) {
                int v = getNextAdjacent(u);
                relax(u, v, distance, predecessor);
            }
        }
        displayShortestPath(distance, predecessor, n);
    }
    for (int u = 0; u < n; u++) {
        setCurrentVertex(u);
        while (hasAdjacent(u)) {
            int v = getNextAdjacent(u);
            double w = getEdgeWeight(u, v);
            if (distance[v] > distance[u] + w) {
                return false;
            }
        }
    }
    return true;
}

// Finds the shortest path from a source node to all other nodes using Dijkstra's algorithm.
void GraphAL::dijkstra(int src, double *&distance, int *&predecessor) {
    distance = new double[n];
    predecessor = new int[n];
    PriorityQueue *pQueue = new PriorityQueue(n);
    Queue *queue = new Queue(n);
    initializeSingleSource(distance, predecessor);
    distance[src] = 0;
    for (int u = 0; u < n; u++) {
        pQueue->push(u, distance[u]);
    }
    while (!pQueue->isEmpty()) {
        queueElement elem = pQueue->pop();
        queue->enqueue(elem.index);
        int u = elem.index;
        setCurrentVertex(u);
        while (hasAdjacent(u)) {
            int v = getNextAdjacent(u);
            double w = getEdgeWeight(u, v);
            if (distance[v] > distance[u] + w) {
                distance[v] = distance[u] + w;
                predecessor[v] = u;
                pQueue->decreaseKey(v, distance[v]);
            }
        }
    }
    delete pQueue;
    delete queue;
}

// Helper function to extract the path.
void GraphAL::extractPath(int dest, int* predecessor, Queue* path) {
    if(dest == -1) {
        return;
    }
    extractPath(predecessor[dest], predecessor, path);
    path->enqueue(dest);
}

// Creates a random graph.
void GraphAL::makeRandomGraph(int n, double p) {

}

// Displays a graph.
void GraphAL::display() const {
    for (int u = 0; u < n; u++) {
        std::cout << u << ": ";
        NodeAL *current = lists[u];
        while (current) {
            std::cout << "(v: " << current->v << ", w: " << current->w << ") ";
            current = current->next;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


