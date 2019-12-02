CC=g++

pool: main.o Vec2.o Ball.o putils.o
	$(CC) -o pool main.o Vec2.o Ball.o putils.o -lGL -lGLU -lglut -lm

main.o: main.cpp
	$(CC) -c main.cpp

Vec2.o: Vec2.cpp Vec2.h
	$(CC) -c Vec2.h Vec2.cpp 

Ball.o: Ball.cpp Ball.h
	$(CC) -c  Ball.h Ball.cpp

putils.o: putils.cpp putils.h
	$(CC) -c putils.h putils.cpp
