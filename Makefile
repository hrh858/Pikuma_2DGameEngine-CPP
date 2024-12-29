build:
	clang++ -Wall -std=c++17 -I"./libs" src/*.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua -o 2dge

run:
	./2dge

clean:
	rm 2dge
