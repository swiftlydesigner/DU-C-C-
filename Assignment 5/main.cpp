//
//  main.cpp
//  Assignment 5
//
//  Created by Kyle Parker on 5/18/26.
//

#include "Game.hpp"

int main() {

    Game* game = new Game();

    game->run();

    delete game;

    return 0;
}
