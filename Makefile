# steps for git
# git add .
# git status
# git stage
# git commit -m "Message"
# git push -u origin master
debug:
	g++ src/*.cpp -std=c++14 -m64 -g -Wall -I include -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o bin/debug/main && ./bin/debug/main
release:
	g++ src/*.cpp -std=c++14 -m64 -O3 -Wall -I include -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o bin/release/main && ./bin/release/main