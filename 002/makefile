#!/bin/sh
CC = gcc
CCFLAGS =-Wall -std=c11
CLEAN = rm -f
PROG_NAME = l002
OBJECT_FILES = *.o
# SOURCE_FILES = others.c array.c item.c main.c table.c
SOURCE_FILES = main.c table.c item.c others.c array.c

$(PROG_NAME): $(OBJECT_FILES)
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJECT_FILES): $(SOURCE_FILES)
	$(CC) $(CCFLAGS) -c $^

clean :
	$(CLEAN) *.o $(PROG_NAME)