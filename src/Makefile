CC=g++
PROGRAM = pool

pool: main.o Vec2.o Ball.o putils.o Game.o image.o shconsts.o
	$(CC) -o $(PROGRAM) main.o Vec2.o Ball.o putils.o image.o -lGL -lGLU -lglut -lm -lpthread

main.o: main.cpp shconsts.h
	$(CC) -c main.cpp shconsts.h

Vec2.o: Vec2.cpp Vec2.h
	$(CC) -c Vec2.h Vec2.cpp 

Ball.o: Ball.cpp Ball.h
	$(CC) -c  Ball.h Ball.cpp

Game.o: Game.cpp Game.h
	$(CC) -c Game.h Game.cpp

putils.o: putils.cpp putils.h
	$(CC) -c putils.h putils.cpp

image.o: image.cpp image.h
	$(CC) -c image.h image.cpp

shconsts.o: shconsts.h
	$(CC) -c shconsts.h

run: $(PROGRAM)
	./$(PROGRAM)
