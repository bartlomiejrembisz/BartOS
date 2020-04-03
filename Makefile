DIRECTIVES			:= 
ARCH				:= x86_64
CC					:= /home/bartek/opt/cross/bin/$(ARCH)-elf-gcc
GCC_FLAGS			:= -fpic -ffreestanding $(DIRECTIVES) -lgcc -g -Wall -Wextra -mno-red-zone -mno-80387 -mno-sse -mno-sse2 -mno-mmx
ASSEMBLER_FLAGS		:= -felf64 -F dwarf -g
C_FLAGS				:= -std=c99 $(GCC_FLAGS)
CXX_FLAGS 			:= -std=c++17 $(GCC_FLAGS) -fno-exceptions -fno-rtti
LINKER_FLAGS 		:= -nostdlib -n -z max-page-size=0x1000

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
RUNTIME_ASM_SOURCES	:= $(shell find . -name "*.asm" -path '*/Runtime/*')
RUNTIME_C_SOURCES	:= $(shell find . -name "*.c" -path '*/Runtime/*')
CPP_SOURCES			:= $(shell find . -name "*.cpp" ! -path '*/Runtime/*')
ASM_SOURCES			:= $(shell find . -name "*.asm" ! -path '*/Runtime/*')

SOURCES				:= $(C_SOURCES) $(CPP_SOURCES) $(ASM_SOURCES)

CRTI_OBJ			:= $(RUNTIME_SRC)/crti.o
CRTBEGIN_OBJ		:= $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ			:= $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ			:= $(RUNTIME_SRC)/crtn.o
ICXXABI_OBJ			:= $(RUNTIME_SRC)/icxxabi.o

OBJECTS				:= ${C_SOURCES:.c=.o} ${ASM_SOURCES:.asm=.o} ${CPP_SOURCES:.cpp=.o}

OBJECTS_RUNTIME		:= ${RUNTIME_ASM_SOURCES:.asm=.o} ${RUNTIME_C_SOURCES:.c=.o}

KERNEL_NAME			:= BartOS
KERNEL_DIR			:= $(BIN)/$(KERNEL_NAME).bin
KERNEL_ISO			:= $(BIN)/$(KERNEL_NAME).iso

QEMU_FLAGS			:= -cdrom $(KERNEL_ISO) -m 4G -d int -D debug.log -no-reboot -vga std

iso: build
	@mkdir -p isofiles/boot/grub
	@cp $(KERNEL_DIR) isofiles/boot/kernel.bin
	@cp grub.cfg isofiles/boot/grub/
	@grub-mkrescue -o $(KERNEL_ISO) isofiles/

build: $(OBJECTS) $(OBJECTS_RUNTIME)
	/home/bartek/opt/cross/bin/$(ARCH)-elf-ld $(LINKER_FLAGS) -T $(LINKER_SCRIPT) -o $(KERNEL_DIR) $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJECTS) $(ICXXABI_OBJ) $(CRTEND_OBJ) $(CRTN_OBJ)
	mkdir -p $(OBJ_DIR)
	mv $(OBJECTS) $(OBJECTS_RUNTIME) $(OBJ_DIR)

%.o: %.c $(HEADERS)
	${CC} ${C_FLAGS} ${INCLUDE} -c $< -o $@

%.o: %.cpp $(HEADERS)
	$(CC) $(CXX_FLAGS) $(INCLUDE) -c $< -o $@

%.o: %.asm
	nasm $(ASSEMBLER_FLAGS) $< -o $@

run: iso
	qemu-system-x86_64 $(QEMU_FLAGS) -monitor stdio

puredebug: iso
	qemu-system-x86_64 $(QEMU_FLAGS) -debugcon stdio -s -S &

debug: iso
	qemu-system-x86_64 $(QEMU_FLAGS) -debugcon stdio -s -S &
	gdb -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL_DIR)"

cleanobjs:
	find . -type f -name '*.o' -delete

clean:
	find . -type f -name '*.o' -delete
	rm -rf $(KERNEL_DIR)
	rm -rf $(KERNEL_ISO)
