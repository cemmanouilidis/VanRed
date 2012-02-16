#ifndef  __DVD_CONST_H
#define __DVD_CONST_H

//Defs
#define MAXSTREAMS  50
#define PROBEBLOCKS 90720
#define MPEG_VIDEO  1

#define MPEG_AUDIO  3
#define AC3_AUDIO   0
#define DTS_AUDIO   6
#define LPCM_AUDIO  4

#define SUBPICTURE  5

#define ERR_DISC        128
#define ERR_MAIN_IFO	129
#define ERR_IFO         130

#define LONGEST_TITLE 9999

//Konstante
const std::string mpegVersion[] = {"MPEG1" , "MPEG2" };
const std::string videoFormat[] =  {"NTSC", "PAL"};
const std::string aspectRatio[] = {"4:3", "?", "?", "16:9"};
const unsigned short pictureSizeWidth[] = { 720, 704,  352, 352 };
const unsigned short pictureSizeHeight[] = { 576, 480, 480, 240 };
const std::string permittedDf[] = { "pan&scan+letterboxed", "only pan&scan" , "only letterboxed" , "not specified" };

const std::string audioFormat[] = {"AC3", "?", "MP1", "MP2", "LPCP", "SDDS", "DTS"};
const std::string quantization[] = { "16bps", "20bps", "24bps", "drc"};
const std::string langExtension[] = {"Unkown", "Normal Caption", "Audio for visually impaired", "Director's comments 1", "Director's comments 2"};
const std::string frequency[] = {"48khz", "96khz"};

const std::string streamTypeNames[] = {"MPEG Video", "MPEG Audio", "AC3 Audio", "DTS Audio", "LPCM-Audio", "Subpicture"};
//const double dvdSizeBytes = 4700000000.;
const double dvdSize = 4482;
const double framesPerS[4] = {-1.0, 25.00, -1.0, 29.97};

struct streamdata {            // data for 1 stream
	unsigned char id;              // stream id
	int nrOfIdType;		  // is it the first, second, third etc. stream of this streamtype? (e.g. AC3)
	long bitrate;                  // bitrate
	int type;                      // type of stream
	double size;                   // size of stream in bytes
};


// bitrate tables stolen from mplex
const unsigned int ac3BitrateIndex[32] = { 32,40,48,56,64,80,96,112,128,160,192,224,256,320,384,448,512,576,640,0,0,0,0,0,0,0,0,0,0,0,0,0 };
const unsigned int dtsBitrateIndex[32] = { 32,56,64,96,112,128,192,224,256,320,384,448,512,576,640,768,960,1024,1152,1280,1344,1408,1411,1472,1536,1920,2048,3072,3840,0,0,0 };

const unsigned int mpaBitrateIndex [2][4][16] =
{
	{ /* MPEG audio V2 */
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,0},
		{0,32,48,56,64 ,80 ,96 ,112,128,160,192,224,256,320,384,0},
		{0,8 ,16,24,32 ,64 ,80 ,56 ,64 ,128,160,112,128,256,320,0}
	},
	{ /* MPEG audio V1 */
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,0},
		{0,32,48,56,64 ,80 ,96 ,112,128,160,192,224,256,320,384,0},
		{0,32,40,48,56 ,64 ,80 ,96 ,112,128,160,192,224,256,320,0}
	}

};

#endif
