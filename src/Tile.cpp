#include <iostream>

#include "Tile.h"

/// THIS FILE IS UNUSED. I WROTE IT BEFORE RE-WRITING THE GAME MAP CLASS, WHICH IS BASICALLY JUST A MATRIX. I FIGURED
/// THERE WAS NO POINT IN USING THIS LITTLE STRUCTURE, WHICH WAS REALLY JUST AN OVERCOMPLICATED SOLUTION TO AVOID MORE
/// COMPLICATED RENDERING LOGIC. I DECIDED TO TACKLE THE MORE COMPLICATED RENDERING LOGIC BECAUSE IT CUT OUT THIS
/// NEEDLESSLY COMPLICATED SOLUTION. I'VE LEFT IT IN FOR POSTERITY'S SAKE, SO THAT I CAN LOOK BACK AND LAUGH AT IT LATER
/// OR SOMETHING. THANKS.

/*
 * Tiles are represented by an array of 9 characters.
 * The state of a tile is represented by a single character, as follows:
 * 'p': Passable
 * 'i': Impassable
 * 'o': Occupied
 * 'g': Goal
*/

// Constructs a new tile. Tiles default to a passable state.
Tile::Tile() : state('p'), tile(new char[10] {"_________"}) {}

// Destroys the tile.
Tile::~Tile() {
    delete[] tile;
}

// Updates the tile it is called on based on the passed state and agent character.
void Tile::update(char state) {
    // We want to default to a passable state, so that will come last. This is so we can allow for other, non-state
    // characters that will denote things like valid enemy starting positions or player start positions.
    // Set state to impassable
    if (state == 'i') {
        // Set the tile to the impassible state
        for (int i = 0; i < 9; i++) {
            tile[i] = '#';
        }
    }
    // Set state to occupied.
    else if (state == 'o') {
        // Reset the tile to a passable state.
        for (int i = 0; i < 9; i++) {
            tile[i] = '_';
        }
    }
    // Set state to goal.
    else if (state == 'g') {
        // Reset the tile to a passable state.
        for (int i = 0; i < 9; i++) {
            tile[i] = '_';
        }
        // Add the goal representation to the middle of the displayed tile.
        tile[4] = 'X';
    }
    // Set the state to passable by default.
    else {
        // Set the tile to the impassible state
        for (int i = 0; i < 9; i++) {
            tile[i] = '_';
        }
    }
}

// Display a single tile, for testng purposes mostly.
void Tile::displayTile() const {
    // Tiles are 3x3, so we just need to do a little math to display the tile as a... tile.
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << tile[3 * i + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}