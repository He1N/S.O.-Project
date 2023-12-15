# Nombre del archivo ejecutable
TARGET = myshell

# Directorio de los archivos fuente
SRC_DIR = .

# Directorio de los archivos objeto
OBJ_DIR = ./obj

# Compilador a usar
CC = g++

# Flags del compilador
CFLAGS = -Wall -Wextra -std=c++11

# Archivos fuente
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Archivos objeto
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Regla por defecto
all: $(TARGET)

# Regla para el archivo ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regla para los archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
