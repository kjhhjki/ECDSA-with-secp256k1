.PHONY: all clean
MKDIR = mkdir
++ = g++

ifeq ($(OS),Windows_NT)
	DELETE = del /q
	RM = rd /s /q
	FLAGS = -Iinclude
else 
	DELETE = rm -f
	RM = rm -rf
	FLAGS = -lpthread -Iinclude
endif

DIR_OBJ = obj
DIR = $(DIR_OBJ)
SRCS = $(wildcard */*.cpp)
OBJS = $(SRCS:.cpp=.o)
OBJS := $(notdir $(OBJS))
OBJS := $(addprefix $(DIR_OBJ)/,$(OBJS))
EXES = $(wildcard *.exe)

all:$(DIR) Sign Veri debug clean

debug:
	@echo $(SRCS)
	@echo $(OBJS)
$(DIR):
	$(MKDIR) $@
Main:$(OBJS)
	$(++) $^ $@.cpp $(FLAGS) -o $@
	@echo making $@
Sign:$(OBJS)
	$(++) $^ $@.cpp $(FLAGS) -o $@
	@echo making $@
Veri:$(OBJS)
	$(++) $^ $@.cpp $(FLAGS) -o $@
	@echo making $@
$(DIR_OBJ)/%.o: */%.cpp
	$(++) -c $^ $(FLAGS) -o $@
	@echo making $@
clean:
	$(RM) $(DIR)
