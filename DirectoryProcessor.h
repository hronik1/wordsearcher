//
//	DirectoryProcessor.h
//	WordSearcher
//
//	Created by Brent Hronk on 2/9/14.
//	Copyright (c) 2014 Hronik. All rights reserved.
//

#ifndef __WordSearcher__DirectoryProcessor__
#define __WordSearcher__DirectoryProcessor__

#include <vector>
#include <boost/regex.hpp>
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
	DirectoryProcessor(unsigned int numberWorkerProcessors, boost::regex filesExtsToProcess, boost::regex splitRegex);
	~DirectoryProcessor();
	void processDirectory(boost::filesystem::path p);
	Storer& getStorer();
};

DirectoryProcessor::DirectoryProcessor(unsigned int numberWorkerProcessors, boost::regex extRegex, boost::regex splitRegex) {
	//Creates a DirectoryProcessor with a max of numberWorkProcessors
	//Only processes files matching extRegex
	//Uses splitRegex to delimit words in file
	for (int i = 0; i < numberWorkerProcessors; i++) {
		FileProcessor *fileProcessor = new FileProcessor(extRegex, splitRegex);
		if (NULL != fileProcessor) {
			fileProcessors.push_back(fileProcessor);
			this->numberWorkerProcessors++;
		}
	}
	currentProcessor = 0;
}

void DirectoryProcessor::processDirectory(boost::filesystem::path p) {
	//Recursively process all files starting at p
	if (fileProcessors.size() < 1)
		return;

	processDirectoryHelper(p);
	for (auto& x: fileProcessors) {
		storer.addStore(x->getStorer());
	}
}

Storer& DirectoryProcessor::getStorer() {
	//Returns storer
	return storer;
}

void DirectoryProcessor::processDirectoryHelper(boost::filesystem::path p) {
	//Helper function to process path p
	//If p is a file, will process it
	//If p is a directory, will recursively process it
	if (!exists(p)) {
		return;
	}

	if (boost::filesystem::is_regular_file(p)) {
		processFile(p);
	} else if (boost::filesystem::is_directory(p)) {
		boost::filesystem::directory_iterator end_iter;
		for (boost::filesystem::directory_iterator dir_itr(p);
			 dir_itr != end_iter;
			 ++dir_itr) {
			processDirectoryHelper(dir_itr->path());
		}
	}
}

void DirectoryProcessor::processFile(boost::filesystem::path p) {
	//Processes file p
	//Must be previously verified to be a file
	fileProcessors[currentProcessor]->addFileToBeProcessed(p);
	currentProcessor = (currentProcessor+1)%numberWorkerProcessors;
}

#endif /* defined(__WordSearcher__DirectoryProcessor__) */
