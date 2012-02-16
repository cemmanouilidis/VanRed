#ifndef __TITLE_H_
#define __TITLE_H_

#include <iostream>
#include <vector>

#include "AudioStream.h"
#include "SubpictureStream.h"
#include "VideoStream.h"

#include "dvdStream.h"
#include "Pgc.h"

class Title : public dvdStreamSelectable {
	int vtsNumber;
	int vtsTitleNumber;
	VideoStream videoStream;
	std::vector<AudioStream> audioStreams;
	std::vector<SubpictureStream> subpictureStreams;
	Pgc pgc;
	int nrOfChapter;
	
public:
	Title(int id);
	
	void setVtsNumber(int vtsNumber);
	int getVtsNumber(void);
	
	void setVtsTitleNumber(int vtsTitleNumber);
	int getVtsTitleNumber(void);
	
	void addVideoStream(VideoStream& videoStream);
	VideoStream& getVideoStream(void);

	void addAudioStream(AudioStream& audioStream);
	AudioStream& getAudioStream(int id);
	std::vector<AudioStream> getSelectedAudioStreams(void);
	
	void addSubpictureStream(SubpictureStream& subpictureStream);
	SubpictureStream& getSubpictureStream(int id);
	std::vector<SubpictureStream> getSelectedSubpictureStreams(void);
	
	int getNumberOfAudioStreams(void);
	int getNumberOfSubpictureStreams(void);
	
	void addPgc(Pgc& pgc);
	Pgc& getPgc(void);
	
	void setNumberOfChapter(int nrOfChapter);
	int getNumberOfChapter(void);
	
	void print (int colorPaletteBase);
	
	void print(void);
	void printXml(void);
	
};

#endif
