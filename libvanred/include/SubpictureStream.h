#ifndef __SUBPICTURE_STREAM_H_
#define __SUBPICTURE_STREAM_H_

#include <iostream>
#include <sstream>

#include "dvdStreamSelectable.h"

class SubpictureStream : public dvdStreamSelectable {
	std::string langCode;
	std::string language;

public:
	SubpictureStream(std::string langCode, std::string language);
	SubpictureStream();

	void setLangCode (std::string langCode);
	std::string getLangCode(void);

	void setLanguage(std::string language);
	std::string getLanguage(void);

	void print (void);
	void printXml(void);
	
	std::string toXml(void);
};
#endif
