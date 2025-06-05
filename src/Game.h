#ifndef GAME_H
#define GAME_H

#include "GameMap.h"
#include "Agent.h"
#include "Queue.h"

class Game {
public:

    using PathFunction = Queue* (Game::*)(int srcNode, int destTile);

    explicit Game(const char* filename);
    Game();
    ~Game();

    Queue* pathDijkstra(int src, int dest);
    Queue* pathBFS(int src, int dest);

    void loadMap(const char* filename);
    void addPlayer();
    void addEnemy();

    void render() const;
    void render2() const;
    void displayNav() const;

    void play();

private:
    GameMap* map;
    Agent* character;
    Agent** enemies;
    int numEnemies;
    PathFunction pathAlgorithm;

};

#endif //GAME_H
