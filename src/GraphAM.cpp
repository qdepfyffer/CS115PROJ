#include "GraphAM.h"
#include "Queue.h"
#include "PriorityQueue.h"

#include <iostream>
#include <cfloat>
#include <random>

// Enum class for state tracking.
enum class State {
    Unvisited,
    Visited,
    Finished,
};

// Constructor with number of vertices
GraphAM::GraphAM(int vertices) : vertices(vertices) {
    // Initialize matrix and current arrays, set all initial values to 0.
    matrix = new double[vertices * vertices];
    current = new int[vertices];
    for (int i = 0; i < vertices * vertices; i++) {
        matrix[i] = 0;
    }
    for (int i = 0; i < vertices; i++) {
        current[i] = 0;
    }
}

// Default constructor (defaults to a graph with 10 vertices).
GraphAM::GraphAM() : GraphAM(10) {}

// Destructor.
GraphAM::~GraphAM() {
    // Free memory allocated to matrix and current arrays.
    delete[] matrix;
    delete[] current;
}

// Returns the number of vertices in the graph.
int GraphAM::getVertices() const {
    return vertices;
}


int GraphAM::getDegree(int u) const {
    // Initialize variable to track degree of vertex u.
    int degree = 0;
    // Check whether an edge exists between u and all other nodes v, incrementing degree if the edge exists.
    for (int v = 0; v < vertices; v++) {
        if (hasEdge(u, v)) {
            degree++;
        }
    }
    // After checking all nodes, return degree.
    return degree;
}

// Returns the edge's index in the matrix.
int GraphAM::getIndex(int u, int v) const {
    return u * vertices + v;
}


double GraphAM::getEdgeWeight(int u, int v) const {
    return matrix[getIndex(u, v)];
}

// Returns true if there exists an edge connecting vertex u to vertex v.
bool GraphAM::hasEdge(int u, int v) const {
    return getEdgeWeight(u, v) != 0;
}

// Adds a directed edge with weight w between vertices u and v.
void GraphAM::addDirectedEdge(int u, int v, double w) {
    matrix[getIndex(u, v)] = w;
}

// Adds an unweighted directed edge between vertices u and v.
void GraphAM::addDirectedEdge(int u, int v) {
    addDirectedEdge(u, v, 1);
}

// Adds an undirected edge with weight w between vertices u and v.
void GraphAM::addUndirectedEdge(int u, int v, double w) {
    addDirectedEdge(u, v, w);
    addDirectedEdge(v, u, w);
}

// Adds an unweighted undirected edge between vertices u and v.
void GraphAM::addUndirectedEdge(int u, int v) {
    addUndirectedEdge(u, v, 1);
}

// Removes a directed edge between vertices u and v.
void GraphAM::removeDirectedEdge(int u, int v) {
    matrix[getIndex(u, v)] = 0;
}

// Removes an undirected edge between vertices u and v.
void GraphAM::removeUndirectedEdge(int u, int v) {
    removeDirectedEdge(u, v);
    removeDirectedEdge(v, u);
}

// Returns true if any edge exists between a node and itself. Otherwise, returns false.
bool GraphAM::hasLoops() const {
    // Iterate through all vertices in the graph, checking if there exists an edge between it and itself. If so, return true.
    for (int u = 0; u < vertices; u++) {
        if (hasEdge(u, u)) {
            return true;
        }
    }
    // If the function hasn't yet returned true, there are no loops. Return false.
    return false;
}

// Returns true if the graph is undirected. Otherwise, returns false.
bool GraphAM::isUndirected() const {
    // Use nested loops to check all edges against their inverse, returning false if the weights do not match.
    for (int u = 0; u < vertices; u++) {
        for (int v = u + 1; v < vertices; v++) {
            if (getEdgeWeight(u, v) != getEdgeWeight(v, u)) {
                return false;
            }
        }
    }
    // If the function exits the loops without having returned, we can return the inverse of whether the graph has loops, which would make it directed.
    return !hasLoops();
}

