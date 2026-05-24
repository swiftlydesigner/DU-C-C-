//
//  Game.cpp
//  Assignment 5
//
//  Created by Kyle Parker on 5/18/26.
//

#include <algorithm>
#include <sstream>

#include "Game.hpp"
#include "GameUtils.hpp"
#include "EnglishPhrases.hpp"

// MARK: - Private

// MARK: Configuration
void Game::configureRooms(const RoomMap& rooms) {
    if (rooms.size() == 0) {

        Room::ExitMap emptyMap;

        Room cell("Cell", "A small, damp cell. There's a loose stone in the wall.",
                  { {"north", "hallway"} },
                  false, false, false);
        this->rooms["cell"] = cell;

        Room hallway("Hallway", "A narrow hallway with torches. There is a small brass key on the floor.",
                     { {"south", "cell"}, {"east", "armory"}, {"west", "storeroom"}, {"north", "antechamber"} },
                     true, false, false);
        this->rooms["hallway"] = hallway;

        Room storeroom("Storeroom", "Dusty crates and barrels. Something clinks in the corner — could be useful.",
                       { {"east", "hallway"}, {"north", "library"} },
                       false, false, false);
        this->rooms["storeroom"] = storeroom;

        Room library("Library", "Shelves of mouldering books. A ladder leads up to a narrow balcony.",
                     { {"south", "storeroom"}, {"up", "balcony"} },
                     false, false, false);
        this->rooms["library"] = library;

        Room balcony("Balcony", "A narrow wooden balcony overlooking the hallway. A loose beam looks like it can be removed.",
                     { {"down", "library"}, {"east", "alcove"} },
                     false, false, false);
        this->rooms["balcony"] = balcony;

        Room alcove("Alcove", "A small alcove with a mosaic on the floor. The pattern seems to indicate a safe step sequence.",
                    { {"west", "balcony"}, {"north", "puzzle"} },
                    false, false, false);
        this->rooms["alcove"] = alcove;

        Room puzzle("Puzzle Chamber", "Four pedestals stand in a square, each with a different emblem. Pressing them in the right order might open something.",
                    { {"south", "alcove"}, {"east", "treasure"} },
                    false, false, false);
        this->rooms["puzzle"] = puzzle;

        Room armory("Armory", "Old weapons and a locked chest sit here. The chest contains an iron key.",
                    { {"west", "hallway"}, {"north", "guardpost"} },
                    true, true, false);
        this->rooms["armory"] = armory;

        Room guardPost("Guard Post", "Broken spear racks and a sleeping guard (now gone). A reinforced door to the north is barred from the other side.",
                       { {"south", "armory"}, {"north", "vault"} },
                       false, false, false);
        this->rooms["guardpost"] = guardPost;

        Room vault("Vault", "A heavily secured vault room. The chest here is large and ornate — but it requires a heavy key (found elsewhere).",
                   { {"south", "guardpost"}, {"up", "oubliette"} },
                   false, true, false);
        this->rooms["vault"] = vault;

        Room antechamber("Antechamber", "A small room before the treasure room with two plinths. Both must hold keys to unlock the treasure room's exit.",
                         { {"south", "hallway"}, {"north", "treasure"} },
                         false, false, false);
        this->rooms["antechamber"] = antechamber;

        Room treasure("Treasure Room", "A glittering room with a locked exit door. You need 2 keys here to leave.",
                      { {"south", "antechamber"}, {"east", "secretlibrary"} },
                      false, false, false);
        this->rooms["treasure"] = treasure;

        Room secretLibrary("Secret Library", "A cramped secret study. On a pedestal rests a weathered map showing a hidden passage from the treasure room to the outside.",
                           { {"west", "treasure"}, {"south", "trapdoor"} },
                           false, false, false);
        this->rooms["secretlibrary"] = secretLibrary;

        Room trapdoor("Hidden Trapdoor", "A cramped space beneath the floor. You notice scratch marks like something crawled through recently.",
                      { {"north", "secretlibrary"}, {"east", "sewer"} },
                      false, false, false);
        this->rooms["trapdoor"] = trapdoor;

        Room sewer("Sewer", "A foul-smelling tunnel with shallow water. A rusted grate leads up toward the outside.",
                   { {"west", "trapdoor"}, {"up", "outside"} },
                   false, false, false);
        this->rooms["sewer"] = sewer;

        Room outside("Outside", "Bright sunlight. Freedom!",
                     { {"down", "sewer"} },
                     false, false, false);
        this->rooms["outside"] = outside;

        Room trap("Trap Room", "As you enter, the floor gives way. You've fallen into a deadly pit.",
                  { },
                  false, false, true);
        this->rooms["trap"] = trap;

        Room oubliette("Oubliette", "A dark pit with damp walls. Graffiti reads: 'One who takes the left path will never leave.'",
                       { },
                       false, false, true);
        this->rooms["oubliette"] = oubliette;

    } else {
        this->rooms = rooms;
    }
}

