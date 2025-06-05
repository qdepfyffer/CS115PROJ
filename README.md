
## What's in this folder?

---
### Main CSCI115_PROJ folder:

This folder holds the subfolders, the CMakeLists.txt, and the build scripts to allow for easier building for those <br>
who don't want to use the command line to build the program. It also holds the report on the project and this <br>
readme file.

---

### CSCI115_PROJ subfolders:

#### ./build:

This folder holds the build files for the program. It contains all cmake output along with the compiled .exe that <br>
runs the program. There are two subfolders, ./build/unix and ./build/win, which hold files for their respective <br>
systems. The aforementioned build scripts ensure that the CMake output ends up in the proper directory, so that <br>
the program can be run on macOS or Windows by utilizing the .exe contained in the respective subfolder. Each of the <br>
subfolders also holds a copy of the ./maps folder, so that the program can locate the maps to be loaded easier. <br>
Annoyingly, when you double-click a .exe, it isn't always run in the folder you think it will be run in. This is <br>
remedied by running the program through the command line or with a script that does that for you. <br>
That script is in the main project folder, named either runWin.bat or runUnix.sh

#### ./maps:

This folder holds a collection of .txt files that are to be turned into maps for the program to display. They <br>
contain some information about the map and the player agent, along with what is essentially a matrix of characters <br>
to be turned into the map. This folder is copied to each ./build subfolder so that the relative path used to open <br>
the maps works when the program is run from its subfolder.

#### ./src:

This is where all the source code for the program is. There's also some stuff that ultimately was not used in the <br>
program, but is there because this was my original home folder for other projects this semester, and I was too lazy <br>
to remove them. There are also a few artifacts from earlier builds of this program that were cut before I reached <br>
the current product. Contained files are as follows:

---

### Source Code Files:

For most, if not all, classes / structs, documentation is in the .cpp file for the given object. More general <br>
overviews of how the project functions is available in the REPORT.md file, also contained in the main folder. <br>

**Agent.cpp / .h:** The agent class definitions / interface. This class represents the agents or characters in the <br>
simulation.<br>

**Array.cpp / .h:** The array class definitions / interface. This class is a simple dynamic array that stores <br>
integers. It is (most likely) unused in the overall project, but I'm leaving it here because otherwise I'll forget <br>
it exists.<br>

**Game.cpp / .h:** The game class definitions / interface. This class does all the heavy lifting in terms of <br>
displaying things and allowing the agents to navigate the map / graph. The functions here got pretty heavy after a <br>
while, so maybe I'll refactor things over the summer.<br>

**GameMap.cpp / .h:** The game's map class definitions / interface. This class is essentially a matrix, with <br>
functionality trimmed down to be of use in this specific program. That's really all it is. Aside from that, it <br>
contains the function that builds the graph based on the data in the matrix.<br>

**GraphAL.cpp / .h:** The adjacency list graph class definitions / interface. This class was the one I settled on <br>
to represent the graph used in navigation. The navigation graphs end up being quite sparse, so an adjacency list <br>
ends up being a better fit here.

**GraphAM.cpp / .h:** The adjacency matrix graph class definitions / interface. This was going to be included as <br>
an option in order to demonstrate the parity between the two, but it just didn't make sense to support both <br>
implementations when the adjacency list implementation was so much better. The entirety of the navigation data <br>
could not even be displayed in the console with this implementation due to the sheer size of the navigation graph <br>
for larger maps.

**main.cpp / .h:** The main c++ file. This doesn't really do anything besides MAYBE setting the current working <br>
directory to allow for double-clicking the .exe file instead of running it with a script or through the command <br>
line. If it doesn't do that, it's because I ran out of time and figured there were more important aspects of the <br>
project to focus on.

**Matrix.cpp / .h:** The matrix class definitions / interface. This class isn't really used in the program. It was <br>
going to replace the raw matrix contained in the game map class, but that introduced needless complexity, so really <br>
this class served mostly as a template for the functions I would need to build for the game map class. <br>

**NodeAL.cpp / .h:** The node class for the adjacency list graph definitions / interface. This is just a node for <br>
the adjacency list graph implementation. It doesn't do much besides provide a constructor.<br>

**PriorityQueue.cpp / .h:** The priority queue class definitions / interface. This class is a priority queue <br>
implemented as a min-heap. It's used in Dijkstra's shortest path algorithm, and that is about it. <br>

**Queue.cpp / .h:** The queue class definitions / interface. This class ended up being used in a few places I didn't<br>
expect. Specifically, in the enemy placing logic. It provided a really simple solution to a problem I had with that <br>
part of the game logic. I know the enemies weren't necessary apparently, but oh well.

**Tile.cpp / .h:** The tile class definitions / interface. This class didn't end up being used. At one point, I felt<br>
the need to represent each square on the map as its own object. This proved to be a rather silly solution, though it<br>
made certain aspects of the project easier when I was using it. I eventually decided I could make things much more <br>
lightweight by using a single character in the place of this entire class, at the cost of more complicated <br>
operations further down the line. <br>

---

### Miscellaneous:

**build.bat:** A windows batch script to allow for easier building of the project. Not necessary if you know how to <br>
do it through the command line, but this ensures things don't go awry (usually). <br>

**build.sh:** A unix shell script to allow for easier building of the project. Not necessary if you know how to do <br>
it through the command line, but this ensures things don't go awry (usually). <br>

**CMakeLists.txt:** The CMakeLists.txt file for the project. I'm not sure what else to say about it. It lets me use <br>
CMake to build the project much more comfortably and lets me do some fun stuff that would otherwise be more <br>
annoying using other build pipelines. <br>

**README.md:** This file you're currently reading. That is all.

**REPORT.md:** A report on the project.