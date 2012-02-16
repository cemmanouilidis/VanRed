#ifndef __CELL_H
#define __CELL_H
#include <iostream>

#include "dvdStream.h"

class Cell : public dvdStream {
	private:
		int cellId;
		bool startOfChapter;
		int chapter;
		double playtimeSec;
	public:
		Cell(int cellId);
		Cell();
		void setCellId(int cellId);
		int getCellId(void);

		void setIsStartOfChapter(bool startOfChapter);
		bool isStartOfChapter(void);

		void setStartOfChapter(int chapter);
		int getStartOfChapter(void);

		void setPlaytimeSec(double playtimeSec);
		double getPlaytimeSec(void);

		void print(void);
		void printXml(void);
};

#endif
