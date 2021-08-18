CXX = g++
CXX_FLAGS = -Wall -g -std=c++03

RM = rm

BINARIES = vector rational_number matrix

SOURCES = \
		dictm.cpp\
		dictv.cpp\
		exception.cpp\
		matrix.cpp\
		rational_number.cpp\
		vector.cpp\
	
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))

all: $(BINARIES)


rational_number: rational_number.o $(OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $^

vector: vector.o $(OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $^

matrix: matrix.o $(OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $^


%.o : %.cpp %.h
	$(CXX) $(CXX_FLAGS) -c $<

clean:
	$(RM) -f $(OBJS) $(FILES) 

dist_clean: clean
	$(RM) -f $(BINARIES)

.PHONY: all clean dist_clean

