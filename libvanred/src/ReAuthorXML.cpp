#include "ReAuthorXML.h"

void ReAuthorXML::writeXml(DVD& dvd, std::string source, std::string destination, std::ostringstream& xml, std::ostringstream& palette) 
{
	// Generating XML file for the longest title
	xml << "<dvdauthor dest=\"" << destination << "\">" << std::endl;
	xml << "  <titleset>" << std::endl;

	Title title = dvd.getTitle(dvd.getLongestTitle());
	xml << "    <titles>" << std::endl;

	// Print Video 
	VideoStream v = title.getVideoStream();
	xml << "      " << v.toXml() << std::endl;

	std::vector<AudioStream> a = title.getSelectedAudioStreams();
	std::ostringstream aStreams;

	for (int j = 0; j < a.size(); j++) 
	{
		xml << "      " << a.at(j).toXml() << std::endl;
		aStreams << a.at(j).getId(); 
		if (a.size()-1 != j)
			aStreams << ",";
	}

	std::vector<SubpictureStream> s= title.getSelectedSubpictureStreams();
	std::ostringstream sStreams;

	for (int j = 0; j < s.size(); j++) 
	{
		xml << "      " << s.at(j).toXml() << std::endl;
		sStreams << s.at(j).getId(); 
		if (s.size()-1 != j)
			sStreams << ",";
	}

	xml << "      " << "<pgc palette=\"" << destination << "/palette.raw\">" << std::endl;

	float titleSize = title.getSize();
	float evaporateFaktor = titleSize / dvd.getOutputSize();
	std::cout << "Evaporate Faktor: " << evaporateFaktor << std::endl;

	int vts = title.getVtsNumber();
	int pgc = title.getVtsTitleNumber();

	for (int cell = 1; cell <= title.getPgc().getNumberOfCells(); cell++) 
	{

		if (title.getPgc().getCell(cell).getSize()*1048576 > 0) //create <vob> only if the size of vob file is GT 0
		{
			xml << "        " << "<vob file=\"play_cell " << source << " " << vts << " " << pgc << " " << std::setw(3) << std::left << cell; 
			xml << " |"; 

			//if evaporate faktor is GT 1 or evaporate faktor is LT 1 but any audio or subs are selected to be removed, we have to call vamps
			if (evaporateFaktor > 1 || evaporateFaktor < 1 && (a.size() != title.getNumberOfAudioStreams() || s.size() != title.getNumberOfSubpictureStreams()))
			{
				xml << " vamps";
				if (evaporateFaktor > 1)
					xml << " -E  " << evaporateFaktor; 

				if (a.size() > 0)
					xml << " --audio " << aStreams.str();
				if (s.size() > 0)
					xml <<  " --subpictures " << sStreams.str();

				//calculate the size of the stream, vamps option since V0.98
				xml << " --inject " << destination << "/injectionFile -S " << title.getPgc().getCell(cell).getSize()*1048576;
				xml <<  " |"; 
			}

			xml << "\"";

			if (title.getPgc().getCell(cell).isStartOfChapter())
			{
				xml << " chapters=\"0\"";
			}
			else
			{
				xml << "             ";
			}

			xml << "/>" << std::endl;
		}
	}

	xml << "      " << "</pgc>" << std::endl;
	xml << "    </titles>" << std::endl;
	xml << "  </titleset>" << std::endl;
	xml << "</dvdauthor>" << std::endl;

	//Generating palette with color information
	ColorPalette p = title.getPgc().getColorPalette();

	for (int j = 0; j < 16; j++)
	{
		palette << std::hex << std::setw(8) << std::setfill('0') << p.getColor(j) << std::endl;
	}
}
