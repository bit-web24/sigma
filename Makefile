#!/usr/bin/make

SHELL := bash
obj := main.o
LIB := -l ncurses
sigma: main.o
	gcc obj/$(obj) $(LIB) -o bin/$@

main.o: main.c
	gcc -c $< -o obj/$@

clear:
	rm obj/*.o bin/sigma
