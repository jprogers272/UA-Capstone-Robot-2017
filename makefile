CC = g++
C_EXT = .cpp
H_EXT = .hpp
FLAGS = -Wall -c
LDFLAGS = -Wall -g
OBJ_DIR = obj
SRC_DIR = src
BIN_DIR = bin
MAINFILES = robotMain example compass_test stop pid_rotate pid_straight ir_drive_test Stage3Op compass_csv_write
SRC_ = $(wildcard $(SRC_DIR)/*$(C_EXT))
SRC = $(filter-out $(addsuffix $(C_EXT),$(addprefix $(SRC_DIR)/,$(MAINFILES))),$(SRC_))
OBJ_ = $(SRC_:$(SRC_DIR)/%$(C_EXT)=$(OBJ_DIR)/%.o)
OBJ = $(filter-out $(addsuffix .o,$(addprefix $(OBJ_DIR)/,$(MAINFILES))),$(OBJ_))
DEP = $(wildcard $(SRC_DIR)/*$(H_EXT))

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%$(C_EXT) $(DEP)
	$(CC) $(FLAGS) -o $@ -c $<

$(MAINFILES): % : $(OBJ_DIR)/%.o $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $< -o $(BIN_DIR)/$@

#Should be able to remove all the commented lines below this
#compass_csv_write:	$(OBJ) $(OBJ_DIR)/compass_csv_write.o
#	$(CC) $(LDFLAGS) $(OBJ) $(OBJ_DIR)/compass_csv_write.o -o $(BIN_DIR)/compass_csv_write

#compass_test:	$(OBJ) $(OBJ_DIR)/compass_test.o
#	$(CC) $(LDFLAGS) $(OBJ) $(OBJ_DIR)/compass_test.o -o $(BIN_DIR)/compass_test

#Stage3Op:	$(OBJ) $(OBJ_DIR)/Stage3Op.o
#	$(CC) $(LDFLAGS) $(OBJ) $(OBJ_DIR)/Stage3Op.o -o $(BIN_DIR)/Stage3Op

#example:	$(OBJ) $(OBJ_DIR)/example.o
#	$(CC) $(LDFLAGS) $(OBJ) $(OBJ_DIR)/example.o -o $(BIN_DIR)/example 

#ir_drive_test:	$(OBJ) $(OBJ_DIR)/ir_drive_test.o
#	$(CC) $(LDFLAGS) $(OBJ) $(OBJ_DIR)/ir_drive_test.o -o $(BIN_DIR)/ir_drive_test

#pid_rotate:	$(OBJ) $(OBJ_DIR)/pid_rotate.o
#	$(CC) $(LDFLAGS) $(OBJ) $(OBJ_DIR)/pid_rotate.o -o $(BIN_DIR)/pid_rotate

#pid_straight:	$(OBJ) $(OBJ_DIR)/pid_straight.o
#	$(CC) $(LDFLAGS) $(OBJ) $(OBJ_DIR)/pid_straight.o -o $(BIN_DIR)/pid_straight

#stop:	$(OBJ) $(OBJ_DIR)/stop.o
#	$(CC) $(LDFLAGS) $(OBJ) $(OBJ_DIR)/stop.o -o $(BIN_DIR)/stop 

#all_obj:	$(SRC) $(DEP)
#	make $(OBJ_DIR)/*.o

.PHONY: clean print-%
clean:
	rm -f obj/*.o

print-%:
	@echo $* = $($*)
