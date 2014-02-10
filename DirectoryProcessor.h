//
//  DirectoryProcessor.h
//  WordSearcher
//
//  Created by Brent Hronk on 2/9/14.
//  Copyright (c) 2014 Hronik. All rights reserved.
//

#ifndef __WordSearcher__DirectoryProcessor__
#define __WordSearcher__DirectoryProcessor__

#include <iostream>
#include <regex>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include "Storer.h"
#include "FileProcessor.h"

class DirectoryProcessor {
    
    std::vector<FileProcessor*> fileProcessors;
    Storer storer;
    unsigned int currentProcessor;
    unsigned int numberWorkerProcessors;
    void processDirectoryHelper(boost::filesystem::path p);
    void processFile(boost::filesystem::path p);
    
public:
    DirectoryProcessor(unsigned int numberWorkerProcessors, std::regex filesExtsToProcess, std::regex splitRegex);
    ~DirectoryProcessor();
    void processDirectory(boost::filesystem::path p);
    Storer& getStorer();
};

DirectoryProcessor::DirectoryProcessor(unsigned int numberWorkerProcessors, std::regex extRegex, std::regex splitRegex) {
    for (int i = 0; i < numberWorkerProcessors; i++)
        fileProcessors.push_back(new FileProcessor(extRegex, splitRegex));
    currentProcessor = 0;
    this->numberWorkerProcessors = numberWorkerProcessors;
    std::cout << "initied DP" << std::endl;
}

void DirectoryProcessor::processDirectory(boost::filesystem::path p) {
      std::cout << "processing directories" << std::endl;
    processDirectoryHelper(p);
    std::cout << "processed directories" << std::endl;
    for (auto& x: fileProcessors) {
        storer.addStore(x->getStorer());
    }
}

Storer& DirectoryProcessor::getStorer() {
    return storer;
}

void DirectoryProcessor::processDirectoryHelper(boost::filesystem::path p) {
    using namespace boost::filesystem;
    std::cout << "processed directory helper" << std::endl;
    std::cout << p.string() << std::endl;
    if (!exists(p)) {
        std::cout << "does not exist" << std::endl;
        return;
    }
      std::cout << "checked that it exists" << std::endl;
    if (is_regular_file(p)) {
        processFile(p);
    } else if (is_directory(p)) {
          std::cout << "is directory" << std::endl;
        boost::filesystem::directory_iterator end_iter;
        for (boost::filesystem::directory_iterator dir_itr(p);
             dir_itr != end_iter;
             ++dir_itr) {
            processDirectoryHelper(dir_itr->path());
        }
    }
}

void DirectoryProcessor::processFile(boost::filesystem::path p) {
    fileProcessors[currentProcessor]->addFileToBeProcessed(p);
    currentProcessor = (currentProcessor+1)%numberWorkerProcessors;
}

#endif /* defined(__WordSearcher__DirectoryProcessor__) */
