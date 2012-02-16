#ifndef __DVD_H_
#define __DVD_H_

#include <iostream>
#include <vector>

#include "Title.h"
#include "DVDConst.h"

#include "SubpictureStream.h"
#include "VideoStream.h"

class DVD {
	std::string volumeIdentifier;
	std::vector<Title> titles;
	int numberOfTitles;
	int outputSize;
public:
	DVD(void);
	int getLongestTitle(void);
	void addTitle(const Title& title);
	int getNumberOfTitles(void);
	void setVolumeIdentifier(std::string volumeIdentifier);
	std::string getVolumeIdentifier(void);
	Title& getTitle(int id);
	int getDiscSize(void);
	int getOutputSize(void);
	void setOutputSize(int outputSize);
	void print(void);
	void printXml(void);
	enum {TITLE = 1, VIDEOSTREAM, AUDIOSTREAM, SUBPICTURESTREAM, CELL, PGC};
};
#endif
