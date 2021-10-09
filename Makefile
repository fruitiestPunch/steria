all:
	g++ src/*.cpp -std=c++14 -m64 -g -Wall -I include -lSDL2main -lSDL2 -lSDL2_image -o bin/debug/main && ./bin/debug/main