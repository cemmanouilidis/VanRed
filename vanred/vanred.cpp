#include <libvanred/DVD.h>
#include <libvanred/ReAuthorXML.h>
#include <libvanred/SimpleDVDReader.h>
#include <libvanred/ExtendedDVDReader.h> 
#include <libvanred/StringUtils.h>
#include <libvanred/DVDConst.h>

#include <getopt.h>
#include <stdexcept>
#include <iostream>
#include <fstream>

#include <unistd.h>
#include <sys/wait.h>

#define VERSION 0.6

int idsOfSelectedStreams(std::string& selectedStreams, std::vector<int>& idsVector);
int setAudioStreamsSelected(Title& t, std::vector<int>& idsVector);
int setSubpictureStreamsSelected(Title& t, std::vector<int>& idsVector);
void usage(void);
void print (std::string printFormat, DVD& dvd);
void version(void);

int main (int argc, char** argv) 
{
	std::string xmlDvdauthor = "dvdauthor.xml";
	std::string paletteFile = "palette.raw";

	std::string source = "/dev/dvd";
	std::string destination = ".";

	std::string printFormat = "human";
	std::string reading = "simple";

	bool printOnly = false;

	char currentPath[256];
	int title = LONGEST_TITLE;
	int outputSize = 4300;

	std::string selectedAStreams = "";
	std::vector<int> aIdsVector;

	std::string selectedSStreams = "";
	std::vector<int> sIdsVector;


	/*if (argc != 5)
	  usage();
	  */

	static struct option longOptions [] = {
		{ "input",		1, NULL, 'i' },
		{ "output",		1, NULL, 'o' },
		{ "output-size",0, NULL, 'S' },
		{ "title",		1, NULL, 't' },
		{ "audio", 		1, NULL, 'a' },
		{ "subpicture", 1, NULL, 's' },
		{ "reading", 	1, NULL, 'r' },
		{ "xml",		1, NULL, 'x' },
		{ "help",		0, NULL, 'h' },
		{ "version",	0, NULL, 'V' },
		{ "information",1, NULL, 'I' },
	};

	char c;	
	while ((c = getopt_long_only (argc, argv, "i:o:S:t:a:s:r:x:h::V::I:",longOptions, NULL)) >= 0) 
	{
		switch (c) {
			case 'i':
				source = optarg;
				break;
			case 'o':
				destination = optarg;
				break;
			case 'S':
				outputSize = atoi(optarg);
				break;
			case 't':
				title = atoi(optarg);
				break;
			case 'a':
				selectedAStreams = optarg;
				break;
			case 's':
				selectedSStreams = optarg;
				break;
			case 'r':
				reading = optarg;
				if (! (reading =="simple" || reading == "extended") )
					usage();
				break;
			case 'x':
				xmlDvdauthor = optarg;
				break;
			case 'h':
				usage();
				break;
			case 'V':
				version();
				break;
			case 'I':
				printFormat = optarg;
				printOnly = true;
				break;			
			default:
				usage();
		}
	}

	// when streams are specified, check first if we can read them as ids (numbers)
	if (selectedAStreams.size() > 0)
	{
		if ( !(selectedAStreams.size() == 1 && (selectedAStreams[0] == 'n' || selectedAStreams[0] == 'a')))
		{
			if (idsOfSelectedStreams(selectedAStreams, aIdsVector) < 0) 
			{
				std::cerr << "bad audio streams: " << selectedAStreams << std::endl;
				exit(-2);
			}
		}
	}

	if (selectedSStreams.size() > 0)
	{
		if ( !(selectedSStreams.size() == 1 && (selectedSStreams[0] == 'n' || selectedSStreams[0] == 'a')))
		{
			if (idsOfSelectedStreams(selectedSStreams, sIdsVector) < 0) 
			{
				std::cerr << "bad subpicture streams: " << selectedSStreams << std::endl;
				exit(-2);
			}
		}
	}

	if(source[0] != '/' && source[0] != '.') 
	{
		std::string tmp = source;
		source = getcwd(currentPath, sizeof(currentPath));
		source = source + "/" + tmp;
	}

	DVD dvd;
	
	std::cerr << std::endl << "Reading DVD: " << source << std::endl;
	try 
	{

		if (reading == "simple")
		{
			SimpleDVDReader reader = SimpleDVDReader(source);
			dvd = reader.readDvd(title);
		}	
		else
		{
			ExtendedDVDReader reader = ExtendedDVDReader(source);
			dvd = reader.readDvd(title);
		}
	} 
	catch (DVDReader::readException e)
	{
		std::cerr << "Read Exception: " << e.getMessage() << std::endl;
		exit(-1);	
	}

	dvd.setOutputSize(outputSize);

	Title& t = dvd.getTitle(title);

	//Check if any audio streams were selected
	if (selectedAStreams.size() > 0) {
		if (selectedAStreams.size() == 1 && selectedAStreams[0] == 'n') {
			for (int j=1; j <= t.getNumberOfAudioStreams(); j++) {
				t.getAudioStream(j).setStreamSelected(false); //unselect all audio streams
			}
		}
		else if (selectedAStreams.size() == 1 && selectedAStreams[0] == 'a') {
			//nothing todo...streams are selected by default
		}
		else {
			try {
				if (setAudioStreamsSelected(t,aIdsVector) < 0) { //set selected all audio streams in aIdsVector
					std::cerr << "bad audio stream: " << selectedAStreams << std::endl;
					exit(-2);
				}
			}
			catch (int err) {
				std::cout << "audio stream: " << err << " does not exist" << std::endl;
				exit(-2);
			}		
		}	
	}

	// Check if any subpicture streams were selected
	if (selectedSStreams.size() > 0) {
		if (selectedSStreams[0] == 'n') {
			for (int j=1; j <= t.getNumberOfSubpictureStreams(); j++) {
				t.getSubpictureStream(j).setStreamSelected(false);
			}
		}
		else if (selectedSStreams[0] == 'a') {
			//nothing todo
		} 
		else {
			try {
				if (setSubpictureStreamsSelected(t,sIdsVector) < 0) {
					std::cerr << "bad subpicture stream: " << selectedSStreams << std::endl;
					exit(-2);
				}
			}
			catch (int err) {
				std::cout << "subpicture stream: " << err << " does not exist" << std::endl;
				exit(-2);
			}			
		}
	}


	//std::cout << std::endl << "Printing DVD information " << std::endl;
	if (printOnly) {
		print(printFormat, dvd);
	}

	else {
		print("human", dvd);
		std::cout << std::endl;

		//check if any audio streams are available
		if ( (t.getNumberOfAudioStreams() > 0) && (selectedAStreams.size() == 0) )
		{
			std::cout << "Please select which audio streams do you want: " << std::endl;
			std::cout << "e.g. for the first two streams enter: 1,2" << std::endl;

			bool nok;
			do {
				nok = false;
				std::cin >> selectedAStreams;

				if (selectedAStreams.size() == 1 && selectedAStreams[0] == 'n') {
					for (int j=1; j <= t.getNumberOfAudioStreams(); j++) {
						t.getAudioStream(j).setStreamSelected(false);
					}
				}
				else if (selectedAStreams.size() == 1 && selectedAStreams[0] == 'a') {
					//nothing todo...
				}
				else {
					if (idsOfSelectedStreams(selectedAStreams, aIdsVector) < 0) {
						std::cerr << "bad audio streams: " << selectedAStreams << std::endl;
						nok = true;
					}
					try {
						if (setAudioStreamsSelected(t,aIdsVector) < 0) {
							std::cerr << "bad audio stream: " << selectedAStreams << std::endl;
							nok = true;
						}
					}
					catch (int err) {
						std::cout << "audio stream: " << err << " does not exist" << std::endl;
						nok = true;
					}		
				}	

			}while(nok);
		}

		//check if any subpicture streams are available
		if ( (t.getNumberOfSubpictureStreams() > 0) && (selectedSStreams.size() == 0) ) {
			std::cout << std::endl << "Please select which subpicture streams do you want: " << std::endl;
			std::vector<std::string> s;

			bool nok;	
			do {
				nok = false;
				std::cin >> selectedSStreams;

				if (selectedSStreams.size() == 1 && selectedSStreams[0] == 'n') {
					for (int j=1; j <= t.getNumberOfSubpictureStreams(); j++) {
						t.getSubpictureStream(j).setStreamSelected(false);
					}
				}
				else if (selectedSStreams.size() == 1 && selectedSStreams[0] == 'a') {
					//nothing todo...
				}
				else {
					if (idsOfSelectedStreams(selectedSStreams, sIdsVector) < 0) {
						std::cerr << "bad subpicture streams: " << selectedSStreams << std::endl;
						nok = true;
					}
					try {
						if (setSubpictureStreamsSelected(t, sIdsVector) < 0) {
							std::cerr << "bad subpicture stream: " << selectedSStreams << std::endl;
							nok = true;
						}
					}
					catch (int err) {
						std::cout << "subpicture stream: " << err << " does not exist" << std::endl;
						nok = true;
					}		
				}	

			}while(nok);
		}

		// Create XML file and Color Palette
		if(destination[0] != '/' && destination[0] != '.') 
		{
			std::string tmp = destination;
			destination = getcwd(currentPath, sizeof(currentPath));
			destination = destination + "/" + tmp;
		}
		std::ostringstream xml;
		std::ostringstream palette;

		std::string absPathXml = destination + "/" + xmlDvdauthor;
		std::string absPathPalette = destination + "/" + paletteFile;

		std::ofstream xmlF(absPathXml.c_str());
		if (! xmlF.is_open()) 
		{
			std::cerr << std::endl << "Could not create file " << xmlDvdauthor << " in " << destination << std::endl;
			perror ("");
			exit(-1);
		}
		std::ofstream paletteF(absPathPalette.c_str());
		
		if (! paletteF.is_open()) 
		{
			std::cerr << std::endl << "Could not create file " << paletteFile << " in " << destination << std::endl;
			perror ("");
			exit(-1);
		}

		ReAuthorXML::writeXml(dvd, source, destination, xml, palette);

		std::cout << std::endl;
		std::cout << "Writing file " << destination << "/" << xmlDvdauthor << std::endl;
		xmlF << xml.str();
		std::cout << "Writing file " << destination << "/" << paletteFile << std::endl;
		paletteF << palette.str();
		std::cout << std::endl;

		xmlF.close();
		paletteF.close();

		int pid, retPid, status;

		if ((pid = fork())==0) 
		{
			std::cout << std::endl << "Running: dvdauthor -x " << absPathXml << std::endl;
			if(execlp("dvdauthor","dvdauthor", "-x", absPathXml.c_str() ,NULL) == -1)
			{
				perror("Error running dvdauthor:");
				exit(-1);
			}
		}

		if( (retPid = wait( &status )) < 0 )
		{
			perror("Error while waiting for dvdauthor:");
			exit(-1);
		}


		if ((pid = fork())==0) {
			std::cout << std::endl << "Running: dvdauthor -T -o " << destination << std::endl;

			if(execlp("dvdauthor","dvdauthor", "-T", "-o", destination.c_str() ,NULL) == -1){
				perror("Error running dvdauthor:");
				exit(-1);
			}
		}
		if( (retPid = wait( &status )) < 0 )
		{
			perror("Error while waiting for dvdauthor:");
			exit(-1);
		}

		std::cout << std::endl << "new DVD stored in " << destination << std::endl;
		std::cout << "now you can remove the files: " << std::endl;
		std::cout << absPathXml << std::endl << absPathPalette << std::endl << destination << "/injectionFile" << std::endl;
	}
	return 1;
}

