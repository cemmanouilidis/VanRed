#ifndef __SIMPLE_DVD_READER_H
#define __SIMPLE_DVD_READER_H

#include "DVDReader.h"

class SimpleDVDReader : public DVDReader {	
private:
	Title readTitle(int title);
	int calcTitleSize(Title&);
	
public:
	//static DVDReader getDvdReaderRef(void);
	SimpleDVDReader(std::string dvdDevice);
};

#endif
