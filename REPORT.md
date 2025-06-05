## Introduction

---

This is my CSCI 115 project. I did all the work and programming, all the planning and organization. <br>
This was a solo venture, and I'm fairly happy with how it turned out. <br>

## What is this?

---

This project demonstrates a 2-dimensional representation of the shortest path between two nodes on a graph. <br>
It contains a map represented by a matrix of characters, which can be navigated by an agent. There are hostile <br>
and non-hostile agents. The non-hostile agent will attempt to navigate the maze or map it is on, and the hostile <br>
agents will attempt to chase down the non-hostile agent as it completes the maze. <br>

The algorithms demonstrated in this project for finding the shortest path include Dijkstra's algorithm and <br>
breadth-first search. Whichever is specified will ultimately be used by both hostile and non-hostile agents in <br>
the simulations. Dijkstra's functions better because the non-hostile agent must weigh the cost of the different <br>
moves it can make from wherever it currently is on the map. Breadth-first search notably doesn't account for this <br>
in any significant way, so it doesn't demonstrate a consideration for the different weights of different moves. <br>

As far as different moves go, the agent can move forward or backward, but only in the four cardinal directions <br>
(North/Up, East/Right, South/Down, West/Left). To move to a certain tile, the agent must currently be <br>
facing that direction. If it is not, it must turn to face that direction before moving (unless it chooses to move <br>
backward, which it might do in certain scenarios).<br>

Costs to move are as follows, as outlined in the project specifications:

| Move         | Cost |
|--------------|------|
| Go Forward   | 2    |
| Go Backward  | 4    |
| Turn Left    | 8    |
| Turn Right   | 4    |

From the table above, you can see that turning right and moving backward (cost of 8) could result in a more <br>
optimal path than turning left and going forward (cost of 10). The agents in the simulation can be seen abiding <br>
by this.<br>

The simulation runs until the non-hostile agent reaches the goal (if that is possible) or is caught by the hostile <br>
agents (if that is possible).

## How do I run the project?

---
Before running the project, it must be built using the provided build scripts if the executables are not already <br>
present in their respective folders (/build/win or /build/unix) for the user's chosen system. This also must be <br>
done any time the source code (.cpp/.h files in /src) is changed or updated, or when you wish to use a different <br>
map (contained in /maps) to demonstrate the algorithms. <br>

After the project is built, it can be run through the terminal or, if I've decided on either of the following, you <br>
can either:
- Run the .exe with the provided run script, depending on the user's chosen system.
- Navigate to the build directory for the chosen system and double-click the .exe

I'm writing this before deciding on a solution. The first one is easier, but the second is more professional. We'll <br>
see. <br>

EDIT (05/07/2025): Both should work.

Either way, once the project is built and run, it should launch a terminal where the simulation will be shown. <br>
The simulation waits between each step for the user to enter some input, as a way to allow the user to "pause" <br>
between steps to see what exactly the agent is doing.

When run, the program will prompt the user to enter a number corresponding to the map they wish to use. Upon <br>
entering the number, the simulation will be run with the current map.

## What does the simulation look like?

---

After running the .exe, the simulation will be shown in a terminal, where the map is represented by a matrix of <br>
"tiles." The "tiles" are nothing more than a representation of a character matrix and solely exist as a display.

Valid tile representations can be seen below:

```c++
Valid non-player tiles:
# # #  - - -  - - -  - - -
# # #  - - -  - x -  - o -
# # #  - - -  - - -  - - -

Valid player tiles:
- - -  - - -  - - -  - - -
- ^ -  - > -  - v -  - < -
- - -  - - -  - - -  - - -
```
In the top row, from left to right, we have: impassable terrain, empty passable terrain, passable terrain occupied <br>
by the non-hostile agent's "goal," and passable terrain occupied by the enemy. The bottom row shows all the <br>
valid representations of the non-hostile agent, where the center character "points" in the direction the agent is <br>
currently facing. As the agent moves through the map, it must change its direction to make valid moves, and the <br>
character at the center of the agent's current tile will be "rotated" to reflect that. <br>

As for the hostile agents, they are simply represented by an 'o' character. This is merely to make them visually <br>
distinct from the non-hostile agent, and while they cannot be seen "turning" like the non-hostile agent, they <br>
ultimately follow all the same restrictions and steps. They could be represented by a differently colored set of <br>
characters, otherwise identical to the non-hostile agent, and be observed to do so.

## How does this all work?

---

### The game:

The game consists of the map, the graph, the non-hostile agent, and however many hostile agents have been added,<br>
as determined by the number of valid enemy starting positions in the map, which is covered below. Navigation <br>
of the map is handled by whichever shortest-path algorithm is used (breadth-first search or Dijkstra's <br>
algorithm) with the graph, which will also be covered below. As mentioned in the section *What is this?*, the game <br>
continues until the non-hostile agent either makes its way to the goal or gets intercepted by the hostile agents. <br>
On the occurrence of either event, the simulation will end and the program will exit.

### The map:

First things first, the map is created at runtime by a function that reads a .txt file into an array and then uses <br>
the information contained in the array to construct a new map. The "map" is, in effect, a matrix with a bit more <br>
functionality attached. The .txt file that gets converted into a map must contain data that follows a specific <br>
format, as illustrated by the below example:

