//
//	FileProcessor.h
//	WordSearcher
//
//	Created by Brent Hronk on 2/9/14.
//	Copyright (c) 2014 Hronik. All rights reserved.
//

#ifndef __WordSearcher__FileProcessor__
#define __WordSearcher__FileProcessor__

#include <boost/regex.hpp>
#include <boost/filesystem/path.hpp>
#include <queue>
#include <fstream>
#include <thread>
#include "Storer.h"

class FileProcessor {

	Storer storer;
	std::queue<boost::filesystem::path> fileQueue;
	boost::regex extRegex;
	boost::regex splitRegex;
	std::thread processingThread;
	volatile bool shouldExecute;
	void processQueue();
	void processFile(boost::filesystem::path p);
	void processLine(std::string line);
	
public:
	FileProcessor(boost::regex extRegex, boost::regex splitRegex);
	~FileProcessor(void);
	void addFileToBeProcessed(boost::filesystem::path p);
	Storer& getStorer();
};

FileProcessor::FileProcessor(boost::regex extRegex, boost::regex splitRegex) {
	//Creates a FileProcessor object
	//Processes only files that match extRegex
	//Ues splitRegex to delimit words within files to be processed
	this->extRegex = extRegex;
	this->splitRegex = splitRegex;
	shouldExecute = true;
	processingThread = std::thread(&FileProcessor::processQueue, this);
}

FileProcessor::~FileProcessor(void) {
	//Waits for underlying thread to finish processing, and then rejoins it
	while (!fileQueue.empty());
	shouldExecute = false;
	processingThread.join();
}

void FileProcessor::addFileToBeProcessed(boost::filesystem::path p) {
	//Tests if the input file matches the file extensions to process
	//If matches, adds to the to process queue
	//Must ensure that p is a valid file before calling
	if (boost::regex_match(p.extension().string(), extRegex))
		fileQueue.push(p);
}

Storer& FileProcessor::getStorer() {
	//Wait until underlying thread finishes processing and returns storer
	while (!fileQueue.empty());
	shouldExecute = false;
	processingThread.join();
	return storer;
}

void FileProcessor::processQueue() {
	//Worker function for underlying thread
	//Processes underlying queue until notified to exit
	while (shouldExecute) {
		if (!fileQueue.empty()) {
			processFile(fileQueue.front());
			fileQueue.pop();
		}
	}
}

void FileProcessor::processFile(boost::filesystem::path p) {
	//Adds all words in file to underlying storer
	//p must be guaranteed to be a valid file before calling
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
	//Adds all words within line to underlying storer
	boost::regex_token_iterator<std::string::iterator> end;
	boost::regex_token_iterator<std::string::iterator> words ( line.begin(), line.end(), splitRegex, -1);
	
	for (;words != end; words++) {
		storer.addKey(*words);
	}
	
}

#endif /* defined(__WordSearcher__FileProcessor__) */
