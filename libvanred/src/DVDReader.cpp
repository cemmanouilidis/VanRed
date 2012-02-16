#include "DVDReader.h"

#include <dvdread/ifo_print.h>
#include <string.h>


DVDReader::DVDReader(std::string dvdDevice) throw (DVDReader::readException) 
{
	//Open DVD
	this->dvd = DVDOpen(dvdDevice.c_str());
	if (!dvd) 
	{
		DVDClose(dvd);
		std::string error = strerror(errno);
		DVDReader::readException ex("Open DVD: " + error);
		throw ex;
		return;
	}
	
	//Open Main IFO
	this->ifoZero = ifoOpen(dvd, 0);

	if (!ifoZero) 
	{
		ifoClose(ifoZero);
		std::string error = strerror(errno);
		DVDReader::readException ex("Open main IFO: " + error);
		throw ex;
		return;
	}

	//Open all IFOs
	ifo = (ifo_handle_t **)malloc((ifoZero->vts_atrt->nr_of_vtss + 1) * sizeof(ifo_handle_t *));

	for (int i=1; i <= ifoZero->vts_atrt->nr_of_vtss; i++) 
	{
		ifo[i] = ifoOpen(dvd, i);
		if (!ifo[i]) 
		{
			std::stringstream sstr;
			std::string error = strerror(errno);
			sstr << "Open IFO for title " << i << ":" << error;
			DVDReader::readException ex(sstr.str());
			throw ex;
			return;
		}
	}

	//Init languages
	languageMap["de"] = "Deutsch";
	languageMap["  "] = "Not Specified";
	languageMap["aa"] = "Afar";
	languageMap["ab"] = "Abkhazian";
	languageMap["af"] = "Afrikaans";
	languageMap["am"] = "Amharic";
	languageMap["ar"] = "Arabic";
	languageMap["as"] = "Assamese";
	languageMap["ay"] = "Aymara";
	languageMap["az"] = "Azerbaijani";
	languageMap["ba"] = "Bashkir";
	languageMap["be"] = "Byelorussian"; 
	languageMap["bg"] = "Bulgarian";
	languageMap["bh"] = "Bihari";
	languageMap["bi"] = "Bislama";
	languageMap["bn"] = "Bengali; Bangla";
	languageMap["bo"] = "Tibetan";
	languageMap["br"] = "Breton";
	languageMap["ca"] = "Catalan";
	languageMap["co"] = "Corsican";
	languageMap["cs"] = "Czech";
	languageMap["cy"] = "Welsh";
	languageMap["da"] = "Dansk";
	languageMap["de"] = "German";
	languageMap["dz"] = "Bhutani";
	languageMap["el"] = "Greek";
	languageMap["en"] = "English";
	languageMap["eo"] = "Esperanto";
	languageMap["es"] = "Espanol";
	languageMap["et"] = "Estonian";
	languageMap["eu"] = "Basque";
	languageMap["fa"] = "Persian";
	languageMap["fi"] = "Suomi";
	languageMap["fj"] = "Fiji";
	languageMap["fo"] = "Faroese";
	languageMap["fr"] = "Francais";
	languageMap["fy"] = "Frisian";
	languageMap["ga"] = "Gaelic";
	languageMap["gd"] = "Scots Gaelic";
	languageMap["gl"] = "Galician";
	languageMap["gn"] = "Guarani";
	languageMap["gu"] = "Gujarati";
	languageMap["ha"] = "Hausa";
	languageMap["he"] = "Hebrew";
	languageMap["hi"] = "Hindi";
	languageMap["hr"] = "Hrvatski";
	languageMap["hu"] = "Magyar";
	languageMap["hy"] = "Armenian";
	languageMap["ia"] = "Interlingua";
	languageMap["id"] = "Indonesian";
	languageMap["ie"] = "Interlingue";
	languageMap["ik"] = "Inupiak";
	languageMap["in"] = "Indonesian";
	languageMap["is"] = "Islenska";
	languageMap["it"] = "Italiano";
	languageMap["iu"] = "Inuktitut";
	languageMap["iw"] = "Hebrew";
	languageMap["ja"] = "Japanese";
	languageMap["ji"] = "Yiddish";
	languageMap["jw"] = "Javanese";
	languageMap["ka"] = "Georgian";
	languageMap["kk"] = "Kazakh";
	languageMap["kl"] = "Greenlandic";
	languageMap["km"] = "Cambodian";
	languageMap["kn"] = "Kannada";
	languageMap["ko"] = "Korean";
	languageMap["ks"] = "Kashmiri";
	languageMap["ku"] = "Kurdish";
	languageMap["ky"] = "Kirghiz";
	languageMap["la"] = "Latin";
	languageMap["ln"] = "Lingala";
	languageMap["lo"] = "Laothian";
	languageMap["lt"] = "Lithuanian";
	languageMap["lv"] = "Latvian,Lettish";
	languageMap["mg"] = "Malagasy";
	languageMap["mi"] = "Maori";
	languageMap["mk"] = "Macedonian";
	languageMap["ml"] = "Malayalam";
	languageMap["mn"] = "Mongolian";
	languageMap["mo"] = "Moldavian";
	languageMap["mr"] = "Marathi";
	languageMap["ms"] = "Malay";
	languageMap["mt"] = "Maltese";
	languageMap["my"] = "Burmese";
	languageMap["na"] = "Nauru";
	languageMap["ne"] = "Nepali";
	languageMap["nl"] = "Nederlands";
	languageMap["no"] = "Norsk";
	languageMap["oc"] = "Occitan";
	languageMap["om"] = "Oromo";
	languageMap["or"] = "Oriya";
	languageMap["pa"] = "Punjabi";
	languageMap["pl"] = "Polish";
	languageMap["ps"] = "Pashto, Pushto";
	languageMap["pt"] = "Portugues";
	languageMap["qu"] = "Quechua";
	languageMap["rm"] = "Rhaeto-Romance";
	languageMap["rn"] = "Kirundi";
	languageMap["ro"] = "Romanian" ;
	languageMap["ru"] = "Russian";
	languageMap["rw"] = "Kinyarwanda";
	languageMap["sa"] = "Sanskrit";
	languageMap["sd"] = "Sindhi";
	languageMap["sg"] = "Sangho";
	languageMap["sh"] = "Serbo-Croatian";
	languageMap["si"] = "Sinhalese";
	languageMap["sk"] = "Slovak";
	languageMap["sl"] = "Slovenian";
	languageMap["sm"] = "Samoan";
	languageMap["sn"] = "Shona" ;
	languageMap["so"] = "Somali";
	languageMap["sq"] = "Albanian";
	languageMap["sr"] = "Serbian";
	languageMap["ss"] = "Siswati";
	languageMap["st"] = "Sesotho";
	languageMap["su"] = "Sundanese";
	languageMap["sv"] = "Svenska";
	languageMap["sw"] = "Swahili";
	languageMap["ta"] = "Tamil";
	languageMap["te"] = "Telugu";
	languageMap["tg"] = "Tajik";
	languageMap["th"] = "Thai";
	languageMap["ti"] = "Tigrinya";
	languageMap["tk"] = "Turkmen";
	languageMap["tl"] = "Tagalog";
	languageMap["tn"] = "Setswana";
	languageMap["to"] = "Tonga";
	languageMap["tr"] = "Turkish";
	languageMap["ts"] = "Tsonga";
	languageMap["tt"] = "Tatar";
	languageMap["tw"] = "Twi";
	languageMap["ug"] = "Uighur";
	languageMap["uk"] = "Ukrainian";
	languageMap["ur"] = "Urdu";
	languageMap["uz"] = "Uzbek";
	languageMap["vi"] = "Vietnamese";
	languageMap["vo"] = "Volapuk";
	languageMap["wo"] = "Wolof";
	languageMap["xh"] = "Xhosa";
	languageMap["yi"] = "Yiddish";
	languageMap["yo"] = "Yoruba";
	languageMap["za"] = "Zhuang";
	languageMap["zh"] = "Chinese";
	languageMap["zu"] = "Zulu";
	languageMap["xx"] = "Unknown"; 
}

