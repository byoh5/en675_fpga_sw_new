# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.13

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\EN675\work8\code\en675_fpga_rc001

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\EN675\work8\code\en675_fpga_rc001\Debug

# Utility rule file for SECTION.

# Include the progress variables for this target.
include CMakeFiles/SECTION.dir/progress.make

CMakeFiles/SECTION:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=E:\EN675\work8\code\en675_fpga_rc001\Debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Merges the binaries."
	copy /Y /B bootloader.bin + core.bin EN675.bin

SECTION: CMakeFiles/SECTION
SECTION: CMakeFiles/SECTION.dir/build.make

.PHONY : SECTION

# Rule to build all files generated by this target.
CMakeFiles/SECTION.dir/build: SECTION

.PHONY : CMakeFiles/SECTION.dir/build

CMakeFiles/SECTION.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\SECTION.dir\cmake_clean.cmake
.PHONY : CMakeFiles/SECTION.dir/clean

CMakeFiles/SECTION.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\EN675\work8\code\en675_fpga_rc001 E:\EN675\work8\code\en675_fpga_rc001 E:\EN675\work8\code\en675_fpga_rc001\Debug E:\EN675\work8\code\en675_fpga_rc001\Debug E:\EN675\work8\code\en675_fpga_rc001\Debug\CMakeFiles\SECTION.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SECTION.dir/depend

