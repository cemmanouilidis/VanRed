#include "ExtendedDVDReader.h"
#include <string.h>
#include <stdio.h>


ExtendedDVDReader::ExtendedDVDReader(std::string dvdDevice) : DVDReader(dvdDevice) 
{
	startBlock = 0;
	stopBlock = 0;
	nrOfAudio = 0;
	nrOfSubs = 0;
	streamCount = 0;
	nonvideoSize = 0;

}

Title ExtendedDVDReader::readTitle(int titleId) 
{
	vtsi_mat_t* vtsiMat;
	pgcit_t* vtsPgcit;
	video_attr_t *videoAttr;
	audio_attr_t *audioAttr;
	subp_attr_t *subpAttr;
	pgc_t *pgc;
	vts_ptt_srpt_t *vtsPttSrpt;
	uint16_t vtsTtn,titleSetNr;
	ifo_handle_t *vtsFile;
	char playbackTime[9];
//	double nonvideoSize = 0, overheadSize = 0, videoSize = 0;
	double overheadSize = 0, videoSize = 0;

	std::string langCode;
	std::string language;

	if (ifo[ifoZero->tt_srpt->title[titleId-1].title_set_nr]->vtsi_mat)
		vtsiMat = ifo[ifoZero->tt_srpt->title[titleId-1].title_set_nr]->vtsi_mat;

	vtsPgcit  = ifo[ifoZero->tt_srpt->title[titleId-1].title_set_nr]->vts_pgcit;
	vtsTtn = ifoZero->tt_srpt->title[titleId-1].vts_ttn;
	titleSetNr = ifoZero->tt_srpt->title[titleId-1].title_set_nr;
	pgc = vtsPgcit->pgci_srp[ifo[titleSetNr]->vts_ptt_srpt->title[vtsTtn-1].ptt[0].pgcn - 1].pgc;
	int chapts = ifoZero->tt_srpt->title[titleId-1].nr_of_ptts;


	int nrOfAc3 = 0;
	int nrOfDts = 0;
	int nrOfLpcm = 0;
	int nrOfMpgAudio = 0;

	// Titles
	Title title(titleId);
	Pgc pgcObj;

	title.setVtsNumber(titleSetNr);
	title.setVtsTitleNumber(vtsTtn);
	title.setNumberOfChapter(chapts);
	//std::cout << "VTS Title Number is: " << vtsTtn << std::endl;	

	//Playback Time	
	snprintf(playbackTime, sizeof(playbackTime), "%02x:%02x:%02x", pgc->playback_time.hour, pgc->playback_time.minute, pgc->playback_time.second);
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
		for(j = 0; j < 16; j++) 
		{
			colorPalette.setColor(j,pgc->palette[j]);
		}
		pgcObj.addColorPalette(colorPalette);
	}
	title.addPgc(pgcObj);

	double playbackTimeSec = dvdtime2msec(&pgc->playback_time) / 1000;
	streamanalyze(titleId, titleSetNr, playbackTimeSec);

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


	overheadSize = (stopBlock-startBlock) * 2048 / 50;   // rough guess about space wasted by padding, pack headers, 
	nonvideoSize += overheadSize;                                          // sum of all non video stuff
	videoSize = ((double) (stopBlock-startBlock) * 2048) - nonvideoSize;  // video size = vob size - non video stuff
	videoSize = videoSize / 1048576;

	videoStream.setSize((long unsigned int)videoSize);
	title.addVideoStream(videoStream);


	// Audio Streams
	for (int j=0; j < nrOfAudio; j++) 
	{
		audioAttr = &vtsiMat->vts_audio_attr[j];
		static AudioStream audioStream;
		double streamsize = 0;

		int nrOfId;

		int audioId = j+1;
		switch (audioAttr->audio_format) 
		{
			case 0:
				nrOfId = ++nrOfAc3;
				break;
			case 2:
			case 3:
				nrOfId = ++nrOfMpgAudio;
				break;
			case 4:
				nrOfId = ++nrOfLpcm;
				break;
			case 6:
				nrOfId = ++nrOfDts;
				break;
			default:
				nrOfId = 0;
		}

		for (int i=0;i<streamCount;i++) 
		{
			//std::cout << "Stream: " << i << std::endl;
			switch (streamdataList[i]->type) 
			{
				case LPCM_AUDIO :
				case MPEG_AUDIO :
				case AC3_AUDIO  :
				case DTS_AUDIO  :
					if (streamdataList[i]->type == audioAttr->audio_format && streamdataList[i]->nrOfIdType == nrOfId)
					{
						streamsize = streamdataList[i]->size/1048576; // 1048576 ->convert to MB
					}
					break;
				default:
					break;
			}
		}	

		if (streamsize > 0 ) 
		{
			std::ostringstream lang;
			audioStream.setSize((long unsigned int)streamsize);
			audioStream.setId(audioId);

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
	}

	// Subpicture Streams
	for (int j=0; j< nrOfSubs; j++) 
	{
		subpAttr = &vtsiMat->vts_subp_attr[j];
		static SubpictureStream subpictureStream;
		double streamsize = 0;
		int subpictureId = j+1;

		for (int i=0;i<streamCount;i++) 
		{
			switch( streamdataList[i]->type ) 
			{
				case MPEG_VIDEO :
					break;
				case SUBPICTURE :
					if (streamdataList[i]->id-0x1F==subpictureId) 
					{
						streamsize = streamdataList[i]->size / 1048576;
					}
					break;
				case LPCM_AUDIO :
				case MPEG_AUDIO :
				case AC3_AUDIO  :
				case DTS_AUDIO  : 
					break;
				default:
					break;
			}
		}	
		if (streamsize > 0 ) 
		{
			std::ostringstream lang;
			subpictureStream.setId(subpictureId);
			subpictureStream.setSize((long unsigned int)streamsize);

			lang << (char) (subpAttr->lang_code>>8) << (char) (subpAttr->lang_code & 0xff);

			langCode = lang.str();

			subpictureStream.setLanguage(getLanguageFromCode(langCode));
			subpictureStream.setLangCode(langCode);

			title.addSubpictureStream(subpictureStream);
		}
	}

	//Calc the cell sizes
	tt_srpt_t*	ttSrpt;
	cell_playback_t cpb;

	//std::cerr << "CalcTitleSize()" << std::endl;
	int ttn, tsNr, pgcId;
	tsNr = title.getVtsNumber();    

	ttSrpt = ifoZero->tt_srpt;
	ifo_handle_t* ifoTitle = ifo[ifoZero->tt_srpt->title[titleId-1].title_set_nr];

	if( ifoTitle->vtsi_mat) 
	{
		ttn        = title.getVtsTitleNumber();
		vtsPttSrpt = ifoTitle->vts_ptt_srpt;

		pgcId      = vtsPttSrpt->title[ttn - 1].ptt[0].pgcn; 
		pgc        = ifoTitle->vts_pgcit->pgci_srp[pgcId - 1].pgc;

		for (int i = 1; i <= title.getPgc().getNumberOfCells(); i++)
		{
			int cellSize = 0;
			cpb = pgc->cell_playback[i-1];
			cellSize = (cpb.last_sector - cpb.first_sector + 1) / 512;
			//std::cerr << "Cell " << i << " size: " << cellSize << " MB" << std::endl;
			title.getPgc().getCell(i).setSize(cellSize);
		}
	}
	
	return title;
}


