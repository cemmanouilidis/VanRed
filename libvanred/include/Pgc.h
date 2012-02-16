#ifndef __PGC_H_
#define __PGC_H_

#include <string>
#include <vector>
#include "ColorPalette.h"
#include "Cell.h"

class Pgc {
private:
	ColorPalette colorPalette;
	std::vector<Cell> cells;

	int nrOfPrograms;
	std::string playbackTime;
	double playtimeSec;

public:
	Pgc();

	void addCell(Cell& cell);
	Cell& getCell(int cellId);
	
	void addColorPalette(ColorPalette& colorPalette);
	ColorPalette& getColorPalette();

	int getNumberOfCells(void);
	
	void setNumberOfPrograms(int nrOfPrograms);
	int getNumberOfPrograms(void);

	void setPlaybackTime(std::string playbackTime);
	std::string getPlaybackTime(void);

	void setPlaytimeSec(double playtimeSec);
	double getPlaytimeSec(void);
	
	void print(void);
	void printXml(void);
};

#endif
