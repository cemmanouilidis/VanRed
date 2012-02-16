#include <stdio.h>
#include "VideoStream.h"

VideoStream::VideoStream(double fps, std::string format, std::string asr, unsigned short width, unsigned short height, std::string df) {
	setFramesPerSecond(fps);
	setFormat(format);
	setAspectRatio(asr);
	setWidth(width);
	setHeight(height);
	setDf(df);
}

VideoStream::VideoStream() {
	VideoStream(0.0, "na", "na", 0 , 0 , "na");
}
	
/*
void VideoStream::setTitleset (Titleset& vts) {

}
Titleset* VideoStream::getTitleset(void) {

}
*/

void VideoStream::setMpegVersion (const std::string& mpegVersion) {
	this->mpegVersion=mpegVersion;
}
std::string VideoStream::getMpegVersion(void) {
	return mpegVersion;
}

void VideoStream::setFramesPerSecond (const double& fps) {
	this->framesPerSecond = fps;
}
double VideoStream::getFramesPerSecond (void) {
	return framesPerSecond;
}

void VideoStream::setFormat (const std::string& format) {
	this->format = format;
}
std::string VideoStream::getFormat (void) {
	return format;
}

void VideoStream::setAspectRatio(const std::string& asr) {
	this->aspectRatio = asr;
}
std::string VideoStream::getAspectRatio (void) {
	return aspectRatio;
}

void VideoStream::setWidth (const unsigned short& width) {
	this->width = width;
}
unsigned short VideoStream::getWidth (void) {
	return width;
}

void VideoStream::setHeight (const unsigned short& height) {
	this->height = height;
}
unsigned short VideoStream::getHeight (void) {
	return height;
}

void VideoStream::setDf (const std::string& df) {
	this->df = df;
}
std::string VideoStream::getDf(void) {
	return df;
}
	
std::string VideoStream::getResolution (void) {
	char sizePtr[8]; //e.g 720x576
	std::string sizeStr;
	snprintf (sizePtr, sizeof(sizePtr), "%dx%d", getWidth(), getHeight());
	sizeStr = sizePtr;
	return sizeStr;
}

void VideoStream::print (void) {
	std::cout << "MPEG Version: " << getMpegVersion();
	std::cout << ", Format: " << getFormat();
	std::cout << ", Aspect Ratio: " << getAspectRatio();
	std::cout << ", Picture Size: " <<getResolution();
	std::cout << ", Rate: " << getFramesPerSecond();
	std::cout << ", DF: " << getDf();

	if (getSize() == 0)
	{
		std::cout << ", Size: na" << std::endl;
	}
	else
	{
		std::cout << ", Size: " << getSize() << " MB" << std::endl;
	}
}

void VideoStream::printXml (void) {

	std::stringstream size;
	if (getSize() == 0)
	{
		size << "na";
	}
	else
	{
		size << getSize();
	}
	
	std::cout << "      <video size=\"" << size.str() << "\" mpegVersion=\"" << getMpegVersion() << "\" format=\"" << getFormat() << "\" aspectRatio=\"" << getAspectRatio() << "\" pictureSize=\"" << getResolution() << "\" rate=\"" << getFramesPerSecond() << "\" df=\"" << getDf() << "\"/>" << std::endl;
}
std::string VideoStream::toXml(void) {
    std::ostringstream xml;
    xml << "<video format=\"" << getFormat() << "\" aspect=\"" << getAspectRatio() << "\" ";
    xml << "resolution=\"" << getResolution() << "\"/>";
    return xml.str();
}
