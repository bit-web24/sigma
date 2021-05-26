#!/usr/bin/make

SHELL := bash
obj := main.o
LIB := -l ncurses
sigma: main.o
	test -e bin/ || mkdir bin/
	gcc obj/$(obj) $(LIB) -o bin/$@

main.o: src/main.c
	test -e obj/ || mkdir obj/
	gcc -c $< -o obj/$@

clear:
	rm -drv obj/ bin/
