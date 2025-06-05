#include "Agent.h"

// Constructs an agent.
Agent::Agent(bool isPlayer) : pos(-1), player(isPlayer) {}

// Default constructor for Agent.
Agent::Agent() : pos(-1), player(false) {}

// Default destructor for Agent.
Agent::~Agent() = default;

// Returns the agent's current position.
int Agent::getPos() {
    return pos;
}

// Returns the agent's character representation based on its current direction.
char Agent::getCharRep() {
    if (getPos() % 4 == 0) {
        return '^';
    }
    if (getPos() % 4 == 1) {
        return '<';
    }
    if (getPos() % 4 == 2) {
        return '>';
    }
    if (getPos() % 4 == 3) {
        return 'v';
    }
    return 'o';
}


// Updates an agent's position.
void Agent::move(int newPos) {
    pos = newPos;
}