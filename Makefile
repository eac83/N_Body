CC = g++
CFLAGS = -Wall -std=c++11 -fopenmp

SRC_DIR = src
BIN_DIR = bin

MAIN_SRC = $(SRC_DIR)/main.cc $(SRC_DIR)/particle.cc $(SRC_DIR)/utils.cc $(SRC_DIR)/expansion.cc
LATTICE_SRC = $(SRC_DIR)/lattice.cc $(SRC_DIR)/particle.cc $(SRC_DIR)/utils.cc
RANDOM_SRC = $(SRC_DIR)/random.cc $(SRC_DIR)/particle.cc $(SRC_DIR)/utils.cc

MAIN_OBJ = $(patsubst $(SRC_DIR)/%.cc,$(BIN_DIR)/%.o,$(MAIN_SRC))
LATTICE_OBJ = $(patsubst $(SRC_DIR)/%.cc,$(BIN_DIR)/%.o,$(LATTICE_SRC))
RANDOM_OBJ = $(patsubst $(SRC_DIR)/%.cc,$(BIN_DIR)/%.o,$(RANDOM_SRC))

MAIN_EXEC = main
LATTICE_EXEC = lattice
RANDOM_EXEC = random

all: $(MAIN_EXEC) $(LATTICE_EXEC) $(RANDOM_EXEC)

$(MAIN_EXEC): $(MAIN_OBJ)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$(MAIN_EXEC)

$(LATTICE_EXEC): $(LATTICE_OBJ)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$(LATTICE_EXEC)

$(RANDOM_EXEC): $(RANDOM_OBJ)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$(RANDOM_EXEC)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN_DIR)/*.o $(BIN_DIR)/$(MAIN_EXEC) $(BIN_DIR)/$(LATTICE_EXEC) $(BIN_DIR)/$(RANDOM_EXEC)