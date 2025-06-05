#include <iostream>
#include <fstream>
#include <cstring>
#include <cfloat>

#include "Game.h"

// Constructs a new game where the map is loaded from the provided filename.
Game::Game(const char* filename) : map(nullptr), character(new Agent()), enemies(nullptr), numEnemies(0) {
    pathAlgorithm = (&Game::pathDijkstra); // Change this to determine which pathfinding algorithm is used.
    loadMap(filename);
}


Game::Game() : map(nullptr), character(new Agent()), enemies(nullptr), numEnemies(0) {}


Game::~Game() {
    delete[] enemies;
}


void Game::loadMap(const char* filename) {
    // Attempt to open the file.
    std::ifstream infile(filename);
    if (infile.is_open()) {
        std::cout << "File successfully opened. Loading map and locating points of interest...\n";
        // Initialize some variables to be used in processing the file and creating the map.
        char* arr[512]; // To store the lines from the file in
        int line = 0; // To keep track of what line we're on
        char buf[512]; // Buffer for processing lines. 512 should be a sufficient size, adjust if necessary.
        // Traverse the file line-by-line and add information to the array to be processed later.
        while(infile.getline(buf, 512)) {
            arr[line] = new char[strlen(buf) + 1]; //
            std::strcpy(arr[line], buf);
            line++;
        }
        infile.close(); // Close the file since all the information we need is in the array now.
        // Use the first two lines of the file as the specifiers for rows and columns and create a new map.
        if (line >= 3) {
            int rows = 0, cols = 0, dir = 0;
            rows = std::atoi(arr[0]);
            cols = std::atoi(arr[1]);
            dir = std::atoi(arr[2]) % 4;
            GameMap* newMap = new GameMap(rows, cols);
            bool playerFound = false;
            bool goalFound = false;
            int enemyCount = 0;
            for (int i = 3; i < rows + 3; i++) {
                for (int j = 0; j < cols; j++) {
                    if (char c = arr[i][j]) {
                        // Find where the player is going to start
                        if (c == 's') {
                            std::cout << "Player agent start found at tile " << ((i - 3) * newMap->y) + j << std::endl;
                            newMap->playerStart = ((((i - 3) * newMap->y) + j) * 4 + dir); // Start node

                            playerFound = true;
                        }
                        // Find the goal.
                        if (c == 'g') {
                            // Goal will be stored as a simple flattened index, because one of the 4 nodes will be
                            // closer to the player than the other 3, and we want to navigate to that node and stop
                            // there. So the goal NODE is calculated later, based on the flattened goal index.
                            std::cout << "Goal found at tile " << ((i - 3) * newMap->y) + j << std::endl;
                            newMap->goal = ((i - 3) * newMap->y) + j;
                            goalFound = true;
                        }
                        // Count enemies.
                        if (c == 'e') {
                            std::cout << "Enemy found at tile " << ((i - 3) * newMap->y) + j << std::endl;
                            newMap->enemyStarts->enqueue(((((i - 3) * newMap->y) + j) * 4));
                            enemyCount++;
                        }
                        // Translate file contents to matrix.
                        newMap->matrix[i - 3][j] = (arr[i][j]);
                    }
                }
            }
            // Initialize enemy information for when enemies are added.
            numEnemies = 0;
            enemies = new Agent*[enemyCount];
            // Inform the user if any important information was not found.
            if (!playerFound) {
                std::cout << "[ERROR] Player agent start not found!\n";
            }
            if (!goalFound) {
                std::cout << "[ERROR] Goal not found!\n";
            }
            // Clean up all the memory allocated for storing the lines in the file
            for (int i = 0; i < line; i++) {
                delete[] arr[i];
            }

            map = newMap;
            // Initialize the map to build the graph.
            map->init();
        }
    }
    else {
        // If there's some issue with the file, the line variable will not be greater than or equal to 3, so return a pointer to a default map.
        map = new GameMap();
        // Initialize the map to build the graph.
        map->init();
    }
}

// Adds the player agent.
void Game::addPlayer() {
    Agent *player = new Agent(true);
    player->move(map->playerStart);
    character = player;
}

// Adds an enemy agent. The number of enemy agents is determined by numEnemies.
void Game::addEnemy() {
    if (!map->enemyStarts->isEmpty()) {
        Agent *enemy = new Agent(false);
        enemy->move(map->enemyStarts->dequeue());
        enemies[numEnemies] = enemy;
        numEnemies++;
    }
}

