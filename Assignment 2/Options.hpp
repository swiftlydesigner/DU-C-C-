//
//  Options.hpp
//  Assignment 2
//
//  Created by main on 5/18/26.
//

#ifndef Options_hpp
#define Options_hpp

#include <pair>
#include <make_pair>
#include <vector>
#include <string>
using std::string;
using std::vector;
using std::pair;
using std::make_pair;

class Options {
    vector<pair> _options;

public:
    Options() {
        self._options = {};
    }

    void addOption(char hotKey, string desc) {
        self.options.push_front(make_pair(hotKey, desc));
    }

    print();
};

#endif /* Options_hpp */
