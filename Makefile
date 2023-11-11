NAME = main
TEST_NAME = $(NAME)Test
CC = g++
SRC_DIR = ./src
INC_DIR = ./include
LIB_DIR = ./lib
BIN_DIR = ./bin
BUILD_DIR = ./build
TEST_DIR = ./tests
CFLAGS = --std=c++11 -Wall -O1 -g
HFIlE = hpp
CFILE = cpp
EXE = $(BIN_DIR)/$(NAME)
EXE_TEST = $(BIN_DIR)/$(TEST_NAME)

define GENERATE_OBJS
$(shell for f in $$(ls -R $(1) | grep $(CFILE) | sed 's/$(CFILE)/o/;s/$(2).o//'); do echo $(BUILD_DIR)/$$f; done)
endef

OBJS = $(call GENERATE_OBJS,$(SRC_DIR),$(NAME))
TEST_OBJS = $(call GENERATE_OBJS,$(TEST_DIR),$(TEST_NAME))

all: $(EXE)

run: all
	$(EXE) < $(ARGS)

$(EXE): $(BUILD_DIR)/$(NAME).o $(OBJS) 
	$(CC) $(CFLAGS) $< $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(CFILE) $(INC_DIR)/%.$(HFILE)
	$(CC) -c $(CFLAGS) $< -I $(INC_DIR) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(CFILE)
	$(CC) -c $(CFLAGS) $< -I $(INC_DIR) -o $@ 

test: $(EXE_TEST)

run_test: test
	$(EXE_TEST)

$(EXE_TEST): $(BUILD_DIR)/$(TEST_NAME).o $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $< $(OBJS) $(TEST_OBJS) -o $(EXE_TEST)

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.$(CFILE)
	$(CC) -c $(CFLAGS) $< -I $(INC_DIR) -I $(LIB_DIR) -o $@

leak_check: all
	valgrind --leak-check=full --show-leak-kinds=all $(EXE) < $(ARGS) > /dev/null 

heap_profiler: all
	valgrind --tool=massif --massif-out-file=massif.txt $(EXE) < $(ARGS) > /dev/null
	ms_print massif.txt > ms_out.txt

clean:
	rm -f $(BUILD_DIR)/*
	rm -f $(EXE)
	rm -f $(EXE_TEST)

file:
	cpp-merge -i $(INC_DIR) -s $(SRC_DIR) $(SRC_DIR)/$(NAME).$(CFILE) -o tp.$(CFILE)