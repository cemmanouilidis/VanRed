OBJS= vanred.o

TARGET= vanred
TARGET_PATH = /usr/local/bin

CFLAGS=-O2 -L../libvanred/src -lvanred -ldvdread
CC=g++ 

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

vanred.o: vanred.cpp
	$(CC) -c vanred.cpp -I../libvanred

install: $(TARGET)
	install -v $(TARGET) $(TARGET_PATH)

uninstall:
	@ rm -v $(TARGET_PATH)/$(TARGET)
clean:
	@ rm -fv *.o *~ $(TARGET)
