#include "SubpictureStream.h"

SubpictureStream::SubpictureStream(std::string langCode, std::string language) {
	setLangCode(langCode);
	setLanguage(language);
}
SubpictureStream::SubpictureStream() {
	setLangCode("na");
	setLanguage("undefined");
}

void SubpictureStream::setLangCode (std::string langCode) {
	this->langCode = langCode;
}
std::string SubpictureStream::getLangCode (void) {
	return langCode;
}

void SubpictureStream::setLanguage(std::string language){
	this->language = language;
}
std::string SubpictureStream::getLanguage(void){
	return language;
}

void SubpictureStream::print (void) {
     if (streamIsSelected())
        std::cout << "  + ";
     else
         std::cout << "  - ";
         
	std::cout << "SPU Stream: " << getId();
	std::cout << ", Language Code: " << getLangCode();
	std::cout << ", Language: " << getLanguage();

	if (getSize() == 0)
	{
		std::cout << ", Size: na" << std::endl;
	}
	else
	{
		std::cout << ", Size: " << getSize() << " MB" << std::endl;
	}
}

void SubpictureStream::printXml(void) {
		
	std::stringstream size;
	if (getSize() == 0)
	{
		size << "na";
	}
	else
	{
		size << getSize();
	}
	
	std::cout << "      <subpicture size=\"" << size.str() << "\" id=\"" << getId() << "\" language=\"" << getLanguage() << "\" languageCode=\"" << getLangCode() << "\"/>" << std::endl;
}
std::string SubpictureStream::toXml (void) {
     std::ostringstream xml;
     xml << "<subpicture lang=\"" << getLangCode() << "\"/>";     
     return xml.str();
}
