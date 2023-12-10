# Variáveis
CC = gcc
CFLAGS = -Wall -Wextra

# Diretórios
SRC_DIR = src
OBJ_DIR = obj

# Arquivos fonte
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Nome do executável
EXECUTABLE = vina++

# Regras
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(EXECUTABLE) 

.PHONY: all clean