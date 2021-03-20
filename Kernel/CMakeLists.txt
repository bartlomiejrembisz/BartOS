set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/linker.ld)

add_link_options(-T ${LINKER_SCRIPT} -n -z max-page-size=0x1000 -nostdlib -mcmodel=large)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/Lib)

link_libraries(c g m gcc supc++ stdc++)

set(ASM_SOURCES
	Arch/x86_64/x86_64.asm
	Arch/x86_64/Interrupts/Isrs.asm
)

set(CPP_SOURCES
	Arch/x86_64/GDT.cpp
	Arch/x86_64/Interrupts/PageFaultHandler.cpp
	Arch/x86_64/Interrupts/InterruptDescriptorTable.cpp
	Arch/x86_64/Interrupts/Interrupt.cpp
	Arch/x86_64/Paging/PageTableEntry.cpp
	Arch/x86_64/Paging/PageTable.cpp
	Arch/x86_64/CPU.cpp
	Multiboot2.cpp
	#Memory/SlabAllocator.cpp
	Memory/HeapManager.cpp
	Memory/PhysAddr.cpp
	Memory/Page.cpp
	Memory/Memory.cpp
	Memory/VirtAddr.cpp
	#Memory/Pmm.cpp
	Memory/Address.cpp
	#Memory/Paging/Pml.cpp
	#Memory/AddressSpace.cpp
	Memory/PhysicalRegion.cpp
	Memory/Vmm.cpp
	Memory/VmArea.cpp
	Kernel.cpp
)

# Runtime
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtbegin.o OUTPUT_VARIABLE crtbegin OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtend.o OUTPUT_VARIABLE crtend OUTPUT_STRIP_TRAILING_WHITESPACE)

add_library(crt0 OBJECT Arch/x86_64/boot.asm)
add_library(crti OBJECT Runtime/crti.asm)
add_library(crtn OBJECT Runtime/crtn.asm)

set(SOURCES ${ASM_SOURCES} ${C_SOURCES} ${CPP_SOURCES})

add_library(Kernel OBJECT ${SOURCES})

#add_custom_target(BartOS
#	OUTPUT ${BUILD_DIRECTORY}/BartOS.bin
#	COMMAND "$(TOOLCHAIN)$(ARCH)-elf-ld ${LINKER_FLAGS} -T $(LINKER_SCRIPT) $<TARGET_OBJECTS:Kernel> $(CRTBEGIN_OBJ) $(OBJECTS) $(ICXXABI_OBJ) $(CRTEND_OBJ) $(CRTN_OBJ)"
#)

add_executable(BartOS $<TARGET_OBJECTS:crt0> $<TARGET_OBJECTS:crti> ${crtbegin} $<TARGET_OBJECTS:Kernel> ${crtend} $<TARGET_OBJECTS:crtn>)
add_dependencies(BartOS Kernel crt0 crti crtn)


add_custom_command(
	TARGET BartOS POST_BUILD
	COMMAND cp $<TARGET_FILE:BartOS> ${BUILD_DIRECTORY}/isofiles/boot/kernel.bin
	COMMAND grub-mkrescue -o kernel.img ${BUILD_DIRECTORY}/isofiles/)
