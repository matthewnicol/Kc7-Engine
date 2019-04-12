default: 
	c99 -o bin/chesseng -g -Wall -Werror -Wextra -pedantic src/main.c
run:
	./bin/chesseng
lint:
	splint +boolint ./src/main.c
valgrind:
	valgrind -v --leak-check=yes ./bin/chesseng
cppcheck:
	cppcheck src/main.c
