CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude -Iboost_1_61_0
LDFLAGS:=-lboost_system -pthread -lboost_thread
SRCS = $(wildcard ./src/*.cpp)
OBJS = $(patsubst ./src/%.cpp,./bin/%.o, $(SRCS))
DEPS := $(patsubst %.o,%.d, $(OBJS))
all: EchoClient
	g++ -o bin/echo.elf bin/connectionHandler.o bin/echoClient.o $(LDFLAGS)

EchoClient: bin/connectionHandler.o bin/echoClient.o

bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

.PHONY: clean
clean:
	rm -f bin/*
