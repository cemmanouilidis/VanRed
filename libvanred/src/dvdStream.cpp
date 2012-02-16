#include "dvdStream.h"

dvdStream::dvdStream(unsigned int id, unsigned long streamsize) {
	this->id = id;
	this->size = streamsize;
}

dvdStream::dvdStream() {
	this->id = 0;
	this->size = 0;
}

unsigned int dvdStream::getId (void) {
	return id;
}
void dvdStream::setId (unsigned int id) {
	this->id = id;
}

unsigned long dvdStream::getSize (void){
	return size;
}

void dvdStream::setSize(unsigned long size) {
	this->size = size;
}
