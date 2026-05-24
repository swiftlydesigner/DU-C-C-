//
//  Player.hpp
//  Assignment 5
//
//  Created by Kyle Parker on 5/20/26.
//

#ifndef Player_hpp
#define Player_hpp

#include <string>

class Player {
    std::string currentRoomId;
    int keysCollected;
    bool alive;

public:

    // MARK: Public constructor
    Player() : currentRoomId("N/A"), keysCollected(0), alive(true) {}

    // MARK: Public setters/getters
    void setCurrentRoom(const std::string& roomId) {
        currentRoomId = roomId;
    }

    const std::string& currentRoom() const {
        return currentRoomId;
    }

    void addKey() {
        ++keysCollected;
    }

    void setKeys(int count) {
        keysCollected = count;
    }

    const int& getKeys() const {
        return keysCollected;
    }

    void die() {
        alive = false;
    }

    void revive() {
        alive = true;
    }
    
    bool isAlive() const {
        return alive;
    }
};


#endif /* Player_hpp */
