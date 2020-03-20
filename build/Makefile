rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

DIRECTIVES			:= 
CC					:= i686-elf-gcc
ASSEMBLER_FLAGS		:= -fpic -ffreestanding $(DIRECTIVES) -lgcc -g -Wall -Wextra
C_FLAGS				:= -std=c99 $(ASSEMBLER_FLAGS)
CXX_FLAGS 			:= -std=c++17 $(ASSEMBLER_FLAGS) -fno-exceptions -fno-rtti
LINKER_FLAGS 		:= -ffreestanding -nostdlib

BIN					:= ../bin
SRC					:= ../src
KERNEL_SRC 			:= $(SRC)/Kernel
CORE_SRC			:= $(SRC)/Core
RUNTIME_SRC			:= $(SRC)/Runtime
OBJ_DIR				:= $(BIN)/obj
INCLUDE_DIRS		:= ../include ../src
INCLUDE				:= $(addprefix -I, $(INCLUDE_DIRS))
LIB					:= ../lib

LINKER_SCRIPT		:= linker.ld
KERCPPOBJECTS		:= $(call rwildcard,$(KERNEL_SRC),*.cpp) 
CORECPPOBJECTS		:= $(call rwildcard,$(CORE_SRC),*.cpp)
CORECOBJECTS		:= $(call rwildcard,$(CORE_SRC),*.c)
ASMOBJECTS			:= $(call rwildcard,$(KERNEL_SRC),*.S)
RUNTIMEOBJECTS		:= $(call rwildcard,$(RUNTIME_SRC),*.c)

CRTI_OBJ			:= $(OBJ_DIR)/crti.o
CRTBEGIN_OBJ		:= $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ			:= $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ			:= $(OBJ_DIR)/crtn.o

OBJECTS				:= $(patsubst $(KERNEL_SRC)/%.cpp, $(OBJ_DIR)/%.o, $(KERCPPOBJECTS))
OBJECTS				+= $(patsubst $(CORE_SRC)/%.cpp, $(OBJ_DIR)/%.o, $(CORECPPOBJECTS))
OBJECTS				+= $(patsubst $(CORE_SRC)/%.c, $(OBJ_DIR)/%.o, $(CORECOBJECTS))
OBJECTS				+= $(patsubst $(KERNEL_SRC)/%.S, $(OBJ_DIR)/%.o, $(ASMOBJECTS))

RUNTIME_OBJECTS		:= $(patsubst $(RUNTIME_SRC)/%.c, $(OBJ_DIR)/%.o, $(RUNTIMEOBJECTS))

KERNEL_NAME			:= ROSeless.bin
KERNEL_DIR			:= $(BIN)/$(KERNEL_NAME)

build: $(OBJECTS) $(RUNTIME_OBJECTS)
	echo $(CORECPPOBJECTS)
	$(CC) $(LINKER_FLAGS) -T $(LINKER_SCRIPT) -o $(KERNEL_DIR) $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJECTS) $(CRTEND_OBJ) $(CRTN_OBJ)

$(OBJ_DIR)/%.o: $(KERNEL_SRC)/%.cpp
	$(CC) $(CXX_FLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(KERNEL_SRC)/%.S
	$(CC) $(ASSEMBLER_FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(CORE_SRC)/%.cpp
	$(CC) $(CXX_FLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(CORE_SRC)/%.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(RUNTIME_SRC)/%.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c $< -o $@

run: build
	qemu-system-i386 -kernel $(KERNEL_DIR) -debugcon stdio	

debug: build
	qemu-system-i386 -kernel $(KERNEL_DIR) -s -S -debugcon stdio

clean:
	rm -rf $(OBJ_DIR)/*
	rm -f $(KERNEL_DIR)
