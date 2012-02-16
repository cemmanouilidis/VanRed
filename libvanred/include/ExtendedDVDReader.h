#ifndef __EXTENDED_DVD_READER_H
#define __EXTENDED_DVD_READER_H

#include "DVDReader.h"

class ExtendedDVDReader : public DVDReader {	
private:
	uint32_t startBlock, stopBlock;
	double nonvideoSize;
	int nrOfAudio;
	int nrOfSubs;
	struct streamdata *streamdataList[MAXSTREAMS];
	int streamCount;
	uint8_t tsNr;
	
	int getStartStopBlocks (int title, double& playbackTimeSec);
	int probeDvd (dvd_file_t *vob);

	void streamanalyze(int title, int titlesetNr,	double playbackTimeSec);
	Title readTitle(int title);
	
public:
	ExtendedDVDReader(std::string dvdDevice);
};

#endif

