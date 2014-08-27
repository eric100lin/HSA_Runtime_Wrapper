CC = gcc
CXX = g++
AR = ar
LD = g++
CFLAGS = -g -Wall -std=c99 -fPIC
CXXFLAGS = -g -Wall -std=gnu++0x -fPIC

PROGRAM_NAME = ./HSA_Runtime_Wrapper_VectorCopy
SOURCE_PATH = src/
CL_SOURCE_PATH = src_cl/	
INCLUDE_PATH = \
	-I../library/inc \
	-I$(HSA_RUNTIME_PATH)/include
LD_LIBS = -L../library -lHsaWrapper
OBJ_DIR = .objs
Csources = $(wildcard $(SOURCE_PATH:=*.c))
CXXsources = $(wildcard $(SOURCE_PATH:=*.cpp))
CLsources = $(wildcard $(CL_SOURCE_PATH:=*.cl))
PROGRAM_OBJS := ${Csources:.c=.o} ${CXXsources:.cpp=.o}
PROGRAM_OBJS := $(addprefix $(OBJ_DIR)/,$(PROGRAM_OBJS))
CLbins = ${CLsources:.cl=.brig}

all: hsa_cloc_compile before_compile COMPILING after_compile

hsa_cloc_compile: $(CLsources)
	for CLsource in $(CLsources); do \
		echo "========= Compiling $$CLsource =================="; \
		$(HSA_CLOC_PATH)/cloc $$CLsource; \
		echo "========= Done compiling $$CLsource!! =================="; \
		echo ""; \
	done;

before_compile: 
	test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR)

after_compile:

COMPILING: $(PROGRAM_OBJS)
	$(LD) -o $(PROGRAM_NAME) $(addprefix $(OBJ_DIR)/, $(notdir $(PROGRAM_OBJS))) $(LD_LIBS)
	
$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) -c $< -o $(OBJ_DIR)/$(@F)

$(OBJ_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $(OBJ_DIR)/$(@F)

clean:
	rm $(CLbins)
	rm -fr $(OBJ_DIR)/
	rm $(PROGRAM_NAME)

check-cloc-env:
ifndef HSA_CLOC_PATH
	$(error HSA_CLOC_PATH is undefined)
endif
ifndef HSA_LIBHSAIL_PATH
	$(error HSA_LIBHSAIL_PATH is undefined - Required by CLOC)
endif
ifndef HSA_LIBHSAIL_PATH
	$(error HSA_CLOC_PATH is undefined - Required by CLOC)
endif
ifndef HSA_KMT_PATH
	$(error HSA_KMT_PATH is undefined)
endif
ifndef HSA_RUNTIME_PATH
	$(error HSA_RUNTIME_PATH is undefined)
endif

.PHONY: hsa_cloc_compile before_compile after_compile check-cloc-env
