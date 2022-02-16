build: # build the executables
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lSDL2_ttf -o game

run: # run the executable
	./game

clean: # delete the executable
	rm game