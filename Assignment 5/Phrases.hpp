//
//  Phrases.hpp
//  Assignment 5
//
//  Created by Kyle Parker on 5/20/26.
//

#ifndef Phrases_hpp
#define Phrases_hpp

#include <map>
#include <string>
#include <optional>

// Purpose: Mock strings language support file
// Singleton pattern
class Phrases {
    std::map<std::string, std::string> phrases;

protected:
    explicit Phrases(std::map<std::string, std::string> phraseMap): phrases(std::move(phraseMap)) {}

public:

    Phrases(Phrases& other) = delete;
    Phrases& operator=(Phrases& other) = delete;

    std::string getPhrase(const std::string& id) {
        auto iter = phrases.find(id);
        if (iter != phrases.end()) {
            return iter->second;
        } else {
            return id;
        }
    }
};

#endif /* Phrases_hpp */
