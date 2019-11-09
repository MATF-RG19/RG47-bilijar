CC=g++

pool: main.o Vec2.o Ball.o 
	$(CC) -o pool main.o Vec2.o Ball.o -lGL -lGLU -lglut -lm

main.o: main.cpp
	$(CC) -c main.cpp

Vec2.o: Vec2.cpp
	$(CC) -c Vec2.cpp

Ball.o: Ball.cpp
	$(CC) -c Ball.cpp 



	


