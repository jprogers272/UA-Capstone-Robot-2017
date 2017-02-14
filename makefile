CC = g++
FLAGS = -Wall -c
LFLAGS = -Wall -g
OBJ_DIR = obj
SRC_DIR = src
BIN_DIR = bin
EXCLUDES = example compass_test stop pid_test ir_drive_test Stage3Op
SRC_ = $(wildcard $(SRC_DIR)/*.cpp)
SRC = $(filter-out $(addsuffix .cpp,$(addprefix $(SRC_DIR)/,$(EXCLUDES))),$(SRC_))
OBJ_ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ = $(filter-out $(addsuffix .o,$(addprefix $(OBJ_DIR)/,$(EXCLUDES))),$(OBJ_))
DEP = $(wildcard $(SRC_DIR)/*.hpp)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp $(DEP)
	$(CC) $(FLAGS) -o $@ -c $<

compass_test:	$(OBJ) $(OBJ_DIR)/compass_test.o
	g++ $(LFLAGS) $(OBJ) $(OBJ_DIR)/compass_test.o -o $(BIN_DIR)/compass_test

Stage3Op:	$(OBJ) $(OBJ_DIR)/Stage3Op.o
	g++ $(LFLAGS) $(OBJ) $(OBJ_DIR)/Stage3Op.o -o $(BIN_DIR)/Stage3Op

example:	$(OBJ) $(OBJ_DIR)/example.o
	g++ $(LFLAGS) $(OBJ) $(OBJ_DIR)/example.o -o $(BIN_DIR)/example 

ir_drive_test:	$(OBJ) $(OBJ_DIR)/ir_drive_test.o
	g++ $(LFLAGS) $(OBJ) $(OBJ_DIR)/ir_drive_test.o -o $(BIN_DIR)/ir_drive_test

pid_test:		$(OBJ) $(OBJ_DIR)/pid_test.o
	g++ $(LFLAGS) $(OBJ) $(OBJ_DIR)/pid_test.o -o $(BIN_DIR)/pid_test

stop:	$(OBJ) $(OBJ_DIR)/stop.o
	g++ $(LFLAGS) $(OBJ) $(OBJ_DIR)/stop.o -o $(BIN_DIR)/stop 

all_obj:	$(SRC) $(DEP)
	make $(OBJ_DIR)/*.o

.PHONY: clean
clean:
	rm -f obj/*.o

.PHONY: print-%
print-%  : ; @echo $* = $($*)
