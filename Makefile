# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Iinclude -std=c++17 -g -O0

# Pastas
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

# Arquivos-fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Executável
TARGET = $(BIN_DIR)/tp1.out

# Regra padrão
all: $(TARGET)

# Linkagem
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compilação
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Criação das pastas, se não existirem
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Limpeza
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean
