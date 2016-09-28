CC = g++
CFLAGS = -c -Wall -Wextra -g
PORTAUDIO_DIR = portaudio
ESPEAK_DIR = espeak/src
LDFLAGS = -L $(ESPEAK_DIR)
IFLAGS = -I $(ESPEAK_DIR)
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS = $(patsubst %.cpp,%.d,$(SOURCES))
LIBS = -lespeak -lportaudio
EXECUTABLE = bin/espeak

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	TARGET=linux
else
	TARGET=windows
endif

all: $(TARGET)
	#

linux: espeak-linux synthesizer-linux
	#

windows: portaudio espeak-windows synthesizer-windows
	#

portaudio:

	cd $(PORTAUDIO_DIR) && ./configure && $(MAKE)

espeak-linux:
	cd $(ESPEAK_DIR) && $(MAKE)
	
synthesizer-linux: $(DEPENDS) $(SOURCES) $(EXECUTABLE)
	
	cp $(ESPEAK_DIR)/libespeak.so ./bin
	cp -R espeak/espeak-data ./bin

espeak-windows:
	cd $(ESPEAK_DIR) && $(MAKE) libespeak.dll PLATFORM_WINDOWS=1 LIB_AUDIO="-L ../../portaudio/lib/.libs -lportaudio -lwinmm"
	
synthesizer-windows: $(DEPENDS) $(SOURCES) $(EXECUTABLE)
	
	cp $(PORTAUDIO_DIR)/lib/.libs/libportaudio-2.dll ./bin
	cp $(ESPEAK_DIR)/libespeak.dll ./bin
	cp -R espeak/espeak-data ./bin

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

%.d:
	$(CC) $(IFLAGS) -MM $*.cpp > $*.d

%.o: %.cpp
	$(CC) $(IFLAGS) $(CFLAGS) -c $*.cpp

clean:
	rm -vf $(EXECUTABLE)
	cd $(PORTAUDIO_DIR) && make clean
	cd $(ESPEAK_DIR) && make clean
	find -name "*~" -exec rm -vf {} \;
	find -name "*.o" -exec rm -vf {} \;
	find -name "*.d" -exec rm -vf {} \;
	
.PHONY: clean all espeak-windows synthesizer-windows espeak-linux synthesizer-linux portaudio

#
# This line includes all the dependencies.
# There is one for each file.
# Check the *.d files after you run make
-include $(DEPENDS)
