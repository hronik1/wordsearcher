//
//  FileProcessor.h
//  WordSearcher
//
//  Created by Brent Hronk on 2/9/14.
//  Copyright (c) 2014 Hronik. All rights reserved.
//

#ifndef __WordSearcher__FileProcessor__
#define __WordSearcher__FileProcessor__

#include <iostream>
#include <regex>
#include <boost/filesystem/path.hpp>
#include <queue>
#include <fstream>
#include <thread>
#include "Storer.h"

class FileProcessor {
    
    Storer storer;
    std::queue<boost::filesystem::path> fileQueue;
    std::regex extRegex;
    std::regex splitRegex;
    std::thread processingThread;
    volatile bool shouldExecute;
    void processQueue();
    void processFile(boost::filesystem::path p);
    void processLine(std::string line);
    
public:
    FileProcessor(std::regex extRegex, std::regex splitRegex);
    ~FileProcessor(void);
    void addFileToBeProcessed(boost::filesystem::path p);
    Storer& getStorer();
};

FileProcessor::FileProcessor(std::regex extRegex, std::regex splitRegex) {
    this->extRegex = extRegex;
    this->splitRegex = splitRegex;
    shouldExecute = true;
    processingThread = std::thread(&FileProcessor::processQueue, this);
}

FileProcessor::~FileProcessor(void) {
    while (!fileQueue.empty());
    shouldExecute = false;
    processingThread.join();
}

void FileProcessor::addFileToBeProcessed(boost::filesystem::path p) {
    if (std::regex_match(p.extension().string(), extRegex))
        fileQueue.push(p);
}

Storer& FileProcessor::getStorer() {
    //wait until done processing
    while (!fileQueue.empty());
    shouldExecute = false;
    processingThread.join();
    return storer;
}

void FileProcessor::processQueue() {
    while (shouldExecute) {
        if (!fileQueue.empty()) {
            processFile(fileQueue.front());
            fileQueue.pop();
        }
    }
}

void FileProcessor::processFile(boost::filesystem::path p) {
    std::string line;
    std::ifstream file(p.string());
    if (file.is_open())
    {
        while ( getline (file,line) )
        {
            processLine(line);
        }
        file.close();
    }
}

void FileProcessor::processLine(std::string line) {
    std::regex_token_iterator<std::string::iterator> end;
    std::regex_token_iterator<std::string::iterator> words ( line.begin(), line.end(), splitRegex, -1);
    for (;words != end; words++) {
        storer.addKey(*words);
    }
    
}
#endif /* defined(__WordSearcher__FileProcessor__) */