int idsOfSelectedStreams(std::string& selectedStreams, std::vector<int>& idsVector) 
{
	int count = 0;
	std::vector<std::string> strVector;

	if (StringUtils::splitString(selectedStreams, ",", strVector) > 0) 
	{
		for (int j=0; j < strVector.size(); j++) 
		{
			int a = atoi(strVector.at(j).c_str());
			if( a != 0) 
			{
				idsVector.push_back(a);
				count++;
			}
			else
				return -1;
		}
	}
	else 
	{
		int a = atoi(selectedStreams.c_str());
		if (a != 0) 
		{
			idsVector.push_back(a);
			count++;
		}
		else
			return -2;
	}
	return count;
}

int setAudioStreamsSelected(Title& t, std::vector<int>& idsVector)  
{
	int count = 0;

	for (int j=1; j <= t.getNumberOfAudioStreams(); j++) 
	{
		bool found = false;
		AudioStream& aStream = t.getAudioStream(j);

		for (int k=0; k < idsVector.size(); k++) 
		{
			try 
			{	
				AudioStream aStreamSelected = t.getAudioStream(idsVector.at(k));
				if (aStream.getId() == aStreamSelected.getId()) 
				{
					found = true;
					//break;
				}
			} 
			catch (std::out_of_range err) 
			{
				throw idsVector.at(k);
			}
		}

		if (!found)
			aStream.setStreamSelected(false);
		else 
		{
			aStream.setStreamSelected(true);
			count++;
		}
	}
	return count;
}

