#include <iostream>
#include <chrono>
#include <filesystem>

#include "GraphAM.h"
#include "GraphAL.h"
#include "GameMap.h"
#include "Game.h"


int main(int argc, char *argv[]) {

    if (argc > 0) {
        std::filesystem::path exe = std::filesystem::absolute(argv[0]);
        std::filesystem::current_path(exe.parent_path());
    }

    std::cout << "Current working directory is " << std::filesystem::current_path() << std::endl;

    std::cout << "Available maps:\n";
    int i = 0;
    for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator("./maps")) {
        if (entry.path().filename().string()[0] != '.') {
            std::cout << i << ": " << entry.path().filename() <<std::endl;
            i++;
        }
    }

    int j;
    std::cout << "Please enter the number next to the map you would like to use: ";
    std::cin >> j;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int k = 0;
    std::filesystem::path chosenPath;
    for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator("./maps")) {
        if (entry.path().filename().native()[0] != '.') {
            if (k == j) {
                chosenPath = entry.path();
            }
            k++;
        }
    }

    if (!chosenPath.empty()) {
        Game *newGame = new Game(chosenPath.string().c_str()); // Change the filename before building to change the map.
        newGame->play();
    }
    else {
        std::cout << "Invalid map selection.\n";
    }
}