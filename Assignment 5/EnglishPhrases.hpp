//
//  EnglishPhrases.hpp
//  Assignment 5
//
//  Created by main on 5/23/26.
//

#ifndef EnglishPhrases_hpp
#define EnglishPhrases_hpp

#include "Phrases.hpp"

class EnglishPhrases : public Phrases {
public:
    EnglishPhrases() : Phrases({
        {"kMISSING_DIRECTION", "Go where? Try a direction."},
        {"kUNKNOWN_COMMAND", "Try again buster. Maybe look at the help menu if you are that dumb..."},
        {"kINVALID_DIRECTION", "You can't go that way."},
        {"kFOUND_KEY_IN_CHEST", "Your pea-brain found a key inside the chest!"},
        {"kSUICIDE_LOST", "\nCongrats! You killed yourself and lost the game!"},
        {"kWON", "Somehow you escaped and won. I guess it was too easy for you..."},
        {"kMISSING_KEY_FOR_WIN", "What a loser, you think you can escape with less than 2 keys!"},
        {"kNO_EXIT", "There is no exit here."},
        {"kNO_KEYS_IN_ROOM", "No keys! Keep looking you hooligan!"},
        {"kKEY_IN_ROOM", "There is a key here."},
        {"kNO_CHEST_IN_ROOM", "There is no chest here."},
        {"kCHEST_IN_ROOM", "There is a locked chest here."},
        {"kNO_CHEST_FOUND", "There is no chest here."},
        {"kNOTHING_TO_USE_KEY_ON", "There's nothing here to use a key on you fool."},
    }) {}

    EnglishPhrases(Phrases& other) = delete;
    EnglishPhrases& operator=(Phrases& other) = delete;
};
#endif /* EnglishPhrases_hpp */
