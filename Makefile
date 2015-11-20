CC	= gcc
CXX	= g++
SRCS	= Io.cpp IoFile.cpp PatternData.cpp main.cpp
BIN	= modplay
CCFLAGS = -Wall -O3
CXXFLAGS = -std=c++11 -Wall -O3
LDFLAGS =  -lm -lpthread 
#===============================================================================

OBJS = $(SRCS:.cpp=.o)
	
all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(OBJS) $(LDFLAGS)

.PHONY: clean
clean: 
	rm -f $(BIN) $(OBJS)


%.o: %.c
	$(CC) $(CCFLAGS) -c $<
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<
	

