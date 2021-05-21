#!/usr/bin/make

SHELL := bash
obj := main.o
LIB := -l ncurses
sigma: main.o
	gcc $(obj) $(LIB) -o $@

main.o: main.c
	gcc -c $< -o $@

clear:
	rm *.o sigma
