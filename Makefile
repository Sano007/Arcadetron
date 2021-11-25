all:
	gcc -std=c11 -Wall -Werror -g main.c functions.c -lm -lcurses -o arcadetron