// Renders the current state of the game.
void Game::render() const {
    // The time complexity here is insane.
    for (int i = 0; i < map->x; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < map->y; k++) {
                for (int l = 0; l < 3; l++) {
                    char c = map->matrix[i][k];
                    int tile = i * map->y + k;
                    int pos = character->getPos();
                    if (j == 1 && l == 1) {
                        if (tile * 4 <= pos && tile * 4 + 3 >= pos) {
                            std::cout << character->getCharRep() << ' ';
                        }
                        else if (tile == map->goal) {
                            std::cout << 'x' << ' ';
                        }
                        else if (numEnemies > 0) {
                            bool hasEnemy = false;
                            for (int m = 0; m < numEnemies; m++) {
                                int ePos = enemies[m]->getPos();
                                if (tile * 4 <= ePos && tile * 4 + 3 >= ePos) {
                                    hasEnemy = true;
                                }
                            }
                            if (hasEnemy) {
                                std::cout << 'o' << ' ';
                            }
                            else {
                                // If the character in the map matrix is 'i', display it as a block of '#', denoting a wall.
                                if (c == 'i') {
                                    std::cout << '#' << ' ';
                                }
                                // Otherwise, display everything as a block of '-', denoting passable terrain.
                                else {
                                    std::cout << '-' << ' ';
                                }
                            }
                        }
                        else {
                            // If the character in the map matrix is 'i', display it as a block of '#', denoting a wall.
                            if (c == 'i') {
                                std::cout << '#' << ' ';
                            }
                            // Otherwise, display everything as a block of '-', denoting passable terrain.
                            else {
                                std::cout << '-' << ' ';
                            }
                        }
                    }
                    else {
                        // If the character in the map matrix is 'i', display it as a block of '#', denoting a wall.
                        if (c == 'i') {
                            std::cout << '#' << ' ';
                        }
                        // Otherwise, display everything as a block of '-', denoting passable terrain.
                        else {
                            std::cout << '-' << ' ';
                        }
                    }
                }
                std::cout << "  ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void Game::render2() const {
    for (int i = 0; i < map->x; i++) {
        for (int j = 0; j < map->y; j++) {
            char c = map->matrix[i][j];
            if (c == 'i') {
                std::cout << '#' << ' ';
            }
            else {
                int tile = i * map->y + j;
                int pos = character->getPos();
                if (tile * 4 <= pos && tile * 4 + 3 >= pos) {
                    std::cout << character->getCharRep() << ' ';
                }
                else if (tile == map->goal) {
                    std::cout << 'x' << ' ';
                }
                else if (numEnemies > 0) {
                    bool hasEnemy = false;
                    for (int m = 0; m < numEnemies; m++) {
                        int ePos = enemies[m]->getPos();
                        if (tile * 4 <= ePos && tile * 4 + 3 >= ePos) {
                            hasEnemy = true;
                        }
                    }
                    if (hasEnemy) {
                        std::cout << 'o' << ' ';
                    }
                    else {
                        std::cout << ' ' << ' ';
                    }
                }
                else {
                    std::cout << ' ' << ' ';
                }
            }
        }
        std::cout << std::endl;
    }
}

// Displays the navigation graph for the current map.
void Game::displayNav() const {
    map->nav->display();
}

// Finds the shortest path from a source node to a destination tile.
Queue* Game::pathDijkstra(int srcNode, int destTile) {
    // Technically, the goal has 4 nodes. We'll pick the one with the actual shortest path.
    double* d; // Distance array
    int* pi; // Predecessor array

    // If there is no goal, just return an empty queue.
    if (destTile == -1) {
        return new Queue(1);
    }

    map->nav->dijkstra(srcNode, d, pi);

    // Pick the nearest goal node.
    int shortest = 4 * destTile + 0; // Assume north is the shortest path
    for (int i = 1; i < 4; i++) {
        if (d[4 * destTile + i] < d[shortest])
            shortest = 4 * destTile + i;
    }

    Queue *path = new Queue(4 * map->x * map->y);

    if (d[shortest] == DBL_MAX) {
        delete[] d;
        delete[] pi;
        return new Queue(1);
    }

    map->nav->extractPath(shortest, pi, path);

    delete[] d;
    delete[] pi;
    return path;
}

// Finds the shortest path from the current node to a given TILE. This is because the path to a given side of a tile can
// be shorter than the path to another side of that same tile
Queue* Game::pathBFS(int srcNode, int destTile) {
    // Technically, the goal has 4 nodes. We'll pick the one with the actual shortest path.
    State *state;
    double *d;
    int *pi;

    // If there is no goal, just return an empty queue.
    if (destTile == -1) {
        return new Queue(1);
    }

    Graph::BFS_MAIN(srcNode, map->nav, state, d, pi);


    // Pick the shortest of the above paths;
    int shortest = 4 * destTile + 0; // Assume north is the shortest path
    for (int i = 1; i < 4; i++) {
        if (d[4 * destTile + i] < d[shortest])
            shortest = 4 * destTile + i;
    }


    Queue *path = new Queue(4 * map->x * map->y);

    if (d[shortest] == 0) {
        delete[] d;
        delete[] pi;
        return new Queue(1);
    }

    map->nav->extractPath(shortest, pi, path);

    delete[] d;
    delete[] pi;

    return path;
}

void Game::play() {

    bool caught = false;

    // Try to add the player character.
    std::cout << "Attempting to add player...\n";
    if (map->playerStart != -1) {
        addPlayer();
        std::cout << "Character successfully added.\n";
    }
    else {
        std::cout << "[ERROR] Failed to add player. No valid player start found.\n";
    }

    // Try to build a path to the goal for the character.
    std::cout << "Attempting to build path to goal...\n";
    Queue *path = new Queue();
    if (character->getPos() != -1) {
        path = (this->*pathAlgorithm)(character->getPos(), map->goal);
        std::cout << "Successfully built path to goal.\n";
        // This is to clear the initial useless move for the player.
        if (!path->isEmpty()) {
            character->move(path->dequeue());
        }
    }
    else {
        std::cout << "[ERROR] Failed to build path to goal. No character found.\n";
    }

    // Try to add the enemies to the game.
    std::cout << "Attempting to add enemies...\n";
    int maxEnemies = map->enemyStarts->getSize();
    std::cout << "Maximum number of enemies: " << maxEnemies << std::endl;
    for (int i = 0; i < maxEnemies; i++) {
        addEnemy();
    }
    std::cout << "Number of enemies after adding: " << numEnemies << std::endl;

    render2();

    // Game loop logic. The game will continue so long as the player agent has not reached the goal node.
    while (!path->isEmpty() && !caught) {
        std::cin.get();
        // Enemies move first if they have a valid path. Otherwise, they won't move.
        for (int i = 0; i < numEnemies; i++) {
            Queue *ePath = (this->*pathAlgorithm)(enemies[i]->getPos(), character->getPos()/4);
            if (!ePath->isEmpty()) {
                // Technically, the enemies move two times every turn because the first move in a queue is always wasted.
                // This is because the first move is always the agent's current tile.
                enemies[i]->move(ePath->dequeue());
                if (!ePath->isEmpty()) {
                    enemies[i]->move(ePath->dequeue());
                }
            }
            int pTile = character->getPos() / 4;
            int eTile = enemies[i]->getPos() / 4;
            if (eTile == pTile) {
                caught = true;
            }
            delete ePath;
        }

        // If the character has a valid path to the goal, move the character.
        if (!path->isEmpty() && !caught) {
            int newPos = path->dequeue();
            // Calculate difference in positions.
            int diff = newPos - character->getPos();
            // Get old and new facing directions for character.
            int newDir = newPos % 4;
            int oldDir = character->getPos() % 4;

            std::cout << "Difference In Position: " << diff << ". Old Direction: " << oldDir << ". New Direction: " << newDir << ".\n";

            // Below is just math to list the player agent's moves at every step. I won't go into too much detail
            // about how it works. Simply take the difference of the old and new directions, along with the new
            // direction, and use that information to determine which way the character is now facing. This is slightly
            // less code than simply comparing old and new directions directly.
            if (oldDir - newDir == -2) {
                if (newDir == 2) {
                    std::cout << "Player agent turns right from north to east.\n";
                }
                else {
                    std::cout << "Player agent turns left from west to south.\n";
                }
            }
            else if (oldDir - newDir == -1) {
                if (newDir == 3) {
                    std::cout << "Player agent turns right from east to south.\n";
                }
                else {
                    std::cout << "Player agent turns left from north to west.\n";
                }
            }
            else if (oldDir - newDir == 2) {
                if (newDir == 1) {
                    std::cout << "Player agent turns right from south to west.\n";
                }
                else {
                    std::cout << "Player agent turns left from east to north.\n";
                }
            }
            else if (oldDir - newDir == 1) {
                if (newDir == 0) {
                    std::cout << "Player agent turns right from west to north.\n";
                }
                else {
                    std::cout << "Player agent turns left from south to east.\n";
                }
            }
            if (diff == 4) {
                if (oldDir == 2) {
                    std::cout << "Player agent moves forward to the east.\n";
                }
                else {
                    std::cout << "Player agent moves backward to the east.\n";
                }
            }
            else if (diff == 4 * map->y) {
                if (oldDir == 3) {
                    std::cout << "Player agent moves forward to the south.\n";
                }
                else {
                    std::cout << "Player agent moves backward to the south.\n";
                }
            }
            else if (diff == -4) {
                if (oldDir == 1) {
                    std::cout << "Player agent moves forward to the west.\n";
                }
                else {
                    std::cout << "player agent moves backward to the west.\n";
                }
            }
            else if (diff == -4 * map->y) {
                if (oldDir == 0) {
                    std::cout << "Player agent moves forward to the north.\n";
                }
                else {
                    std::cout << "Player agent moves backward to the north.\n";
                }
            }

            character->move(newPos);

        }

        // render the final state of the map after the current turn.
        render2();
    }

    // Inform the user that the game is over and wait for input so that it doesn't immediately exit.
    if (caught) {
        std::cout << "Game over! Player agent was caught. Press enter to exit.\n";
    }
    else {
        std::cout << "Game over! Press enter to exit.\n";
    }

    std::cin.get();

    delete path;
}
