//
//  Room.hpp
//  Assignment 5
//
//  Created by Kyle Parker on 5/20/26.
//

#ifndef Room_hpp
#define Room_hpp

#include <string>
#include <map>
#include <ostream>

#include "EnglishPhrases.hpp"

class Room {
public:
    typedef std::map<std::string, std::string> ExitMap;

    // Default for implicit init calls
    Room() : roomName("N/A"), roomDesc("N/A"), roomExits({}), containsKey(false),
    containsChest(false), isDeathTrap(false) {}

    // Primary constructor
    Room(const std::string& name, const std::string& desc,
         const ExitMap& exits,
         bool containsKey, bool containsChest,
         bool isDeathTrap) :
    roomName(name), roomDesc(desc), roomExits(exits), containsKey(containsKey),
    containsChest(containsChest), isDeathTrap(isDeathTrap),
    phrases(new EnglishPhrases()){}

    const std::string& name() const {
        return this->roomName;
    }

    const std::string& description() const {
        return this->roomDesc;
    }

    const std::map<std::string, std::string>& exits() const {
        return this->roomExits;
    }

    bool hasKey() const {
        return this->containsKey;
    }

    bool hasChest() const {
        return this->containsChest;
    }

    bool isTrap() const {
        return this->isDeathTrap;
    }

    const std::string getKeyDescription() const {
        if (this->hasKey()) {
            return phrases->getPhrase("kKEY_IN_ROOM");
        } else {
            return phrases->getPhrase("kNO_KEYS_IN_ROOM");
        }
    }

    const std::string getChestDescription() const {
        if (this->hasChest()) {
            return phrases->getPhrase("kCHEST_IN_ROOM");
        } else {
            return phrases->getPhrase("kNO_CHEST_IN_ROOM");
        }
    }

    std::ostream& appendExitsDescription(std::ostream& stream) const {
        if (!this->exits().empty()) {
            stream << "Exits:";

            for (const auto& p : this->exits()) {
                stream << " " << p.first;
            }

            stream << "\n";
        } else {
            stream << "No visible exits.\n";
        }

        return stream;
    }

    void removeKey() {
        this->containsKey = false;
    }

    void removeChest() {
        this->containsChest = false;
    }

// Last this time due to use of ExitMap
private:
    std::string roomName;
    std::string roomDesc;
    ExitMap roomExits; // direction -> room id
    bool containsKey;
    bool containsChest;
    bool isDeathTrap;

    Phrases * phrases;
};

#endif /* Room_hpp */
