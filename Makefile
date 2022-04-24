CC = g++
CFLAGS = -Wall -g
objects = main.o CHIP8.o Display.o

main: $(objects)
	$(CC) $(CFLAGS) -o main $(objects)
	make cleanos

main.o: main.cpp Display.h CHIP8.h
	$(CC) -c $(CFLAGS) main.cpp

display.o: Display.h
	$(CC) -c $(CFLAGS) Display.h

chip8.o: CHIP8.h Display.h
	$(CC) -c $(CFLAGS) CHIP8.h

clean:
	rm -f main $(objects)

cleanos:
	rm -f $(objects)
