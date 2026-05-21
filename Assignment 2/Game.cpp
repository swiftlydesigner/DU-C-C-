//
//  Game.cpp
//  Assignment 2
//
//  Created by main on 5/18/26.
//

#include "Game.hpp"
#include "GameUtils.hpp"

#include <algorithm>
#include <sstream>

static map<string, string> phrases = {
    {"missingDir", "Go where? Try a direction.\n"},
    {"invalidCmd", "Invalid command. Try again.\n"},
    {"invalidDir", "You can't go that way.\n"},
    {"foundKeyInChest", "Your pea-brain found a key inside the chest!"},
    {"killedLost", "\nCongrats! You killed yourself and lost the game!\n"},
    {"won", "Somehow you escaped and won. I guess it was too easy for you...\n"},
    {"missingKey", "What a loser, you think you can escape with less than 2 keys!\n"},
    {"noexit", "There is no exit here.\n"}
};

void initGame(RoomMap& rooms, int& keysCollected, bool& isRunning, bool& playerAlive, string& currentRoomId) {
    rooms.clear();
    keysCollected = 0;
    isRunning = true;
    playerAlive = true;

    Room cell;
    cell.name = "Cell";
    cell.description = "A small, damp cell. There's a loose stone in the wall.";
    cell.exits = { {"north", "hallway"} };
    cell.hasKey = false;
    cell.hasChest = false;
    cell.isTrap = false;
    rooms["cell"] = cell;

    Room hallway;
    hallway.name = "Hallway";
    hallway.description = "A narrow hallway with torches. You see doors to east and west.";
    hallway.exits = { {"south", "cell"}, {"east", "armory"}, {"west", "trap"}, {"north", "treasure"} };
    hallway.hasKey = true;
    hallway.hasChest = false;
    hallway.isTrap = false;
    rooms["hallway"] = hallway;

    Room armory;
    armory.name = "Armory";
    armory.description = "Old weapons and a locked chest sit here.";
    armory.exits = { {"west", "hallway"} };
    armory.hasKey = false;
    armory.hasChest = true;
    armory.isTrap = false;
    rooms["armory"] = armory;

    Room trap;
    trap.name = "Trap Room";
    trap.description = "As you enter, you feel the floor give way. It's a trap!";
    trap.exits = {};
    trap.hasKey = false;
    trap.hasChest = false;
    trap.isTrap = true;
    rooms["trap"] = trap;

    Room treasure;
    treasure.name = "Treasure Room";
    treasure.description = "A glittering room with a locked exit door. You need 2 keys to leave.";
    treasure.exits = { {"south", "hallway"}, {"exit", "outside"} };
    treasure.hasKey = false;
    treasure.hasChest = false;
    treasure.isTrap = false;
    rooms["treasure"] = treasure;

    Room outside;
    outside.name = "Outside";
    outside.description = "Bright sunlight. Freedom!";
    outside.exits = {};
    outside.hasKey = false;
    outside.hasChest = false;
    outside.isTrap = false;
    rooms["outside"] = outside;

    currentRoomId = "cell";
}

void processCommand(const string& raw, RoomMap &rooms, int &keysCollected, bool &isRunning, bool &playerAlive, string &currentRoomId) {
    string cmd = normalize(raw);

    // Seperate at spaces (easy way)
    std::istringstream iss(cmd);

    string action;
    iss >> action;

    if (action == "go") {
        string direction;
        iss >> direction;
        if (direction.empty()) {
            cout << phrases["missingDir"];
            return;
        }
        movePlayer(direction, rooms, keysCollected, isRunning, playerAlive, currentRoomId);
    } else if (action == "look") {
        describeCurrentRoom(rooms[currentRoomId], keysCollected);
    } else if (action == "take") {
        string what;
        iss >> what;
        if (what == "key") {
            takeKey(rooms, keysCollected, currentRoomId);
        }
    } else if (action == "open") {
        string obj;
        iss >> obj;

        if (obj == "chest") {
            openChest(rooms, keysCollected, currentRoomId);
            return;
        }
    } else if (action == "use") {
        string obj;
        iss >> obj;
        if (obj == "key") {
            useKey(keysCollected, currentRoomId, isRunning);
            return;
        }
    } else if (action == "exit") {
        tryExit(keysCollected, currentRoomId, isRunning);
    } else if (action == "inventory" || action == "inv") {

        if (keysCollected == 0) {
            cout << "Inventory: (no keys)\n";
        } else {
            cout << "Keys: " << keysCollected << "\n";
        }

    } else if (action == "help") {
        displayHelpMenu();
    } else if (action == "quit" || action == "q") {
        cout << "Quitting.\n";
        isRunning = false;
    } else {
        cout << phrases["invalidCmd"];
    }
}

