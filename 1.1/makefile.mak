#--------------------------------------------
# makefile zur Erzeugung der .exe zur AKTERM
#--------------------------------------------
.PHONY: all clean

# Definition der Variablen
CXX := g++
CXXFLAGS := -std=c++11 -c -Wall -IS:\Datenban\Wetter\Programme\akterm\1.1\lib\\
LDFLAGS := -g
HEADERS := $(wildcard S\:\Datenban\Wetter\Programme\akterm\1.1\lib\*.h)
objs := main.o file_creation.o Date.o Coordinates.o Situation.o

default: akterm_v1-1.exe

# Ziel 1
akterm_v1-1.exe: $(objs)
	$(CXX) $(LDFLAGS) -o $@ $^

# Ziel 2
main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $<

# Ziel 3
file_creation.o: file_creation.cpp 
	$(CXX) $(CXXFLAGS) $<

# Ziel 4
Date.o: Date.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $<

# Ziel 5
Coordinates.o: Coordinates.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $<
	
# Ziel 5
Situation.o: Situation.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $<

# Alle *.o Dateien löschen
clean:
	del $(objs)

