#ifndef TILE_H
#define TILE_H

struct Tile {

    char state; // Determines whether a tile is Passable, Impassable, Occupied, or the Goal.
    char* tile; // String representation of a tile.

    Tile();
    ~Tile();

    void update(char state);

    void displayTile() const;

};

#endif //TILE_H