int setSubpictureStreamsSelected(Title& t, std::vector<int>& idsVector) {
	int count = 0;

	for (int j=1; j <= t.getNumberOfSubpictureStreams(); j++) 
	{
		bool found = false;
		SubpictureStream& sStream = t.getSubpictureStream(j);

		for (int k=0; k < idsVector.size(); k++) 
		{
			try 
			{
				SubpictureStream sStreamSelected = t.getSubpictureStream(idsVector.at(k));
				if (sStream.getId() == sStreamSelected.getId()) 
				{
					found = true;
					//break;
				}
			} 
			catch (std::out_of_range err) 
			{
				throw idsVector.at(k);
			}
		}
		
		if (!found)
			sStream.setStreamSelected(false);
		else 
		{
			sStream.setStreamSelected(true);
			count++;
		}
	}
	return count;
}

void print (std::string printFormat, DVD& dvd) 
{
	if (printFormat == "human") 
	{
		dvd.print();
		std::cerr << "Vamps Evaporate Faktor: " << (dvd.getTitle(dvd.getLongestTitle()).getSize() * 1.01) / dvd.getOutputSize() << std::endl;
	}
	else if (printFormat == "xml")
	{
		dvd.printXml();
	}
	else 
	{ 
		std::cerr << "Unknown print format: " << printFormat << std::endl;
		std::cerr << "Printing human readable format" << std::endl;
		print("human",dvd);
	}
}

