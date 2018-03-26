INCLUDES = 
LIBS = 
DEBUG = -g

CXX = g++
CXXFLAGS = -O2 -Wall -fno-use-linker-plugin -std=c++1y -static-libstdc++ -m32 -pthread -D_REENTRANT $(INCLUDES)

all: tickerplant generatetickerfile

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

.cpp:
	$(CXX) $(CXXFLAGS) $< -o $@ 

SRC = main.cpp TickerplantPrintOptimize.cpp TickerplantProductOptimize.cpp CmdProcessor.cpp CommandFactory.cpp Commands.cpp
OBJ = $(addsuffix .o, $(basename $(SRC)))

AUX_SRC = generatetickerfile.cpp
AUX_OBJ = $(addsuffix .o, $(basename $(AUX_SRC)))


tickerplant: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

generatetickerfile: $(AUX_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(AUX_OBJ)

clean:
	rm -f $(OBJ) $(AUX_OBJ) tickerplant generatetickerfile
