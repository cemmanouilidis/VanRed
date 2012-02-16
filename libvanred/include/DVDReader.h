#ifndef __DVD_READER_H
#define __DVD_READER_H

#include <dvdread/dvd_reader.h>
#include <dvdread/ifo_read.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <map>

#include "DVD.h"
#include "DVDConst.h"

class DVDReader {	
	protected:
		std::map<std::string,std::string> languageMap;
		std::string dvdDevice;

		dvd_reader_t* dvd;
		dvd_file_t* dvdFile;
		ifo_handle_t *ifoZero, **ifo;

		std::string getLanguageFromCode(std::string& code);
		double dvdtime2msec(dvd_time_t *dt);
		void exitReader(int exitCode);

		virtual Title readTitle(int title) = 0;
	public:
		class readException {
			private:
				std::string message;
			public:
				readException(std::string message)
				{	this->message = message; }
				const std::string getMessage()
				{ 	return message; }		
		};

		DVDReader(std::string dvdDevice) throw (DVDReader::readException);
		~DVDReader();
		DVD readDvd();
		DVD readDvd(int title);

		void close(void);
};

#endif

