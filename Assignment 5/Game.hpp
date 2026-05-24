//
//  Game.hpp
//  Assignment 5
//
//  Created by Kyle Parker on 5/18/26.
//

#ifndef Game_hpp
#define Game_hpp

#include <string>
#include <map>

#include "Room.hpp"
#include "Player.hpp"

using std::string;
using std::map;

typedef map<string, Room> RoomMap;

class Game {
    RoomMap rooms;
    Player player;
    Phrases * phrases;
    bool isRunning;

    // Setup function
    void configureRooms(const RoomMap& rooms);
    void configurePlayer();

    // Command Handlers
    void goCommand(std::istringstream& iss);
    void takeCommand(std::istringstream& iss);
    void openCommand(std::istringstream& iss);
    void useCommand(std::istringstream& iss);
    void inventoryCommand();
    void quitCommand();
    void unknownCommand();

    // Command Helpers
    void takeKey();
    void useKey();
    void openChest();
    void tryExit();

    // Game play
    void processCommand(const string& raw);
    void movePlayer(const string& direction);

    // Info
    void describeCurrentRoom();
    void displayWelcomeMessage();
    void displayInstructions();
    void displayHelpMenu();
    void displayGameWon(bool didWin);


    bool isValid();
    bool didUserWin();

public:
    Game(const RoomMap& rooms = {});
    ~Game();

    // Public handles
    void run();
    void kill();
};

#endif /* Game_hpp */
