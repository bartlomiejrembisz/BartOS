# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bartek/BartOS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bartek/BartOS

# Include any dependencies generated for this target.
include Kernel/CMakeFiles/crt0.dir/depend.make

# Include the progress variables for this target.
include Kernel/CMakeFiles/crt0.dir/progress.make

# Include the compile flags for this target's objects.
include Kernel/CMakeFiles/crt0.dir/flags.make

Kernel/CMakeFiles/crt0.dir/Arch/x86_64/boot.asm.obj: Kernel/CMakeFiles/crt0.dir/flags.make
Kernel/CMakeFiles/crt0.dir/Arch/x86_64/boot.asm.obj: Kernel/Arch/x86_64/boot.asm
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bartek/BartOS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building ASM_NASM object Kernel/CMakeFiles/crt0.dir/Arch/x86_64/boot.asm.obj"
	cd /home/bartek/BartOS/Kernel && /usr/bin/nasm $(ASM_NASM_INCLUDES) $(ASM_NASM_FLAGS) -f elf64 -o CMakeFiles/crt0.dir/Arch/x86_64/boot.asm.obj /home/bartek/BartOS/Kernel/Arch/x86_64/boot.asm

crt0: Kernel/CMakeFiles/crt0.dir/Arch/x86_64/boot.asm.obj
crt0: Kernel/CMakeFiles/crt0.dir/build.make

.PHONY : crt0

# Rule to build all files generated by this target.
Kernel/CMakeFiles/crt0.dir/build: crt0

.PHONY : Kernel/CMakeFiles/crt0.dir/build

Kernel/CMakeFiles/crt0.dir/clean:
	cd /home/bartek/BartOS/Kernel && $(CMAKE_COMMAND) -P CMakeFiles/crt0.dir/cmake_clean.cmake
.PHONY : Kernel/CMakeFiles/crt0.dir/clean

Kernel/CMakeFiles/crt0.dir/depend:
	cd /home/bartek/BartOS && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bartek/BartOS /home/bartek/BartOS/Kernel /home/bartek/BartOS /home/bartek/BartOS/Kernel /home/bartek/BartOS/Kernel/CMakeFiles/crt0.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Kernel/CMakeFiles/crt0.dir/depend