void ExtendedDVDReader::streamanalyze(int title, int titleSet, double playbackTimeSec) {
	dvd_file_t *vob;
	//double nonvideoSize = 0;

	if(getStartStopBlocks(title, playbackTimeSec)) {
		vob = DVDOpenFile(dvd, titleSet, DVD_READ_TITLE_VOBS);
		int i = 0;
		if( !vob ) 
		{
			std::cerr << "ERROR: opening vobs for title " << tsNr << " failed" << std::endl;
			perror("DVDOpenFile");
			DVDClose(dvd);
			exitReader(-5);
		}
		streamCount = probeDvd(vob); // probe some blocks to find all streams


		for (i=0;i<streamCount;i++) 
		{
			switch( streamdataList[i]->type ) 
			{
				case MPEG_VIDEO : 
					streamdataList[i]->size = 0;
					break;
				case SUBPICTURE : 
					streamdataList[i]->size = (double)(stopBlock-startBlock) * 2048 / 1450;
					nrOfSubs++;
					break;
				case LPCM_AUDIO :
				case MPEG_AUDIO :
				case AC3_AUDIO  :
				case DTS_AUDIO  : 
					streamdataList[i]->size = streamdataList[i]->bitrate /8 * playbackTimeSec;
					nrOfAudio++;
					break;
			}
			nonvideoSize += streamdataList[i]->size;

		}

		DVDCloseFile(vob);
	}

}

