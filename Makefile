main	:	main.cpp Maze.cpp Maze.h
	g++ -o main main.cpp Maze.cpp -g
run	: main
	./main