void movePlayer(const string& direction, RoomMap& rooms, const int& keysCollected, bool& isRunning, bool& playerAlive, string& currentRoomId) {
    Room& room = rooms[currentRoomId];

    auto directionIt = room.exits.find(direction);

    if (directionIt == room.exits.end()) {
        cout << phrases["invalidDir"];
        return;
    }

    currentRoomId = directionIt->second;

    const Room& newRoom = rooms.at(currentRoomId);

    if (newRoom.isTrap) {
        cout << newRoom.description << phrases["killedLost"];

        playerAlive = false;
        isRunning = false;
    } else if (currentRoomId == "outside") {

        if (keysCollected >= 2) {
            cout << phrases["won"];
            isRunning = false;
        } else {
            cout << phrases["missingKey"];
            currentRoomId = "treasure";
        }

    } else {
        describeCurrentRoom(newRoom, keysCollected);
    }

}

void takeKey(RoomMap& rooms, int& keysCollected, const string& currentRoomId) {
    Room& room = rooms[currentRoomId];

    if (room.hasKey) {
        room.hasKey = false;
        ++keysCollected;
        cout << "You take the key. Keys collected: " << keysCollected << "\n";
    } else {
        cout << "There is no key here.\n";
    }
}

void openChest(RoomMap& rooms, int& keysCollected, const string& currentRoomId) {
    Room& room = rooms[currentRoomId];

    if (!room.hasChest) {
        cout << "There is no chest here.\n";
        return;
    }

    room.hasChest = false;

    ++keysCollected;
    cout << phrases["foundKeyInChest"] <<"Keys collected: " << keysCollected << "\n";
}

void useKey(const int& keysCollected, const string& currentRoomId, bool& isRunning) {
    if (keysCollected == 0) {
        cout << "You have no keys to use.\n";
        return;
    }
    
    if (currentRoomId == "treasure") {
        if (keysCollected >= 2) {
            cout << phrases["won"];
            isRunning = false;
        } else {
            cout << phrases["missingKey"];
        }
    } else {
        cout << "There's nothing here to use a key on.\n";
    }
}

void tryExit(const int &keysCollected, const string &currentRoomId, bool &isRunning) {
    if (currentRoomId == "treasure")
    {
        useKey(keysCollected, currentRoomId, isRunning);
    } else {
        cout << phrases["noExit"];
    }
}

void describeCurrentRoom(const Room& room, const int& keysCollected) {

    cout << "You are in the " << room.name << ".\n";
    cout << room.description << "\n";

    if (room.hasKey) {
        cout << "There is a key here.\n";
    }

    if (room.hasChest) {
        cout << "There is a locked chest here.\n";
    }

    if (!room.exits.empty()) {
        cout << "Exits:";

        for (const auto& p : room.exits) {
            cout << " " << p.first;
        }

        cout << "\n";
    } else {
        cout << "No visible exits.\n";
    }
    cout << "Keys collected: " << keysCollected << "\n";
}

bool isGameValid(const bool& isRunning, const bool& playerAlive) {
    return isRunning && playerAlive;
}
