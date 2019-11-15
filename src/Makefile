CC=g++

pool: main.o Vec2.o Ball.o TheGame.o
	$(CC) -o pool main.o Vec2.o Ball.o TheGame.o -lGL -lGLU -lglut -lm

main.o: main.cpp
	$(CC) -c main.cpp

Vec2.o: Vec2.cpp Vec2.h
	$(CC) -c Vec2.cpp Vec2.h

Ball.o: Ball.cpp Ball.h
	$(CC) -c Ball.cpp Ball.h

TheGame.o: TheGame.cpp TheGame.h
	$(CC) -c TheGame.cpp TheGame.h