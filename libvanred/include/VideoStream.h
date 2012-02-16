#ifndef __VIDEO_STREAM_H_
#define __VIDEO_STREAM_H_

#include <iostream>
#include <sstream>

#include "dvdStream.h"

class VideoStream : public dvdStream {
	//Titleset* vts = NULL;
	std::string mpegVersion;
	double framesPerSecond;
	std::string format;
	std::string aspectRatio;
	unsigned short width;
	unsigned short height;
	std::string df;

public:
	//VideoStream(Titleset& vts, unsigned long fps, std::string format, std::string asr, unsigned short width, unsigned short height, std::string df);
	VideoStream(double fps, std::string format, std::string asr, unsigned short width, unsigned short height, std::string df);
	VideoStream();
	
/*
	void setTitleset (Titleset& vts);
	Titleset* getTitleset(void);
*/
	void setMpegVersion (const std::string& mpegVersion);
	std::string getMpegVersion(void);

	void setFramesPerSecond (const double& fps);
	double getFramesPerSecond (void);

	void setFormat (const std::string& format);
	std::string getFormat (void);

	void setAspectRatio(const std::string& asr);
	std::string getAspectRatio (void);

	void setWidth (const unsigned short& width);
	unsigned short getWidth (void);

	void setHeight (const unsigned short& height);
	unsigned short getHeight (void);

	void setDf (const std::string& df);
	std::string getDf(void);
	
	std::string getResolution (void); //e.g. 720x576

	void print (void);
	void printXml(void);

	std::string toXml(void);
};
#endif
