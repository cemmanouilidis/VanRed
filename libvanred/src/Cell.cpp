#include "Cell.h"

Cell::Cell(int cellId) : dvdStream(cellId,0) {
	setIsStartOfChapter(false);
	setStartOfChapter(0);
}

Cell::Cell() : dvdStream(0,0) {
	setIsStartOfChapter(false);
	setStartOfChapter(0);
}

void Cell::setIsStartOfChapter(bool startOfChapter) {
	this->startOfChapter = startOfChapter;
}
bool Cell::isStartOfChapter(void) {
	return startOfChapter;
}
	
void Cell::setStartOfChapter(int chapter) {
	this->chapter = chapter;
}
int Cell::getStartOfChapter(void) {
	return chapter;
}

void Cell::setPlaytimeSec(double playtimeSec) {
	this->playtimeSec = playtimeSec;
}
double Cell::getPlaytimeSec(void) {
	return playtimeSec;
}

void Cell::print (void)
{

}
void Cell::printXml (void) {
	std::string chapterStart = "yes";
	if (!isStartOfChapter())
		chapterStart = "no";
		
	std::string size = "0";

	std::cout << "         <cell size=\"" << getSize() << "\" id=\"" << getId() << "\" isStartOfChapter=\"" << chapterStart << "\" playbackSec=\"" << getPlaytimeSec() << "\"/>" << std::endl;
}