void Game::configurePlayer() {
    this->player.setKeys(0);
    this->player.setCurrentRoom("cell");
    this->player.revive();
}

// MARK: Command Handlers

void Game::goCommand(std::istringstream& iss) {
    string direction;
    iss >> direction;
    if (direction.empty()) {
        cout << phrases->getPhrase("kMISSING_DIRECTION") << endl;
        return;
    }
    this->movePlayer(direction);
}

void Game::takeCommand(std::istringstream& iss) {
    string what;
    iss >> what;

    if (what == "key") {
        this->takeKey();
    }
}

void Game::openCommand(std::istringstream& iss) {
    string obj;
    iss >> obj;

    if (obj == "chest") {
        this->openChest();
        return;
    }
}

void Game::useCommand(std::istringstream& iss) {
    string obj;
    iss >> obj;

    if (obj == "key") {
        this->useKey();
    }
}

void Game::inventoryCommand() {
    if (player.getKeys() == 0) {
        cout << "Inventory: (no keys)\n";
    } else {
        cout << "Keys: " << player.getKeys() << "\n";
    }
}

void Game::quitCommand() {
    cout << "Quitting.\n";
    isRunning = false;
}

void Game::unknownCommand() {
    cout << phrases->getPhrase("kUNKNOWN_COMMAND") << endl;
}

// MARK: Command Helpers
void Game::movePlayer(const string& direction) {
    Room& room = rooms[player.currentRoom()];

    auto directionIt = room.exits().find(direction);

    if (directionIt == room.exits().end()) {
        cout << phrases->getPhrase("kINVALID_DIRECTION") << endl;
        return;
    }

    player.setCurrentRoom(directionIt->second);

    const Room& newRoom = rooms.at(player.currentRoom());

    if (newRoom.isTrap()) {
        cout << newRoom.description() << phrases->getPhrase("kSUICIDE_LOST") << endl;

        player.die();
        isRunning = false;
    } else if (player.currentRoom() == "outside") {

        bool userWon = this->didUserWin();

        this->displayGameWon(userWon);

        if (!userWon) {
            player.setCurrentRoom("treasure");
        }

    } else {
        this->describeCurrentRoom();
    }
}

void Game::takeKey() {
    Room& room = rooms[player.currentRoom()];

    if (room.hasKey()) {
        room.removeKey();
        player.addKey();
        cout << "You take the key. Keys collected: " << player.getKeys() << "\n";
    } else {
        cout << "There is no key here.\n";
    }
}

void Game::useKey() {
    if (player.getKeys() == 0) {
        cout << "You have no keys to use.\n";
        return;
    }

    if (player.currentRoom() == "treasure") {
        this->displayGameWon(this->didUserWin());
    } else {
        cout << phrases->getPhrase("kNOTHING_TO_USE_KEY_ON") << endl;
    }
}

void Game::openChest() {
    Room& room = rooms[player.currentRoom()];

    if (!room.hasChest()) {
        cout << phrases->getPhrase("kNO_CHEST_FOUND") << endl;
        return;
    }

    room.removeChest();

    player.addKey();
    cout << phrases->getPhrase("kFOUND_KEY_IN_CHEST") <<" Keys collected: " << player.getKeys() << endl;
}