// Sets the current adjacent vertex for the given vertex.
void GraphAM::setCurrentVertex(int u) {
    current[u] = -1;
}

// Updates the current adjacent vertex for the given vertex.
bool GraphAM::getNextAdjacent(int u, int &vOut) {
    int v = current[u] + 1;
    vOut = -1;
    bool found = false;
    while (!found && v < vertices) {
        if (hasEdge(u, v)) {
            found = true;
            vOut = v;
        }
        v++;
    }
    current[u] = vOut;
    return found;
}

// Main breadth-first search function.
void BFS_MAIN_AM(int src, GraphAM *graph, State *&state, int *&distance, int *&predecessor) {
    int vertices = graph->getVertices();
    if (src >= 0 && src < vertices) {
        Queue *queue = new Queue(vertices);
        state = new State[vertices];
        distance = new int[vertices];
        predecessor = new int[vertices];
        for (int u = 0; u < vertices; u++) {
            state[u] = State::Unvisited;
            distance[u] = 0;
            predecessor[u] = 0;
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
                        distance[v] = distance[u] + 1;
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

// Performs a breadth-first search of the graph.
void GraphAM::BFS(int src) {
    std::cout << "Breadth-First Search:\n";
    State *state = nullptr;
    int *distance = nullptr;
    int *predecessor = nullptr;
    BFS_MAIN_AM(src, this, state, distance, predecessor);
    for (int u = 0; u < vertices; u++) {
        std::cout << "From " << src << " to " << u << ": Distance = " << distance[u] << ". Predecessor = " << predecessor[u] << ".\n";
    }
    std::cout << std::endl;
    delete[] state;
    delete[] distance;
    delete[] predecessor;
}

// Visits nodes in order for depth-first search.
void DFS_VISIT_AM(int u, GraphAM *graph, State *state, int *discovered, int *finished, int *predecessor, int &t) {
    state[u] = State::Visited;
    t++;
    discovered[u] = t;
    graph->setCurrentVertex(u);
    int v;
    while (graph->getNextAdjacent(u, v)) {
        if (v != -1) {
            if (state[v] == State::Unvisited) {
                predecessor[v] = u;
                DFS_VISIT_AM(v, graph, state, discovered, finished, predecessor, t);
            }
        }
    }
    state[u] = State::Finished;
    t++;
    finished[u] = t;
}

// Main depth-first search function.
void DFS_MAIN_AM(GraphAM *graph, State *&state, int *&discovered, int *&finished, int *&predecessor) {
    int vertices = graph->getVertices();
    state = new State[vertices];
    discovered = new int[vertices];
    finished = new int[vertices];
    predecessor = new int[vertices];
    for (int u = 0; u < vertices; u++) {
        state[u] = State::Unvisited;
        discovered[u] = 0;
        finished[u] = 0;
        predecessor[u] = 0;
    }
    int t = 0;
    for (int u = 0; u < vertices; u++) {
        if (state[u] == State::Unvisited) {
            DFS_VISIT_AM(u, graph, state, discovered, finished, predecessor, t);
        }
    }
}

// Performs a depth-first search of the graph.
void GraphAM::DFS() {
    std::cout << "Depth-First Search:\n";
    State *state = nullptr;
    int *discovered = nullptr;
    int *finished = nullptr;
    int *predecessor = nullptr;
    DFS_MAIN_AM(this, state, discovered, finished, predecessor);
    for (int u = 0; u < vertices; u++) {
        std::cout << u << ": (" << discovered[u] << ", " << finished[u] <<  ") Predecessor: " << predecessor[u] << std::endl;
    }
    std::cout << std::endl;
    delete[] state;
    delete[] discovered;
    delete[] finished;
    delete[] predecessor;
}

// Display the shortest path between two vertices.
void GraphAM::displayShortestPath(double *distance, int *predecessor, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << "Vertex: " << i << " : Distance = " << distance[i] << ". Predecessor = " << predecessor[i] << ".\n";
    }
    std::cout << std::endl;
}

// Initializes the necessary variables/arrays for single-source shortest path algorithms.
void GraphAM::initializeSingleSource(double *distance, int *predecessor) const {
    for (int u = 0; u < vertices; u++) {
        distance[u] = DBL_MAX; // Infinity.
        predecessor[u] = -1; // Null.
    }
}

// Relaxes an edge u, v.
void GraphAM::relax(int u, int v, double *distance, int *predecessor) const {
    double weight = getEdgeWeight(u, v);
    if (distance[v] > distance[u] + weight) {
        distance[v] = distance[u] + weight;
        predecessor[v] = u;
    }
}

// Finds the shortest path from a single source node using the Bellman Ford shortest path algorithm.
bool GraphAM::bellmanFord(int src, double *&distance, int *&predecessor) {
    distance = new double[vertices];
    predecessor = new int[vertices];
    initializeSingleSource(distance, predecessor);
    distance[src] = 0;
    for (int i = 0; i < vertices -1; i++) {
        for (int u = 0; u < vertices; u++) {
            setCurrentVertex(u);
            int v;
            while (getNextAdjacent(u, v)) {
                relax(u, v, distance, predecessor);
            }
        }
        displayShortestPath(distance, predecessor, vertices);
    }
    for (int u = 0; u < vertices; u++) {
        setCurrentVertex(u);
        int v;
        while (getNextAdjacent(u, v)) {
            double weight = getEdgeWeight(u, v);
            if (distance[v] > distance[u] + weight) {
                return false;
            }
        }
    }
    return true;
}

// Finds the shortest path from a single source node using Dijkstra's algorithm.
void GraphAM::dijkstra(int src, double *&distance, int *&predecessor) {
    distance = new double[vertices];
    predecessor = new int[vertices];
    PriorityQueue *pQueue = new PriorityQueue(vertices);
    Queue *queue = new Queue(vertices);
    initializeSingleSource(distance, predecessor);
    distance[src] = 0;
    for (int u = 0; u < vertices; u++) {
        pQueue->push(u, distance[u]);
    }
    //pQueue->display();
    while (!pQueue->isEmpty()) {
        queueElement output = pQueue->pop();
        //pQueue->display();
        queue->enqueue(output.index);
        int u = output.index;
        //std::cout << "Use: " << output.index << " with distance " << output.value << std::endl;
        setCurrentVertex(u);
        int v;
        while (getNextAdjacent(u, v)) {
            double weight = getEdgeWeight(u, v);
            if (distance[v] > distance[u] + weight) {
                distance[v] = distance[u] + weight;
                predecessor[v] = u;
                pQueue->decreaseKey(v, distance[v]);
            }
        }
        //displayShortestPath(distance, predecessor, vertices);
        //pQueue->display();
    }
    delete pQueue;
    delete queue;
}

// Prints the shortest path between all possible pairs of vertices.
void GraphAM::printAllPairs(Matrix *predecessor, int i, int j) {
    if (i == j) {
        std::cout << i;
    }
    else if (predecessor->getCell(i, j) == -1) {
        std::cout << "No path from " << i << " to " << j << ".";
    }
    else {
        printAllPairs(predecessor, i, predecessor->getCell(i, j));
        std::cout << ", " << j;
    }
}

// Finds all pairs of shortest paths using the Floyd Warshall algorithm.
void GraphAM::floydWarshall(Matrix *&distanceOut, Matrix *&predecessorOut) {
    Matrix** distance = new Matrix*[vertices + 1];
    Matrix** predecessor = new Matrix*[vertices + 1];
    distance[0] = new Matrix(vertices, vertices);
    predecessor[0] = new Matrix(vertices, vertices);
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            double weight = getEdgeWeight(i, j);
            if (i == j) {
                distance[0]->setCell(i, j, 0);
            }
            else {
                if (weight != 0) {
                    distance[0]->setCell(i, j, weight);
                }
                else {
                    distance[0]->setCell(i, j, DBL_MAX);
                }
            }
            if (i == j || weight == 0) {
                predecessor[0]->setCell(i, j, -1);
            }
            else {
                predecessor[0]->setCell(i, j, i);
            }
        }
    }
    for (int k = 1; k <= vertices; k++) {
        distance[k] = new Matrix(vertices, vertices);
        predecessor[k] = new Matrix(vertices, vertices);
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                distance[k]->setCell(i, j, std::min(distance[k - 1]->getCell(i, j), distance[k - 1]->getCell(i, k - 1) + distance[k - 1]->getCell(k - 1, j)));
                if (distance[k]->getCell(i, j) == distance[k - 1]->getCell(i , j)) {
                    predecessor[k]->setCell(i, j, predecessor[k - 1]->getCell(i, j));
                }
                else {
                    predecessor[k]->setCell(i , j, predecessor[k - 1]->getCell(k - 1, j));
                }
            }
        }
    }
    distanceOut = distance[vertices];
    predecessorOut = predecessor[vertices];
    for (int k = 0; k < vertices; k++) {
        delete distance[k];
        delete predecessor[k];
    }
    delete[] distance;
    delete[] predecessor;
}

