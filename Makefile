default: 
	c99 -o bin/chesseng -g -pedantic src/main.c
run:
	./bin/chesseng
lint:
	splint +boolint ./src/main.c
valgrind:
	valgrind -v --leak-check=full --show-leak-kinds=all ./bin/chesseng 
cppcheck:
	cppcheck src/main.c
profile:
	rm -rf callgrind*
	valgrind --tool=callgrind ./bin/chesseng
	kcachegrind callgrind*
test:
	c99 -o bin/tests src/test.c
	./bin/tests
