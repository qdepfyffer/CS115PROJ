#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <iostream>
#include <fstream>

#include "GraphAM.h"
#include "GraphAL.h"

typedef GraphAL Graph; // I'm not going to support GraphAM at the moment because it's a poor fit for this application.
                       // Ultimately, the graphs end up VERY sparse in almost every case, so GraphAL ends up being a
                       // much better fit. I planned to support it originally, so that's why i used a typedef here.

struct GameMap {
    int x; // Rows.
    int y; // Cols.
    char** matrix; // The actual map.
    Graph* nav; // Navigation graph of the map.
    int playerStart;
    int goal;
    Queue* enemyStarts;

    GameMap(int rows, int cols);
    GameMap();
    ~GameMap();

    [[nodiscard]] char getTile(int tileIndex) const;
    void setTileState(int tileIndex, char state);

    void init();

};



#endif //GAMEMAP_H