// Uses Dijkstra's shortest path algorithm to determine the shortest paths between all pairs of vertices.
void GraphAM::dijkstraMultiSource(Matrix *&distanceOut, Matrix *&predecessorOut) {
    distanceOut = new Matrix(vertices, vertices);
    predecessorOut = new Matrix(vertices, vertices);
    for (int i = 0; i < vertices; i++) {
        double *distance = nullptr;
        int *predecessor = nullptr;
        dijkstra(i, distance, predecessor);
        for (int j = 0; j < vertices; j++) {
            distanceOut->setCell(i, j, distance[j]);
            predecessorOut->setCell(i, j, predecessor[j]);
        }
        delete[] distance;
        delete[] predecessor;
    }
}

// Provides transitive closure matrix for instance of graph.
void GraphAM::transitiveClosure(Matrix *&tcOut) {
    Matrix** TC = new Matrix*[vertices + 1];
    TC[0] = new Matrix(vertices, vertices);
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            double weight = getEdgeWeight(i, j);
            if (i == j || weight != 0) {
                TC[0]->setCell(i, j, 1);
            }
            else {
                TC[0]->setCell(i, j, 0);
            }
        }
    }
    for (int k = 1; k < vertices; k++) {
        TC[k] = new Matrix(vertices, vertices);
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; i++) {
                TC[k]->setCell(i, j, std::min(TC[k - 1]->getCell(i, j), TC[k - 1]->getCell(i, k - 1) + TC[k - 1]->getCell(k - 1, j)));
            }
        }
    }
    tcOut = TC[vertices];
    for (int k = 0; k < vertices; k++) {
        delete[] TC[k];
    }
    delete[] TC;

}

