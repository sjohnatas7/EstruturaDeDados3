# Variáveis para diretórios e nomes de arquivos
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
PROGRAM := $(BIN_DIR)/programa

# Lista de arquivos fonte
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)

# Lista de arquivos objeto correspondentes
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Comando de compilação
CC := gcc
CFLAGS := -Wall -Wextra -g

# Regras de compilação
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra de construção do programa
$(PROGRAM): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para executar o programa
run: $(PROGRAM)
	./$< < ./CasosTestes/1.in

# Regra para limpar os arquivos objeto e o executável
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/*

# Indicar que 'all' depende do programa
all: $(PROGRAM)

.PHONY: run clean
