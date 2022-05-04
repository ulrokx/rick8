CXX = g++
OBJS = Display.o CHIP8.o Keypad.o
CXXFLAGS = `sdl2-config --cflags` -Wall`
LFLAGS = `sdl2-config --libs`
.DELETE_ON_ERROR:
all: $(OBJS)
	$(CXX) main.cpp $(OBJS) $(CFLAGS) $(LFLAGS) -o main
	make clean

$(OBJS): %.o: %.cpp
	$(CXX) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o