CC = g++
CFLAGS = -c -Wall -Wextra -g
PORTAUDIO_DIR = portaudio
ESPEAK_DIR = espeak-1.48.04-source/src
LDFLAGS = -L $(ESPEAK_DIR)
IFLAGS = -I $(ESPEAK_DIR)
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS = $(patsubst %.cpp,%.d,$(SOURCES))
LIBS = -lespeak
EXECUTABLE = bin/espeak-sample

all: portaudio espeak $(DEPENDS) $(SOURCES) $(EXECUTABLE)
	#

portaudio:

	cd $(PORTAUDIO_DIR) && ./configure && $(MAKE)

espeak:
	cd $(ESPEAK_DIR) && $(MAKE) libespeak.dll PLATFORM_WINDOWS=1 LIB_AUDIO="-L ../../portaudio/lib/.libs -lportaudio -lwinmm"

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)
	cp $(PORTAUDIO_DIR)/lib/.libs/libportaudio-2.dll ./bin
	cp $(ESPEAK_DIR)/libespeak.dll ./bin

%.d:
	$(CC) $(IFLAGS) -MM $*.cpp > $*.d

%.o: %.cpp
	$(CC) $(IFLAGS) $(CFLAGS) -c $*.cpp

clean:
	rm -vf $(EXECUTABLE)
	find -name "*~" -exec rm -vf {} \;
	find -name "*.o" -exec rm -vf {} \;
	find -name "*.d" -exec rm -vf {} \; 

.PHONY: clean all espeak

#
# This line includes all the dependencies.
# There is one for each file.
# Check the *.d files after you run make
-include $(DEPENDS)
