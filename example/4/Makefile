MAIN_FILENAME_1 = scalar.c
MAIN_FILENAME_2 = vectorial.c

LIB = utils.c
OBJ_LIB = utils.o

OUTPUT_NAME_1 = scalar
OUTPUT_NAME_2 = vectorial

CC = gcc
STD = c11

DEBUG_OPT = -ggdb

OPTIMIZATION_OPT = -O3 

all: lib
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) $(OBJ_LIB) $(MAIN_FILENAME_1) -o $(OUTPUT_NAME_1) 
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) $(OBJ_LIB) $(MAIN_FILENAME_2) -o $(OUTPUT_NAME_2) 

lib: $(LIB)
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OBJ_LIB) -c $(LIB)

clean:
	rm $(OUTPUT_NAME_1)
	rm $(OUTPUT_NAME_2)
	rm $(OBJ_LIB)