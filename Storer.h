//
//	Storer.h
//	WordSearcher
//
//	Created by Brent Hronk on 2/9/14.
//	Copyright (c) 2014 Hronik. All rights reserved.
//

#ifndef __WordSearcher__Storer__
#define __WordSearcher__Storer__

#include <unordered_map>
#include <vector>
#include <algorithm>

bool reverseSort(std::pair<std::string, int> first, std::pair<std::string, int> second);

bool reverseSort(std::pair<std::string, int> first, std::pair<std::string, int> second) {
	//Sorting function to sort map by decreasing value
	return first.second > second.second;
}

class Storer {

	std::unordered_map<std::string, int> store;
	std::vector<std::pair<std::string, int> > sortedStore; 

public:
	void addKey(std::string key);
	void addStore(Storer &other);
	std::vector<std::pair<std::string, int> >& getSortedStore();
};

void Storer::addKey(std::string key) {
	//Lower cases key and adds it to the map
	for (int i = 0; i < key.size(); i++)
		key[i] = tolower(key[i]);
	store[key]++;
}

void Storer::addStore(Storer &other) {
	//Adds all keys in the input storer reference
	for (auto& x: other.store) {
		store[x.first] += x.second;
	}
	
}

std::vector<std::pair<std::string, int> >& Storer::getSortedStore() {
	//Returns the words in order of decreasing count
	sortedStore = std::vector<std::pair<std::string, int> >(store.begin(), store.end());
	std::sort(sortedStore.begin(), sortedStore.end(), reverseSort);
	return sortedStore;
}

#endif /* defined(__WordSearcher__Storer__) */
