#ifndef __AUDIO_STREAM_H_
#define __AUDIO_STREAM_H_

#include <iostream>
#include <sstream>

#include "dvdStreamSelectable.h"

class AudioStream : public dvdStreamSelectable 
{
	std::string langCode;
	std::string language;
	std::string langExtension;
	std::string format;
	std::string quantization;
	std::string frequency;
	unsigned int channels;
	int cool;

	public:	
	AudioStream(std::string langCode, std::string language, std::string format, std::string quantization, std::string frequency, unsigned int channels);
	AudioStream();

	void setLangCode (std::string langCode);
	std::string getLangCode(void);

	void setLanguage(std::string language);
	std::string getLanguage(void);

	void setLangExtension (std::string langExtension);
	std::string getLangExtension(void);

	void setFormat (std::string format);
	std::string getFormat (void);

	void setQuantization (std::string quantization);
	std::string getQuantization (void);

	void setFrequency (std::string frequency);
	std::string getFrequency (void);

	void setChannels (unsigned int channels);
	unsigned int getChannels (void);

	void print (void);
	void printXml(void);

	std::string toXml(void);
};
#endif