void usage (void) 
{
	std::cerr << "usage: vanred [options]" << std::endl;

	std::cerr << std::endl << "Options:" << std::endl;
	std::cerr << "\t-i, " << std::endl;
	std::cerr << "\t--input <DVD> can be device or path, default: /dev/dvd" << std::endl << std::endl;

	std::cerr << "\t-o, " << std::endl;
	std::cerr << "\t--output <target directory>, default: current working directory " << std::endl << std::endl;

	std::cerr << "\t-S, " << std::endl;
	std::cerr << "\t--output-size <size in MB>, default: 4300. Do not use values above 4460 for a DVD5 target size" << std::endl << std::endl;

	std::cerr << "\t-t, " << std::endl;  
	std::cerr << "\t--title <title id>, default: vanred will try to calculate the longest title" << std::endl << std::endl;
	std::cerr << "\t-a, " << std::endl;
	std::cerr << "\t--audio <a1,a2,a3,...| a (all)| n (none)>, default: vanred will ask you which streams do you want to keep" << std::endl << std::endl;
	std::cerr << "\t-s, " << std::endl;
	std::cerr << "\t--subpicture <s1,s2,s3,...| a (all) | n (none)>, default: vanred will ask you which streams do you want to keep" << std::endl << std::endl;

	std::cerr << "\t-r, " << std::endl;
	std::cerr << "\t--reading <simple|extended>, default: simple" << std::endl;
	std::cerr << "\t-x, " << std::endl;
	std::cerr << "\t--xml <XML file name>, default: dvdauthor.xml" << std::endl << std::endl;
	std::cerr << "\t-h, " << std::endl;
	std::cerr << "\t--help, print this message" << std::endl << std::endl;
	std::cerr << "\t-I, " << std::endl;
	std::cerr << "\t--information <human | xml> , print only information and exit" << std::endl << std::endl;

	std::cerr << std::endl << "Examples:" << std::endl;

	std::cerr << "\tPrint this help:" << std::endl;
	std::cerr << "\tvanred -h" << std::endl << std::endl;

	std::cerr << "\tRead from /dev/dvd and store config files and the new DVD structure in the current working directory:" << std::endl;
	std::cerr << "\tvanred" << std::endl << std::endl;

	std::cerr << "\tAs above but copy only the first and the third audio stream and the first subpicture stream:" << std::endl;
	std::cerr << "\tvanred -a 1,3 -s 1" << std::endl << std::endl;

	std::cerr << "\tCopy all audio streams and no subpicture streams:" << std::endl;
	std::cerr << "\tvanred -a a -s n" << std::endl << std::endl;

	std::cerr << "\tCopy all audio streams and all subpicture streams:" << std::endl;
	std::cerr << "\tvanred -a a -s a" << std::endl << std::endl;

	std::cerr << "\tRead simple (fast, do not analyze each stream):" << std::endl;
	std::cerr << "\tvanred -r simple" << std::endl << std::endl;

	std::cerr << "\tRead extended (slower, analyze size of each stream):" << std::endl;
	std::cerr << "\tvanred -r extended" << std::endl << std::endl;

	std::cerr << "\tRead from /dev/hdd and store config files and the new DVD structure in /mnt/multimedia/DVD:" << std::endl;
	std::cerr << "\tvanred -i /dev/hdd -o /mnt/multimedia/DVD" << std::endl << std::endl;

	std::cerr << "\tSet the target size of the new DVD struckture to 4400 MB:" << std::endl;
	std::cerr << "\tvanred -S 4400 " << std::endl << std::endl;
	std::cerr << "\tRead from /dev/dvd an save information about the DVD Strukture in file.xml:" << std::endl;
	std::cerr << "\tvanred -I xml > file.xml" << std::endl << std::endl;

	exit(-1);
}

void version () 
{
	std::cerr << "Version: VanReD-" << VERSION << std::endl;
	exit(-2);
}
