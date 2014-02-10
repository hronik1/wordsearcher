//
//  main.cpp
//  WordSearcher
//
//  Created by Brent Hronk on 2/9/14.
//  Copyright (c) 2014 Hronik. All rights reserved.
//

#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <boost/filesystem/path.hpp>
#include "Storer.h"
#include "DirectoryProcessor.h"

int main(int argc, const char * argv[])
{
    unsigned int numWorkers = 3;
    std::regex extRegex(".+\.txt\b");
    std::regex splitRegex("[^:alnum:]");
    DirectoryProcessor* directoryProcessor = new DirectoryProcessor(3, extRegex, splitRegex);
    if (NULL != directoryProcessor) {
        directoryProcessor->processDirectory(boost::filesystem::path(argv[1]));
        for (auto& x: directoryProcessor->getStorer().getStore()) {
            //std::cout << x.first << ": " << x.second << std::endl;
        }
        
    }
    
    
    return 0;
}
