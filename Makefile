CC = g++
CFLAGS = -c -Wall -Wextra -g
ESPEAK_DIR = espeak-1.48.04-source/src
LDFLAGS = -L $(ESPEAK_DIR)
IFLAGS = -I $(ESPEAK_DIR)
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS = $(patsubst %.cpp,%.d,$(SOURCES))
LIBS = -lespeak
EXECUTABLE = espeak-sample

all: espeak $(DEPENDS) $(SOURCES) $(EXECUTABLE)
	#

espeak:
	cd $(ESPEAK_DIR) && $(MAKE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

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
