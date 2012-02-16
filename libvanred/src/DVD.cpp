#include "DVD.h"

DVD::DVD (void) 
{
	this->volumeIdentifier = "Unkown";
	this->outputSize = 4300;
}

int DVD::getLongestTitle(void) 
{
	int chapts = 0;
	int longestTitle = 1;
	int static maxChapts = 0;
	
	for (int i = 0; i < getNumberOfTitles(); i++) 
	{
		chapts = titles.at(i).getNumberOfChapter();
		if (chapts >= maxChapts)
		{
			maxChapts = chapts;
			longestTitle = titles.at(i).getId();
		}
	}
	
	return longestTitle;
}

void DVD::addTitle (const Title& title) 
{
	titles.push_back(title);
}

void DVD::setVolumeIdentifier(std::string volumeIdentifier) 
{
	this->volumeIdentifier = volumeIdentifier;
}
	
int DVD::getNumberOfTitles(void) 
{
	return titles.size();
}

std::string DVD::getVolumeIdentifier(void) 
{
	return volumeIdentifier;
}

Title& DVD::getTitle(int id) 
{
	int j;
	if (id == LONGEST_TITLE)
	{
		id = getLongestTitle();
	}

	for (int i = 0; i < getNumberOfTitles(); i++) 
	{
		if (titles.at(i).getId() == id)
		{
			j = i;
			break;
		}
	}

	return titles.at(j);
}

int DVD::getDiscSize(void) 
{
	int discSize = 0;
	for (int i = 0; i < getNumberOfTitles(); i++) 
	{
		discSize += titles.at(i).getSize();
	}
	return discSize;
}

int DVD::getOutputSize(void) 
{
	return outputSize;
}

void DVD::setOutputSize(int outputSize) 
{
	this->outputSize = outputSize;
}

void DVD::print(void) 
{
	std::cout << "DVD Structure " << std::endl;
	std::cout << "Volume Identifier: " << getVolumeIdentifier() << std::endl;
	std::cout << "Disc Size: " << getDiscSize() << " MB" << std::endl;
	std::cout << "Longest Title: " << getLongestTitle() << std::endl << std::endl;
	
	for (int i = 0; i < getNumberOfTitles(); i++) 
	{
		titles.at(i).print();
	}
}

void DVD::printXml(void) 
{
	std::cout << "<DVD size=\"" << getDiscSize() << "\" volumeIdentifier=\"" << getVolumeIdentifier() << "\" longestTitle=\"" << getLongestTitle() << "\">" << std::endl;
	for (int i = 0; i < getNumberOfTitles(); i++) 
	{
		titles.at(i).printXml();
	}
	std::cout << "</DVD>";
}