// stolen from streamanalyze
int ExtendedDVDReader::getStartStopBlocks (int title, double& playbackTimeSec) 
{
	int ok = 0;
	ifo_handle_t   *ifoTitle;
	ifo_handle_t   *ifoMain = ifoZero;
	vts_ptt_srpt_t *vtsPttSrpt;
	tt_srpt_t      *ttSrpt;
	pgc_t          *pgc, *fullPgc;
	int titleid =  title-1, ttn, pgcId, pgn, startCell, endCell, i;
	int chapstart = 0;
	int chapstop = 0;


	ttSrpt = ifoMain->tt_srpt;
	tsNr = ttSrpt->title[ titleid ].title_set_nr;                       // save title set nr

	ifoTitle = ifoOpen(dvd, tsNr);      // open title ifo file
	if( !ifoTitle ) 
	{
		std::cerr << "streamanalyze: Error opening IFO file for title " << title << std::endl;
		perror ("ifoOpen");
		ifoClose(ifoMain);
		exitReader(-4);
	}

	if( ifoTitle->vtsi_mat) 
	{                               		
		if( chapstart == 0 ) chapstart = 1;                                    // set first chapter if unset
		if( chapstop == 0 ) chapstop = ttSrpt->title[ titleid ].nr_of_ptts;   // set last chapter if unset
		ttn          = ttSrpt->title[ titleid ].vts_ttn;                   // mostly stolen from transcode
		vtsPttSrpt = ifoTitle->vts_ptt_srpt;

		pgcId       = vtsPttSrpt->title[ttn - 1].ptt[chapstart-1].pgcn;
		pgn          = vtsPttSrpt->title[ttn - 1].ptt[chapstart-1].pgn;
		pgc          = ifoTitle->vts_pgcit->pgci_srp[pgcId - 1].pgc;
		fullPgc     = pgc;

		startCell   = pgc->program_map[pgn - 1] - 1;
		startBlock  = pgc->cell_playback[startCell].first_sector;         // first block of start cell

		if( chapstop == ttSrpt->title[ titleid ].nr_of_ptts ) 
		{           // if stop chapter = last chapter
			endCell     = pgc->nr_of_cells -1;
			stopBlock   = pgc->cell_playback[endCell].last_sector;
		}
		else 
		{
			pgcId       = vtsPttSrpt->title[ttn - 1].ptt[chapstop].pgcn;
			pgn          = vtsPttSrpt->title[ttn - 1].ptt[chapstop].pgn;
			pgc          = ifoTitle->vts_pgcit->pgci_srp[pgcId - 1].pgc;
			endCell     = pgc->program_map[pgn - 1] - 2;
			stopBlock   = pgc->cell_playback[endCell].last_sector;         // last block of end cell
		}

		if( (chapstart == 1) && (chapstop == ttSrpt->title[ titleid ].nr_of_ptts) ) 
		{   // all chapters selected ?
			//allready calculated
		}                        // runtime = movie runtime
		else 
		{                                                                             // only some chapters selected ?
			playbackTimeSec = 0;
			for( i=startCell ; i<=endCell ; i++ ) 
			{                                          // sum runtime of selected cells
				playbackTimeSec += dvdtime2msec(&pgc->cell_playback[i].playback_time) / 1000; 
			}
		}
		ok = 1;	
	}

	ifoClose(ifoTitle);	
	return(ok);
}

