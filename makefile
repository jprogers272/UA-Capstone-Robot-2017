# Author: John Rogers
CC = g++
C_EXT = .cpp
H_EXT = .hpp
FLAGS = -Wall -c -I/usr/local/include
LDFLAGS = -Wall -g
LDLIBS = 
LDLIBS = -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs -lpthread
OBJ_DIR = obj
SRC_DIR = src
BIN_DIR = bin
MAINFILES = robotMain example compass_test stop pid_rotate pid_straight ir_drive_test Stage3Op compass_csv_write display_test position_test camera_test
SRC_ = $(wildcard $(SRC_DIR)/*$(C_EXT))
SRC = $(filter-out $(addsuffix $(C_EXT),$(addprefix $(SRC_DIR)/,$(MAINFILES))),$(SRC_))
OBJ_ = $(SRC_:$(SRC_DIR)/%$(C_EXT)=$(OBJ_DIR)/%.o)
OBJ = $(filter-out $(addsuffix .o,$(addprefix $(OBJ_DIR)/,$(MAINFILES))),$(OBJ_))
DEP = $(wildcard $(SRC_DIR)/*$(H_EXT))

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%$(C_EXT) $(DEP)
	$(CC) $(FLAGS) -o $@ -c $<

$(MAINFILES): % : $(OBJ_DIR)/%.o $(OBJ)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJ) $< -o $(BIN_DIR)/$@

.PHONY: clean print-%
clean:
	rm -f obj/*.o

print-%:
	@echo $* = $($*)
