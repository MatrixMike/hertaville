
CXX=g++

TARGET=adcHackTest

INCLUDEDIR = ./

INCDIR  = $(patsubst %,-I%,$(INCLUDEDIR))

OPT = -O0
DEBUG = -g
WARN= -Wall

CXXFLAGS= $(OPT) $(DEBUG) $(WARN) $(INCDIR)

INC = rpiPWM1.h mmapGpio.h adcHack.h
SRC = rpiPWM1.cpp mmapGpio.cpp adcHack.cpp customAdc.cpp

OBJ = $(SRC:.cpp=.o)

all: $(OBJ)
	    $(CXX)  $(OBJ) -o $(TARGET)

%.o:%.cpp
	    $(CXX) $(CXXFLAGS)  -c $<  
		 
clean:
	    -rm *.o
		-rm $(TARGET)
