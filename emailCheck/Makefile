TARGET = test_app
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
CC = g++
INCLUDES = 
LIBS = 
CCFLAGS = -std=c++11

all:$(TARGET)
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(INCLUDES) $(LIBS)
$(OBJS): $(SRCS)
	$(CC) -c $< $(CCFLAGS)

clean:
	rm $(TARGET)
	rm $(OBJS)

.PHONY:all
