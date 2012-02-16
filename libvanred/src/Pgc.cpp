#include "Pgc.h"

Pgc::Pgc() {
	setNumberOfPrograms(0);
	setPlaybackTime(std::string(""));
	setPlaytimeSec(0.0);
}

void Pgc::addCell(Cell& cell) {
	cells.push_back(cell);
}
Cell& Pgc::getCell(int cellId) {
	return cells.at(cellId-1);
}

void Pgc::addColorPalette(ColorPalette& colorPalette) {
	this->colorPalette = colorPalette;
}
ColorPalette& Pgc::getColorPalette() {
	return colorPalette;
}


int Pgc::getNumberOfCells(void) {
	return cells.size();
}
	
void Pgc::setNumberOfPrograms(int nrOfPrograms) {
	this->nrOfPrograms= nrOfPrograms;
}
int Pgc::getNumberOfPrograms(void) {
	return nrOfPrograms;
}

void Pgc::setPlaybackTime(std::string playbackTime) {
	this->playbackTime = playbackTime;
}
std::string Pgc::getPlaybackTime(void) {
	return playbackTime;
}

void Pgc::setPlaytimeSec(double playtimeSec) {
	this->playtimeSec = playtimeSec;
}
double Pgc::getPlaytimeSec(void) {
	return playtimeSec;
}
void Pgc::print() {
	//std::cout << "  Number of cells: "  << getNumberOfCells() << ", Number of programs: " << getNumberOfPrograms() << std::endl;
	std::cout << "  Number of cells: "  << getNumberOfCells() << std::endl;
	getColorPalette().print();
}

void Pgc::printXml() {
	std::cout << "      <pgc>" << std::endl;
	for (int i = 0; i < getNumberOfCells(); i++) {
		cells.at(i).printXml();		
	}
	getColorPalette().printXml();
	std::cout << "      </pgc>" << std::endl;
}
