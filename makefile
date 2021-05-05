src=$(wildcard src/*.cpp)
obj=$(src:%.cpp=%.o)
cflags=-I./include

.PHONY: run clean

all: main

main: $(obj)
	$(CXX) -o $@ $^ $(cflags)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(cflags)

run: main
	./main

clean:
	rm -rf src/*.o
	rm -rf main
