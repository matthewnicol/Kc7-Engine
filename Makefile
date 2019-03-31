default: 
	gcc -o bin/chesseng -g -Wall -Werror -Wextra -pedantic src/main.c
run:
	./bin/chesseng
