CC = g++
FLAGS = -Wall -c
OBJ_DIR = obj
SRC_DIR = src
BIN_DIR = bin
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ_RULES = $(SRC:$(SRC_DIR)/%.cpp=%.o)
DEP = $(wildcard $(SRC_DIR)/*.hpp)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp $(DEP)
	$(CC) $(FLAGS) -o $@ -c $<

example:		example.o $(OBJ_RULES)
	g++ -Wall -g $(OBJ_DIR)/example.o $(OBJ) -o $(BIN_DIR)/example 

all_obj:		$(SRC) $(DEP)
	make $(OBJ_DIR)/*.o

.PHONY: clean
clean:
	rm -f obj/*.o

.PHONY: print-%
print-%  : ; @echo $* = $($*)