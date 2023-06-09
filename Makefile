# Makefile for Final Project in ECE586 - Spring 2023
# Created by Alexander Maso
########################################################################


main: main.o parse.o
	gcc main.o parse.o -o main -lm

main.o: main.c
	gcc -c -g -Wall main.c

parse.o: parse.c header.h instruction.h
	gcc -c -g -Wall parse.c

clean:
	rm main parse.o main.o




