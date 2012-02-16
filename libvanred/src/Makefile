OBJS= \
	dvdStream.o \
	dvdStreamSelectable.o \
	SubpictureStream.o \
	AudioStream.o \
	VideoStream.o \
	ColorPalette.o \
	DVD.o \
	Title.o \
	Pgc.o \
	Cell.o \
	DVDReader.o \
	SimpleDVDReader.o \
	ExtendedDVDReader.o \
	StringUtils.o \
	ReAuthorXML.o 

TARGET= libvanred.so.1.0
SOFTLINK=libvanred.so
SONAME= libvanred.so.1
TARGETPATH = /usr/local/lib
INCLUDE=/usr/local/include

CFLAGS=-O2 -fPIC -I../include
CC=g++ -g $(CFLAGS)

all: $(OBJS)
	$(CC) --shared -Wl,-soname,$(SONAME) -o $(TARGET) -ldvdread $(OBJS)
	@unlink $(SONAME) 2> /dev/null ; unlink $(SOFTLINK) 2> /dev/null; ln -s $(TARGET) $(SONAME); ln -s $(SONAME) $(SOFTLINK)

dvdStream.o: dvdStream.cpp
	$(CC) -c dvdStream.cpp

dvdStreamSelectable.o: dvdStreamSelectable.cpp
	$(CC) -c dvdStreamSelectable.cpp

SubpictureStream.o: SubpictureStream.cpp
	$(CC) -c SubpictureStream.cpp

AudioStream.o: AudioStream.cpp
	$(CC) -c AudioStream.cpp

VideoStream.o: VideoStream.cpp
	$(CC) -c VideoStream.cpp

ColorPalette.o: ColorPalette.cpp
	$(CC) -c $<
DVD.o: DVD.cpp
	$(CC) -c DVD.cpp

Title.o: Title.cpp
	$(CC) -c Title.cpp

Pgc.o: Pgc.cpp
	$(CC) -c Pgc.cpp
	
Cell.o: Cell.cpp
	$(CC) -c Cell.cpp

DVDReaderFabrik.o: DVDReaderFabrik.cpp
	$(CC) -c DVDReaderFabrik.cpp
	
DVDReader.o: DVDReader.cpp
	$(CC) -c DVDReader.cpp

SimpleDVDReader.o: SimpleDVDReader.cpp
	$(CC) -c SimpleDVDReader.cpp

ExtendedDVDReader.o: ExtendedDVDReader.cpp
	$(CC) -c ExtendedDVDReader.cpp

StringUtils.o: StringUtils.cpp
	$(CC) -c StringUtils.cpp

ReAuthorXML.o: ReAuthorXML.cpp
	$(CC) -c ReAuthorXML.cpp

install: $(TARGET)
	install -v $(TARGET) $(TARGETPATH)
	ldconfig
	@ rm -rf $(INCLUDE)/libvanred
	@ mkdir $(INCLUDE)/libvanred
	@ cp -rfv ../include/*.h $(INCLUDE)/libvanred
	@ cd $(TARGETPATH)
	@ unlink $(SONAME) 2> /dev/null ; unlink $(SOFTLINK) 2> /dev/null; ln -s $(TARGET) $(SONAME); ln -s $(SONAME) $(SOFTLINK)

uninstall:
	@ rm -rfv $(INCLUDE)/libvanred
	@ rm -v $(TARGETPATH)/$(TARGET)
	unlink $(TARGETPATH)/$(SONAME)
	unlink $(TARGETPATH)/$(SOFTLINK)
clean:
	@ rm -fv *.o *~ $(TARGET) $(SONAME) $(SOFTLINK) 2> /dev/null

