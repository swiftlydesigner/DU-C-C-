//
//  GameUtils.hpp
//  Assignment 2
//
//  Created by main on 5/18/26.
//

#ifndef GameUtils_hpp
#define GameUtils_hpp

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

inline void clearScreen() {
    system("clear || cls");
}

inline void waitForEnter(string message = "(Press enter to continue)", bool shouldClear = false) {
    cout << message << endl;
    string s;
    std::getline(cin, s); // Wait for return

    if (shouldClear) {
        clearScreen();
    }
}

inline void waitAndClear(string message = "(Press enter to continue") {
    waitForEnter(message, true);
}

inline void displayWelcomeMessage() {
    clearScreen();
    cout << "Welcome to Text Based Game!\n\n" << endl;
}

inline void displayInstructions() {
    clearScreen();
    cout << "Instructions:\n"
    << "- Commands are case-insensitive.\n"
    << "- Movement: go north / go south / go east / go west\n"
    << "- Inspection: look\n"
    << "- Interaction: take key, open chest, use key, exit\n"
    << "- Goal: collect 2 keys, find the Treasure Room, and exit.\n"
    << endl;

    waitForEnter("Press Enter to start...");
}

inline void displayHelpMenu() {
    cout << "Valid commands: go <direction>, look, take key, open chest, use key, inventory, exit, quit\n";
}


inline string normalize(const string &s) {
    string normalized;
    normalized.reserve(s.size());

    for (char c : s) { normalized.push_back(static_cast<char>(std::tolower(c)));
    }

    return normalized;
}

#endif // !GameUtils_hpp
