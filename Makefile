CFLAGS += -O3 -D_DRAW -mavx2 -DREP_NUM=1
CC = g++
SFMLFLAGS += -lsfml-graphics -lsfml-window -lsfml-system

all: v1 v2 v3 v4

v1: obj/1.o obj/main.o
	$(CC) -o v1 obj/1.o obj/main.o $(SFMLFLAGS)

v2: obj/2.o obj/main.o
	$(CC) -o v2 obj/2.o obj/main.o $(SFMLFLAGS)

v3: obj/3.o obj/main.o
	$(CC) -o v3 obj/3.o obj/main.o $(SFMLFLAGS)

v4: obj/4.o obj/main.o
	$(CC) -o v4 obj/4.o obj/main.o $(SFMLFLAGS)

obj/1.o: src/1.cpp 
	$(CC) -o obj/1.o src/1.cpp -c $(CFLAGS)

obj/2.o: src/2.cpp 
	$(CC) -o obj/2.o src/2.cpp -c $(CFLAGS)

obj/3.o: src/3.cpp 
	$(CC) -o obj/3.o src/3.cpp -c $(CFLAGS)

obj/4.o: src/4.cpp 
	$(CC) -o obj/4.o src/4.cpp -c $(CFLAGS)

obj/main.o: src/main.cpp
	$(CC) -o obj/main.o src/main.cpp -c $(CFLAGS)

clean:
	rm obj/*.o -f
	clear
	
.PHONY: clean