// also stolen from streamanalyze
int ExtendedDVDReader::probeDvd (dvd_file_t *vob) 
{
	int x, y, z, found, count = 0;
	unsigned char buffer[DVD_VIDEO_LB_LEN], *searchptr;
	uint8_t packetType;
	struct streamdata *newData;
	uint32_t maxblocks = stopBlock, i;
	int debug = 0;    

	int nrOfAc3 = 0;
	int nrOfDts = 0;
	int nrOfLpcm = 0;
	int nrOfMpgAudio = 0;

	if( (stopBlock-startBlock) > PROBEBLOCKS ) 
	{
		maxblocks = startBlock + PROBEBLOCKS; 
	}

	for( i=startBlock ; i<maxblocks ; i++ ) 
	{                                  // read 10mb from dvd
		if( !DVDReadBlocks(vob, i, 1, buffer) ) 
		{
			std::cerr << "ERROR probing for streams" << std::endl;
			perror("");
		}

		found = 0;
		packetType = buffer[17];                                                // get stream id
		if( packetType == 0xBD ) 
		{                                              // if private stream
			packetType = buffer[23+buffer[22]];                                     // get sub-id
			if(debug) 
				printf ("\rfound stream 0x%x",  packetType);
		}
		// check for video, audio, subpicture
		if( ((packetType >= 0xE0) && (packetType <= 0xEF)) ||
				((packetType >= 0x20) && (packetType <= 0x3F)) ||
				((packetType >= 0xC0) && (packetType <= 0xCF)) ||
				((packetType >= 0xA0) && (packetType <= 0xAF)) ||
				((packetType >= 0x80) && (packetType <= 0x8F)) ) {
			for( x=0 ; x<count ; x++ ) 
			{                                          // scan if we already know this stream
				if( streamdataList[x]->id == packetType ) 
				{   // makes sure, we only get headers at stream start
					found = 1;
					break;
				}
			}
			if(!found ) 
			{                                                  // unknown stream, add to list
				newData = new streamdata;

				newData->id = packetType;
				newData->nrOfIdType = 0;

				if( (packetType >= 0xE0) && (packetType <= 0xEF) ) 
				{
					newData->type = MPEG_VIDEO;
					newData->bitrate = 0;                                                      // dummy entry for video
				}
				else if( (packetType >= 0xC0) && (packetType <= 0xCF) ) 
				{
					newData->type = MPEG_AUDIO;
					newData->bitrate = 0;
					newData->nrOfIdType = ++nrOfMpgAudio;

					searchptr = buffer +17;
					searchptr = (unsigned char*)memchr(searchptr, 0xFF, searchptr-buffer);      // search for 1st byte of MPA Syncword 
					while( searchptr != NULL ) 
					{
						if( searchptr <= (buffer+2045) ) 
						{                                       // check for search hit at end of buffer
							if( (searchptr[1] & 0xF0) == 0xF0 ) 
							{                                 // if next 4 bits set, we found the sync word
								x = (searchptr[1] & 0x08) >> 3;                                    // version id
								y = (searchptr[1] & 0x06) >> 1;                                    // layer code
								z = (searchptr[2] & 0xF0) >> 4;                                    // bitrate code
								newData->bitrate = mpaBitrateIndex[x][y][z] * 1024;
								if( debug) 
									std::cerr << "MPEGAUDIO bitrate: %d" << mpaBitrateIndex[x][y][z] << std::endl;
								break;
							}
						}
						searchptr++;
						searchptr = (unsigned char*)memchr(searchptr, 0xFF, searchptr-buffer);
					}
					if( newData->bitrate == 0 ) 
					{                                              // no syncword found, try next packet
						std::cerr << "unable to get MPEG-Audio bitrate" << std::endl;
						continue;
					}
				}
				else if( (packetType >= 0x80) && (packetType <= 0x87) ) 
				{
					y = buffer[22] + buffer[buffer[22]+26] + (unsigned char)26;                 // calc start offsets
					if( (buffer[y] != 0x0B) || (buffer[y+1] != 0x77) ) 
					{                        // check for AC3 Syncword
						continue; 
					}
					newData->type = AC3_AUDIO;
					newData->nrOfIdType = ++nrOfAc3;
					x = buffer[y + 4] & 0x3F;                                                   // 6 lowest bits
					newData->bitrate = ac3BitrateIndex[ x>>1 ] * 1024;                       // shift index >> 1 (same as /2 )
					if( debug) 
						std::cerr << "AC3AUDIO bitrate: " << ac3BitrateIndex[ x>>1 ] << std::endl;
				}
				else if( (packetType >= 0x88) && (packetType <= 0x8F) ) 
				{
					newData->type = DTS_AUDIO;
					newData->nrOfIdType = ++nrOfDts;

					x = (buffer[buffer[22] + 35] & 0x3) << 3;                                   // 3 lowest bits of byte
					x = x | ((buffer[buffer[22] + 36] & 0xE0) >> 5);                            // 2 highest bits of next byte
					newData->bitrate = dtsBitrateIndex[x] * 1000;                            // kbit * 1000 fits better to filesize, so what ...
					if( debug) 
						std::cerr << "DTSAUDIO bitrate: " << dtsBitrateIndex[x] << std::endl;
				}
				else if( (packetType >= 0xA0) && (packetType <= 0xAF) ) 
				{
					newData->type = LPCM_AUDIO;
					newData->nrOfIdType = ++nrOfLpcm;

					newData->bitrate = 2000000;                                                // FIXME - hard coded, no detection yet
				}
				else if( (packetType >= 0x20) && (packetType <= 0x3F) ) 
				{
					newData->type = SUBPICTURE;
					newData->bitrate = 0;                                                      // dummy entry for subpicture
				}

				streamdataList[count] = newData;
				count++;
			}
		}
	}
	return(count);
}
