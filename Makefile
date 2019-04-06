default: 
	gcc -o bin/chesseng -g -Wall -Werror -Wextra -pedantic src/main.c
run:
	./bin/chesseng
lint:
	splint +boolint ./src/main.c
valgrind:
	valgrind --leak-check=yes ./bin/chesseng