// TODO
int GraphAM::minKey(int *key, bool *mstSet) {
    int min = INT_MAX;
    int minIndex = INT_MAX;
    for (int v = 0; v < vertices; v++) {
        if (mstSet[v] == false && key[v] < min) {
            min = key[v];
            minIndex = v;
        }
    }
    return minIndex;
}

// Helper function for isAcyclic.
void acyclic(int u, GraphAM &graph, State *state, int *predecessor, int *back, int parent) {
    state[u] = State::Visited;
    int v;
    graph.setCurrentVertex(u);
    while (graph.getNextAdjacent(u, v)) {
        if (graph.hasEdge(u, v) && v != u && v != parent) {
            if (state[v] == State::Unvisited) {
                predecessor[v] = u;
                acyclic(v, graph, state, predecessor, back, u);
            }
            else {
                back[v] = u;
            }
        }
    }
}

// Determines whether a graph is acyclic.
bool GraphAM::isAcyclic(int u, GraphAM &graph) {
    int vertices = graph.getVertices();
    State *state = new State[vertices];
    int *back = new int[vertices];
    int *predecessor = new int[vertices];
    for (int i = 0; i < vertices; i++) {
        state[i] = State::Unvisited;
        back[i] = -1;
        predecessor[i] = -1;
    }
    acyclic(u, graph, state, predecessor, back, -1);
    int i = 0;
    bool acyclic = true;
    bool found = false;
    while (i < vertices && !found) {
        if (back[i] == u && predecessor[i] != u) {
            found = true;
            acyclic = false;
        }
        i++;
    }
    delete[] state;
    delete[] back;
    delete[] predecessor;
    return acyclic;
}

