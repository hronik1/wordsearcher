//
//  Storer.h
//  WordSearcher
//
//  Created by Brent Hronk on 2/9/14.
//  Copyright (c) 2014 Hronik. All rights reserved.
//

#ifndef __WordSearcher__Storer__
#define __WordSearcher__Storer__

#include <iostream>
#include <unordered_map>

class Storer {
    
    std::unordered_map<std::string, int> store;
    
public:
    void addKey(std::string key);
    void addStore(Storer &other);
    std::unordered_map<std::string, int>& getStore();
    //sort
};

void Storer::addKey(std::string key) {
    store[key]++;
}

void Storer::addStore(Storer &other) {
    for (auto& x: other.store) {
        store[x.first] += x.second;
    }
    
}

std::unordered_map<std::string, int>& Storer::getStore() {
    return this->store;
}
#endif /* defined(__WordSearcher__Storer__) */
