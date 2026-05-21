//
//  Game.hpp
//  Assignment 2
//
//  Created by main on 5/18/26.
//

#ifndef Game_hpp
#define Game_hpp

#include <string>
#include <map>

using std::string;
using std::map;

struct Room {
    string name;
    string description;
    map<string, string> exits; // direction -> room id
    bool hasKey;
    bool hasChest;
    bool isTrap;
};

typedef map<string, Room> RoomMap;

void initGame(RoomMap& rooms, int& keysCollected, bool& isRunning, bool& playerAlive, string& currentRoomId);
void processCommand(const string& raw, RoomMap &rooms, int &keysCollected, bool &isRunning, bool &playerAlive, string &currentRoomId);
void movePlayer(const string& direction, RoomMap& rooms, const int& keysCollected, bool& isRunning, bool& playerAlive, string& currentRoomId);
void takeKey(RoomMap& rooms, int& keysCollected, const string& currentRoomId);
void openChest(RoomMap& rooms, int& keysCollected, const string& currentRoomId);
void useKey(const int& keysCollected, const string& currentRoomId, bool& isRunning);
void tryExit(const int &keysCollected, const string &currentRoomId, bool &isRunning);

void describeCurrentRoom(const Room& room, const int& keysCollected);
bool isGameValid(const bool& isRunning, const bool& playerAlive);


#endif /* Game_hpp */
