#include "AudioStream.h"

AudioStream::AudioStream(std::string langCode, std::string language, std::string format, std::string quantization, std::string frequency, unsigned int channels) 
{
	setLangCode(langCode);
	setLanguage(language);
	setFormat(format);
	setQuantization(quantization);
	setFrequency(frequency);
	setChannels(channels);
}
AudioStream::AudioStream() 
{
	setLangCode("na");
	setLanguage("undefined");
	setLangExtension("undefined");
	setFormat("undefined");
	setQuantization("undefined");
	setFrequency("undefined");
	setChannels(0);

}

//Language Code
void AudioStream::setLangCode (std::string langCode) 
{
	this->langCode = langCode;
}
std::string AudioStream::getLangCode (void) 
{
	return langCode;
}

//Language
void AudioStream::setLanguage(std::string language)
{
	this->language = language;
}
std::string AudioStream::getLanguage(void)
{
	return language;
}

//Language Extension
void AudioStream::setLangExtension (std::string langExtension) 
{
	this->langExtension = langExtension;
}
std::string AudioStream::getLangExtension (void) 
{
	return langExtension;
}

//Format
void AudioStream::setFormat (std::string format) 
{
	this->format = format;	
}
std::string AudioStream::getFormat (void)
{
	return format;
}

//Quantization
void AudioStream::setQuantization (std::string quantization)
{
	this->quantization = quantization;
}
std::string AudioStream::getQuantization (void) 
{
	return quantization;
}

//Frequency
void AudioStream::setFrequency (std::string frequency) 
{
	this->frequency = frequency;
}
std::string AudioStream::getFrequency (void) 
{
	return frequency;
}

//Channels
void AudioStream::setChannels (unsigned int channels) 
{
	this->channels = channels;
}
unsigned int AudioStream::getChannels (void) 
{
	return channels;
}

//print method
void AudioStream::print (void) {
    if (streamIsSelected())
       std::cout << "  + ";
    else
        std::cout << "  - ";
        
	std::cout << "Audio Stream: " << getId(); 
    	std::cout << ", Language: " << getLangCode();
	std::cout << "-" << getLanguage();
	std::cout << ", Format: " << getFormat();
	std::cout << ", Quantization: " << getQuantization();
	std::cout << ", Frequency: " << getFrequency();
	std::cout << ", Channels: " << getChannels();
	std::cout << ", Type: " << getLangExtension();
	
	if (getSize() == 0)
	{
		std::cout << ", Size: na" << std::endl;
	}
	else
	{
		std::cout << ", Size: " << getSize() << " MB" << std::endl;
	}
}

void AudioStream::printXml (void) 
{
	std::stringstream size;
	if (getSize() == 0)
	{
		size << "na";
	}
	else
	{
		size << getSize();
	}
	
	std::cout << "      <audio size=\"" << size.str() << "\" id=\"" << getId() << "\" language=\"" << getLanguage() << "\" languageCode=\"" << getLangCode() << "\" format=\"" << getFormat() << "\" quantization=\"" << getQuantization() << "\" frequency=\"" << getFrequency() << "\" channels=\"" << getChannels() << "\" type=\"" << getLangExtension() << "\"/>" << std::endl;
}

//Format as XML tag for dvdauthor
std::string AudioStream::toXml(void) 
{
     std::ostringstream xml;
     xml << "<audio format=\"" << getFormat() << "\" channels=\"" << getChannels() << "\" ";
     xml << "quant=\"" << getQuantization() << "\" samplerate=\"" << getFrequency() << "\" lang=\"" << getLangCode() << "\"/>";    
     return xml.str();
}
