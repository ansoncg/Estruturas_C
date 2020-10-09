# Makefile para ficar fora da pasta de estruturas

# Target: Pre-requitos
# <TAB> <como fazer>
# $@ -> Nome do target
# $< -> Primeiro pre-requisito
# $^ -> Todos os pre-requisitos

FLAGS_FINAL = -O3 -march=native #-w
FLAGS_AVISO = -Wall -Wextra -Wconversion -pedantic
FLAGS_DEBUG = -ggdb3 
FLAGS_VALGRIND = --leak-check=full --show-leak-kinds=all --track-origins=yes
FLAGS_SAN = -fsanitize=address #-fsanitize=undefined #-fsanitize=thread 
FLAGS_LIGA = #-lm
FLAGS_SUAS = #-fexceptions

CC = gcc

EXE = programa # Final
EXE_V = programaV # Valgrind
EXE_S = programaS # Sanitize

FONTE_ESTR = $(wildcard Estruturas/src/*.c) 
FONTE_PROG = $(wildcard Teste_Estruturas/*.c) # Pasta do programa aqui
OBJ = $(FONTE_ESTR:.c=.o) $(FONTE_PROG:.c=.o)

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@ $(FLAGS_FINAL)

run: $(EXE)
	./$(EXE)

s: $(EXE_S)
	./$(EXE_S)

$(EXE_S): $(OBJ)
	$(CC) $^ -o $@ $(FLAGS_AVISO) $(FLAGS_DEBUG) $(FLAGS_SAN)

v: $(EXE_V)
	valgrind $(FLAGS_VALGRIND) ./$(EXE_V)

$(EXE_V): $(OBJ)
	$(CC) $^ -o $@ $(FLAGS_AVISO) $(FLAGS_DEBUG)

clean:
	rm -f $(OBJ) 
	rm programa*

.PHONY: all clean
