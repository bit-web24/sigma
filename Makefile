#!/usr/bin/make

SHELL := bash
obj := main.o scroll.o display.o buffer.o
LIB := -l ncurses

sigma: ${obj}
	test -e bin/ || mkdir bin/
	gcc obj/main.o obj/scroll.o obj/display.o obj/buffer.o $(LIB) -o bin/$@
	
%.o:src/%.c
	test -e obj/ || mkdir obj/
	gcc -c $< -o obj/$@

clear:
	rm -drv obj/ bin/
