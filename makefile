INC_DIR:=./include
SRC_DIR:=./src
OBJ_DIR:=./obj
BIN_DIR:=./bin

SRC=${wildcard ${SRC_DIR}/*.cpp}
OBJ=${patsubst %.cpp, ${OBJ_DIR}/%.o,${notdir ${SRC}}}

CC:=g++
CPPFLAGS:=-Wall -lpthread -std=c++11 -I${INC_DIR} #-g -rdynamic
TARGET=server
BIN_TARGET=$(BIN_DIR)/$(TARGET)
.PHONY:clean
#ALL:
#	@echo $(SRC)
#	@echo $(DIR)


$(BIN_TARGET):$(OBJ)
	$(CC) $(OBJ) -o $@ $(CPPFLAGS)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	find $(OBJ_DIR) -name *.o -exec rm -rf {} \;