```
1| 5        | Line 1: Positive integer value, the number of rows in the map to be created.
2| 5        | Line 2: Positive integer value, the number of columns in the map to be created.
3| 0        | Line 3: Positive integer value, 0 - 3, determines starting direction of agent
4| ppppp    | Line 4+: The actual character representation of the map. The number of lines from
5| ppppp    |          here onward should be equal to the integer in line 1, in other words,   
6| ppppp    |          (total lines - 3) = integer in line 1. The number of characters in each
7| ppppp    |          line should be equal to the integer in line 2. Each line should consist
8| ppppp    |          only of characters that denote something meaningful, as covered below.
```

As mentioned above, the map is a matrix of characters where each index in the matrix holds a single character. <br>
The character in held at that index represents a kind of "state" for that given tile. As mentioned above, there <br>
are only a few characters that mean anything significant to the function that builds the map:

| Character | Meaning                                         |
|-----------|-------------------------------------------------|
| 'i'       | Impassable terrain                              |
| 'p'       | Passable terrain                                |
| 'g'       | The goal for the non-hostile agent              |
| 's'       | The starting location for the non-hostile agent |
| 'e'       | The starting location for the hostile agents.   |

Additionally, it would make little sense to have a map for the simulation without at least a non-hostile agent and <br>
a goal for it to navigate toward, along with some impassable terrain to provide it with some obstacles. It is <br>
technically possible, but would result in little action.

### The agents:

Admittedly, the agents do not do much. They change their own position, and that is about it. The agent also <br>
uses this position information to determine what it should "look like," so to speak, or which way it is "facing" <br>
so that it can be properly rendered by the game. The position stored by the agent corresponds to a node on the <br>
navigational graph, which is covered below. There are non-hostile and hostile agents. Each kind of agent starts <br>
the game at its corresponding start location, which is determined by the presence of 's' and 'e' characters in the <br>
map. Non-hostile agents will pathfind using the chosen shortest-path algorithm to the goal, denoted by 'g' in the <br>
map, while hostile agents will pathfind using that same algorithm to the non-hostile agent. If either agent is <br>
successful in its pursuit, the game ends.

### The graph:

The graph is procedurally generated using the information from the map matrix. Since the character needs to be <br>
able to "turn," it must therefore be able to face four directions at any given tile. The solution I came up with <br>
was to allow each tile to be represented by four separate nodes on the graph. Therefore, each tile has a <br>
corresponding north, east, south, and west node. The math behind this was rather straightforward once I <br>
planned things out. Given a tile n where n is a flattened index corresponding to a position in a two-dimensional <br>
matrix starting from 0, we can represent each node as follows: 

| Node Index | Direction |
|------------|-----------|
| 4n + 0     | North     |
| 4n + 1     | West      |
| 4n + 2     | East      |
| 4n + 3     | South     |

The graph is then built by iterating through the matrix. For each tile, we check its validity along with that of <br>
the tile to the right and the tile below the current tile. If our current tile is anything but impassable, its <br> 
nodes will be linked with any to the right of / below it, so long as those correspond to a node that is anything <br>
but impassable. The nodes are linked internally (linkages between the four nodes corresponding to a single tile) <br>
and externally (linkages between nodes which correspond to different tiles).

## What difficulties did I have with this project?

---

I would love to say "none, I'm a genius and this was one of the easiest things I've ever done." Things are rarely <br>
as we wish they were though, and problems were numerous throughout the development. I struggled a lot initially <br>
with how to arrange the project. I had a lot of questions like "Should the character be aware of the map?" and <br>
"Should the map be aware of the character?" But I didn't like the answer to either question, because of course <br>
neither needs any real awareness of the other, since they never truly interact in this medium. That is why the game <br>
class exists.<br>

Another problem I encountered was the logic for displaying the game. If you've seen the function, I'm sure you can <br>
imagine why. The logic for turning a matrix of single characters into a matrix of three by three blocks of <br>
characters eluded me for quite a while. Then came the difficulty of ensure things were rendered at the center of <br>
those blocks. It's all math though, and I've always thought math was my strong suit, so this problem proved <br>
rather frustrating.<br>

Finally, and probably most infuriating, was ensuring I had functioning build pipelines for both Windows and macOS. <br>
this was probably the most annoying part of the project, when it shouldn't have been. I am too much of a <br>
perfectionist sometimes, and I was convinced I would never be happy unless I had working build scripts and <br>
functionality on both platforms. In the end, I got it figured out. I think.<br>

Bonus problem: Double-clicking the .exe. Why, oh why, does double-clicking the .exe run the program in a <br>
different directory from which it is located? I never really fixed this problem, that's why it's a bonus. Run the <br>
program with the batch file, and we'll be okay.<br>

## Conclusion

---

The overall project is probably a bit more complex than covered above. It was the culmination of weeks' worth of <br>
work, and certainly some details were lost along the way. Documentation for anything that was left out here is <br>
likely present within the source code files as comments, and should help to clear up any further questions about <br>
how the project functions.
