CFLAGS += -O3 -D_DRAW -mavx2
CC = g++
SFMLFLAGS += -lsfml-graphics -lsfml-window -lsfml-system

all: v1 v2 v3

v1: obj/1.o
	$(CC) -o v1 obj/1.o $(SFMLFLAGS)

v2: obj/2.o
	$(CC) -o v2 obj/2.o $(SFMLFLAGS)

v3: obj/3.o
	$(CC) -o v3 obj/3.o $(SFMLFLAGS)

obj/1.o: 1.cpp 
	$(CC) -o obj/1.o 1.cpp -c $(CFLAGS)

obj/2.o: 2.cpp 
	$(CC) -o obj/2.o 2.cpp -c $(CFLAGS)

obj/3.o: 3.cpp 
	$(CC) -o obj/3.o 3.cpp -c $(CFLAGS)

clean:
	rm obj/*.o -f
	clear
	
.PHONY: clean