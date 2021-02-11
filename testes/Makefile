# Anderson Cardoso Gonçalves - github.com/ansoncg 

# Makefile para ficar fora da pasta de estruturas

# Target: Pre-requitos
# <TAB> <como fazer>
# $@ -> Nome do target
# $< -> Primeiro pre-requisito
# $^ -> Todos os pre-requisitos

FLAGS_FINAL = -O3 -march=native#-w
FLAGS_AVISO = -Wall -Wextra -Wconversion -pedantic
FLAGS_DEBUG = -ggdb3 -O0
FLAGS_VALGRIND = --leak-check=full --show-leak-kinds=all --track-origins=yes
FLAGS_COBERTURA = -fprofile-arcs -ftest-coverage
FLAGS_SAN = -fsanitize=address
FLAGS_LIGA = -lgcov --coverage #-lm

CC = gcc

EXE = programa # Final
EXE_V = programaV # Valgrind
EXE_S = programaS # Sanitize
EXE_T = programaT # Testes

FONTE_ESTR = $(wildcard ../src/*.c) # Caminho das estruturas
FONTE_PROG = $(wildcard *.c) # Caminho da main
OBJ = $(FONTE_PROG:.c=.o) $(FONTE_ESTR:.c=.o) 

#--------------

# Final
all: CFLAGS = $(FLAGS_FINAL)
all: $(EXE) 

$(EXE): $(OBJ) 
	$(CC) $^ -o $@ $(FLAGS_LIGA) 

run: all
	./$(EXE)

#--------------

# Sanitize
s: CFLAGS = $(FLAGS_AVISO) $(FLAGS_DEBUG) 
s: $(EXE_S)
	./$(EXE_S) 

$(EXE_S): $(OBJ)
	$(CC) $(FLAGS_SAN) $^ -o $@ $(FLAGS_LIGA) 

#--------------

# Valgrind
v: CFLAGS = $(FLAGS_AVISO) $(FLAGS_DEBUG) 
v: $(EXE_V)
	valgrind $(FLAGS_VALGRIND) ./$(EXE_V)

$(EXE_V): $(OBJ)
	$(CC) $^ -o $@ $(FLAGS_LIGA)

#--------------

# Testes
t: CFLAGS = $(FLAGS_COBERTURA)
t: $(EXE_T)
	./$(EXE_T)
	gcov $(FONTE_ESTR) $(FONTE_PROG)

$(EXE_T): $(OBJ)
	$(CC) $^ -o $@ $(FLAGS_LIGA)

clean:
	rm -f $(OBJ) 
	rm programa* *.gc*
	rm ../src/*.gc*

.PHONY: all clean
