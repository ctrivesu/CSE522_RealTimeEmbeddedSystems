# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /home/sushant/cmake-3.8.2-Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/sushant/cmake-3.8.2-Linux-x86_64/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sushant/Desktop/Assignment-4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sushant/Desktop/Assignment-4/build/galileo

# Utility rule file for linker_pass2_script.

# Include the progress variables for this target.
include zephyr/CMakeFiles/linker_pass2_script.dir/progress.make

zephyr/CMakeFiles/linker_pass2_script: zephyr/linker_pass2.cmd


zephyr/linker_pass2.cmd: /home/sushant/Desktop/zephyr_1.10/arch/x86/soc/intel_quark/quark_x1000/linker.ld
zephyr/linker_pass2.cmd: /home/sushant/Desktop/zephyr_1.10/arch/x86/soc/intel_quark/quark_x1000/linker.ld
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/sushant/Desktop/Assignment-4/build/galileo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating linker_pass2.cmd"
	cd /home/sushant/Desktop/Assignment-4/build/galileo/zephyr && /opt/zephyr-sdk/sysroots/x86_64-pokysdk-linux/usr/bin/i586-zephyr-elf/i586-zephyr-elf-gcc -x assembler-with-cpp -nostdinc -undef -MD -MF linker_pass2.cmd.dep -MT zephyr/linker_pass2.cmd -I/home/sushant/Desktop/zephyr_1.10/kernel/include -I/home/sushant/Desktop/zephyr_1.10/arch/x86/include -I/home/sushant/Desktop/zephyr_1.10/arch/x86/soc/intel_quark/quark_x1000 -I/home/sushant/Desktop/zephyr_1.10/arch/x86/soc/intel_quark/quark_x1000/include -I/home/sushant/Desktop/zephyr_1.10/arch/x86/soc/intel_quark/include -I/home/sushant/Desktop/zephyr_1.10/boards/x86/galileo -I/home/sushant/Desktop/zephyr_1.10/include -I/home/sushant/Desktop/zephyr_1.10/include/drivers -I/home/sushant/Desktop/Assignment-4/build/galileo/zephyr/include/generated -I/opt/zephyr-sdk/sysroots/x86_64-pokysdk-linux/usr/lib/i586-zephyr-elf/gcc/i586-zephyr-elf/6.2.0/include -I/opt/zephyr-sdk/sysroots/x86_64-pokysdk-linux/usr/lib/i586-zephyr-elf/gcc/i586-zephyr-elf/6.2.0/include-fixed -I/home/sushant/Desktop/zephyr_1.10/lib/libc/minimal/include -I/home/sushant/Desktop/zephyr_1.10/include/drivers -D__GCC_LINKER_CMD__ -DLINKER_PASS2 -E /home/sushant/Desktop/zephyr_1.10/arch/x86/soc/intel_quark/quark_x1000/linker.ld -P -o linker_pass2.cmd

linker_pass2_script: zephyr/CMakeFiles/linker_pass2_script
linker_pass2_script: zephyr/linker_pass2.cmd
linker_pass2_script: zephyr/CMakeFiles/linker_pass2_script.dir/build.make

.PHONY : linker_pass2_script

# Rule to build all files generated by this target.
zephyr/CMakeFiles/linker_pass2_script.dir/build: linker_pass2_script

.PHONY : zephyr/CMakeFiles/linker_pass2_script.dir/build

zephyr/CMakeFiles/linker_pass2_script.dir/clean:
	cd /home/sushant/Desktop/Assignment-4/build/galileo/zephyr && $(CMAKE_COMMAND) -P CMakeFiles/linker_pass2_script.dir/cmake_clean.cmake
.PHONY : zephyr/CMakeFiles/linker_pass2_script.dir/clean

zephyr/CMakeFiles/linker_pass2_script.dir/depend:
	cd /home/sushant/Desktop/Assignment-4/build/galileo && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sushant/Desktop/Assignment-4 /home/sushant/Desktop/zephyr_1.10 /home/sushant/Desktop/Assignment-4/build/galileo /home/sushant/Desktop/Assignment-4/build/galileo/zephyr /home/sushant/Desktop/Assignment-4/build/galileo/zephyr/CMakeFiles/linker_pass2_script.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : zephyr/CMakeFiles/linker_pass2_script.dir/depend