void Game::tryExit() {
    if (player.currentRoom() == "treasure")
    {
        this->useKey();
    } else {
        cout << phrases->getPhrase("kNO_EXIT") << endl;
    }
}

// MARK: Game play
void Game::processCommand(const string& raw) {
    string cmd = GameUtils::normalize(raw);

    // Seperate at spaces (easy way)
    std::istringstream iss(cmd);

    string action;
    iss >> action;

    if (action == "go") {
        this->goCommand(iss);
    } else if (action == "look") {
        this->describeCurrentRoom();
    } else if (action == "take") {
        this->takeCommand(iss);
    } else if (action == "open") {
        this->openCommand(iss);
    } else if (action == "use") {
        this->useCommand(iss);
    } else if (action == "exit") {
        this->tryExit();
    } else if (action == "inventory" || action == "inv") {
        this->inventoryCommand();
    } else if (action == "help" || action == "h") {
        this->displayHelpMenu();
    } else if (action == "quit" || action == "q") {
        this->quitCommand();
    } else {
        this->unknownCommand();
    }
}


void Game::describeCurrentRoom() {
    const Room& room = rooms[player.currentRoom()];

    cout << "You are in the " << room.name() << ".\n";
    cout << room.description() << "\n";
    cout << room.getKeyDescription() << "\n";
    cout << room.getChestDescription() << "\n";

    room.appendExitsDescription(cout);

    cout << "Keys collected: " << player.getKeys() << "\n";
}

void Game::displayWelcomeMessage() {
    GameUtils::clearScreen();
    cout << "Welcome to Text Based Game!\n\n" << endl;
}

void Game::displayInstructions() {
    GameUtils::clearScreen();

    cout << "Instructions:\n"
    << "- Commands are case-insensitive.\n"
    << "- Movement: go north / go south / go east / go west / go up / up down\n"
    << "- Inspection: look\n"
    << "- Interaction: take key, open chest, use key, exit\n"
    << "- Goal: collect 2 keys, find the Treasure Room, and exit.\n"
    << endl;

    GameUtils::waitForEnter("Press Enter to start...");
}

void Game::displayHelpMenu() {
    static int helpCount = 0;

    if (helpCount == 5) {
        cout << "Take a rest and make sure your little brain isnt going to blow up. This is your fifth time looking at the help menu.\n" << endl;
    } else if (helpCount == 10) {
        cout << "WARNING! IDIOT USING SYSTEM!\n" << endl;
    } else if (helpCount == 20) {
        cout << "This is sad. I would feel bad for you, but I am a system and I fortunately have more computing power and memory than you.\n" << endl;
    }

    cout << "Valid commands: go <direction>, look, take key, open chest, use key, inventory, exit, quit" << endl;

    ++helpCount;
}

void Game::displayGameWon(bool didWin) {
    if (didWin) {
        cout << phrases->getPhrase("kWON") << endl;
        isRunning = false;
    } else {
        cout << phrases->getPhrase("kMISSING_KEY_FOR_WIN") << endl;
    }
}

bool Game::isValid() {
    return isRunning && this->player.isAlive();
}

bool Game::didUserWin() {
    return player.getKeys() >= 2;
}

// MARK: - Public
// MARK: Constructor
Game::Game(const RoomMap& rooms) {

    this->configureRooms(rooms);

    this->configurePlayer();

    this->isRunning = true;

    this->phrases = new EnglishPhrases();
}

// MARK: Destructor
Game::~Game() {
    if (this->phrases) {
        delete this->phrases;
    }
}

// MARK: Public funcs
void Game::run() {
    displayWelcomeMessage();
    displayInstructions();

    while (this->isValid()) {
        std::cout << "\nEnter command:\n> ";
        std::string rawCmd;
        std::getline(std::cin, rawCmd);

        if (rawCmd.empty()) {
            std::cout << "Please enter a command (type 'help').\n";
            continue;
        }

        processCommand(rawCmd);
    }
}

void Game::kill() {
    this->isRunning = false;
}