DVDReader::~DVDReader()
{
	close();
}

DVD DVDReader::readDvd()
{
	// Read number of titles
	int nrOfTitles = ifoZero->tt_srpt->nr_of_srpts;

	//Create a new DVD object
	DVD dvdObj;

	//Volume Name of Disc, this does not work when the movie is stored on your hard disc
	char discId[256];
	unsigned char udiscID[256];

	if ( (DVDUDFVolumeInfo(dvd, discId, 256*10, udiscID, 256*10)) == 0) 
	{
		dvdObj.setVolumeIdentifier(discId);	
	}
	else if ( (DVDISOVolumeInfo(dvd, discId, 256*10, udiscID, 256*10)) == 0) 
	{
		dvdObj.setVolumeIdentifier(discId);	
	}
	else
	{
		dvdObj.setVolumeIdentifier("Unknown");	
	}


	//Calc the longest title
	pgcit_t* vtsPgcit;
	pgc_t *pgc;
	int playbackTime = 0;
	int titleSetNr = 0;
	int vtsTTN = 0;
	int longestTitle = 0;
	int title = 0;

	std::cerr << "DVD has " << nrOfTitles << " titles" << std::endl;
	for (int i =0; i< nrOfTitles; i++) 
	{
		title = i+1;
		static int playbackMax = 0;
		vtsPgcit  = ifo[ifoZero->tt_srpt->title[i].title_set_nr]->vts_pgcit;
	 	titleSetNr = ifoZero->tt_srpt->title[i].title_set_nr;
		vtsTTN = ifoZero->tt_srpt->title[i].vts_ttn;
		pgc = vtsPgcit->pgci_srp[ifo[titleSetNr]->vts_ptt_srpt->title[vtsTTN-1].ptt[0].pgcn - 1].pgc;
		playbackTime = (int) dvdtime2msec(&pgc->playback_time);

		if (playbackTime > playbackMax)
		{
			playbackMax = playbackTime;
			longestTitle = i+1;
		}
		//Read Title
		//std::cerr << "Reading title: " << title << std::endl;
		dvdObj.addTitle(readTitle(title));
	}

	std::cerr << "Longest title: " << longestTitle << std::endl;

	return dvdObj;
}

