//
//	main.cpp
//	WordSearcher
//
//	Created by Brent Hronk on 2/9/14.
//	Copyright (c) 2014 Hronik. All rights reserved.
//

#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/regex.hpp>
#include "Storer.h"
#include "DirectoryProcessor.h"

int main(int argc, const char * argv[])
{
	if (argc < 2)
		return 0;

	unsigned int numWorkers = 3;
	unsigned int countToPrint = 10;
	boost::regex extRegex("\\.txt");
	boost::regex splitRegex("[^a-zA-Z0-9]+");

	DirectoryProcessor* directoryProcessor = new DirectoryProcessor(numWorkers, extRegex, splitRegex);
	if (NULL != directoryProcessor) {
		directoryProcessor->processDirectory(boost::filesystem::path(argv[1]));
		std::vector<std::pair<std::string, int> > sortedStore = directoryProcessor->getStorer().getSortedStore();
		for (int i = 0; i < countToPrint && i < sortedStore.size(); i++) {
			std::cout << sortedStore[i].first << ": " << sortedStore[i].second << std::endl;
		}
	}  

	return 0;
}