// Finds the MST of the graph using the Kruskal algorithm.
void GraphAM::MSTKruskal() {
    GraphAM tmpGraph(vertices);
    Matrix tmpMatrix(vertices, vertices);
    tmpMatrix.init(0);
    int total = 0;
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            tmpMatrix(i, j) = getEdgeWeight(i, j);
        }
    }
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            if (tmpMatrix(i, j) != 0) {
                total++;
            }
        }
    }
    int argi;
    int argj;
    double value;
    std::tie(value, argi, argj) = tmpMatrix.getMin();
    tmpMatrix(argi, argj) = 0;
    tmpGraph.addUndirectedEdge(argi, argj, value);
    for (int i = 0; i < total; i++) {
        std::tie(value, argi, argj) = tmpMatrix.getMin();
        tmpMatrix(argi, argj) = 0;
        tmpGraph.addUndirectedEdge(argi, argj, value);
        if (!isAcyclic(argi, tmpGraph)) {
            tmpGraph.removeDirectedEdge(argi, argj);
        }
    }
    std::cout << "Display Graph:\n";
    tmpGraph.displayUndirected();
}

// Finds the MST of the graph using Prim's algorithm.
void GraphAM::MSTPrim() {
    // TODO
}

// Displays the MST of the graph.
void GraphAM::displayMST(int *parent) {
    std::cout << "MST Edge Weight:\n";
    for (int i = 0; i < vertices; i++) {
        std::cout << "(" << parent[i] << " -> " << i << ") Weight: " << getEdgeWeight(i, parent[i]) << std::endl;
    }
}

// Generates a graph of n vertices where an edge exists between u and v with probability p.
void GraphAM::makeRandomGraph(int n, double p) {

    delete[] matrix;
    delete[] current;

    // Initialize matrix and current arrays, set all initial values to 0.
    vertices = n;
    matrix = new double[vertices * vertices];
    current = new int[vertices];
    for (int i = 0; i < vertices * vertices; i++) {
        matrix[i] = 0;
    }
    for (int i = 0; i < vertices; i++) {
        current[i] = 0;
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, 1.0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist(mt) <= p) {
                addDirectedEdge(i, j);
            }
        }
    }
}

// Displays the adjacency matrix for the graph.
void GraphAM::display() const {
    int k = 0;
    std::cout << " : ";
    for (int v = 0; v < vertices; v++) {
        std::cout << v << "\t";
    }
    std::cout << std::endl;
    for (int u = 0; u < vertices; u++) {
        std::cout << u << ": ";
        for (int v = 0; v < vertices; v++) {
            std::cout << matrix[k] << "\t";
            k++;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Displays a list of directed edges within the graph.
void GraphAM::displayDirected() const {
    std::cout << "List of directed edges:" << std::endl;
    for (int u = 0; u < vertices; u++) {
        for (int v = 0; v < vertices; v++) {
            if (hasEdge(u, v)) {
                std::cout << "(" << u << ", " << v << ") Weight: " << getEdgeWeight(u, v) << std::endl;
            }
        }
    }
}

// Displays a list of undirected edges within the graph.
void GraphAM::displayUndirected() const {
    std::cout << "List of undirected edges:" << std::endl;
    for (int u = 0; u < vertices; u++) {
        for (int v = u + 1; v < vertices; v++) {
            if (hasEdge(u, v)) {
                std::cout << "(" << u << ", " << v << ") Weight: " << getEdgeWeight(u, v) << std::endl;
            }
        }
    }
}