DVD DVDReader::readDvd(int title)
{
	// Read number of titles
	int nrOfTitles = ifoZero->tt_srpt->nr_of_srpts;

	//Create a new DVD object
	DVD dvdObj;

	//Volume Name of Disc, this does not work when the movie is stored on your hard disc
	char discId[256];
	unsigned char udiscID[256];

	if ( (DVDUDFVolumeInfo(dvd, discId, 256*10, udiscID, 256*10)) == 0) 
	{
		dvdObj.setVolumeIdentifier(discId);	
	}
	else if ( (DVDISOVolumeInfo(dvd, discId, 256*10, udiscID, 256*10)) == 0) 
	{
		dvdObj.setVolumeIdentifier(discId);	
	}
	else
	{
		dvdObj.setVolumeIdentifier("Unknown");	
	}

	//Calc the longest title
	pgcit_t* vtsPgcit;
	pgc_t *pgc;
	int playbackTime = 0;
	int titleSetNr = 0;
	int vtsTTN = 0;
	int longestTitle = 0;

	std::cerr << "DVD has " << nrOfTitles << " titles" << std::endl;
	for (int i =0; i< nrOfTitles; i++) 
	{
		static int playbackMax = 0;
		vtsPgcit  = ifo[ifoZero->tt_srpt->title[i].title_set_nr]->vts_pgcit;
		titleSetNr = ifoZero->tt_srpt->title[i].title_set_nr;
		vtsTTN = ifoZero->tt_srpt->title[i].vts_ttn;
		pgc = vtsPgcit->pgci_srp[ifo[titleSetNr]->vts_ptt_srpt->title[vtsTTN-1].ptt[0].pgcn - 1].pgc;
		playbackTime = (int) dvdtime2msec(&pgc->playback_time);

		if (playbackTime > playbackMax)
		{
			playbackMax = playbackTime;
			longestTitle = i+1;
		}
	}
	
	std::cerr << "Longest title: " << longestTitle << std::endl;
	if (title == LONGEST_TITLE)
	{
		dvdObj.addTitle(readTitle(longestTitle));
	}
	else 
	{
		dvdObj.addTitle(readTitle(title));
	}

	return dvdObj;
}
std::string DVDReader::getLanguageFromCode(std::string& code)
{
	if (languageMap.find(code) != languageMap.end()) 
	{
		return languageMap[code];
	}
	else 
	{
		code = "xx";
		return languageMap[code];
	}
}

double DVDReader::dvdtime2msec(dvd_time_t *dt)
{
	double fps = framesPerS[(dt->frame_u & 0xc0) >> 6];
	double   ms;
	ms  = (((dt->hour &   0xf0) >> 3) * 5 + (dt->hour   & 0x0f)) * 3600000;
	ms += (((dt->minute & 0xf0) >> 3) * 5 + (dt->minute & 0x0f)) * 60000;
	ms += (((dt->second & 0xf0) >> 3) * 5 + (dt->second & 0x0f)) * 1000;

	if(fps > 0)
		ms += ((dt->frame_u & 0x30) >> 3) * 5 + (dt->frame_u & 0x0f) * 1000.0 / fps;

	return ms;
}

void DVDReader::close() {
	//close all IFOs
	for (int i=1; i <= ifoZero->vts_atrt->nr_of_vtss; i++) 
		ifoClose(ifo[i]);
	//close Main IFO
	ifoClose(ifoZero);
	//close DVD
	DVDClose(dvd);
}

void DVDReader::exitReader(int exitCode) {
	for (int i=1; i <= ifoZero->vts_atrt->nr_of_vtss; i++) 
		ifoClose(ifo[i]);

	ifoClose(ifoZero);
	DVDClose(dvd);

	exit(exitCode);
}

