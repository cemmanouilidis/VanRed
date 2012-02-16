#include "Title.h"

Title::Title(int id) {
	setVtsNumber(1);
	setVtsTitleNumber(1);
	setId(id);
	setNumberOfChapter(0);
}

void Title::setVtsNumber(int vtsNumber)  {
	this->vtsNumber = vtsNumber;
}

int Title::getVtsNumber(void) {
	return vtsNumber;
}

void Title::setVtsTitleNumber(int vtsTitleNumber) {
	this->vtsTitleNumber = vtsTitleNumber;
}
int Title::getVtsTitleNumber(void) {
	return vtsTitleNumber;
}

void Title::addVideoStream(VideoStream& videoStream) {
	this->videoStream = videoStream;
	setSize(getSize() + videoStream.getSize());
}

VideoStream& Title::getVideoStream(void) {
	return videoStream;
}

void Title::addAudioStream(AudioStream& audioStream) {
	this->audioStreams.push_back(audioStream);
	setSize(getSize() + audioStream.getSize());
}
AudioStream& Title::getAudioStream(int id) {
    return audioStreams.at(id-1);             
}
std::vector<AudioStream> Title::getSelectedAudioStreams(void) {
    std::vector<AudioStream> a;
    for (int lv = 0; lv < getNumberOfAudioStreams(); lv++) {
        if (audioStreams.at(lv).streamIsSelected())
           a.push_back(audioStreams.at(lv));
    }
    return a;
}

void Title::addSubpictureStream(SubpictureStream& subpictureStream) {
	this->subpictureStreams.push_back(subpictureStream);
	setSize(getSize() + subpictureStream.getSize());
}
SubpictureStream& Title::getSubpictureStream(int id) {
    return subpictureStreams.at(id-1);                  
}
std::vector<SubpictureStream> Title::getSelectedSubpictureStreams(void) {
    std::vector<SubpictureStream> s;
    for (int lv = 0; lv < getNumberOfSubpictureStreams(); lv++) {
        if (subpictureStreams.at(lv).streamIsSelected())
           s.push_back(subpictureStreams.at(lv));
    }
    return s;                         
}
int Title::getNumberOfAudioStreams(void) {
	return audioStreams.size();
}

int Title::getNumberOfSubpictureStreams(void) {
	return subpictureStreams.size();
}
void Title::addPgc(Pgc& pgc) {
	this->pgc = pgc;
}
Pgc& Title::getPgc(void) {
	return pgc;
}

void Title::setNumberOfChapter(int nrOfChapter) {
	this->nrOfChapter = nrOfChapter;
}

int Title::getNumberOfChapter(void) {
	return nrOfChapter;
}
	
void Title::print (int colorPaletteBase)
{
	if ( streamIsSelected() )
		std::cout << "+"; 
	else
		std::cout << "-"; 
	
 	std::cout << " Title " << getId() << ": ["<< getPgc().getPlaybackTime() << "], Sec. " << getPgc().getPlaytimeSec() << std::endl;
 	std::cout << "Size: " << getSize() << " MB" << std::endl;
	std::cout << "VTS Title Number: " << getVtsTitleNumber() << ", VTS: " << getVtsNumber() << std::endl;
	std::cout << "Title has " << getNumberOfChapter() << " chapters" << std::endl << std::endl;
	std::cout << "Video Stream Settings" << std::endl;
	videoStream.print();
	
	if (getNumberOfAudioStreams() == 0)
		std::cout << std::endl << "No Audio Streams available " << std::endl << std::endl;
	else {	
		std::cout << getNumberOfAudioStreams() << " Audio Streams" << std::endl;
		for (int lv = 1; lv <= getNumberOfAudioStreams(); lv++)
	    		getAudioStream(lv).print();
	}

	if (getNumberOfSubpictureStreams() == 0)
		std::cout << std::endl << "No Subpicture Streams available " << std::endl << std::endl;
	else {	
		std::cout << std::endl << getNumberOfSubpictureStreams() << " Subpicture Streams" << std::endl;
		for (int lv = 1; lv <= getNumberOfSubpictureStreams(); lv++)
			getSubpictureStream(lv).print();
	}
	std::cout << std::endl << "PGC" << std::endl;
		getPgc().print();
}

void Title::print(void)
{
     print(ColorPalette::RAW);
}

void Title::printXml(void) 
{
	std::cout << "   <title size=\"" << getSize() << "\" id=\"" << getId() << "\" vtsTitleNumber=\"" << getVtsTitleNumber() << "\" vts=\"" << getVtsNumber() << "\" playbackTime=\"" << getPgc().getPlaybackTime() << "\" playbackSec=\"" << getPgc().getPlaytimeSec() << "\" numberOfChapter=\"" << getNumberOfChapter() << "\">" << std::endl;

	videoStream.printXml();
	
	for (int lv = 1; lv <= getNumberOfAudioStreams(); lv++)
   		getAudioStream(lv).printXml();
	
	for (int lv = 1; lv <= getNumberOfSubpictureStreams(); lv++)
		getSubpictureStream(lv).printXml();
	
	getPgc().printXml();
	std::cout << "   </title>" << std::endl;
	
}
