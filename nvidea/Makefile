TARGET = test_app
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
CC = g++
INCLUDES = 
LIBS = 
CCFLAGS = -g -std=c++11

all:$(TARGET)

#$(OBJS): $(SRCS)
	#$(CC) -c $< $(CCFLAGS)
DataStore.o : DataStore.cpp DataStore.h
	$(CC) -c $< $(CCFLAGS)
Parser.o : Parser.cpp Parser.h
	$(CC) -c $< $(CCFLAGS)
test.o : test.cpp
	$(CC) -c $< $(CCFLAGS)
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(INCLUDES) $(LIBS)


clean:
	rm $(TARGET)
	rm $(OBJS)

.PHONY:all
