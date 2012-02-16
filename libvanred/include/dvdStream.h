#ifndef __DVD_STREAM_H_
#define __DVD_STREAM_H_

class dvdStream {
	unsigned int id;
	unsigned long size;
public:
	dvdStream(unsigned int id, unsigned long streamsize);
	dvdStream();

	unsigned int getId (void);
	void setId (unsigned int id);
	unsigned long getSize (void);
	void setSize(unsigned long streamsize);
	virtual	void print(void) = 0;
	virtual void printXml(void) = 0;
};
#endif
