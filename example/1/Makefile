MAIN_FILENAME_1 = vectorial.c
MAIN_FILENAME_2 = vectorial.c
MAIN_FILENAME_3 = scalar.c

OUTPUT_NAME_1 = vectorial
OUTPUT_NAME_2 = vectorial_1
OUTPUT_NAME_3 = scalar

CC = gcc
STD = c11

DEBUG_OPT = 

OPTIMIZATION_OPT = 

all:
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OUTPUT_NAME_1) $(MAIN_FILENAME_1)
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OUTPUT_NAME_2) $(MAIN_FILENAME_2)
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OUTPUT_NAME_3) $(MAIN_FILENAME_3)
clean:
	rm $(OBJ_NAME_1) $(OUTPUT_NAME_1) $(OBJ_NAME_2) $(OUTPUT_NAME_2)
