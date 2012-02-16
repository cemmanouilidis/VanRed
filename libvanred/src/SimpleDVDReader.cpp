#include "SimpleDVDReader.h"
#include <stdio.h>

SimpleDVDReader::SimpleDVDReader(std::string dvdDevice) : DVDReader(dvdDevice) {

}

Title SimpleDVDReader::readTitle(int titleId) 
{
	vtsi_mat_t* vtsiMat;
	pgcit_t* vtsPgcit;
	video_attr_t* videoAttr;
	audio_attr_t* audioAttr;
	subp_attr_t* subpAttr;
	pgc_t* pgc;
	vts_ptt_srpt_t* vtsPttSrpt;
	uint16_t vtsTTN, titleSetNr;
	ifo_handle_t* vtsFile;
	char playbackTime[9];
	std::string langCode;
	std::string language;

	if (ifo[ifoZero->tt_srpt->title[titleId-1].title_set_nr]->vtsi_mat)
		vtsiMat = ifo[ifoZero->tt_srpt->title[titleId-1].title_set_nr]->vtsi_mat;

	vtsPgcit  = ifo[ifoZero->tt_srpt->title[titleId-1].title_set_nr]->vts_pgcit;
	vtsTTN = ifoZero->tt_srpt->title[titleId-1].vts_ttn;
	titleSetNr = ifoZero->tt_srpt->title[titleId-1].title_set_nr; 
	pgc = vtsPgcit->pgci_srp[ifo[titleSetNr]->vts_ptt_srpt->title[vtsTTN-1].ptt[0].pgcn - 1].pgc;
	int chapts = ifoZero->tt_srpt->title[titleId-1].nr_of_ptts;

	// Titles
	Title title(titleId);
	Pgc pgcObj;

	title.setVtsNumber(titleSetNr);
	title.setVtsTitleNumber(vtsTTN);
	title.setNumberOfChapter(chapts);
	//std::cerr << "VTS Title Number is: " << vts_ttn << std::endl;	

	//Playback Time	
	snprintf(playbackTime,sizeof(playbackTime), "%02x:%02x:%02x", pgc->playback_time.hour, pgc->playback_time.minute, pgc->playback_time.second);
	std::string str =(playbackTime);

	pgcObj.setPlaybackTime(str);
	pgcObj.setPlaytimeSec(dvdtime2msec(&pgc->playback_time) / 1000);

	//CELLs
	for (int i=1; i <= pgc->nr_of_cells; i++) 
	{
		Cell cell(i);
		cell.setPlaytimeSec(dvdtime2msec(&pgc->cell_playback[i-1].playback_time) / 1000 );
		pgcObj.addCell(cell);
		//std::cout << "Cell " << i << " Playbacktime: " << cell.getPlaytimeSec() << std::endl;
	}
	for (int i=0; i<pgc->nr_of_programs; i++)
	{	   
		int startCell = pgc->program_map[i];
		//std::cout << "Start Cell is: " << startCell << ", nrOfPrograms: " << (int) pgc->nrOfPrograms << std::endl;
		pgcObj.getCell(startCell).setIsStartOfChapter(true);
	}

	//Number of Programs
	pgcObj.setNumberOfPrograms(pgc->nr_of_programs);

	if(pgc->nr_of_programs != 0) 
	{
		int j;
		ColorPalette colorPalette;
		for(j = 0; j < 16; j++) {
			colorPalette.setColor(j,pgc->palette[j]);
		}
		pgcObj.addColorPalette(colorPalette);
	}
	title.addPgc(pgcObj);

	double playbackTimeSec = dvdtime2msec(&pgc->playback_time) / 1000;


	//Video Stream
	static VideoStream videoStream;
	videoAttr = &vtsiMat->vts_video_attr;
	videoStream.setMpegVersion(mpegVersion[videoAttr->mpeg_version]);
	videoStream.setFormat(videoFormat[videoAttr->video_format]);
	videoStream.setAspectRatio(aspectRatio[videoAttr->display_aspect_ratio]);		
	videoStream.setWidth(pictureSizeWidth[videoAttr->picture_size]);
	videoStream.setHeight(pictureSizeHeight[videoAttr->picture_size]);
	videoStream.setFramesPerSecond(framesPerS[(pgc->playback_time.frame_u & 0xc0) >> 6]);
	videoStream.setDf(permittedDf[videoAttr->permitted_df]);

	title.addVideoStream(videoStream);


	// Audio Streams
	for (int j=0; j < vtsiMat->nr_of_vts_audio_streams; j++) 
	{
		audioAttr = &vtsiMat->vts_audio_attr[j];
		static AudioStream audioStream;

		std::ostringstream lang;

		audioStream.setId(j+1);

		lang << (char) (audioAttr->lang_code>>8) << (char) (audioAttr->lang_code & 0xff);
		langCode = lang.str();

		audioStream.setLanguage(getLanguageFromCode(langCode));
		audioStream.setLangCode(langCode);
		audioStream.setLangExtension(langExtension[audioAttr->lang_extension]);
		audioStream.setFormat(audioFormat[audioAttr->audio_format]);
		audioStream.setQuantization(quantization[audioAttr->quantization]);
		audioStream.setFrequency(frequency[audioAttr->sample_frequency]);
		audioStream.setChannels(audioAttr->channels + 1);

		title.addAudioStream(audioStream);
	}

	// Subpicture Streams
	for (int j=0; j< vtsiMat->nr_of_vts_subp_streams; j++) 
	{
		subpAttr = &vtsiMat->vts_subp_attr[j];
		static SubpictureStream subpictureStream;

		std::ostringstream lang;
		subpictureStream.setId(j+1);

		lang << (char) (subpAttr->lang_code>>8) << (char) (subpAttr->lang_code & 0xff);

		langCode = lang.str();

		subpictureStream.setLanguage(getLanguageFromCode(langCode));
		subpictureStream.setLangCode(langCode);

		title.addSubpictureStream(subpictureStream);
	}

	//Calc the title size
	title.setSize(calcTitleSize(title));

	return title;
}


int SimpleDVDReader::calcTitleSize (Title& title) 
{
	int titleSize = 0;
	vts_ptt_srpt_t* vtsPttSrpt;
	tt_srpt_t*	ttSrpt;
	pgc_t*		pgc; 
	cell_playback_t cpb;

	//std::cerr << "CalcTitleSize()" << std::endl;
	int ttn, tsNr, pgcId;
	int titleId =  title.getId(); 

	tsNr = title.getVtsNumber();    

	ttSrpt = ifoZero->tt_srpt;
	ifo_handle_t* ifoTitle = ifo[ifoZero->tt_srpt->title[titleId-1].title_set_nr];


	if( ifoTitle->vtsi_mat) 
	{
		ttn          = title.getVtsTitleNumber();
		vtsPttSrpt = ifoTitle->vts_ptt_srpt;

		pgcId       = vtsPttSrpt->title[ttn - 1].ptt[0].pgcn; 
		pgc         = ifoTitle->vts_pgcit->pgci_srp[pgcId - 1].pgc;

		for (int i = 1; i <= title.getPgc().getNumberOfCells(); i++)
		{
			int cellSize = 0;
			cpb = pgc->cell_playback[i-1];
			cellSize = (cpb.last_sector - cpb.first_sector + 1) / 512;
			//std::cerr << "Cell " << i << " size: " << cellSize << " MB" << std::endl;
			title.getPgc().getCell(i).setSize(cellSize);
			titleSize += cellSize;
		}
	}

	//std::cerr << "Title " << titleId << " size: " << titleSize << " MB" << std::endl;
	return titleSize;
}
