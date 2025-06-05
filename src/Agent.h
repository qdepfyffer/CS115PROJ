#ifndef AGENT_H
#define AGENT_H

class Agent {
public:
    explicit Agent(bool isPlayer);
    Agent();
    ~Agent();

    int getPos();
    char getCharRep();

    void move(int newPos);

private:
    int pos; // Position of the agent. This is initially determined by a map's player agent start position.
    bool player; // Whether the agent is the player agent or not. Non-player agents will be hostile.

};

#endif //AGENT_H
