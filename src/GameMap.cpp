#include "GameMap.h"
#include "GraphAM.h"
#include "GraphAL.h"


// Change "Graph" typedef in GameMap.h to change the type of graph used for Nav.

// A map is a matrix of characters. The character denotes what a given "tile" or "square" is meant to be. The character
// 'i' represents impassable terrain, for example.
// 'i': impassable
// 'p': passable
// 'g': goal (where the player agent will pathfind to)
// 's': start (where the player agent will start)
// 'e': enemy (where enemies can start and begin pathfinding to the player agent location)

// Construct a new map of size x * y.
GameMap::GameMap(int x, int y) : x(x), y(y), matrix(new char*[x]), nav(new Graph(x * y * 4)), playerStart(-1), goal(-1), enemyStarts(new Queue(x * y)) {
    // Initialize matrix of map squares.
    for (int i = 0; i < x; i++) {
        matrix[i] = new char[y];
    }
}

// Default constructor to initialize a map of size 10 * 10.
GameMap::GameMap() : GameMap(10, 10) {}

// Destroy the map.
GameMap::~GameMap() {
    for (int i = 0; i < x; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Returns a pointer to the map tile at the current index.
char GameMap::getTile(int idx) const {
    // Use integer arithmetic to determine which array it is in, and then modulo for the offset from the start of that array.
    return (matrix[idx / y][idx % y]);
}

// Initializes the Navigation graph of the map.
void GameMap::init() {
    // Iterate through the whole map.
    for (int i = 0; i < (x * y); i++) {
        // Make sure our current tile is one that should even be linked (not 'i' for impassible).
        if (getTile(i) != 'i') {
            // Link internal nodes.
            // Left turns:
            nav->addDirectedEdge((4 * i + 0), (4 * i + 1), 8); // North to West
            nav->addDirectedEdge((4 * i + 1), (4 * i + 3), 8); // West to South
            nav->addDirectedEdge((4 * i + 3), (4 * i + 2), 8); // South to East
            nav->addDirectedEdge((4 * i + 2), (4 * i + 0), 8); // East to North
            // Right Turns:
            nav->addDirectedEdge((4 * i + 0), (4 * i + 2), 4); // North to East
            nav->addDirectedEdge((4 * i + 2), (4 * i + 3), 4); // East to South
            nav->addDirectedEdge((4 * i + 3), (4 * i + 1), 4); // South to West
            nav->addDirectedEdge((4 * i + 1), (4 * i + 0), 4); // West to North
            // Check if the tile to the right is valid, link them if so.
            if (i >= 0 && (i % y) + 1 < y) {
                if (getTile(i + 1) != 'i') {
                    nav->addDirectedEdge((4 * (i + 1) + 1), (4 * i + 1), 2); // Forward West
                    nav->addDirectedEdge((4 * i + 1), (4 * (i + 1) + 1), 4); // Backward West
                    nav->addDirectedEdge((4 * i + 2), (4 * (i + 1) + 2), 2); // Forward East
                    nav->addDirectedEdge((4 * (i + 1) + 2), (4 * i + 2), 4); // Backward East
                }
            }
            // Check if the tile below is valid, link them if so.
            if (i >= 0 && (i + y) < (x * y)) {
                if (getTile(i + y) != 'i') {
                    nav->addDirectedEdge((4 * (i + y) + 0), (4 * i + 0), 2); // Forward North
                    nav->addDirectedEdge((4 * i + 0), (4 * (i + y) + 0), 4); // Backward North
                    nav->addDirectedEdge((4 * i + 3), (4 * (i + y) + 3), 2); // Forward South
                    nav->addDirectedEdge((4 * (i + y) + 3), (4 * i + 3), 4); // Backward South
                }
            }
        }
    }
}


// Sets the state of a given tile to the given state.
void GameMap::setTileState(int idx, char state) {
    matrix[idx / y][idx % y] = state;
}





