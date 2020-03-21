DIRECTIVES			:= 
ARCH				:= i686
CC					:= $(ARCH)-elf-gcc
ASSEMBLER_FLAGS		:= -fpic -ffreestanding $(DIRECTIVES) -lgcc -g -Wall -Wextra
C_FLAGS				:= -std=c99 $(ASSEMBLER_FLAGS)
CXX_FLAGS 			:= -std=c++17 $(ASSEMBLER_FLAGS) -fno-exceptions -fno-rtti
LINKER_FLAGS 		:= -ffreestanding -nostdlib

BIN					:= ./bin
SRC					:= ./src
KERNEL_SRC 			:= $(SRC)/Kernel
CORE_SRC			:= $(SRC)/Core
RUNTIME_SRC			:= $(SRC)/Runtime
OBJ_DIR				:= $(BIN)/obj
INCLUDE_DIRS		:= ./include ./src
INCLUDE				:= $(addprefix -I, $(INCLUDE_DIRS))
LIB					:= ./lib

LINKER_SCRIPT		:= linker.ld
HEADERS				:= $(shell find . -type f -name '*.h')
C_SOURCES			:= $(shell find . -name "*.c" ! -path '*/Runtime/*')
RUNTIME_C_SOURCES	:= $(shell find . -name "*.c" -path '*/Runtime/*')
CPP_SOURCES			:= $(shell find . -name "*.cpp" ! -path '*/Runtime/*')
ASM_SOURCES			:= $(shell find . -name "*.S" ! -path '*/Runtime/*')

SOURCES				:= $(C_SOURCES) $(CPP_SOURCES) $(ASM_SOURCES)

CRTI_OBJ			:= $(RUNTIME_SRC)/crti.o
CRTBEGIN_OBJ		:= $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ			:= $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ			:= $(RUNTIME_SRC)/crtn.o
ICXXABI_OBJ			:= $(RUNTIME_SRC)/icxxabi.o

OBJECTS				:= ${C_SOURCES:.c=.o} ${ASM_SOURCES:.S=.o} ${CPP_SOURCES:.cpp=.o}

OBJECTS_RUNTIME		:= ${RUNTIME_C_SOURCES:.c=.o}

KERNEL_NAME			:= BartOS.bin
KERNEL_DIR			:= $(BIN)/$(KERNEL_NAME)

QEMU_FLAGS			:= -m 1G -debugcon stdio

build: $(OBJECTS) $(OBJECTS_RUNTIME)
	$(CC) $(LINKER_FLAGS) -T $(LINKER_SCRIPT) -o $(KERNEL_DIR) $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJECTS) $(ICXXABI_OBJ) $(CRTEND_OBJ) $(CRTN_OBJ)
	mkdir -p $(OBJ_DIR)
	mv $(OBJECTS) $(OBJECTS_RUNTIME) $(OBJ_DIR)

%.o: %.c $(HEADERS)
	${CC} ${C_FLAGS} ${INCLUDE} -c $< -o $@

%.o: %.cpp $(HEADERS)
	$(CC) $(CXX_FLAGS) $(INCLUDE) -c $< -o $@

%.o: %.S
	$(CC) $(ASSEMBLER_FLAGS) -c $< -o $@

run: build
	qemu-system-i386 $(QEMU_FLAGS) -kernel $(KERNEL_DIR)

debug: build
	qemu-system-i386 $(QEMU_FLAGS) -kernel $(KERNEL_DIR) -s -S &
	gdb -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL_DIR)"

cleanobjs:
	find . -type f -name '*.o' -delete

clean:
	find . -type f -name '*.o' -delete
	rm -rf $(KERNEL_DIR)
