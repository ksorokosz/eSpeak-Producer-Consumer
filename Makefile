CC = g++
CFLAGS = -c -Wall -Wextra -g
LDFLAGS = -L espeak-1.48.04-source/src
IFLAGS = -I espeak-1.48.04-source/src
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS = $(patsubst %.cpp,%.d,$(SOURCES))
LIBS = -lespeak
EXECUTABLE = espeak-sample

all: $(DEPENDS) $(SOURCES) $(EXECUTABLE)
	#
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

%.d:
	$(CC) $(IFLAGS) -MM $*.cpp > $*.d

%.o: %.cpp
	$(CC) $(IFLAGS) $(CFLAGS) -c $*.cpp

clean:
	rm -f *.o *.d *~

.PHONY: clean all 

#
# This line includes all the dependencies.
# There is one for each file.
# Check the *.d files after you run make
-include $(DEPENDS)
