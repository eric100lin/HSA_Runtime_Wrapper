CC = gcc
CXX = g++
AR = ar
LD = g++
CFLAGS = -g -Wall -std=c99 -fPIC
CXXFLAGS = -g -Wall -std=gnu++0x -fPIC

LIBRARY_NAME = libHsaWrapper.so
SOURCE_PATH = \
	src/ \
	src/hsa/
INCLUDE_PATH = -I./inc \
	-I$(HSA_RUNTIME_PATH)/include
LD_LIBS = \
	-L$(HSA_RUNTIME_PATH)/lib/x86_64 \
	-lhsa-runtime64 -lelf
OBJ_DIR = .objs
Csources = $(wildcard $(SOURCE_PATH:=*.c))
CXXsources = $(wildcard $(SOURCE_PATH:=*.cpp))
PROGRAM_OBJS := ${Csources:.c=.o} ${CXXsources:.cpp=.o}
PROGRAM_OBJS := $(addprefix $(OBJ_DIR)/,$(PROGRAM_OBJS))

all: before_compile COMPILE_SHARED_LIBRARY after_compile

before_compile: 
	test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR)

after_compile:

COMPILE_SHARED_LIBRARY: $(PROGRAM_OBJS)
	$(LD) -shared -o $(LIBRARY_NAME) $(addprefix $(OBJ_DIR)/, $(notdir $(PROGRAM_OBJS))) $(LD_LIBS)
	
$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) -c $< -o $(OBJ_DIR)/$(@F)

$(OBJ_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $(OBJ_DIR)/$(@F)

clean:
	rm -fr $(OBJ_DIR)/
	rm -f $(LIBRARY_NAME)

.PHONY: before_compile after_compile
