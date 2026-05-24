//
//  GameUtils.hpp
//  Assignment 5
//
//  Created by Kyle Parker on 5/18/26.
//

#ifndef GameUtils_hpp
#define GameUtils_hpp

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

class GameUtils final {
public:
    GameUtils() = delete;
    GameUtils(GameUtils& other) = delete;

    static void clearScreen() {
        system("clear || cls");
    }

    static void waitForEnter(string message = "(Press enter to continue)", bool shouldClear = false) {
        cout << message << endl;
        string s;
        std::getline(cin, s); // Wait for return

        if (shouldClear) {
            clearScreen();
        }
    }

    static void waitAndClear(string message = "(Press enter to continue") {
        waitForEnter(message, true);
    }


    static string normalize(const string &s) {
        string normalized;
        normalized.reserve(s.size());

        for (char c : s) { normalized.push_back(static_cast<char>(std::tolower(c)));
        }

        return normalized;
    }
};

#endif // !GameUtils_hpp
