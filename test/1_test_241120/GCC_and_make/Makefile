MAIN_FILENAME = main.c
MAIN_OBJ_NAME = main.o
OUTPUT_NAME = sse

CC = gcc
STD = c11

OPTIMIZATION_OPT = -O0

all: compile
	$(CC) -march=native $(OPTIMIZATION_OPT ) -std=$(STD) $(MAIN_OBJ_NAME) -o $(OUTPUT_NAME) -rdynamic -ldl -lpthread

compile: $(MAIN_FILENAME)
	$(CC) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGE_FILES -march=native  $(OPTIMIZATION_OPT ) -std=$(STD) -o $(MAIN_OBJ_NAME) -c $(MAIN_FILENAME)

clean:
	rm $(MAIN_OBJ_NAME) $(OUTPUT_NAME)